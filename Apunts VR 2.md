# Light Painter
# Learning Unreal VR from C++ (2nd part Painter -UI - Save)
---
## User Interface Section Introduction

- Ergonimics in VR (confortable)
- Design process (Interface)
- Radial menus (VR Interface)
- UI selection (How to select elements in VR)

We construct a pannel with captures and buttons UI

A Radial menu in a leftController
and save our 3D draws (SaveGame system in Unreal)

Setup The Base Project

- C++
- As minimal as possible

---
## Creating a VR Pawn Setup

The Pawn Setup

VRPawn

HandController

VPawn has 2 HandController, both are actors

Create BP_VRPawn from VRPawn

Create BP_HandController frem HandController

Create The Pawn Setup
- Pawn and Controller are separate actors
- The controller is a child of the pawn
- Hand controller and heas tracking works
- Both pawn and controller have C++ and BP
- Right hand only is spawned(for now)
- The correct hand controller is shown

*Remember*
```
In file VR_Paint_UI.Build.cs
In PublicDependencyModuleNames.AddRange add "HeadMountedDisplay"
```
---
## Binding Actions To Motion Controllers
Painting Architecture

Draw in 3D space spawning actors.

Each stroke will be an actor itself. Where go to press button and start to paint. Uptdating will spawn control points and finally releasing button will stop process.

- Create an Actor c++ class named Stroke.
- Create a BP class from this Stroke.
- Add a mesh to this actor in order to test behaviour in this lesson.

We want to spawn Stroke

Forward the Input
- Spawn a Stroke on trigger.
- Atention: Trigger is handled in Hand Controller. Pawn only forwards.

```c
void AVRAController::ButtonPressed()
{
	FTransform TR;
	TR.SetTranslation(GetActorLocation());
	AStroke *punter = GetWorld()->SpawnActor<AStroke>(StrokeToPaint,TR);
	punter->SetOwner(this);
}
```
At Controller.h
```
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStroke> StrokeToPaint;
```

---
## 3D Painting With Spline Meshes
Draw until release trigger

Delete from Stroke *BeginPlay* and *Tick*. We do not use it.

Add an Update function
```c
void Update(FVector CursorLocation);
```
Promote Stroke from ButtonPressed to a member class an rename to CurrentStroke.

Call update from stroke in Controller Tick function.
```c
void AVRAController::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentStroke != nullptr) // Only update if there is some stroke.
	{
		CurrentStroke->Update(GetActorLocation());
	}
}
```
Put to nullprt , if Button is released
```c
void AVRAController::ButtonRelease()
{
	CurrentStroke = nullptr;
}
```
- Create spline Mesh to manipulate 
- Update endpoints (using new Cursor Location from Last Cursor Location)
- Look last project how to create an Spline Mesh and setup Start and end position. Tangent now is irrelebant.

Implement The Algoritth
- Spawn spline meshes
- Their endpoints ahould match up
- You should be able to draw

This is not the solution in video. This Solution do not attach SplineMesh to Actor.

```c
void AVRAController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentStroke != nullptr) // Only update if there is some stroke.
	{
		CurrentStroke->Update(GetActorLocation());
	}
}

void AStroke::Update(FVector CursorLocation)
{
	if (LastLocation.IsZero())
	{
		LastLocation = CursorLocation;
		CreateSpline(CursorLocation);
	}
	if (FVector::Distance(LastLocation, CursorLocation) > 2.0f)
	{
		CreateSpline(CursorLocation);
		LastLocation = CursorLocation;
	}
}

void AStroke::CreateSpline(FVector StartPoint)
{
	USplineMeshComponent* DynamicMesh = NewObject<USplineMeshComponent>(this);
	DynamicMesh->SetMobility(EComponentMobility::Static);
	DynamicMesh->SetStaticMesh(Mesh);
	DynamicMesh->SetMaterial(0, Material);
	DynamicMesh->RegisterComponent();
	DynamicMesh->SetStartAndEnd(LastLocation, FVector(0.0f), StartPoint, FVector(0.0f));
	DynamicMesh->SetVisibility(true);
	DynamicMeshArray.Add(DynamicMesh);
}
```
