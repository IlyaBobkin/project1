#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreatheEscapeDoor.generated.h"

class UStaticMeshComponent;

UCLASS()
class BREATHE_API ABreatheEscapeDoor : public AActor
{
    GENERATED_BODY()

public:
    ABreatheEscapeDoor();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Breathe|Door")
    bool TryOpen(APawn* InstigatorPawn);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe|Door")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Door")
    FName RequiredItemId = TEXT("RopeMarker");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Door", meta=(ClampMin="0"))
    int32 RequiredQuantity = 0;

    UPROPERTY(ReplicatedUsing=OnRep_Open, BlueprintReadOnly, Category="Breathe|Door")
    bool bOpen = false;

    UFUNCTION(BlueprintImplementableEvent, Category="Breathe|Door")
    void BP_OnDoorOpened();

    UFUNCTION()
    void OnRep_Open();
};
