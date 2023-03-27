// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnController.h"
#include "Bot.h"
#include "Math/Vector.h"

ASpawnController::ASpawnController()
{
 	//Primary Setup on Controller
	PrimaryActorTick.bCanEverTick = true;
	TeamSize = 5;
	BotListBlue.SetNum(TeamSize);
	BotListRed.SetNum(TeamSize);

}

void ASpawnController::BeginPlay()
{
	Super::BeginPlay();
	RoundNumber = 0;
}

void ASpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASpawnController::SpawnAI()
{
	//Spawns Bots and Keeps them in List of Each Team

	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 500;
		
		BotListBlue[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointBlue->GetActorLocation() + OffSet , SpawnPointBlue->GetActorRotation());
		if(BotListBlue[i] != nullptr)
		{
			BotListBlue[i]->SetBlueTeam();
			BotListBlue[i]->Accuracy = BlueMatchBots[i].ShotAccuracy;
			BotListBlue[i]->ReverseAccuracy = -BlueMatchBots[i].ShotAccuracy;
			BotListBlue[i]->ReactionTime = BlueMatchBots[i].ReactionTime;
			BotListBlue[i]->ReportToController = this;
			BotListBlue[i]->Name = FString::FromInt(BlueMatchBots[i].IDNum);
		}
	}

	
	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 500;
		BotListRed[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointRed->GetActorLocation() + OffSet , SpawnPointRed->GetActorRotation());
		if(BotListRed[i] != nullptr)
		{
			BotListRed[i]->SetRedTeam();
			BotListRed[i]->Accuracy = RedMatchBots[i].ShotAccuracy;
			BotListRed[i]->ReverseAccuracy = -RedMatchBots[i].ShotAccuracy;
			BotListRed[i]->ReactionTime = RedMatchBots[i].ReactionTime;
			BotListRed[i]->ReportToController = this;
			BotListRed[i]->Name = FString::FromInt(RedMatchBots[i].IDNum);
		}
	}
	
	
}
void ASpawnController::CheckEnemysLeft(bool isBlue)
{
	//Check run after Death to see if round is over
	if(isBlue == true)
	{
		DeadCountBlue = DeadCountBlue + 1;
		if(DeadCountBlue == TeamSize)
		{
			Score.X = Score.X + 1;
			ResetRound();
		}
	}
	else
	{
		DeadCountRed = DeadCountRed + 1;
		if(DeadCountRed == TeamSize)
		{
			Score.Y = Score.Y + 1;
			ResetRound();
		}
	}
}

void ASpawnController::ResetRound()
{
	//Reset Of Round Stats
	UE_LOG(LogTemp, Warning, TEXT("Round Reset"));
	DeadCountBlue = 0;
	DeadCountRed = 0;
	RoundNumber = RoundNumber + 1;
	if(RoundNumber == 6)
	{
		FVector temp = SpawnPointBlue->GetActorLocation();
		SpawnPointBlue->SetActorLocation(SpawnPointRed->GetActorLocation());
		SpawnPointRed->SetActorLocation(temp);
	}
	if(Score.X == 7)
	{
		RedTeamWin = true;
		
	}
	if(Score.Y == 7)
	{
		BlueTeamWin = true;
	}
	if(RedTeamWin == true || BlueTeamWin == true)
	{
		EndGame();
	}
	else
	{
		//Respawn the bot after all 5 die
		for ( int i = 0; i < TeamSize; i++ )
		{
			OffSet.X = i * 500;
			BotListBlue[i]->RespawnBlueBot(SpawnPointBlue->GetActorLocation()+ OffSet);
		}
		for ( int j = 0; j < TeamSize; j++ )
		{
			OffSet.X = j * 500;
			BotListRed[j]->RespawnRedBot(SpawnPointRed->GetActorLocation() + OffSet);
		}
	}

}

void ASpawnController::EndGame()
{
	TArray<int> BotIDs;
	
	for(int i = 0; i < TeamSize; i++)
	{
		if(RedTeamWin == true)
		{
			BotListRed[i]->WinString = "Win";
		}
		else
		{
			BotListRed[i]->WinString = "Lose";
		}
		BotListRed[i]->WriteBotDataToFile(); //Write Match Report
		BotIDs.Add(RedMatchBots[i].IDNum);
	}
	for(int i = 0; i < TeamSize; i++)
	{
		if(BlueTeamWin == true)
		{
			BotListBlue[i]->WinString = "Win";
		}
		else
		{
			BotListBlue[i]->WinString = "Lose";
		}
		BotListBlue[i]->WriteBotDataToFile(); //Write Match Report
		BotIDs.Add(BlueMatchBots[i].IDNum);
	}
	OnRoundEnd.Broadcast(SpawnControllerIDNum, BotIDs);
	UE_LOG(LogTemp, Warning, TEXT("SpawnerCalled End Game"));
	RestartGame();
}

void ASpawnController::RestartGame()
{
	for(int i = 0; i < TeamSize; i++)
	{
		BotListBlue[i]->Destroy();
		BotListBlue[i] = nullptr;
	}
	for(int i = 0; i < TeamSize; i++)
	{
		BotListRed[i]->Destroy();
		BotListRed[i] = nullptr;
	}
	RoundNumber = 0;
	RedMatchBots.Empty();
	BlueMatchBots.Empty();
	Score.X = 0;
	Score.Y = 0;
	
}
	

