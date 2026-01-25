// Vince Petrelli All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "AsyncAction_PushSoftWidget.generated.h"

class UCommonActivatableWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UCommonActivatableWidget*, PushedWidget);

/**
 *
 */
UCLASS()
class OMEGA_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack"))
    static UAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UCommonActivatableWidget> InSoftWidgetClass, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag InLayerTag, bool bFocusOnNewlyPushedWidget = true);

    //~ Begin UBlueprintAsyncActionBase Interface
    virtual void Activate() override;
    //~ End UBlueprintAsyncActionBase Interface

    UPROPERTY(BlueprintAssignable)
    FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

    UPROPERTY(BlueprintAssignable)
    FOnPushSoftWidgetDelegate AfterPush;

private:
    TWeakObjectPtr<UWorld> CachedOwningWorld;
    TWeakObjectPtr<APlayerController> CachedOwningPC;
    TSoftClassPtr<UCommonActivatableWidget> CachedSoftWidgetClass;
    FGameplayTag CachedWidgetStackTag;
    bool bCachedFocusOnNewlyPushedWidget = false;
};
