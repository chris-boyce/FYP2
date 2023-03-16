//Server Controller


#include "ServerController.h"

#include "AssetImportTask.h"
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
	AActor* temp = UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnController::StaticClass());
	SpawnController = Cast<ASpawnController>(temp);
	SpawnController->OnRoundEnd.AddDynamic(this, &AServerController::EndGame);
	BotStatsWriteDataTableToArray();
	BotStatsWriteArrayToMap();
	ServerWriteDataTableToArray();
	ServerWriteArrayToMap();

	CreateMatch();
}

void AServerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AServerController::CreateMatch()
{
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
}

void AServerController::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Says Game Over"));
}



