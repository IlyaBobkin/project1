#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BreatheCharacter.generated.h"

class UBreathingComponent;
class UInventoryComponent;
class UTensionComponent;

UCLASS()
class BREATHE_API ABreatheCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABreatheCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe")
    TObjectPtr<UBreathingComponent> BreathingComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe")
    TObjectPtr<UInventoryComponent> InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe")
    TObjectPtr<UTensionComponent> TensionComponent;

    UFUNCTION(Server, Reliable, BlueprintCallable, Category="Breathe|Interaction")
    void ServerCollectItem(FName ItemId, int32 Quantity, int32 MaxStack);
};
