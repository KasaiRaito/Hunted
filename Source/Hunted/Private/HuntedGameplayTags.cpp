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
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Snap, "InputTag.Snap");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
		/** Echo **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Echo, "InputTag.Echo");
		/** Combat **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Aim_Gun, "InputTag.Aim.Gun");
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
	
	/** Player **/
		/** Abilities **/
			/** Combat **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Aim_Gun, "Player.Ability.Aim.Gun");
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
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Gun, "Player.Ability.Equip.Gun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Gun, "Player.Ability.Unequip.Gun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Knife, "Player.Ability.Equip.Knife")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Knife, "Player.Ability.Unequip.Knife")
	
		/** Events **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Gun, "Player.Event.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Gun, "Player.Event.Unequip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Reload_Gun, "Player.Event.Reload.Gun")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Knife, "Player.Event.Equip.Knife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Knife, "Player.Event.Unequip.Knife");
 
		/** Player Weapon Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_BasicKnife, "Player.Weapon.BasicKnife");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Gun, "Player.Weapon.Gun")
	
		/** Player Item Tags **/
	//UE_DEFINE_GAMEPLAY_TAG(Player_Item_Cure, "Player.Item.Cure");
	
		/** Set By Caller **/
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_BasicKnife, "Player.SetByCaller.AttackType.BasicKnife");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Head, "Player.SetByCaller.AttackType.Head");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Body, "Player.SetByCaller.AttackType.Body");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Leg, "Player.SetByCaller.AttackType.Leg");
	
	/** Enemy Weapon Tags **/
		/** Attack **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");
	
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
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Hit, "Shared.Ability.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit, "Shared.Event.HitReact");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Death, "Shared.Status.Death");
}
