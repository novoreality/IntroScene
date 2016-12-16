// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot_Sync.h"
#include "RobotSync.h"
#include <string>

// Sets default values
ARobotSync::ARobotSync()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobotSync::BeginPlay()
{
	Super::BeginPlay();

	ConnectSocket();

	SendReadyStatus();
}

// Called every frame
void ARobotSync::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	ReadData();
}

void ARobotSync::ConnectSocket()
{
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool IsValid = true;
	RemoteAddress->SetIp(TEXT("192.168.1.10"), IsValid);
	RemoteAddress->SetPort(81);
	OrchConn = FTcpSocketBuilder(FString("Socket client to connect to orchestrator server")).AsNonBlocking().Build();
	OrchConn->Connect(*RemoteAddress);
}

void ARobotSync::SendReadyStatus() 
{	
	TCHAR *Status = TEXT("ready");
	int32 size = FCString::Strlen(Status);
	int32 sent = 0;
	bool SentStatus = false;
	while (!SentStatus) {
		SentStatus = OrchConn->Send((uint8*)TCHAR_TO_UTF8(Status), size, sent);
	}
	
}

void ARobotSync::ReadData()
{
	uint8 Buffer[512] = { 0 };
	int32 BytesRead;
	const FString StartString = TEXT("start");

	if (OrchConn->Recv(Buffer, sizeof(Buffer), BytesRead))
	{
		FString SBuffer = FString(UTF8_TO_TCHAR(Buffer));

		if (SBuffer.Compare(StartString, ESearchCase::CaseSensitive) == 0) 
		{
			OnStart();
			return;
		}
	};
}

void ARobotSync::OnStart()
{
	UE_LOG(LogTemp, Warning, TEXT("ON START!!!"));
	OnRobotSyncStart.Broadcast();
}

