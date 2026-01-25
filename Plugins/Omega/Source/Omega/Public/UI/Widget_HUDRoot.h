#pragma once


#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_HUDRoot.generated.h"

class UCommonActivatableWidgetContainerBase;
class UCommonActivatableWidgetStack;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OMEGA_API UWidget_HUDRoot : public UCommonActivatableWidget
{
    GENERATED_BODY()

public:
    UWidget_HUDRoot();

};
