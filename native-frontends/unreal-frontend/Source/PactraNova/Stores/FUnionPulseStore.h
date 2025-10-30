#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"

class FUnionPulseStore;

// Delegate types for reactive updates
DECLARE_MULTICAST_DELEGATE(FOnPresenceUpdated);
DECLARE_MULTICAST_DELEGATE(FOnBillsUpdated);
DECLARE_MULTICAST_DELEGATE(FOnContractsUpdated);
DECLARE_MULTICAST_DELEGATE(FOnTokensUpdated);
DECLARE_MULTICAST_DELEGATE(FOnTierChanged);

/**
 * FUnionPulseStore
 * 
 * Centralized metrics store for Union Pulse header widget.
 * Maintains state for: presence (online count), bills tracked, contracts uploaded, token balance.
 * Provides subscription/dispatch pattern for reactive UI updates.
 * Thread-safe via critical sections.
 */
class FUnionPulseStore
{
public:
	// Singleton accessor
	static FUnionPulseStore& Get();

	// ============ Getters (Thread-safe) ============
	int32 GetOnlineCount() const;
	int32 GetBillsTracked() const;
	int32 GetContractsUploaded() const;
	int32 GetTokenBalance() const;
	FString GetTierLabel() const;
	FString GetUserId() const;
	FString GetTenantId() const;

	// ============ Setters (Dispatch updates) ============
	void SetPresenceCount(int32 Count);
	void SetBillsTracked(int32 Count);
	void SetContractsUploaded(int32 Count);
	void SetTokenBalance(int32 Balance);
	void SetTierLabel(const FString& Tier);
	void SetSession(const FString& InUserId, const FString& InTenantId, const FString& InTier);

	// ============ Subscriptions ============
	FOnPresenceUpdated& OnPresenceUpdated() { return PresenceUpdatedDelegate; }
	FOnBillsUpdated& OnBillsUpdated() { return BillsUpdatedDelegate; }
	FOnContractsUpdated& OnContractsUpdated() { return ContractsUpdatedDelegate; }
	FOnTokensUpdated& OnTokensUpdated() { return TokensUpdatedDelegate; }
	FOnTierChanged& OnTierChanged() { return TierChangedDelegate; }

	// ============ Reset ============
	void Reset();

private:
	FUnionPulseStore();
	~FUnionPulseStore();

	// Prevent copy/move
	FUnionPulseStore(const FUnionPulseStore&) = delete;
	FUnionPulseStore& operator=(const FUnionPulseStore&) = delete;

	// ============ State ============
	mutable FCriticalSection CriticalSection;

	int32 OnlineCount = 0;
	int32 BillsTracked = 0;
	int32 ContractsUploaded = 0;
	int32 TokenBalance = 0;
	FString TierLabel = TEXT("Trial");
	FString UserId;
	FString TenantId;

	// ============ Delegates ============
	FOnPresenceUpdated PresenceUpdatedDelegate;
	FOnBillsUpdated BillsUpdatedDelegate;
	FOnContractsUpdated ContractsUpdatedDelegate;
	FOnTokensUpdated TokensUpdatedDelegate;
	FOnTierChanged TierChangedDelegate;

	static FUnionPulseStore* Instance;
};
