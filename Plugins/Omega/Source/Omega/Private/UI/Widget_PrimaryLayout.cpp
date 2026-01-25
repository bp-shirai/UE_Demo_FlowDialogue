
#include "UI/Widget_PrimaryLayout.h"

#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"

#include "Gameplay/SimpleUIManager.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStack(const FGameplayTag& LayerTag) const
{
    checkf(WidgetStacks.Contains(LayerTag), TEXT("Can not find the widget stack by the tag %s"), *LayerTag.ToString());

    return WidgetStacks.FindRef(LayerTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* WidgetStack)
{
    if (!WidgetStacks.Contains(LayerTag))
    {
        WidgetStacks.Add(LayerTag, WidgetStack);
    }
}

void UWidget_PrimaryLayout::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    RegisterWidgetStack(TAG::UI_Layer_Game, GameLayer_Stack);
    RegisterWidgetStack(TAG::UI_Layer_GameMenu, GameMenu_Stack);
    RegisterWidgetStack(TAG::UI_Layer_Menu, Menu_Stack);
    RegisterWidgetStack(TAG::UI_Layer_Modal, Modal_Stack);
}

void UWidget_PrimaryLayout::PushWidgetToWidgetStack(FGameplayTag LayerTag, TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass)
{
    if (UCommonActivatableWidgetContainerBase* WidgetStack = FindWidgetStack(LayerTag))
    {
        WidgetStack->AddWidget(ActivatableWidgetClass);
    }
}
