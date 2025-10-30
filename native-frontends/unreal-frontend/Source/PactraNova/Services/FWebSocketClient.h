#pragma once

#include "CoreMinimal.h"
#include "IWebSocket.h"
#include "Containers/Queue.h"

// Circuit breaker states
enum class ERealtimeCircuitState : uint8
{
	Closed,      // Normal operation
	Open,        // Too many errors, fallback to HTTP
	HalfOpen     // Testing recovery
};

// WebSocket message v2 schema
struct FWebSocketMessageV2
{
	FString Type;           // message, presence, typing, read, reaction, thread
	FString ConversationId;
	FString MessageId;
	FString Content;
	FString AuthorId;
	FString AuthorEmail;
	FString AuthorName;
	FDateTime Timestamp;
	bool bEdited = false;
	bool bDeleted = false;
	FString ParentId;      // For threaded replies
	TArray<FString> Reactions;
	TArray<FString> AttachmentIds;

	static bool TryParseFromJson(const FString& JsonString, FWebSocketMessageV2& OutMessage);
};

// Delegates for message events
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWebSocketMessage, const FWebSocketMessageV2&);
DECLARE_MULTICAST_DELEGATE(FOnWebSocketConnected);
DECLARE_MULTICAST_DELEGATE(FOnWebSocketDisconnected);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWebSocketError, const FString&);

/**
 * FWebSocketClient
 * 
 * Enhanced WebSocket client with:
 * - Tokenized connections (short-lived JWT)
 * - Token rotation every 25 minutes
 * - Circuit breaker for realtime → HTTP fallback
 * - JSON v2 payload parsing
 * - Thread-safe message dispatch
 */
class FWebSocketClient
{
public:
	static FWebSocketClient& Get();

	// Connect with tokenized URL
	bool Connect(const FString& Token, const FString& TenantId, const FString& UserId);
	bool Disconnect();

	// Send message
	bool SendMessage(const FWebSocketMessageV2& Message);

	// Get circuit breaker state
	ERealtimeCircuitState GetCircuitState() const { return CircuitState; }
	bool IsHealthy() const { return CircuitState == ERealtimeCircuitState::Closed; }

	// Events
	FOnWebSocketMessage& OnMessage() { return MessageDelegate; }
	FOnWebSocketConnected& OnConnected() { return ConnectedDelegate; }
	FOnWebSocketDisconnected& OnDisconnected() { return DisconnectedDelegate; }
	FOnWebSocketError& OnError() { return ErrorDelegate; }

private:
	FWebSocketClient();
	~FWebSocketClient();

	FWebSocketClient(const FWebSocketClient&) = delete;
	FWebSocketClient& operator=(const FWebSocketClient&) = delete;

	// Token rotation (25 min interval)
	void StartTokenRotation();
	void RefreshToken();

	// Circuit breaker
	void RecordError();
	void ResetBreaker();
	void TransitionBreaker();

	// WebSocket callbacks
	void OnWebSocketConnected();
	void OnWebSocketClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnWebSocketMessage(const FString& Message);
	void OnWebSocketError(const FString& Error);

	// State
	TSharedPtr<IWebSocket> WebSocket;
	ERealtimeCircuitState CircuitState = ERealtimeCircuitState::Closed;
	int32 ErrorCount = 0;
	FDateTime LastErrorTime;
	FDateTime CircuitOpenedAt;

	FString CurrentToken;
	FString TenantId;
	FString UserId;
	FTimerHandle TokenRotationTimer;

	mutable FCriticalSection StateLock;

	// Delegates
	FOnWebSocketMessage MessageDelegate;
	FOnWebSocketConnected ConnectedDelegate;
	FOnWebSocketDisconnected DisconnectedDelegate;
	FOnWebSocketError ErrorDelegate;

	static FWebSocketClient* Instance;
};
