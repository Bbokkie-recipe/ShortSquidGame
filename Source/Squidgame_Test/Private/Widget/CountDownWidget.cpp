// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CountDownWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCountDownWidget::UpdateCountdownText(int32 CountdownValue)
{
    if (CountdownText)
    {
        if(CountdownValue == 0){
            CountdownText->SetText(FText::FromString(TEXT("Time over!")));
        }
        else {
            int32 Minutes = (CountdownValue % 3600) / 60;
            int32 Seconds = CountdownValue % 60;

            FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
            CountdownText->SetText(FText::FromString(FormattedTime));
            //CountdownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CountdownValue)));
        }
    }
}
