// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Networking.h"
#include <string.h>
#include "GameFramework/Actor.h"
#include "RobotSync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRobotSyncDelegate);

UCLASS()
class ROBOT_SYNC_API ARobotSync : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARobotSync();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(BlueprintAssignable, Category = "RobotSync")
		FRobotSyncDelegate OnRobotSyncStart;

private:

	void OnStart();

	void ConnectSocket();

	void SendReadyStatus();

	void ReadData();

	FSocket *OrchConn;

};
