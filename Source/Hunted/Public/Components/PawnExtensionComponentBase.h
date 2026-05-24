// KasaiRaito All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HUNTED_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template <class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value,
			"'T' Template Parameter get GetPawn must be derived from APawn");
		// Components can receive late callbacks during teardown; return nullptr instead of asserting on owner state.
		return Cast<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template <class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value,
				"'T' Template Parameter get GetController must be derived from AController");
		APawn* OwningPawn = GetOwningPawn<APawn>();
		return IsValid(OwningPawn) ? OwningPawn->GetController<T>() : nullptr;
	}
};
