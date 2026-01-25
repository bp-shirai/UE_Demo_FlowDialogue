#pragma once


#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_Inventory.generated.h"

class UCommonActivatableWidgetContainerBase;
class UCommonActivatableWidgetStack;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OMEGA_API UWidget_Inventory : public UCommonActivatableWidget
{
    GENERATED_BODY()

public:
    UWidget_Inventory();

};
