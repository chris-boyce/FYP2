//Server Controller


#include "ServerController.h"

#include "SpawnController.h"
#include "Kismet/GameplayStatics.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

AServerController::AServerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AServerController::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnController::StaticClass(), ActorSpawnController);
	for(int i = 0; i < ActorSpawnController.Num(); i++)
	{
		FServerStatus temps;
		temps.SpawnController = ActorSpawnController[i];
		temps.IsActive = false;
		ServerStatus.Add(i, temps);
	}
	
	BotStatsWriteDataTableToArray();
	BotStatsWriteArrayToMap();
	ServerWriteDataTableToArray();
	ServerWriteArrayToMap();

	

	CreateMatch(0);
	CreateMatch(1);
}

void AServerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AServerController::CreateMatch(int ServerToConnect)
{
	//*TODO --- Set up RedLobbyBots and BlueLobbyBots --- Add Server Picking --- *//   
	SpawnController = nullptr;
	SpawnController = Cast<ASpawnController>(ServerStatus[ServerToConnect].SpawnController);
	ServerStatus[ServerToConnect].IsActive = true;
	SpawnController->OnRoundEnd.AddDynamic(this, &AServerController::EndGame);
	
	for(int i = 0; i < RedLobbyBot.Num(); i++)
	{
		LocalBotData = BotStatsMap[RedLobbyBot[i]];
		SpawnController->RedMatchBots.Add(i, LocalBotData );
	}

	for(int i = 0; i < BlueLobbyBot.Num(); i++)
	{
		LocalBotData = BotStatsMap[BlueLobbyBot[i]];
		SpawnController->BlueMatchBots.Add(i, LocalBotData );
	}
	SpawnController->SpawnAI();
}

void AServerController::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Says Game Over"));
}



