// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "SimpleFunctionLibrary.generated.h"

class UCommonActivatableWidget;

/**
 *
 */
UCLASS()
class OMEGA_API USimpleFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
    static TSoftClassPtr<UCommonActivatableWidget> GetSoftWidgetClassByTag(UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag InWidgetTag);

    //	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
    //	static TSoftObjectPtr<UTexture2D> GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "UI.Image")) FGameplayTag InImageTag);
};
