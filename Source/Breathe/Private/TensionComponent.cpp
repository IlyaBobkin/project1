#include "TensionComponent.h"
#include "Net/UnrealNetwork.h"

UTensionComponent::UTensionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UTensionComponent::AddTension(float Amount)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        Tension = FMath::Clamp(Tension + Amount, 0.0f, 1.0f);
        OnRep_Tension();
    }
}

void UTensionComponent::Calm(float Amount)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        Tension = FMath::Clamp(Tension - Amount, 0.0f, 1.0f);
        OnRep_Tension();
    }
}

void UTensionComponent::OnRep_Tension()
{
    OnTensionChanged.Broadcast(Tension);
}

void UTensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UTensionComponent, Tension);
}
