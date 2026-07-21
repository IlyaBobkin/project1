#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BreatheTypes.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup=(Breathe), meta=(BlueprintSpawnableComponent))
class BREATHE_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Breathe|Inventory")
    bool AddItem(FName ItemId, int32 Quantity, int32 MaxStack);

    UFUNCTION(BlueprintCallable, Category="Breathe|Inventory")
    bool RemoveItem(FName ItemId, int32 Quantity);

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Breathe|Inventory")
    TArray<FBreatheItemStack> Items;
};
