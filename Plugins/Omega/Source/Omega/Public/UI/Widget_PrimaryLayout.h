#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;
class UCommonActivatableWidgetStack;
class UCommonActivatableWidget;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class OMEGA_API UWidget_PrimaryLayout : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;

    UCommonActivatableWidgetContainerBase* FindWidgetStack(const FGameplayTag& LayerTag) const;

protected:
    UFUNCTION(BlueprintCallable)
    void RegisterWidgetStack(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* WidgetStack);

    UFUNCTION(BlueprintCallable)
    void PushWidgetToWidgetStack(FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass);

private:
    UPROPERTY(Transient)
    TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> WidgetStacks;

    UPROPERTY(meta = (BindWidget))
    UCommonActivatableWidgetStack* GameLayer_Stack;

    UPROPERTY(meta = (BindWidget))
    UCommonActivatableWidgetStack* GameMenu_Stack;

    UPROPERTY(meta = (BindWidget))
    UCommonActivatableWidgetStack* Menu_Stack;

    UPROPERTY(meta = (BindWidget))
    UCommonActivatableWidgetStack* Modal_Stack;
};
