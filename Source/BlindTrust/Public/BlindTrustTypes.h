#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left		UMETA(DisplayName = "Turning Left"),
	ETIP_Right		UMETA(DisplayName = "Turning Right"),
	ETIP_NotTurning	UMETA(DisplayName = "Not Turning"),

	ETIP_MAX		UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum  class EPlayerType : uint8
{
	EPT_BlindPlayer		UMETA(DisplayName = "BlindPlayer"),
	EPT_GuidePlayer		UMETA(DisplayName = "GuidePlayer"),

	EPT_MAX				UMETA(DisplayName = "DefaultMax")
};

const FName BLIND_PLAYER_TAG = FName("BlindPlayer");
const FName GUIDE_PLAYER_TAG = FName("GuidePlayer");