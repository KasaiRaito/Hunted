// KasaiRaito All Rights Reserved


#include "Widget/Inventory/PlayerInventoryWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Characters/HuntedPlayerCharacter.h"
#include "Components/Inventory/PlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Inventory/HuntedInventoryItemBase.h"
#include "Widget/Inventory/PlayerInventoryGridWidget.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CharacterReference = Cast<AHuntedPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

FReply UPlayerInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

bool UPlayerInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (!InOperation || !InOperation->Payload)
	{
		return false;
	}

	AHuntedInventoryItemBase* PayloadItem = Cast<AHuntedInventoryItemBase>(InOperation->Payload);
	if (!PayloadItem)
	{
		return false;
	}

	UPlayerInventoryComponent* InventoryComponent = CharacterReference
		? CharacterReference->GetPlayerInventoryComponent()
		: nullptr;
	if (!InventoryComponent)
	{
		return false;
	}

	// Remove state: item is discarded from the inventory without spawning in the world.
	if (!PayloadItem->IsItemDroppable())
	{
		InventoryComponent->RemoveItem(PayloadItem);
		CharacterReference->ClearCachedItem();
		if (UPlayerInventoryGridWidget* InventoryGrid = InventoryComponent->GetPlayerInventoryGridWidget())
		{
			InventoryGrid->RefreshItemWidgets();
		}
		InOperation->Tag = TEXT("RemovedFromInventory");
		return true;
	}
	
	//Set the object offset to the ground with a ray cast
	FHitResult GroundPoint = GetLocationBelow(CharacterReference->GetActorLocation() + FVector(0,0,100.0f) + (CharacterReference->GetActorForwardVector() * 150.0f));
	
	FVector SpawnLocation = GroundPoint.ImpactPoint;
	
	FVector SurfaceNormal = GroundPoint.ImpactNormal;
	FRotator SpawnRotation = FRotationMatrix::MakeFromZ(SurfaceNormal).Rotator();
	
	//FRotator SpawnRotation = CharacterReference->GetActorRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	SpawnedItem = GetWorld()->SpawnActor<AHuntedInventoryItemBase>(PayloadItem->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
	if (!SpawnedItem)
	{
		return false;
	}

	SpawnedItem->SetItemInventorySize(PayloadItem->GetItemInventorySize());
	SpawnedItem->SetIcon(PayloadItem->GetIcon());
	SpawnedItem->SetItemData(PayloadItem->GetItemData());
	SpawnedItem->SetActorHiddenInGame(false);
	SpawnedItem->SetActorEnableCollision(true);

	InventoryComponent->RemoveItem(PayloadItem);
	CharacterReference->ClearCachedItem();
	if (UPlayerInventoryGridWidget* InventoryGrid = InventoryComponent->GetPlayerInventoryGridWidget())
	{
		InventoryGrid->RefreshItemWidgets();
	}
	
	InOperation->Tag = TEXT("DroppedToWorld");
	
	return true;
}

FHitResult UPlayerInventoryWidget::GetLocationBelow(FVector Start) const
{
	FHitResult HitResult;
	FVector End = Start - FVector(0, 0, 1000.0f); // Trace 1000 units down

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CharacterReference); // Don't hit yourself

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		Start, 
		End, 
		ECC_Visibility, 
		Params
	);
	
	return HitResult; 
}
