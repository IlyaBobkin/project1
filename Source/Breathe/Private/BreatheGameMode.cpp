#include "BreatheGameMode.h"
#include "BreatheCharacter.h"
#include "BreatheEscapeDoor.h"
#include "BreathePickup.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "WhisperTrigger.h"

ABreatheGameMode::ABreatheGameMode()
{
    DefaultPawnClass = ABreatheCharacter::StaticClass();

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        PrototypeBlockMesh = CubeMesh.Object;
    }
}

void ABreatheGameMode::StartPlay()
{
    Super::StartPlay();

    if (bBuildPrototypeCaveOnPlay)
    {
        BuildPrototypeCave();
    }
}

void ABreatheGameMode::BuildPrototypeCave()
{
    UWorld* World = GetWorld();
    if (!World || !PrototypeBlockMesh)
    {
        return;
    }

    SpawnBlock(TEXT("PrototypeCave_Floor"), FVector(650.0f, 0.0f, -60.0f), FVector(16.0f, 8.0f, 0.4f), CaveFloorMaterial);
    SpawnBlock(TEXT("PrototypeCave_Ceiling"), FVector(650.0f, 0.0f, 520.0f), FVector(16.0f, 8.0f, 0.35f), CaveWallMaterial);
    SpawnBlock(TEXT("PrototypeCave_LeftWall"), FVector(650.0f, -430.0f, 220.0f), FVector(16.0f, 0.35f, 6.0f), CaveWallMaterial);
    SpawnBlock(TEXT("PrototypeCave_RightWall"), FVector(650.0f, 430.0f, 220.0f), FVector(16.0f, 0.35f, 6.0f), CaveWallMaterial);
    SpawnBlock(TEXT("PrototypeCave_BackWall"), FVector(-180.0f, 0.0f, 220.0f), FVector(0.35f, 8.0f, 6.0f), CaveWallMaterial);

    SpawnBlock(TEXT("PrototypeCave_CenterRock"), FVector(620.0f, -110.0f, 20.0f), FVector(1.7f, 1.1f, 1.6f), CaveWallMaterial);
    SpawnBlock(TEXT("PrototypeCave_LowTunnel"), FVector(1160.0f, 0.0f, 15.0f), FVector(3.0f, 2.2f, 1.0f), CaveWallMaterial);

    APointLight* StartLight = World->SpawnActor<APointLight>(FVector(120.0f, 0.0f, 310.0f), FRotator::ZeroRotator);
    if (StartLight)
    {
        StartLight->PointLightComponent->SetIntensity(1800.0f);
        StartLight->PointLightComponent->SetAttenuationRadius(900.0f);
        StartLight->PointLightComponent->SetLightColor(FLinearColor(0.55f, 0.68f, 1.0f));
    }

    APointLight* ExitLight = World->SpawnActor<APointLight>(FVector(1320.0f, 0.0f, 260.0f), FRotator::ZeroRotator);
    if (ExitLight)
    {
        ExitLight->PointLightComponent->SetIntensity(900.0f);
        ExitLight->PointLightComponent->SetAttenuationRadius(650.0f);
        ExitLight->PointLightComponent->SetLightColor(FLinearColor(0.15f, 0.85f, 0.55f));
    }

    World->SpawnActor<ABreathePickup>(FVector(420.0f, 190.0f, 35.0f), FRotator::ZeroRotator);
    World->SpawnActor<AWhisperTrigger>(FVector(760.0f, -120.0f, 75.0f), FRotator::ZeroRotator);
    World->SpawnActor<ABreatheEscapeDoor>(FVector(1480.0f, 0.0f, 100.0f), FRotator::ZeroRotator);
}

AStaticMeshActor* ABreatheGameMode::SpawnBlock(const TCHAR* Name, const FVector& Location, const FVector& Scale, UMaterialInterface* Material) const
{
    UWorld* World = GetWorld();
    if (!World || !PrototypeBlockMesh)
    {
        return nullptr;
    }

    AStaticMeshActor* Block = World->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
    if (!Block)
    {
        return nullptr;
    }

    Block->Rename(Name);
    Block->SetReplicates(false);
    Block->GetStaticMeshComponent()->SetStaticMesh(PrototypeBlockMesh);
    Block->GetStaticMeshComponent()->SetWorldScale3D(Scale);
    Block->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
    if (Material)
    {
        Block->GetStaticMeshComponent()->SetMaterial(0, Material);
    }

    return Block;
}
