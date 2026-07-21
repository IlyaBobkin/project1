#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BreatheTypes.h"
#include "BreathingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOxygenChanged, float, CurrentOxygen, float, MaxOxygen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBreathPhaseChanged, float, BreathPhase);

UCLASS(ClassGroup=(Breathe), meta=(BlueprintSpawnableComponent))
class BREATHE_API UBreathingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBreathingComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category="Breathe|Breathing")
    void SetMovementState(EBreatheMovementState NewState);

    UFUNCTION(BlueprintCallable, Category="Breathe|Breathing")
    void ConsumeOxygen(float Amount);

    UFUNCTION(BlueprintCallable, Category="Breathe|Breathing")
    void RefillOxygen(float Amount);

    UPROPERTY(BlueprintAssignable)
    FOnOxygenChanged OnOxygenChanged;

    UPROPERTY(BlueprintAssignable)
    FOnBreathPhaseChanged OnBreathPhaseChanged;

protected:
    UFUNCTION()
    void OnRep_Oxygen();

    UFUNCTION()
    void OnRep_BreathPhase();

    void TickBreath();
    float GetDrainRate() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Breathing")
    float MaxOxygen = 100.0f;

    UPROPERTY(ReplicatedUsing=OnRep_Oxygen, BlueprintReadOnly, Category="Breathe|Breathing")
    float Oxygen = 100.0f;

    UPROPERTY(ReplicatedUsing=OnRep_BreathPhase, BlueprintReadOnly, Category="Breathe|Breathing")
    float BreathPhase = 0.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Breathe|Breathing")
    EBreatheMovementState MovementState = EBreatheMovementState::Resting;

    UPROPERTY(EditDefaultsOnly, Category="Breathe|Breathing")
    float BreathTickSeconds = 0.2f;

    FTimerHandle BreathTimerHandle;
};
