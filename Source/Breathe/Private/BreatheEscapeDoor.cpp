#include "BreatheEscapeDoor.h"
#include "BreatheCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

ABreatheEscapeDoor::ABreatheEscapeDoor()
{
    bReplicates = true;
    SetReplicateMovement(true);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = DoorMesh;
    DoorMesh->SetRelativeScale3D(FVector(0.25f, 3.0f, 2.4f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        DoorMesh->SetStaticMesh(CubeMesh.Object);
    }
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
