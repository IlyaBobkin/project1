#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WhisperTrigger.generated.h"

class USphereComponent;
class USoundBase;

UCLASS()
class BREATHE_API AWhisperTrigger : public AActor
{
    GENERATED_BODY()

public:
    AWhisperTrigger();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe|Whisper")
    TObjectPtr<USphereComponent> TriggerSphere;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Whisper")
    TObjectPtr<USoundBase> WhisperSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Whisper", meta=(ClampMin="0.0", ClampMax="1.0"))
    float TensionAmount = 0.22f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Breathe|Whisper")
    bool bTriggerOnce = true;

    TSet<TWeakObjectPtr<AActor>> TriggeredActors;

    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
