//Server Controller


#include "ServerController.h"

#include "SpawnController.h"
#include "Kismet/GameplayStatics.h"

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
}

void AServerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AServerController::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Says Game Over"));
}


