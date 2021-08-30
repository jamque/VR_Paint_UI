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
---
## Rotating & Scaling Stroke
Caluclate Rotation And Scale
- Read documentation
```
FQuats::FindBetweenNormals
// Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
FVector::GetSafeNormal
// Gets a normalized copy of the vector, checking it is safe to do so based on the length.
FVector::Size
// Get the length (magnitude) of this vector.
```
- Study the previous slide
- Calculate a rotation
- Calulate a scale
- Test on the instances
```c
void AStroke::Update(FVector CursorLocation)
{
	if (LastLocation.IsZero())
	{
		LastLocation = CursorLocation;
		//CreateSpline(CursorLocation);
	}
	// Create spline Mesh to manipulate (if you don't have it yet)
	if (FVector::Distance(LastLocation, CursorLocation) > 2.0f)
	{
		CreateSpline(CursorLocation);
		LastLocation = CursorLocation;
	}
}

void AStroke::CreateSpline(FVector StartPoint)
{
	StrokeMeshInstanced->AddInstance(GetNextSegmentTransform (StartPoint));
}

FTransform AStroke::GetNextSegmentTransform(FVector Current) const
{
	FTransform SegmentTransform;

	SegmentTransform.SetScale3D(GetNextSegmentScale(Current));
	SegmentTransform.SetRotation(GetNextSegmentRotation(Current));
	SegmentTransform.SetLocation(GetNextSegmentLocation(Current));

	return SegmentTransform;
}

FVector AStroke::GetNextSegmentScale(FVector Current) const
{
	FVector Segment = Current - LastLocation;
	return FVector(Segment.Size(), 1.0f, 1.0f);
}

FQuat AStroke::GetNextSegmentRotation(FVector Current) const
{
	FVector Segment = Current - LastLocation;
	FVector SegmentNormal = Segment.GetSafeNormal();
	return FQuat::FindBetweenNormals(FVector::ForwardVector, SegmentNormal);
}

FVector AStroke::GetNextSegmentLocation(FVector Current) const
{
	return GetActorTransform().InverseTransformPosition(Current);
}

```
---
## Instanced Mesh Materials
Lecture give us two meshes: Cylinder and Sphere

Use Cylinder in Instanced Static Mesh to draw laser

Create a Material, Unlit and with Emissive Color. Use it in Instanced Static Mesh too.

Hide The Joints
- Create a new instandec staic mesh
- Use the sphere mesh
- Spawn to hide the joints
```c
//Stroke.h
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* PelotasMeshInstanced;
	FTransform GetNextPelotasTransform(FVector Current) const;

//Stroke.ccp
void AStroke::Update(FVector CursorLocation)
{
	if (LastLocation.IsZero())
	{
		LastLocation = CursorLocation;
		PelotasMeshInstanced->AddInstance(GetNextPelotasTransform(CursorLocation));
		return;
	}
	// Create spline Mesh to manipulate (if you don't have it yet)
	//if (FVector::Distance(LastLocation, CursorLocation) > 0.4f)
	//{
		CreateSpline(CursorLocation);
		LastLocation = CursorLocation;
	//}
}

void AStroke::CreateSpline(FVector StartPoint)
{
	StrokeMeshInstanced->AddInstance(GetNextSegmentTransform (StartPoint));
	PelotasMeshInstanced->AddInstance(GetNextPelotasTransform(StartPoint));
}

FTransform AStroke::GetNextPelotasTransform(FVector Current) const
{
	FTransform PT;
	PT.SetLocation(GetActorTransform().InverseTransformPosition(Current));
	return PT;
}
```
---
## Saving Game State
Load and save Strokes

USaveGame is a class from UObject. And Save this savegames into Slots.
```c
CreateSaveGameObject
SaveGameToSlot
LoadGameFromSlot
```
Create a subclass from USaveGame

Create ans Save
- Implement Create
- Check it returns something
- Implement Save
- Check ir create a file
```c
// Declare Public
UPainterSaveGame* UPainterSaveGame::Create()
{
	UPainterSaveGame *P = Cast<UPainterSaveGame>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
	return P;
}

bool UPainterSaveGame::Save()
{
	return UGameplayStatics::SaveGameToSlot(this, TEXT("partida"), 0);
}
```
---
## Loading a SaveGame
How to restore the save files.

Let's setup something to save with an Input Action. And other button to load.

UPROPERTYs in SaveGame Objects, will be used to save in slot.

Use LoadGameFromSlot to load a savegame. Thsi function creates a SaveGame Object itself.

Load Some State
- Populate a UPROPERTY
- Setup the loading
- Check ir restore the data correcty
- Check it works between reloads
```c
// In PainterSaveGame

	UPROPERTY()
	float dato = 10.0f;

UPainterSaveGame* UPainterSaveGame::Load()
{
	return Cast<UPainterSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("partida"), 0));
}

// In Pawn
	PlayerInputComponent->BindAction(TEXT("LoadButton"), EInputEvent::IE_Pressed, this, &AVRPawn::Load);
	PlayerInputComponent->BindAction(TEXT("SaveButton"), EInputEvent::IE_Pressed, this, &AVRPawn::Save);

void AVRPawn::Load()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White,
		FString::Printf(TEXT("             ENTRO a LOAD!")));
	UPainterSaveGame* Painting = UPainterSaveGame::Load();
	if (Painting)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CARREGAT! = %f"), Painting->dato);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
			FString::Printf(TEXT("             CARREGAT! = %f"), Painting->dato));
	}
}

void AVRPawn::Save()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White,
		FString::Printf(TEXT("             ENTRO a SAVE!")));
	UPainterSaveGame* Painting = UPainterSaveGame::Create();
	Painting->dato = RightController->GetActorLocation().X;
	if (Painting->Save())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Salvat! = %f"), Painting->dato);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
			FString::Printf(TEXT("             Salvat! = %f"), Painting->dato));
	}
}

```
---
## Serialising the Game State
Save and Load a Level.

The State of our level. Our level to save Stroke, just saving Cursorlocation of every instance is enought to regenerate later with Update function all the Stroke. Let's iterate to take all Cursorlocations. 

Contruct UPROPERTY in PainterSaveGame
```c
TArray<TSubclassOf<class AStroke>> Strokes;
```

The SaveGame serialising API.
```c
void SerializeFromWorld(const UWorld* World);
void DeserializeToWorld(Uworld* World);
```
In our pawn, when we want to Save, use SerializeFromWorld, and in Load use DeserializeToWorld.

SerializeFromWorld should iterate over all strokes and serialize them, and Store class type.

DeserializeToWorld for all strokes, spawn stroke fo the type. First clear the world.

Capture and Spawn
- Implement the two methods
- Check that the strokes disappear
- Check the outlines for the classes
- Are they the right number?
- Are they th right type?

```c
void UPainterSaveGame::SerializeFromWorld(UWorld* World)
{
	Strokes.Empty();
	for (TActorIterator<AStroke> StrokeIter(World); StrokeIter; ++StrokeIter)
	{
		// TO DO : Serialize
		Strokes.Add(StrokeIter->GetClass());
	}
}

void UPainterSaveGame::DeserializeToWorld(UWorld* World)
{
	ClearWorld(World);
	for (TSubclassOf<AStroke> StrokeClass : Strokes)
	{
		World->SpawnActor<AStroke>(StrokeClass);
	}
}

void UPainterSaveGame::ClearWorld(UWorld* World)
{
	for (TActorIterator<AStroke> StrokeIter(World); StrokeIter; ++StrokeIter)
	{
		StrokeIter->Destroy();
	}
}
// In Pawn Load
Painting->DeserializeToWorld(GetWorld());
// In Pawn Save
Painting->SerializeFromWorld(GetWorld());
```
---
## Serialising Actors To Structs
Let's visualize strokes.
how to store savedata
We must take the state of all strokes. List of location of each stroke. We use
structs to save it.
```c
//PainterSvaeGame.h
USTRUCT()
struct FStrokeState
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AStroke> Class;

	UPROPERTY()
	TArray<FVector> ControlPoints;
};

class
{
	UPROPERTY(VisibleAnywhere)
	TArray<FStrokeState> Strokes;
}
```
Serialise with Structs
- Implement the two methods
```c
	FStrokeState SerializeToStruct() const;
	static AStroke* DeserializeFromStruct(UWorld* World, const FStrokeState& StrokeState);
```
- How will you populate the array?
- How will you reconstruct the stroke?
- Test that you can restore a drawing.

*Save ControlPoints in Update function. Create a new Array in Stroke to save them.*

*Call update function to recreate Stroke. Do not use CreateSpline, beacuse you need
 previous point*
---
## Renaming C++ Base Classes
UiPointer PaintBrush from HandControllerBase. One for MainMenu, other to paint.

Let's Create a MainMenu from a new Level.

Rename HandController class to PaintBrushController class. And now BP_HandController it doesn't work. You must add this lines to *Config/DefaultEngine.ini*
```
[/Script/Engine.Engine]
+ActiveClassRedirects=(OldClassName="OldName", NewClassName="NewName")
```
You can remove these sentence after resave BP Asset uasset.

Extract The Hand Controller Stuff
- Should be able to use Base in VRPawn
- Can you use virtual methods to help?
- Movement should happen in the Base.
- What need to move up for that?
- Check painting still works.