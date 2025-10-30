#include "WebSocketClient.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"

FWebSocketClient::FWebSocketClient()
	: bIsConnected(false)
{
}

FWebSocketClient::~FWebSocketClient()
{
	Disconnect();
}

bool FWebSocketClient::Connect(const FString& WebSocketUrl)
{
	if (bIsConnected)
	{
		UE_LOG(LogTemp, Warning, TEXT("WebSocket already connected"));
		return false;
	}

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(WebSocketUrl, TEXT("ws"));
	
	if (!WebSocket.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create WebSocket for URL: %s"), *WebSocketUrl);
		return false;
	}

	// Bind event handlers
	WebSocket->OnConnected().AddRaw(this, &FWebSocketClient::OnWebSocketConnected);
	WebSocket->OnConnectionError().AddRaw(this, &FWebSocketClient::OnWebSocketError);
	WebSocket->OnClosed().AddRaw(this, &FWebSocketClient::OnWebSocketDisconnected);
	WebSocket->OnMessage().AddRaw(this, &FWebSocketClient::OnWebSocketMessage);
	WebSocket->OnRawMessage().AddRaw(this, &FWebSocketClient::OnWebSocketMessage);

	// Attempt connection
	WebSocket->Connect();
	
	UE_LOG(LogTemp, Log, TEXT("WebSocket connecting to: %s"), *WebSocketUrl);
	return true;
}

void FWebSocketClient::Disconnect()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Close();
		UE_LOG(LogTemp, Log, TEXT("WebSocket disconnecting"));
	}

	bIsConnected = false;
	WebSocket.Reset();
}

bool FWebSocketClient::IsConnected() const
{
	return bIsConnected && WebSocket.IsValid() && WebSocket->IsConnected();
}

void FWebSocketClient::SendMessage(const FString& MessageText)
{
	if (!IsConnected())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot send message; WebSocket not connected"));
		return;
	}

	WebSocket->Send(MessageText);
	UE_LOG(LogTemp, Log, TEXT("WebSocket message sent: %s"), *MessageText);
}

void FWebSocketClient::OnWebSocketConnected()
{
	bIsConnected = true;
	UE_LOG(LogTemp, Log, TEXT("WebSocket connected"));
	
	ConnectedEvent.Broadcast();
}

void FWebSocketClient::OnWebSocketDisconnected()
{
	bIsConnected = false;
	UE_LOG(LogTemp, Log, TEXT("WebSocket disconnected"));
	
	DisconnectedEvent.Broadcast();
}

void FWebSocketClient::OnWebSocketMessage(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket message received: %s"), *Message);
	
	// Broadcast message to all listeners (mainly SMessagePane)
	MessageReceivedEvent.Broadcast(Message);
}

void FWebSocketClient::OnWebSocketError(const FString& Error)
{
	bIsConnected = false;
	UE_LOG(LogTemp, Error, TEXT("WebSocket error: %s"), *Error);
	
	ErrorEvent.Broadcast(Error);
}
