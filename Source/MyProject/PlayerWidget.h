// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HelthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Helth_Text;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UsePotion(bool isUsing);
public:
	void SetHelth(float cur_helth, float max);
	void SetText(float cur_helth);

};
