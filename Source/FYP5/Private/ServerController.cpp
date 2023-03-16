//Server Controller


#include "ServerController.h"

#include "AssetImportTask.h"
#include "Hal/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Factories/ReimportDataTableFactory.h"
#include "EditorReimportHandler.h"
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
}

void AServerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AServerController::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Says Game Over"));
}



