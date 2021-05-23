// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    InitGame(); 

    PrintLine(TEXT("The HiddenWord is : %s"), *HiddenWord); 
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else
    {
        if(Input==HiddenWord)
        {
            PrintLine(TEXT("Youy have Won!"));
            EndGame();
        }
        else
        {
            PrintLine(TEXT("Lost a life!"));
            PrintLine(TEXT("%i"), --Lives);
            if(Lives)
            {
                if(Input.Len() != HiddenWord.Len())
                {
                    PrintLine(TEXT("Sorry, try guessing again.\nYou have %i Lives left"), Lives);
                }
            }
            else
            {
                PrintLine(TEXT("You have no lives left!"));
                EndGame();
            }
        }
    }  
}

void UBullCowCartridge::InitGame()
{
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = TEXT("cake");
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter Word"), HiddenWord.Len());
    PrintLine(TEXT("Type your guess.\nPress Enter to continue...")); 
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again"));
}