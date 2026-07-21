#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BreatheTypes.h"
#include "BreathingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOxygenChanged, float, CurrentOxygen, float, MaxOxygen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBreathPhaseChanged, float, BreathPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoldBreathChanged, bool, bHoldingBreath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBreathSyncWindowChanged, bool, bInSyncWindow, float, SyncQuality);

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
    void SetHoldingBreath(bool bNewHoldingBreath);

    UFUNCTION(BlueprintCallable, Category="Breathe|Breathing")
    bool TrySynchronizeBreath(float TargetPhase, float Tolerance = 0.12f);

    UFUNCTION(BlueprintCallable, Category="Breathe|Breathing")
    void ConsumeOxygen(float Amount);

    UFUNCTION(BlueprintCallable, Category="Breathe|Breathing")
    void RefillOxygen(float Amount);

    UFUNCTION(BlueprintPure, Category="Breathe|Breathing")
    float GetOxygen() const { return Oxygen; }

    UFUNCTION(BlueprintPure, Category="Breathe|Breathing")
    float GetMaxOxygen() const { return MaxOxygen; }

    UFUNCTION(BlueprintPure, Category="Breathe|Breathing")
    float GetOxygenRatio() const { return MaxOxygen > 0.0f ? Oxygen / MaxOxygen : 0.0f; }

    UFUNCTION(BlueprintPure, Category="Breathe|Breathing")
    float GetBreathPhase() const { return BreathPhase; }

    UFUNCTION(BlueprintPure, Category="Breathe|Breathing")
    bool IsHoldingBreath() const { return bHoldingBreath; }

    UPROPERTY(BlueprintAssignable)
    FOnOxygenChanged OnOxygenChanged;

    UPROPERTY(BlueprintAssignable)
    FOnBreathPhaseChanged OnBreathPhaseChanged;

    UPROPERTY(BlueprintAssignable)
    FOnHoldBreathChanged OnHoldBreathChanged;

    UPROPERTY(BlueprintAssignable)
    FOnBreathSyncWindowChanged OnBreathSyncWindowChanged;

protected:
    UFUNCTION()
    void OnRep_Oxygen();

    UFUNCTION()
    void OnRep_BreathPhase();

    UFUNCTION()
    void OnRep_HoldingBreath();

    void TickBreath();
    float GetDrainRate() const;
    float GetBreathPhaseDistance(float TargetPhase) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Breathing")
    float MaxOxygen = 100.0f;

    UPROPERTY(ReplicatedUsing=OnRep_Oxygen, BlueprintReadOnly, Category="Breathe|Breathing")
    float Oxygen = 100.0f;

    UPROPERTY(ReplicatedUsing=OnRep_BreathPhase, BlueprintReadOnly, Category="Breathe|Breathing")
    float BreathPhase = 0.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Breathe|Breathing")
    EBreatheMovementState MovementState = EBreatheMovementState::Resting;

    UPROPERTY(ReplicatedUsing=OnRep_HoldingBreath, BlueprintReadOnly, Category="Breathe|Breathing")
    bool bHoldingBreath = false;

    UPROPERTY(EditDefaultsOnly, Category="Breathe|Breathing")
    float BreathTickSeconds = 0.2f;

    UPROPERTY(EditDefaultsOnly, Category="Breathe|Breathing")
    float HoldBreathDrainMultiplier = 0.25f;

    UPROPERTY(EditDefaultsOnly, Category="Breathe|Breathing")
    float HoldBreathPanicThreshold = 18.0f;

    FTimerHandle BreathTimerHandle;
};
