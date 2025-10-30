#include "SMessagePane.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SVerticalBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "../../../Services/ApiClient.h"
#include "../../../Services/WebSocketClient.h"

void SMessagePane::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Message Area - Waiting for messages...")))
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SEditableTextBox)
			// TODO: Bind to input handler for sending messages
		]
	];
}

void SMessagePane::SetApiClient(TSharedPtr<FApiClient> InApiClient)
{
	ApiClient = InApiClient;
	
	if (ApiClient.IsValid())
	{
		WebSocketClient = MakeShared<FWebSocketClient>(*(ApiClient->GetWebSocketClient()));
		
		// Subscribe to realtime message updates
		if (WebSocketClient.IsValid())
		{
			WebSocketClient->OnMessageReceived().AddRaw(this, &SMessagePane::OnRealtimeMessageReceived);
			UE_LOG(LogTemp, Log, TEXT("SMessagePane subscribed to WebSocket messages"));
		}
	}
}

void SMessagePane::OnRealtimeMessageReceived(const FString& MessagePayload)
{
	// Parse message JSON and append to pane
	// TODO: Parse JSON to extract author, text, timestamp
	
	AppendMessageToPane(MessagePayload, TEXT("Remote User"));
}

void SMessagePane::AppendMessageToPane(const FString& MessageText, const FString& Author)
{
	FString FormattedMessage = FString::Printf(TEXT("[%s]: %s"), *Author, *MessageText);
	Messages.Add(FormattedMessage);
	
	UE_LOG(LogTemp, Log, TEXT("Message appended: %s"), *FormattedMessage);
	
	// TODO: Update UI list widget with new message
	// Trigger re-render of message list
}

void SMessagePane::OnSendMessage(const FText& Message)
{
	if (ApiClient.IsValid())
	{
		// Send message via API (WebSocket if connected, HTTP fallback)
		ApiClient->SendMessage(TEXT("default_conversation"), Message.ToString());
		
		// Append to local history
		AppendMessageToPane(Message.ToString(), TEXT("You"));
	}
	
	// TODO: Clear input field
	// TODO: Update message list
}
