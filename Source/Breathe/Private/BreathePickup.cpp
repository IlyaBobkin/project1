#include "BreathePickup.h"
#include "BreatheCharacter.h"
#include "Components/SphereComponent.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

ABreathePickup::ABreathePickup()
{
    bReplicates = true;
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetSphereRadius(120.0f);
    RootComponent = InteractionSphere;
}

bool ABreathePickup::Collect(APawn* Collector)
{
    if (!HasAuthority() || bCollected)
    {
        return false;
    }

    ABreatheCharacter* BreatheCharacter = Cast<ABreatheCharacter>(Collector);
    if (!BreatheCharacter || !BreatheCharacter->InventoryComponent)
    {
        return false;
    }

    bCollected = BreatheCharacter->InventoryComponent->AddItem(ItemId, Quantity, MaxStack);
    OnRep_Collected();
    return bCollected;
}

void ABreathePickup::OnRep_Collected()
{
    SetActorHiddenInGame(bCollected);
    SetActorEnableCollision(!bCollected);
}

void ABreathePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABreathePickup, bCollected);
}
