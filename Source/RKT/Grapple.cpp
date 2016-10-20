// Fill out your copyright notice in the Description page of Project Settings.

#include "RKT.h"
#include "RKTPawn.h"
#include "Grapple.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"
#include <EngineUtils.h>
#include "ConstructorHelpers.h"

#define OUT // Does nothing, Defines for reader that line changes value somewhere

// Sets default values for this component's properties
UGrapple::UGrapple()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

// 
// 
// 	struct FConstructorStatics
// 	{
// 		ConstructorHelpers::FObjectFinderOptional<UBlueprint> GrappleBP;
// 		FConstructorStatics()
// 			: GrappleBP(TEXT("Blueprint'/Game/BP/Grapple_BP.Grapple_BP_C'"))
// 		{
// 		}
// 	};
// 	static FConstructorStatics ConstructorStatics;
// 	
// 	GrappleBP = CreateDefaultSubobject<UBlueprint>(TEXT("Grapple0"));
// 	

// 	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh0"));
// 	RocketMesh->SetStaticMesh(ConstructorStatics.RocketMesh.Get());
// 	RootComponent = RocketMesh;
}


// Called when the game starts
void UGrapple::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();

	SetupInputComponent();
}


// Called every frame
void UGrapple::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) { return; }
}



void UGrapple::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("GrappleLeft", IE_Pressed, this, &UGrapple::Grab);
		InputComponent->BindAction("GrappleLeft", IE_Released, this, &UGrapple::Release);
		InputComponent->BindAction("GrappleRight", IE_Pressed, this, &UGrapple::Grab);
		InputComponent->BindAction("GrappleRight", IE_Released, this, &UGrapple::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}



void UGrapple::FindPhysicsHandleComponent()
{
	///Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrapple::Grab()
{

	
 
  	FVector SpawnLocation = FVector(0.f,0.f,0.f);
  	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
  	UWorld* const World = GetWorld();
// 	
// 	




 	//static ConstructorHelpers::FObjectFinder<UBlueprint> GrappleBP(TEXT("Blueprint'/Game/BP/Grapple_BP.Grapple_BP_C'"));
 	if (World)
 	{
			
	//	World->SpawnActor<AActor>(UGrapple::StaticClass(), SpawnLocation, SpawnRotation);

 		UE_LOG(LogTemp, Warning, TEXT("Something Happened"));

 	}
	
//UBlueprint* SpawnedBP = SpawnBP<UBlueprint>(GetWorld(), GrappleBP, SpawnLocation, SpawnRotation); //bad
//	AActor* SpawnedBP = SpawnBP<AActor>(GetWorld(), BPSpawning, SpawnLocation, SpawnRotation);

// 	static ConstructorHelpers::FClassFinder<AActor> ThisResource(TEXT("/Game/BP/Grapple_BP"));
// 	if (ThisResource.Class != NULL)
// 	{
// 		UClass* GrappleBP = ThisResource.Class;
// 		AActor* GrappleSpawn = World->SpawnActor<AActor>(GrappleBP, SpawnLocation, SpawnRotation);
// 	}
	//UE_LOG(LogTemp, Warning, TEXT("Something Happened"));
	//
}

void UGrapple::Release()
{
}