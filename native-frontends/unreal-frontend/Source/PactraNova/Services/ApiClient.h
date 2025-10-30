#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

class FWebSocketClient;

/**
 * FApiClient - HTTP client for communicating with backend API
 * Handles login, tier configuration, conversations, messages, and presence updates
 * Manages WebSocket lifecycle for realtime Orbit Messenger updates
 */
class FApiClient
{
public:
	FApiClient(const FString& InBaseUrl = TEXT("http://localhost:5000"));
	~FApiClient();

	// Authentication
	bool Login(const FString& Username, const FString& Password);

	// Tier Configuration
	void FetchTierConfig();

	// Conversations
	void FetchConversations();
	void SendMessage(const FString& ConversationId, const FString& MessageText);

	// Presence
	void UpdatePresence(const FString& Status);

	// WebSocket lifecycle management
	void InitializeRealtimeConnection(const FString& ConversationId);
	void ShutdownRealtimeConnection();

	// WebSocket access
	FWebSocketClient* GetWebSocketClient() { return WebSocketClient.Get(); }

private:
	FString BaseUrl;
	FString AuthToken;
	TSharedPtr<FWebSocketClient> WebSocketClient;

	// HTTP response handlers
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnTierConfigResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnConversationsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnMessageResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
