#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TensionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTensionChanged, float, Tension);

UCLASS(ClassGroup=(Breathe), meta=(BlueprintSpawnableComponent))
class BREATHE_API UTensionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTensionComponent();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Breathe|Tension")
    void AddTension(float Amount);

    UFUNCTION(BlueprintCallable, Category="Breathe|Tension")
    void Calm(float Amount);

    UPROPERTY(BlueprintAssignable)
    FOnTensionChanged OnTensionChanged;

protected:
    UFUNCTION()
    void OnRep_Tension();

    UPROPERTY(ReplicatedUsing=OnRep_Tension, BlueprintReadOnly, Category="Breathe|Tension")
    float Tension = 0.0f;
};
