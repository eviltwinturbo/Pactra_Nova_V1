#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * SConversationList - Displays list of conversations/channels
 */
class SConversationList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConversationList)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
