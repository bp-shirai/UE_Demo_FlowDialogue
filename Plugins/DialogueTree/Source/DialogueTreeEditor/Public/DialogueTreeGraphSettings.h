// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Types/SlateVector2.h"
#include "DialogueTreeGraphSettings.generated.h"


/**
 * 
 */
UCLASS(defaultconfig, config=Game)
class DIALOGUETREEEDITOR_API UDialogueTreeGraphSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** The maximum value to clamp the absolute value of the horizontal distance between endpoints when calculating tangents (when the wire is moving forward) */
	UPROPERTY(config, EditAnywhere, Category=Splines)
	float ForwardSplineHorizontalDeltaRange = 1000.f;

	/** The maximum value to clamp the absolute value of the vertical distance between endpoints when calculating tangents (when the wire is moving forward) */
	UPROPERTY(config, EditAnywhere, Category=Splines)
	float ForwardSplineVerticalDeltaRange = 1000.f;

	/** The amount that the horizontal delta affects the generated tangent handle of splines (when the wire is moving forward) */
	UPROPERTY(config, EditAnywhere, Category=Splines)
	FDeprecateSlateVector2D ForwardSplineTangentFromHorizontalDelta = FDeprecateSlateVector2D(1.f, 0.f);

	/** The amount that the vertical delta affects the generated tangent handle of splines (when the wire is moving forward) */
	UPROPERTY(config, EditAnywhere, Category=Splines)
	FDeprecateSlateVector2D ForwardSplineTangentFromVerticalDelta =	FDeprecateSlateVector2D(0.f, 0.f);

	/** The maximum value to clamp the absolute value of the horizontal distance between endpoints when calculating tangents (when the wire is moving backwards) */
	UPROPERTY(config, EditAnywhere, Category=Splines)
	float BackwardSplineHorizontalDeltaRange = 200.f;

	/** The maximum value to clamp the absolute value of the vertical distance between endpoints when calculating tangents (when the wire is moving backwards) */
	UPROPERTY(config, EditAnywhere, Category=Splines)
	float BackwardSplineVerticalDeltaRange = 200.f;
	
	UPROPERTY(config, EditAnywhere, Category=Splines)
	FDeprecateSlateVector2D BackwardSplineTangentFromVerticalDelta = FDeprecateSlateVector2D(0.f, 0.f);

	UPROPERTY(config, EditAnywhere, Category=Splines)
	FDeprecateSlateVector2D BackwardSplineTangentFromHorizontalDelta = FDeprecateSlateVector2D(2.f, 0.f);
};
