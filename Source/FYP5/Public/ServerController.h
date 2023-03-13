//Server Controller - Controls the game start and finish - 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ServerController.generated.h"
class ASpawnController;


UCLASS()
class FYP5_API AServerController : public AActor
{
	GENERATED_BODY()
	
public:	
	AServerController();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASpawnController* SpawnController;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable , Category="Event Reciever")
	void EndGame();

};
