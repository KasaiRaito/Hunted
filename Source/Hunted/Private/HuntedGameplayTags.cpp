// KasaiRaito All Rights Reserved


#include "HuntedGameplayTags.h"

namespace  HuntedGameplayTags
{
	/* Input Tags */
	/* Movement */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sneak, "InputTag.Sneak");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint, "InputTag.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "InputTag.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Snap, "InputTag.Snap");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	
	/* Abilities */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Echo, "InputTag.Echo");
	
	/* Combat */
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Aim_Gun, "InputTag.Aim.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Aim_Gun, "Player.Ability.Aim.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Shoot_Gun, "InputTag.Shoot.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Shoot_Gun, "Player.Ability.Shoot.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Reload_Gun, "InputTag.Reload.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Reload_Gun, "Player.Ability.Reload.Gun");
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Shoot_Knife, "InputTag.Shoot.Knife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Shoot_Knife, "Player.Ability.Shoot.Knife");

	/* Equip / Unequip */	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Gun, "InputTag.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip_Gun, "InputTag.Unequip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Gun, "Player.Ability.Equip.Gun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Gun, "Player.Ability.Unequip.Gun")
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Knife, "InputTag.Equip.Knife");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip_Knife, "InputTag.Unequip.Knife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Knife, "Player.Ability.Equip.Knife")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Knife, "Player.Ability.Unequip.Knife")

	/* Events */
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Gun, "Player.Event.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Gun, "Player.Event.Unequip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Reload_Gun, "Player.Event.Reload.Gun")
	
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Knife, "Player.Event.Equip.Knife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Knife, "Player.Event.Unequip.Knife");
 
	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_BasicKnife, "Player.Weapon.BasicKnife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Gun, "Player.Weapon.Gun")
	
	/** Enemy Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");
}
