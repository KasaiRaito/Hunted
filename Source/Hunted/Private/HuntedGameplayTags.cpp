// KasaiRaito All Rights Reserved


#include "HuntedGameplayTags.h"

namespace  HuntedGameplayTags
{
	/* Input Tags */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sneak, "InputTag.Sneak");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint, "InputTag.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "InputTag.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Snap, "InputTag.Snap");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Echo, "InputTag.Echo");
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");

	/* Equip / Unequip */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_BasicKnife, "InputTag.Equip.BasicKnife");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip_BasicKnife, "InputTag.Unequip.BasicKnife");
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Gun, "InputTag.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip_Gun, "InputTag.Unequip.Gun");

	/* Hold Let Go Weapons */
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Gun, "Player.Event.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Gun, "Player.Event.Unequip.Gun");
 
	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_BasicKnife, "Player.Weapon.BasicKnife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Gun, "Player.Weapon.Gun")
}
