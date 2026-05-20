#pragma once

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	EES_Unequipped UMETA(DisplayName = "Unequipped"),
	EES_Equipped UMETA(DisplayName = "Equipped")
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

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking")
};