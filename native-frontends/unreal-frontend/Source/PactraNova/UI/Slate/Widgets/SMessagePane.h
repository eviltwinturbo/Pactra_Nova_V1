#pragma once

#include "Widgets/SCompoundWidget.h"

class FWebSocketClient;
class FApiClient;

/**
 * SMessagePane - Displays conversation messages and message input area
 * Subscribes to WebSocket for realtime message updates
 */
class SMessagePane : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMessagePane)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// WebSocket integration
	void SetApiClient(TSharedPtr<FApiClient> InApiClient);
	void OnRealtimeMessageReceived(const FString& MessagePayload);

private:
	void OnSendMessage(const FText& Message);
	void AppendMessageToPane(const FString& MessageText, const FString& Author = TEXT("Other"));

	TSharedPtr<FApiClient> ApiClient;
	TSharedPtr<FWebSocketClient> WebSocketClient;
	TArray<FString> Messages; // Local message history
};
