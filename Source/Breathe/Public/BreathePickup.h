#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreathePickup.generated.h"

class USphereComponent;

UCLASS()
class BREATHE_API ABreathePickup : public AActor
{
    GENERATED_BODY()

public:
    ABreathePickup();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Breathe|Pickup")
    bool Collect(APawn* Collector);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe|Pickup")
    TObjectPtr<USphereComponent> InteractionSphere;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Pickup")
    FName ItemId = TEXT("OxygenTank");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Pickup", meta=(ClampMin="1"))
    int32 Quantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Pickup", meta=(ClampMin="1"))
    int32 MaxStack = 2;

    UPROPERTY(ReplicatedUsing=OnRep_Collected, BlueprintReadOnly, Category="Breathe|Pickup")
    bool bCollected = false;

    UFUNCTION()
    void OnRep_Collected();
};
