#include "ApiClient.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "WebSocketClient.h"

FApiClient::FApiClient(const FString& InBaseUrl)
	: BaseUrl(InBaseUrl), AuthToken(TEXT(""))
{
	WebSocketClient = MakeShared<FWebSocketClient>();
}

FApiClient::~FApiClient()
{
	ShutdownRealtimeConnection();
}

bool FApiClient::Login(const FString& Username, const FString& Password)
{
	// TODO: Implement login endpoint call
	// POST /v1/auth/login with username/password
	// Store returned auth token
	// On success, initialize WebSocket connection for realtime
	return false;
}

void FApiClient::FetchTierConfig()
{
	// TODO: Implement tier config fetch
	// GET /v1/tiers with auth token
}

void FApiClient::FetchConversations()
{
	// TODO: Implement conversations fetch
	// GET /v1/orbit/conversations with auth token
}

void FApiClient::SendMessage(const FString& ConversationId, const FString& MessageText)
{
	if (WebSocketClient.IsValid() && WebSocketClient->IsConnected())
	{
		// Send via WebSocket for realtime
		WebSocketClient->SendMessage(MessageText);
	}
	else
	{
		// Fallback to HTTP POST
		// POST /v1/orbit/messages with conversation_id and text
	}
}

void FApiClient::UpdatePresence(const FString& Status)
{
	// TODO: Implement presence update
	// PATCH /v1/presence with status
}

void FApiClient::InitializeRealtimeConnection(const FString& ConversationId)
{
	if (!WebSocketClient.IsValid())
	{
		WebSocketClient = MakeShared<FWebSocketClient>();
	}

	// TODO: Use auth token to construct secure WebSocket URL
	// Format: wss://api.pactranova.local/v1/orbit/realtime?token=<auth_token>&conversation_id=<id>
	FString WebSocketUrl = FString::Printf(TEXT("wss://localhost:5000/v1/orbit/realtime?conversation_id=%s"), *ConversationId);
	
	WebSocketClient->Connect(WebSocketUrl);
	UE_LOG(LogTemp, Log, TEXT("Realtime connection initialized for conversation: %s"), *ConversationId);
}

void FApiClient::ShutdownRealtimeConnection()
{
	if (WebSocketClient.IsValid())
	{
		WebSocketClient->Disconnect();
		WebSocketClient.Reset();
		UE_LOG(LogTemp, Log, TEXT("Realtime connection shutdown"));
	}
}

void FApiClient::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// TODO: Parse response, store auth token
	// On success, call InitializeRealtimeConnection
}

void FApiClient::OnTierConfigResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// TODO: Parse tier config response
}

void FApiClient::OnConversationsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// TODO: Parse conversations response, update UI
}

void FApiClient::OnMessageResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// TODO: Handle message send response
}
