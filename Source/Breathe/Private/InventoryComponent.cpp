#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

bool UInventoryComponent::AddItem(FName ItemId, int32 Quantity, int32 MaxStack)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || ItemId.IsNone() || Quantity <= 0)
    {
        return false;
    }

    for (FBreatheItemStack& Stack : Items)
    {
        if (Stack.ItemId == ItemId && Stack.Quantity < Stack.MaxStack)
        {
            const int32 Added = FMath::Min(Quantity, Stack.MaxStack - Stack.Quantity);
            Stack.Quantity += Added;
            Quantity -= Added;
        }
    }

    while (Quantity > 0)
    {
        FBreatheItemStack NewStack;
        NewStack.ItemId = ItemId;
        NewStack.MaxStack = FMath::Max(1, MaxStack);
        NewStack.Quantity = FMath::Min(Quantity, NewStack.MaxStack);
        Items.Add(NewStack);
        Quantity -= NewStack.Quantity;
    }

    return true;
}

bool UInventoryComponent::RemoveItem(FName ItemId, int32 Quantity)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || Quantity <= 0)
    {
        return false;
    }

    for (int32 Index = Items.Num() - 1; Index >= 0 && Quantity > 0; --Index)
    {
        FBreatheItemStack& Stack = Items[Index];
        if (Stack.ItemId == ItemId)
        {
            const int32 Removed = FMath::Min(Quantity, Stack.Quantity);
            Stack.Quantity -= Removed;
            Quantity -= Removed;
            if (Stack.Quantity <= 0)
            {
                Items.RemoveAt(Index);
            }
        }
    }

    return Quantity == 0;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, Items);
}
