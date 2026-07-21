#include "BreatheCharacter.h"
#include "BreathingComponent.h"
#include "InventoryComponent.h"
#include "TensionComponent.h"

ABreatheCharacter::ABreatheCharacter()
{
    bReplicates = true;
    BreathingComponent = CreateDefaultSubobject<UBreathingComponent>(TEXT("BreathingComponent"));
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    TensionComponent = CreateDefaultSubobject<UTensionComponent>(TEXT("TensionComponent"));
}

void ABreatheCharacter::ServerCollectItem_Implementation(FName ItemId, int32 Quantity, int32 MaxStack)
{
    InventoryComponent->AddItem(ItemId, Quantity, MaxStack);
}
