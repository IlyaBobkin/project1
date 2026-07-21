#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BreatheCharacter.generated.h"

class UBreathingComponent;
class UInputAction;
class UInputMappingContext;
class UInventoryComponent;
class UTensionComponent;
struct FInputActionValue;

UCLASS()
class BREATHE_API ABreatheCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABreatheCharacter();

    virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe")
    TObjectPtr<UBreathingComponent> BreathingComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe")
    TObjectPtr<UInventoryComponent> InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Breathe")
    TObjectPtr<UTensionComponent> TensionComponent;

    UFUNCTION(Server, Reliable, BlueprintCallable, Category="Breathe|Interaction")
    void ServerCollectItem(FName ItemId, int32 Quantity, int32 MaxStack);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category="Breathe|Movement")
    void ServerSetSprinting(bool bNewWantsToSprint);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category="Breathe|Breathing")
    void ServerSetHoldingBreath(bool bNewHoldingBreath);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category="Breathe|Breathing")
    void ServerTrySynchronizeBreath(float TargetPhase);

protected:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartSprint();
    void StopSprint();
    void StartHoldBreath();
    void StopHoldBreath();
    void SynchronizeBreath();
    void UpdateBreathingMovementState() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Input")
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Input")
    TObjectPtr<UInputAction> HoldBreathAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Input")
    TObjectPtr<UInputAction> SyncBreathAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Movement")
    float WalkSpeed = 320.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Breathe|Movement")
    float SprintSpeed = 560.0f;

    bool bWantsToSprint = false;
};
