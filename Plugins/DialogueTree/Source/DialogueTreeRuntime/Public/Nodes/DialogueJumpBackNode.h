// 

#pragma once

#include "CoreMinimal.h"
#include "DialogueNode.h"
#include "DialogueJumpBackNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUETREERUNTIME_API UDialogueJumpBackNode : public UDialogueNode
{
	GENERATED_BODY()

public:
	virtual void EnterNode() override;
};
