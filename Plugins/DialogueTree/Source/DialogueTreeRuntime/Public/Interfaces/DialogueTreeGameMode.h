// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "DialogueTreeGameMode.generated.h"

class ADialogueController;
// This class does not need to be modified.
UINTERFACE()
class UDialogueTreeGameMode : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUETREERUNTIME_API IDialogueTreeGameMode
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual TSubclassOf<ADialogueController> GetDialogueControllerClass() const = 0;
	virtual FText GetSpeechParameter(FGameplayTag SpeechParameterTag) const = 0;
};
