// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnController.h"
#include "BlueSpawns.h"
#include "RedSpawns.h"
#include "Bot.h"
#include "Kismet/GameplayStatics.h"
#include "NativeGameplayTags.h"

// Sets default values
ASpawnController::ASpawnController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TeamSize = 5;

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

	CurrentAI = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointBlue[0]->GetActorLocation() , SpawnPointBlue[0]->GetActorRotation());
	if(CurrentAI != nullptr)
	{
		CurrentAI->SetBlueTeam();
	}

	CurrentAI = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointRed[0]->GetActorLocation() , SpawnPointRed[0]->GetActorRotation());
	if(CurrentAI != nullptr)
	{
		CurrentAI->SetRedTeam();
	}
	
}

