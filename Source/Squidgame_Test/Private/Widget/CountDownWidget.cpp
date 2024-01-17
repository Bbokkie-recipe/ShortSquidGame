// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CountDownWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCountDownWidget::UpdateCountdownText(int32 CountdownValue)
{
    if (CountdownText)
    {
        CountdownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CountdownValue)));
    }
}
