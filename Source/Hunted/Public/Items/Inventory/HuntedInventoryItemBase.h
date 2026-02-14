// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuntedInventoryItemBase.generated.h"

class USphereComponent;

UCLASS()
class HUNTED_API AHuntedInventoryItemBase : public AActor
{
	GENERATED_BODY()
public:
	
	
private:
	UPROPERTY(EditAnywhere, Category = "Item-World")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item-World")
	USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere, Category = "Item-Inventory | Info")
	FIntPoint ObjectInventorySize;
	
	UPROPERTY(EditAnywhere, Category = "Item-Inventory | Info")
	UMaterialInterface* Icon;
	
	UPROPERTY(EditAnywhere, Category = "Item-World")
	bool  ItemIsInVew;
	
	UPROPERTY(EditAnywhere, Category = "Item-World")
	bool IsInRangeOfPlayer;	
	
public:
	AHuntedInventoryItemBase();
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FIntPoint GetItemInventorySize() const { return ObjectInventorySize; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	void SetItemInventorySize(FIntPoint NewSize) { ObjectInventorySize = NewSize; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	bool GetItemIsInVew() { return ItemIsInVew;};
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	void SetItemIsInVew(bool val) { ItemIsInVew = val; };
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	bool GetIsInRangeOfPlayer() { return IsInRangeOfPlayer;};
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	void SetIsInRangeOfPlayer(bool val) { IsInRangeOfPlayer = val; };
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	UMaterialInterface* GetIcon() const { return Icon; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	void SetIcon(UMaterialInterface* NewIcon) { Icon = NewIcon; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
