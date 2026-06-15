#pragma once

/**
 * Enum to track if a character has a weapon (in hands or in the 'inventory') 
 */
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

/**
 * Enum to track if a character has a weapon in the hands 
 */
UENUM(BlueprintType)
enum class EArmedState : uint8
{
	EA_Unarmed UMETA(DisplayName = "Unarmed"),
	EA_Armed UMETA(DisplayName = "Armed")
};