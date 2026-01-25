// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors

#include "Gameplay/SimpleUIManager.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CommonActivatableWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/GameInstance.h"
#include "Templates/Casts.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectHash.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "UI/Widget_PrimaryLayout.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleUIManager)

namespace TAG
{
    UE_DEFINE_GAMEPLAY_TAG(UI_Layer_Game, "UI.Layer.Game");
    UE_DEFINE_GAMEPLAY_TAG(UI_Layer_GameMenu, "UI.Layer.GameMenu");
    UE_DEFINE_GAMEPLAY_TAG(UI_Layer_Menu, "UI.Layer.Menu");
    UE_DEFINE_GAMEPLAY_TAG(UI_Layer_Modal, "UI.Layer.Modal");

    UE_DEFINE_GAMEPLAY_TAG(UI_Widget_PressAnyKeyScreen, "UI.Widget.PressAnyKeyScreen");
    UE_DEFINE_GAMEPLAY_TAG(UI_Widget_MainMenuScreen, "UI.Widget.MainMenuScreen");
    UE_DEFINE_GAMEPLAY_TAG(UI_Widget_OptionsScreen, "UI.Widget.OptionsScreen");
    UE_DEFINE_GAMEPLAY_TAG(UI_Widget_ConfirmScreen, "UI.Widget.ConfirmScreen");
    UE_DEFINE_GAMEPLAY_TAG(UI_Widget_KeyRemapScreen, "UI.Widget.KeyRemapScreen");
} // namespace TAG

USimpleUIManager::USimpleUIManager()
{
}

void USimpleUIManager::OpenWidget(const TSubclassOf<UUserWidget> WidgetClass)
{
    if (WidgetClass == nullptr || WidgetClass->HasAnyClassFlags(CLASS_Abstract) || OpenedWidgets.Contains(WidgetClass))
    {
        return;
    }

    UUserWidget* Widget = Cast<UUserWidget>(UWidgetBlueprintLibrary::Create(GetWorld(), *WidgetClass, GetWorld()->GetFirstPlayerController()));
    OpenedWidgets.Add(WidgetClass, Widget);
    Widget->AddToViewport();
}

void USimpleUIManager::CloseWidget(const TSubclassOf<UUserWidget> WidgetClass)
{
    if (UUserWidget* Widget = OpenedWidgets.FindRef(WidgetClass))
    {
        Widget->RemoveFromParent();
        OpenedWidgets.Remove(WidgetClass);
    }
}

void USimpleUIManager::ToggleWidget(const TSubclassOf<UUserWidget> WidgetClass)
{
    if (OpenedWidgets.Contains(WidgetClass))
    {
        CloseWidget(WidgetClass);
    }
    else
    {
        OpenWidget(WidgetClass);
    }
}

void USimpleUIManager::HideWidgets()
{
    for (auto WidgetIt = OpenedWidgets.CreateIterator(); WidgetIt; ++WidgetIt)
    {
        const TPair<TSubclassOf<UUserWidget>, UUserWidget*> Widget = *WidgetIt;

        HiddenWidgets.Add(Widget.Key);
        CloseWidget(Widget.Key);
    }
}

void USimpleUIManager::RestoreWidgets()
{
    for (const TSubclassOf<UUserWidget>& WidgetClass : HiddenWidgets)
    {
        OpenWidget(WidgetClass);
    }
    HiddenWidgets.Empty();
}

void USimpleUIManager::PushSoftWidgetToStackAsync(const FGameplayTag& LayerTag, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass, TFunction<void(EAsyncPushWidgetState, UCommonActivatableWidget*)> AsyncPushStateCallback)
{
    check(!ActivatableWidgetClass.IsNull());

    UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
        ActivatableWidgetClass.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this, LayerTag, ActivatableWidgetClass, AsyncPushStateCallback]()
            {
                UClass* LoadedWidgetClass = ActivatableWidgetClass.Get();
                check(LoadedWidgetClass && PrimaryLayout);

                UCommonActivatableWidgetContainerBase* FoundWidgetStack = PrimaryLayout->FindWidgetStack(LayerTag);

                UCommonActivatableWidget* CreatedWidget = FoundWidgetStack->AddWidget<UCommonActivatableWidget>(
                    LoadedWidgetClass,
                    [AsyncPushStateCallback](UCommonActivatableWidget& CreatedWidgetInstance)
                    {
                        AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
                    });

                AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
            }));
}

void USimpleUIManager::RegisterPrimaryLayoutWidget(UWidget_PrimaryLayout* InPrimaryLayout)
{
    check(InPrimaryLayout);

    PrimaryLayout = InPrimaryLayout;
}

USimpleUIManager* USimpleUIManager::Get(const UObject* WorldContextObject)
{
    if (GEngine)
    {
        UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
        return UGameInstance::GetSubsystem<USimpleUIManager>(World->GetGameInstance());
    }

    return nullptr;
}

bool USimpleUIManager::ShouldCreateSubsystem(UObject* Outer) const
{
    if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
    {
        TArray<UClass*> FoundClasses;
        GetDerivedClasses(GetClass(), FoundClasses);
        return FoundClasses.IsEmpty();
    }

    return false;
}
