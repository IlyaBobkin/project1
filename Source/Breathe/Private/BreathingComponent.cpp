#include "BreathingComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

UBreathingComponent::UBreathingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UBreathingComponent::BeginPlay()
{
    Super::BeginPlay();
    Oxygen = MaxOxygen;

    if (GetOwner() && GetOwner()->HasAuthority())
    {
        GetWorld()->GetTimerManager().SetTimer(BreathTimerHandle, this, &UBreathingComponent::TickBreath, BreathTickSeconds, true);
    }
}

void UBreathingComponent::SetMovementState(EBreatheMovementState NewState)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        MovementState = NewState;
    }
}

void UBreathingComponent::SetHoldingBreath(bool bNewHoldingBreath)
{
    if (GetOwner() && GetOwner()->HasAuthority() && bHoldingBreath != bNewHoldingBreath)
    {
        bHoldingBreath = bNewHoldingBreath;
        OnRep_HoldingBreath();
    }
}

bool UBreathingComponent::TrySynchronizeBreath(float TargetPhase, float Tolerance)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return false;
    }

    const float Distance = GetBreathPhaseDistance(TargetPhase);
    const bool bInWindow = Distance <= FMath::Clamp(Tolerance, 0.01f, 0.5f);
    const float SyncQuality = bInWindow ? 1.0f - (Distance / Tolerance) : 0.0f;
    OnBreathSyncWindowChanged.Broadcast(bInWindow, SyncQuality);

    if (bInWindow)
    {
        RefillOxygen(2.5f * SyncQuality);
    }

    return bInWindow;
}

void UBreathingComponent::ConsumeOxygen(float Amount)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        Oxygen = FMath::Clamp(Oxygen - Amount, 0.0f, MaxOxygen);
        OnRep_Oxygen();
    }
}

void UBreathingComponent::RefillOxygen(float Amount)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        Oxygen = FMath::Clamp(Oxygen + Amount, 0.0f, MaxOxygen);
        OnRep_Oxygen();
    }
}

void UBreathingComponent::TickBreath()
{
    const float PhaseSpeed = MovementState == EBreatheMovementState::Panicking ? 1.6f : (bHoldingBreath ? 0.35f : 1.0f);
    BreathPhase = FMath::Fmod(BreathPhase + BreathTickSeconds * PhaseSpeed, 1.0f);

    const float DrainMultiplier = bHoldingBreath && Oxygen > HoldBreathPanicThreshold ? HoldBreathDrainMultiplier : 1.0f;
    ConsumeOxygen(GetDrainRate() * BreathTickSeconds * DrainMultiplier);
    OnRep_BreathPhase();
}

float UBreathingComponent::GetDrainRate() const
{
    switch (MovementState)
    {
    case EBreatheMovementState::Walking: return 0.35f;
    case EBreatheMovementState::Running: return 0.85f;
    case EBreatheMovementState::Panicking: return 1.2f;
    case EBreatheMovementState::Resting:
    default: return 0.15f;
    }
}

float UBreathingComponent::GetBreathPhaseDistance(float TargetPhase) const
{
    const float WrappedTarget = FMath::Fmod(TargetPhase + 1.0f, 1.0f);
    const float Direct = FMath::Abs(BreathPhase - WrappedTarget);
    return FMath::Min(Direct, 1.0f - Direct);
}

void UBreathingComponent::OnRep_Oxygen()
{
    OnOxygenChanged.Broadcast(Oxygen, MaxOxygen);
}

void UBreathingComponent::OnRep_BreathPhase()
{
    OnBreathPhaseChanged.Broadcast(BreathPhase);
}

void UBreathingComponent::OnRep_HoldingBreath()
{
    OnHoldBreathChanged.Broadcast(bHoldingBreath);
}

void UBreathingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UBreathingComponent, Oxygen);
    DOREPLIFETIME(UBreathingComponent, BreathPhase);
    DOREPLIFETIME(UBreathingComponent, MovementState);
    DOREPLIFETIME(UBreathingComponent, bHoldingBreath);
}
