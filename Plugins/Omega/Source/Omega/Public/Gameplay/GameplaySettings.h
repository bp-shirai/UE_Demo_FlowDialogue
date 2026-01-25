// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "GameplaySettings.generated.h"

class UUserWidget;
class UCommonActivatableWidget;

/**
 *
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Gameplay"))
class UGameplaySettings final : public UDeveloperSettings
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(Config, EditAnywhere, Category = "Widget Reference")
    TSubclassOf<UUserWidget> InteractionWidget;

    UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "UI.Widget"))
    TMap<FGameplayTag, TSoftClassPtr<UCommonActivatableWidget>> WidgetMap;
};
