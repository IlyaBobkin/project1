#include "WhisperTrigger.h"
#include "BreatheCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TensionComponent.h"

AWhisperTrigger::AWhisperTrigger()
{
    bReplicates = false;
    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
    TriggerSphere->SetSphereRadius(350.0f);
    TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = TriggerSphere;
    TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AWhisperTrigger::HandleBeginOverlap);
}

void AWhisperTrigger::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bTriggerOnce && TriggeredActors.Contains(OtherActor))
    {
        return;
    }

    ABreatheCharacter* BreatheCharacter = Cast<ABreatheCharacter>(OtherActor);
    if (!BreatheCharacter)
    {
        return;
    }

    TriggeredActors.Add(OtherActor);
    if (BreatheCharacter->HasAuthority() && BreatheCharacter->TensionComponent)
    {
        BreatheCharacter->TensionComponent->AddTension(TensionAmount);
    }

    if (WhisperSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, WhisperSound, GetActorLocation());
    }
}
