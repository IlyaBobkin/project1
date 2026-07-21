#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BreatheGameMode.generated.h"

class AStaticMeshActor;
class UMaterialInterface;
class UStaticMesh;

UCLASS()
class BREATHE_API ABreatheGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABreatheGameMode();

    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Prototype")
    bool bBuildPrototypeCaveOnPlay = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Prototype")
    TObjectPtr<UStaticMesh> PrototypeBlockMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Prototype")
    TObjectPtr<UMaterialInterface> CaveWallMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Prototype")
    TObjectPtr<UMaterialInterface> CaveFloorMaterial;

    void BuildPrototypeCave();
    AStaticMeshActor* SpawnBlock(const TCHAR* Name, const FVector& Location, const FVector& Scale, UMaterialInterface* Material) const;
};
