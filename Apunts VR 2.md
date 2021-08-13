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

