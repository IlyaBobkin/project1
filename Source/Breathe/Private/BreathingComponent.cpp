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
    BreathPhase = FMath::Fmod(BreathPhase + BreathTickSeconds * (MovementState == EBreatheMovementState::Panicking ? 1.6f : 1.0f), 1.0f);
    ConsumeOxygen(GetDrainRate() * BreathTickSeconds);
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

void UBreathingComponent::OnRep_Oxygen()
{
    OnOxygenChanged.Broadcast(Oxygen, MaxOxygen);
}

void UBreathingComponent::OnRep_BreathPhase()
{
    OnBreathPhaseChanged.Broadcast(BreathPhase);
}

void UBreathingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UBreathingComponent, Oxygen);
    DOREPLIFETIME(UBreathingComponent, BreathPhase);
    DOREPLIFETIME(UBreathingComponent, MovementState);
}
