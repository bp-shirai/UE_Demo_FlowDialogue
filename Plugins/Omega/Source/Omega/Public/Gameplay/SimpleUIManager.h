// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"
#include "SimpleUIManager.generated.h"

namespace TAG
{
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_Game);
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_GameMenu);
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_Menu);
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Layer_Modal);

    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_PressAnyKeyScreen);
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_MainMenuScreen);
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_OptionsScreen);
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_ConfirmScreen);
    OMEGA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_KeyRemapScreen);
} // namespace TAG

enum class EAsyncPushWidgetState : uint8
{
    OnCreatedBeforePush,
    AfterPush
};

class UUserWidget;
class UCommonActivatableWidget;
class UWidget_PrimaryLayout;

/**
 *
 */
UCLASS()
class USimpleUIManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    USimpleUIManager();

    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

    static USimpleUIManager* Get(const UObject* WorldContextObject);

private:
    UPROPERTY()
    TMap<TSubclassOf<UUserWidget>, TObjectPtr<UUserWidget>> OpenedWidgets;

    UPROPERTY()
    TArray<TSubclassOf<UUserWidget>> HiddenWidgets;

public:
    void OpenWidget(const TSubclassOf<UUserWidget> WidgetClass);
    void CloseWidget(const TSubclassOf<UUserWidget> WidgetClass);
    void ToggleWidget(const TSubclassOf<UUserWidget> WidgetClass);

    void HideWidgets();
    void RestoreWidgets();

    UFUNCTION(BlueprintCallable)
    void RegisterPrimaryLayoutWidget(UWidget_PrimaryLayout* InPrimaryLayout);

    void PushSoftWidgetToStackAsync(const FGameplayTag& LayerTag, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass, TFunction<void(EAsyncPushWidgetState, UCommonActivatableWidget*)> AsyncPushStateCallback);

private:
    UPROPERTY(Transient)
    UWidget_PrimaryLayout* PrimaryLayout;
};
