#include "BreatheCharacter.h"
#include "BreathingComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "InventoryComponent.h"
#include "TensionComponent.h"

ABreatheCharacter::ABreatheCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    BreathingComponent = CreateDefaultSubobject<UBreathingComponent>(TEXT("BreathingComponent"));
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    TensionComponent = CreateDefaultSubobject<UTensionComponent>(TEXT("TensionComponent"));

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABreatheCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }
}

void ABreatheCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateBreathingMovementState();
}

void ABreatheCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABreatheCharacter::Move);
        }
        if (LookAction)
        {
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABreatheCharacter::Look);
        }
        if (SprintAction)
        {
            EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ABreatheCharacter::StartSprint);
            EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABreatheCharacter::StopSprint);
        }
        if (HoldBreathAction)
        {
            EnhancedInput->BindAction(HoldBreathAction, ETriggerEvent::Started, this, &ABreatheCharacter::StartHoldBreath);
            EnhancedInput->BindAction(HoldBreathAction, ETriggerEvent::Completed, this, &ABreatheCharacter::StopHoldBreath);
        }
        if (SyncBreathAction)
        {
            EnhancedInput->BindAction(SyncBreathAction, ETriggerEvent::Started, this, &ABreatheCharacter::SynchronizeBreath);
        }
    }
}

void ABreatheCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ABreatheCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void ABreatheCharacter::StartSprint()
{
    bWantsToSprint = true;
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    ServerSetSprinting(true);
}

void ABreatheCharacter::StopSprint()
{
    bWantsToSprint = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    ServerSetSprinting(false);
}

void ABreatheCharacter::StartHoldBreath()
{
    ServerSetHoldingBreath(true);
}

void ABreatheCharacter::StopHoldBreath()
{
    ServerSetHoldingBreath(false);
}

void ABreatheCharacter::SynchronizeBreath()
{
    ServerTrySynchronizeBreath(0.0f);
}

void ABreatheCharacter::UpdateBreathingMovementState() const
{
    if (!HasAuthority() || !BreathingComponent)
    {
        return;
    }

    const bool bIsMoving = GetVelocity().SizeSquared2D() > 25.0f;
    BreathingComponent->SetMovementState(bIsMoving ? (bWantsToSprint ? EBreatheMovementState::Running : EBreatheMovementState::Walking) : EBreatheMovementState::Resting);
}

void ABreatheCharacter::ServerCollectItem_Implementation(FName ItemId, int32 Quantity, int32 MaxStack)
{
    InventoryComponent->AddItem(ItemId, Quantity, MaxStack);
}

void ABreatheCharacter::ServerSetSprinting_Implementation(bool bNewWantsToSprint)
{
    bWantsToSprint = bNewWantsToSprint;
    GetCharacterMovement()->MaxWalkSpeed = bWantsToSprint ? SprintSpeed : WalkSpeed;
}

void ABreatheCharacter::ServerSetHoldingBreath_Implementation(bool bNewHoldingBreath)
{
    BreathingComponent->SetHoldingBreath(bNewHoldingBreath);
}

void ABreatheCharacter::ServerTrySynchronizeBreath_Implementation(float TargetPhase)
{
    BreathingComponent->TrySynchronizeBreath(TargetPhase);
}
