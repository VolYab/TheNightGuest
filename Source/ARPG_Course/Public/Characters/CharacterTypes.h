#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Arming UMETA(DisplayName = "Arming")
};

UENUM(BlueprintType)
enum class EDeadPose : uint8
{
	EAS_Alive UMETA(DisplayName = "Alive"),
	EAS_DeadForward UMETA(DisplayName = "DeadForward"),
	EAS_DeadBackward UMETA(DisplayName = "DeadBackward"),
	EAS_DeadLeft UMETA(DisplayName = "DeadLeft"),
	EAS_DeadRight UMETA(DisplayName = "DeadRight")
};