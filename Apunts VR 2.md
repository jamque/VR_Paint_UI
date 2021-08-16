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

This Solution attach SplineMesh to Actor and its similar to the lecture, but it's not the same code.

```c

// In .h
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

//In constructor
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

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
	CursorLocation = GetActorTransform().InverseTransformPosition(CursorLocation);
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
	DynamicMesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetIncludingScale);
	DynamicMesh->SetMobility(EComponentMobility::Movable);
	DynamicMesh->SetStaticMesh(Mesh);
	DynamicMesh->SetMaterial(0, Material);
	DynamicMesh->RegisterComponent();
	DynamicMesh->SetStartAndEnd(LastLocation, FVector(0.0f), StartPoint, FVector(0.0f));
	DynamicMesh->SetVisibility(true);
	DynamicMeshArray.Add(DynamicMesh);
}
```
---
## Frame rate in VR & Performance Bottlenecks (2 lectures)
Each eye have the half of time of framerate to render (90 FPS -> 11 ms -> 5.5 ms per eye)
To start a game using VR (like Rift), Set in *Project Setting -> Start in VR*, and Unreal in a standalone exe, try to use VR when game is launched.

CPU works in the next frame, meanwhile GPU is drawing the last calculated frame.
If CPU time is great than GPU, this generate a bottleneck. We must try to have less time in CPU athan GPU.

Unreal use 2 threads, one for game and other for render. This second thread comunicates with GPU in order to draw.

This command shows times of the threads
```
stat unit
```
To show GPU stadistics or game thread or draw.
```
stat gpu
stat game
stat sceneRendering
```

Uses *stat none* to clear stat that you see.

---
## Performance Optimisation && Reducing DrawCalls with Instancing
Set in *Project Setting ->Rendering->Forward Shading*.

By default, Unreal Engine 4 (UE4) uses a Deferred Renderer as it provides the most versatility and grants access to more rendering features. However, there are some trade-offs in using the Deferred Renderer that might not be right for all VR experiences. Forward Rendering provides a faster baseline, with faster rendering passes, which may lead to better performance on VR platforms. Not only is Forward Rendering faster, it also provides better anti-aliasing options than the Deferred Renderer, which may lead to better visuals. 

BEWARE ! When you restars editor, it takes its time.

To reduce draw calls, first let's take a look how many DraCalls our game done. In *stat sceneRendering* watch Counters:Mesh draw calls.

To reduce them, Recapping the instanced Static Mesh. A component that efficiently renders multiple instances of the same StaticMesh. In OpenGL, this is an object to put together all triangles in the same VBO.
```c
UInstancedStaticMeshComponent
and use AddInstance and RemoveInstance to draw triangles.
```
Replace the Spline Mesh with InstancedStaticMeshComponent
- Create the Component in C++
- Add the instance instead of spawning

Eliminate all USplineMeshComponent references and uses.

```c
	UInstancedStaticMeshComponent * StrokeMeshInstanced;

	StrokeMeshInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Stroke Mesh"));
	StrokeMeshInstanced->SetupAttachment(Root);

In AStroke::CreateSpline
	FTransform TR;
	TR.SetLocation(StartPoint);
	StrokeMeshInstanced->AddInstance(TR);
```
