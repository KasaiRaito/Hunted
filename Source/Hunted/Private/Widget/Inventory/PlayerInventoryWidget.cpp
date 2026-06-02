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

	OnNativeVisibilityChanged.RemoveAll(this);
	OnNativeVisibilityChanged.AddUObject(this, &UPlayerInventoryWidget::HandleInventoryVisibilityChanged);
	HandleInventoryVisibilityChanged(GetVisibility());
}

void UPlayerInventoryWidget::NativeDestruct()
{
	OnNativeVisibilityChanged.RemoveAll(this);
	Super::NativeDestruct();
}

FReply UPlayerInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

bool UPlayerInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (!InOperation || !IsValid(InOperation->Payload) || !IsValid(CharacterReference))
	{
		return false;
	}

	AHuntedInventoryItemBase* PayloadItem = Cast<AHuntedInventoryItemBase>(InOperation->Payload);
	if (!IsValid(PayloadItem))
	{
		return false;
	}

	UPlayerInventoryComponent* InventoryComponent = CharacterReference
		? CharacterReference->GetPlayerInventoryComponent()
		: nullptr;
	if (!IsValid(InventoryComponent))
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

		// The drag payload points at the hidden inventory actor; clear and destroy it after removal.
		InOperation->Tag = TEXT("RemovedFromInventory");
		InOperation->Payload = nullptr;
		if (IsValid(PayloadItem))
		{
			PayloadItem->Destroy();
		}
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

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}
	
	SpawnedItem = World->SpawnActor<AHuntedInventoryItemBase>(PayloadItem->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
	if (!IsValid(SpawnedItem))
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
	
	// The world actor now owns the dropped item state; the inventory copy must not remain as a stale payload.
	InOperation->Tag = TEXT("DroppedToWorld");
	InOperation->Payload = nullptr;
	if (IsValid(PayloadItem))
	{
		PayloadItem->Destroy();
	}
	
	return true;
}

void UPlayerInventoryWidget::HandleInventoryVisibilityChanged(ESlateVisibility /*InVisibility*/)
{
	UPlayerInventoryComponent* InventoryComponent = IsValid(CharacterReference)
		? CharacterReference->GetPlayerInventoryComponent()
		: nullptr;
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	if (UPlayerInventoryGridWidget* InventoryGrid = InventoryComponent->GetPlayerInventoryGridWidget())
	{
		InventoryGrid->ResetItemInfoPanel();
	}
}

FHitResult UPlayerInventoryWidget::GetLocationBelow(FVector Start) const
{
	FHitResult HitResult;
	FVector End = Start - FVector(0, 0, 1000.0f); // Trace 1000 units down
	UWorld* World = GetWorld();
	if (!World)
	{
		HitResult.ImpactPoint = End;
		HitResult.ImpactNormal = FVector::UpVector;
		return HitResult;
	}

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CharacterReference); // Don't hit yourself

	bool bHit = World->LineTraceSingleByChannel(
		HitResult, 
		Start, 
		End, 
		ECC_Visibility, 
		Params
	);

	if (!bHit)
	{
		// Avoid spawning dropped items at the world origin when the ground trace misses.
		HitResult.ImpactPoint = End;
		HitResult.ImpactNormal = FVector::UpVector;
	}
	
	return HitResult; 
}
