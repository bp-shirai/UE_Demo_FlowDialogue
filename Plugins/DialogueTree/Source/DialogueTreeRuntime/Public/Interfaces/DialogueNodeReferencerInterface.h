// 

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeSocket.h"
#include "UObject/Interface.h"
#include "DialogueNodeReferencerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDialogueNodeReferencerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIALOGUETREERUNTIME_API IDialogueNodeReferencerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual TArray<UDialogueNodeSocket*> GetReferencedNodesSockets() const = 0;
	
};
