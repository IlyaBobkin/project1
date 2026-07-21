#include "BreatheEscapeDoor.h"
#include "BreatheCharacter.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

ABreatheEscapeDoor::ABreatheEscapeDoor()
{
    bReplicates = true;
    SetReplicateMovement(true);
}

bool ABreatheEscapeDoor::TryOpen(APawn* InstigatorPawn)
{
    if (!HasAuthority() || bOpen)
    {
        return false;
    }

    ABreatheCharacter* BreatheCharacter = Cast<ABreatheCharacter>(InstigatorPawn);
    if (RequiredQuantity > 0)
    {
        if (!BreatheCharacter || !BreatheCharacter->InventoryComponent || !BreatheCharacter->InventoryComponent->RemoveItem(RequiredItemId, RequiredQuantity))
        {
            return false;
        }
    }

    bOpen = true;
    OnRep_Open();
    return true;
}

void ABreatheEscapeDoor::OnRep_Open()
{
    SetActorEnableCollision(!bOpen);
    BP_OnDoorOpened();
}

void ABreatheEscapeDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABreatheEscapeDoor, bOpen);
}
