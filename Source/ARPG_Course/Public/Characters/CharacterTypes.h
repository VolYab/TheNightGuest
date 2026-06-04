#pragma once

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	EES_Unequipped UMETA(DisplayName = "Unequipped"),
	EES_Equipped UMETA(DisplayName = "Equipped")
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
