# Labrinth

Labrinth is a Windows C++ reverse-engineering prototype for exploring Unreal Engine object layouts and building an ImGui overlay. The source includes Unreal-style object, actor, player and skeletal-mesh abstractions alongside driver and rendering code.

## Build

Open `Labrinth.sln` in Visual Studio and build the x64 configuration. The solution uses vcpkg manifest dependencies.

## Status

This is a game-specific research scaffold. Its data layouts and offsets are version-dependent, and the project is not a general Unreal Engine SDK or a released overlay tool.

## Defensive analysis

See [DEFENSIVE_ANALYSIS.md](DEFENSIVE_ANALYSIS.md) for a discussion of client trust boundaries and privileged access. It is a design note, not a driver audit or a tested game defense.
