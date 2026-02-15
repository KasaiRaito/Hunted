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
	FORCEINLINE FIntPoint GetItemInventorySize() const { return ObjectInventorySize; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetItemInventorySize(FIntPoint NewSize) { ObjectInventorySize = NewSize; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE bool GetItemIsInVew() { return ItemIsInVew;};
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetItemIsInVew(bool val) { ItemIsInVew = val; };
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE bool GetIsInRangeOfPlayer() { return IsInRangeOfPlayer;};
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetIsInRangeOfPlayer(bool val) { IsInRangeOfPlayer = val; };
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE UMaterialInterface* GetIcon() const { return Icon; }
	
	UFUNCTION(BlueprintCallable, Category = "Item-Inventory")
	FORCEINLINE void SetIcon(UMaterialInterface* NewIcon) { Icon = NewIcon; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
