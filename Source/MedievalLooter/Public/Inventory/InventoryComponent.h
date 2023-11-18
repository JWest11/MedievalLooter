// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryDataAsset.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemovedDelegate, FInventoryItemInfo, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAddedDelegate, FInventoryItemInfo, ItemInfo);

UCLASS(BlueprintType, Category = "Inventory")
class MEDIEVALLOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


private:

	UPROPERTY(Replicated, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxWeight;

	UPROPERTY(Replicated)
	float CurrentWeight;

	UFUNCTION()
	void OnRep_Inventory();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Inventory)
	TArray<FInventoryItemInfo> Inventory;

	// Returns -1 if not found
	int GetItemIndex(const FInventoryItemInfo ItemInfo) const;

	virtual bool TryAddItem(const FInventoryItemInfo ItemInfo);

	virtual bool TryRemoveItem(const FInventoryItemInfo ItemInfo);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CanAddItemWeight(const FInventoryItemInfo ItemInfo) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryItemInfo> GetInventory() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetMaxWeight() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetCurrentWeight() const;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_AddItemFromPickup(const FInventoryItemInfo ItemInfo, class APickup* Pickup);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_TryAddItem(const FInventoryItemInfo ItemInfo);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_TryRemoveItem(const FInventoryItemInfo ItemInfo);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_SetMaxWeight(float NewMaxWeight);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_GiveItem(const FInventoryItemInfo ItemInfo, UInventoryComponent* receivingComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool ContainsItem(const FInventoryItemInfo ItemInfo) const;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FInventoryChangedDelegate InventoryChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemRemovedDelegate RemoveItemDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FItemAddedDelegate AddItemDelegate;


};
