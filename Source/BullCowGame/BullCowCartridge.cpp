// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word) { return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word); });

    InitGame();
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
        ProcessGuess(Input);
        
    }  
}

void UBullCowCartridge::InitGame()
{
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    Lives = HiddenWord.Len()*2;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter Word"), HiddenWord.Len());
    // PrintLine(TEXT("The Hiddenword is : %s"), *HiddenWord);
    PrintLine(TEXT("You have %i Lives."), Lives);
    PrintLine(TEXT("Type your guess and Press Enter")); 
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess==HiddenWord)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }
    
    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again.\nYou have %i Lives left"), Lives);
        return;
    }

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    PrintLine(TEXT("Lost a life!"));
    Lives--;

    if(!Lives)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was : %s"), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bull and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for(int32 i=0;i<Word.Len()-1;i++)
    {
        for(int32 j=i+1;j<Word.Len();j++)
        {
            if(Word[i]==Word[j])
            return false;
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for(FString Word : WordList)
    {
        if(IsIsogram(Word)&&Word.Len()>=4&&Word.Len()<=8)
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    
    for(int i=0;i<Guess.Len();i++)
    {
        if(Guess[i] == HiddenWord[i])
        {
            Count.Bulls++;
            continue;
        }

        for(int32 j=0; j<HiddenWord.Len();j++)
        {
            if(Guess[i]  == HiddenWord[j])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}