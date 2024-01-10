// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Slider.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	s1_maxPlayers->SetMinValue(2.0f);
	s1_maxPlayers->SetMaxValue(10.0f);
	s1_maxPlayers->SetStepSize(1.0f);

}
