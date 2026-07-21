#pragma once

#include "CoreMinimal.h"
#include "BreatheTypes.generated.h"

UENUM(BlueprintType)
enum class EBreatheMovementState : uint8
{
    Resting UMETA(DisplayName = "Resting"),
    Walking UMETA(DisplayName = "Walking"),
    Running UMETA(DisplayName = "Running"),
    Panicking UMETA(DisplayName = "Panicking")
};

USTRUCT(BlueprintType)
struct FBreatheItemStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    int32 Quantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
    int32 MaxStack = 1;
};
