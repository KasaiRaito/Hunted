// KasaiRaito All Rights Reserved


#include "AnimInstances/Player/HuntedPlayerLinkedAnimLayer.h"
#include "AnimInstances/Player/HuntedPlayerAnimInstance.h"

UHuntedPlayerAnimInstance* UHuntedPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UHuntedPlayerAnimInstance>( GetOwningComponent()->GetAnimInstance());
}
