// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnController.h"

#include <xkeycheck.h>

#include "BlueSpawns.h"
#include "RedSpawns.h"
#include "Bot.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "NativeGameplayTags.h"

// Sets default values
ASpawnController::ASpawnController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TeamSize = 5;
	BotListBlue.SetNum(TeamSize);
	BotListRed.SetNum(TeamSize);

}

// Called when the game starts or when spawned
void ASpawnController::BeginPlay()
{
	Super::BeginPlay();
	RoundNumber = 0;
	SpawnAI();
}

// Called every frame
void ASpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASpawnController::SpawnAI()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlueSpawns::StaticClass(), SpawnPointBlue);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARedSpawns::StaticClass(), SpawnPointRed);

	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 200;
		
		BotListBlue[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointBlue[0]->GetActorLocation() + OffSet , SpawnPointBlue[0]->GetActorRotation());
		if(BotListBlue[i] != nullptr)
		{
			BotListBlue[i]->SetBlueTeam();
		}
	}

	
	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 200;
		BotListRed[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointRed[0]->GetActorLocation() , SpawnPointRed[0]->GetActorRotation());
		if(BotListRed[i] != nullptr)
		{
			BotListRed[i]->SetRedTeam();
		}
	}
	
	
}

void ASpawnController::CheckEnemysLeft(bool isBlue)
{
	UE_LOG(LogTemp, Warning, TEXT("has Run"));
	if(isBlue == true)
	{
		DeadCountBlue = DeadCountBlue + 1;
		if(DeadCountBlue >= TeamSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("Red Team win"));
			RedTeamWinRound = true;
			Score.X = Score.X + 1;
			ResetRound();
		}
	}
	else
	{
		DeadCountRed = DeadCountRed + 1;
		if(DeadCountRed >= TeamSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("Blue Team win"));
			BlueTeamWinRound = true;
			Score.Y = Score.Y + 1;
			ResetRound();
		}
	}
}

void ASpawnController::ResetRound()
{
	DeadCountBlue = 0;
	DeadCountRed = 0;
	RedTeamWinRound = false;
	BlueTeamWinRound = false;
	RoundNumber = RoundNumber + 1;
	
	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 200;
		BotListRed[i]->TeleportTo(SpawnPointRed[0]->GetActorLocation() + OffSet , SpawnPointRed[0]->GetActorRotation());
	}

	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 200;
		BotListBlue[i]->TeleportTo(SpawnPointBlue[0]->GetActorLocation() + OffSet , SpawnPointBlue[0]->GetActorRotation());
	}
	
}
	

