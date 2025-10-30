#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "Delegates/Delegate.h"

class IWebSocket;

/**
 * FWebSocketClient - Realtime WebSocket client for Orbit Messenger
 * Manages connection lifecycle and message dispatch
 * Integrates with SMessagePane for live message updates and FApiClient for authentication
 */
class FWebSocketClient
{
public:
	FWebSocketClient();
	~FWebSocketClient();

	// Lifecycle management
	bool Connect(const FString& WebSocketUrl);
	void Disconnect();
	bool IsConnected() const;

	// Message sending
	void SendMessage(const FString& MessageText);

	// Event system
	DECLARE_EVENT_OneParam(FWebSocketClient, FOnMessageReceived, const FString&);
	FOnMessageReceived& OnMessageReceived() { return MessageReceivedEvent; }

	DECLARE_EVENT(FWebSocketClient, FOnConnected);
	FOnConnected& OnConnected() { return ConnectedEvent; }

	DECLARE_EVENT(FWebSocketClient, FOnDisconnected);
	FOnDisconnected& OnDisconnected() { return DisconnectedEvent; }

	DECLARE_EVENT_OneParam(FWebSocketClient, FOnError, const FString&);
	FOnError& OnError() { return ErrorEvent; }

private:
	// Internal event handlers
	void OnWebSocketConnected();
	void OnWebSocketDisconnected();
	void OnWebSocketMessage(const FString& Message);
	void OnWebSocketError(const FString& Error);

	// State
	TSharedPtr<IWebSocket> WebSocket;
	bool bIsConnected;

	// Event delegates
	FOnMessageReceived MessageReceivedEvent;
	FOnConnected ConnectedEvent;
	FOnDisconnected DisconnectedEvent;
	FOnError ErrorEvent;

	// Message queue for thread safety
	TArray<FString> PendingMessages;
	FCriticalSection MessageQueueLock;
};
