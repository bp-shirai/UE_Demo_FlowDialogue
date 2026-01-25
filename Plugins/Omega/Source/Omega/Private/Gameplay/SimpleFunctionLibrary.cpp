// Vince Petrelli All Rights Reserved

#include "Gameplay/SimpleFunctionLibrary.h"
#include "Gameplay/GameplaySettings.h"

TSoftClassPtr<UCommonActivatableWidget> USimpleFunctionLibrary::GetSoftWidgetClassByTag(UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag InWidgetTag)
{
    const UGameplaySettings* GameplaySettings = GetDefault<UGameplaySettings>();

    checkf(GameplaySettings->WidgetMap.Contains(InWidgetTag), TEXT("Could not find the corresponding widget under the tag %s"), *InWidgetTag.ToString());

    return GameplaySettings->WidgetMap.FindRef(InWidgetTag);
}

// TSoftObjectPtr<UTexture2D> UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FGameplayTag InImageTag)
// {
// 	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

// 	checkf(FrontendDeveloperSettings->OptionsScreenSoftImageMap.Contains(InImageTag),TEXT("Could not find an image accociated with tag %s"),*InImageTag.ToString());

// 	return FrontendDeveloperSettings->OptionsScreenSoftImageMap.FindRef(InImageTag);

// }