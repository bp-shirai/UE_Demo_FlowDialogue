// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "GameFramework/PlayerController.h"
// #include "InputMappingContext.h"
#include "SimplePlayerController.generated.h"

class UInteractionComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 *
 */
UCLASS()
class ASimplePlayerController : public APlayerController
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> ExplorationContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> GenericContext;

    // Contains debug inputs, inactive in Shipping builds
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DebugContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Move;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Look;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Journal;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Interaction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> IA_Pause;


private:
    TWeakObjectPtr<class UEnhancedInputComponent> EnhancedInputComponent;

public:
    virtual void SetupInputComponent() override;
    void SetExplorationContext() const;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    virtual void PlayerTick(float DeltaTime) override;

    UFUNCTION(BlueprintImplementableEvent)
    void OnOpenJournal();

	UFUNCTION(BlueprintImplementableEvent)
    void OnPauseMenu();

private:
    TArray<TWeakObjectPtr<UInteractionComponent>> PossibleInteractions;
    TWeakObjectPtr<UInteractionComponent> ActiveInteraction;

    void OnInteractionEnter(const TWeakObjectPtr<UInteractionComponent> Interaction);
    void OnInteractionExit(const TWeakObjectPtr<UInteractionComponent> Interaction);

    void ActivateInteraction(const TWeakObjectPtr<UInteractionComponent> Interaction);
    void DeactivateInteraction();

    void OnInteractionUsed();
};
