#include "BreathePickup.h"
#include "BreatheCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"

ABreathePickup::ABreathePickup()
{
    bReplicates = true;
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetSphereRadius(120.0f);
    RootComponent = InteractionSphere;

    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    PickupMesh->SetupAttachment(RootComponent);
    PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PickupMesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        PickupMesh->SetStaticMesh(SphereMesh.Object);
    }
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
