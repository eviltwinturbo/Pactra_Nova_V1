#include "FUnionPulseStore.h"

FUnionPulseStore* FUnionPulseStore::Instance = nullptr;

FUnionPulseStore& FUnionPulseStore::Get()
{
	if (!Instance)
	{
		Instance = new FUnionPulseStore();
	}
	return *Instance;
}

FUnionPulseStore::FUnionPulseStore()
{
}

FUnionPulseStore::~FUnionPulseStore()
{
}

int32 FUnionPulseStore::GetOnlineCount() const
{
	FScopeLock Lock(&CriticalSection);
	return OnlineCount;
}

int32 FUnionPulseStore::GetBillsTracked() const
{
	FScopeLock Lock(&CriticalSection);
	return BillsTracked;
}

int32 FUnionPulseStore::GetContractsUploaded() const
{
	FScopeLock Lock(&CriticalSection);
	return ContractsUploaded;
}

int32 FUnionPulseStore::GetTokenBalance() const
{
	FScopeLock Lock(&CriticalSection);
	return TokenBalance;
}

FString FUnionPulseStore::GetTierLabel() const
{
	FScopeLock Lock(&CriticalSection);
	return TierLabel;
}

FString FUnionPulseStore::GetUserId() const
{
	FScopeLock Lock(&CriticalSection);
	return UserId;
}

FString FUnionPulseStore::GetTenantId() const
{
	FScopeLock Lock(&CriticalSection);
	return TenantId;
}

void FUnionPulseStore::SetPresenceCount(int32 Count)
{
	{
		FScopeLock Lock(&CriticalSection);
		OnlineCount = Count;
	}
	PresenceUpdatedDelegate.Broadcast();
}

void FUnionPulseStore::SetBillsTracked(int32 Count)
{
	{
		FScopeLock Lock(&CriticalSection);
		BillsTracked = Count;
	}
	BillsUpdatedDelegate.Broadcast();
}

void FUnionPulseStore::SetContractsUploaded(int32 Count)
{
	{
		FScopeLock Lock(&CriticalSection);
		ContractsUploaded = Count;
	}
	ContractsUpdatedDelegate.Broadcast();
}

void FUnionPulseStore::SetTokenBalance(int32 Balance)
{
	{
		FScopeLock Lock(&CriticalSection);
		TokenBalance = Balance;
	}
	TokensUpdatedDelegate.Broadcast();
}

void FUnionPulseStore::SetTierLabel(const FString& Tier)
{
	{
		FScopeLock Lock(&CriticalSection);
		TierLabel = Tier;
	}
	TierChangedDelegate.Broadcast();
}

void FUnionPulseStore::SetSession(const FString& InUserId, const FString& InTenantId, const FString& InTier)
{
	{
		FScopeLock Lock(&CriticalSection);
		UserId = InUserId;
		TenantId = InTenantId;
		TierLabel = InTier;
	}
	TierChangedDelegate.Broadcast();
}

void FUnionPulseStore::Reset()
{
	FScopeLock Lock(&CriticalSection);
	OnlineCount = 0;
	BillsTracked = 0;
	ContractsUploaded = 0;
	TokenBalance = 0;
	TierLabel = TEXT("Trial");
	UserId.Empty();
	TenantId.Empty();
}
