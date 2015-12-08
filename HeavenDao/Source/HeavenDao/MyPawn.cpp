// Fill out your copyright notice in the Description page of Project Settings.

#include "HeavenDao.h"
#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 将此pawn设置为由最低数量的玩家进行控制
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// 创建一个可供添加对象的空根组件。
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// 创建相机和可见项目
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	// 附加相机和可见对象到根组件。 偏移并旋转相机。
	OurCamera->AttachTo(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	OurVisibleComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// 基于"Grow"操作来处理增长和收缩
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
		if (bGrowing)
		{
			//  在一秒的时间内增长到两倍的大小
			CurrentScale += DeltaTime;
		}
		else
		{
			// 随着增长收缩到一半
			CurrentScale -= (DeltaTime * 0.5f);
		}
		// 确认永不低于起始大小，或增大之前的两倍大小。
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
	}
	// 基于"MoveX"和 "MoveY"坐标轴来处理移动
	{
		if (!CurrentVelocity.IsZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// 在按下或松开"Grow"键时进行响应。
	InputComponent->BindAction("Grow", IE_Pressed, this, &AMyPawn::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AMyPawn::StopGrowing);

	// 在每一帧都对两个移动坐标轴的值进行响应，它们分别是"MoveX"和"MoveY"。
	InputComponent->BindAxis("MoveX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyPawn::Move_YAxis);
}

void AMyPawn::Move_XAxis(float AxisValue)
{
	// 以每秒100单位的速度向前或向后移动
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyPawn::Move_YAxis(float AxisValue)
{
	// 以每秒100单位的速度向右或向左移动
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyPawn::StartGrowing()
{
	bGrowing = true;
}

void AMyPawn::StopGrowing()
{
	bGrowing = false;
}

