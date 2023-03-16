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
		BlueTeamWin = true;
	}
	if(Score.Y == 7)
	{
		RedTeamWin = true;
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
	OnRoundEnd.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("SpawnerCalled End Game"));
}
	

