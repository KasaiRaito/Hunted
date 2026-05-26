// KasaiRaito All Rights Reserved


#include "HuntedGameplayTags.h"

namespace  HuntedGameplayTags
{
	/** Input Tags **/
		/** Movement **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sneak, "InputTag.Sneak");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Sprint, "InputTag.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "InputTag.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
		/** Echo **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Echo, "InputTag.Echo");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Snap, "InputTag.Snap");
		/** Combat **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Aim, "InputTag.Aim");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Shoot_Gun, "InputTag.Shoot.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Reload_Gun, "InputTag.Reload.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Shoot_Knife, "InputTag.Shoot.Knife");
		/** Equip / Unequip **/	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Gun, "InputTag.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip_Gun, "InputTag.Unequip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Knife, "InputTag.Equip.Knife");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip_Knife, "InputTag.Unequip.Knife");
		/** Inventory / UI **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Inventory_Open, "InputTag.Inventory.Open");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Inventory_Close, "InputTag.Inventory.Close");
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Pause_Open, "InputTag.Pause.Open");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Pause_Close, "InputTag.Pause.Close");
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact, "InputTag.Interact");
	
	/** Player **/
		/** Abilities **/
			/** Echo **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Echo, "Player.Ability.Echo");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Snap, "Player.Ability.Snap");
			/** Combat **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Aim, "Player.Ability.Aim");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Shoot_Gun, "Player.Ability.Shoot.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Reload_Gun, "Player.Ability.Reload.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Shoot_Knife, "Player.Ability.Shoot.Knife");
			/** Hit React **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitReact_HighHP, "Player.Ability.HitReact.HighHP");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitReact_HighHP, "Player.Event.HitReact.HighHP");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitReact_MidHP, "Player.Ability.HitReact.MidHP");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitReact_MidHP, "Player.Event.HitReact.MidHP");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitReact_LowHP, "Player.Ability.HitReact.LowHP");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitReact_LowHP, "Player.Event.HitReact.LowHP");
	
			/** Equip / Unequip **/	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Gun, "Player.Ability.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Gun, "Player.Ability.Unequip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Knife, "Player.Ability.Equip.Knife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Knife, "Player.Ability.Unequip.Knife");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Inventory_Open, "Player.Ability.Inventory.Open");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Inventory_Close, "Player.Ability.Inventory.Close");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Pause_Open, "Player.Ability.Pause.Open");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Pause_Close, "Player.Ability.Pause.Close");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Interact, "Player.Ability.Interact");
	
		/** Events **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Gun, "Player.Event.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Gun, "Player.Event.Unequip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Reload_Gun, "Player.Event.Reload.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Knife, "Player.Event.Equip.Knife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Knife, "Player.Event.Unequip.Knife");
 
	/** Player Weapon Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_BasicKnife, "Player.Weapon.BasicKnife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Gun, "Player.Weapon.Gun");
	
	/** Player Item Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Cure_Small, "Player.Item.Cure.Small");
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Cure_Medium, "Player.Item.Cure.Medium");
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Cure_Large, "Player.Item.Cure.Large");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Scrap_Small, "Player.Item.Scrap.Small");
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Scrap_Medium, "Player.Item.Scrap.Medium");
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Scrap_Large, "Player.Item.Scrap.Large");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Chemical_Small, "Player.Item.Chemical.Small");
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Chemical_Medium, "Player.Item.Chemical.Medium");
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Chemical_Large, "Player.Item.Chemical.Large");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Item_Bullets_BaseGun, "Player.Item.Bullets.BaseGun");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Puzzle_Key, "Player.Item.Puzzle.Key");
	
		/** Set By Caller **/
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_BasicKnife, "Player.SetByCaller.AttackType.BasicKnife");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Head, "Player.SetByCaller.AttackType.Head");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Body, "Player.SetByCaller.AttackType.Body");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Leg, "Player.SetByCaller.AttackType.Leg");
	
	/** Enemy Weapon Tags **/
		/** Attack **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Grab, "Enemy.Ability.Grab");
	
		/** Hit React **/	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact_Head, "Shared.Ability.HitReact.Head");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Head, "Shared.Event.HitReact.Head");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact_Body, "Shared.Ability.HitReact.Body");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Body, "Shared.Event.HitReact.Body");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact_Leg, "Shared.Ability.HitReact.Leg");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Leg, "Shared.Event.HitReact.Leg");
	
		/** Weapons **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");
	
	/** Shared Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Death, "Shared.Status.Death");
}
