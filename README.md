# Teleportation Cheats for Pwn Adventure 3: Pwnie Island

![Pwn Adventure 3: Pwnie Island](pwnie-island.png)

Teleportation cheats for Pwn Adventure 3: Pwnie Island. The cheats allow the player to teleport
around (ie. modify X, Y, Z coordinates). The project is a Visual Studio Solution. It produces an
injectable DLL that can be injected into the Pwn Adventure 3 process on Windows.

### Description

The cheats allow the player to teleport around the map, even when connected to a game server, as the
player's position is not checked and can be modified freely. The cheat has utilities to teleport to
most common locations such as the village, and perform common tasks (eg. sinking into the ground).

The cheat allows the player to obtain the bear and egg flags easily. Pressing `H key` will teleport
the player to the bear chest location. Pressing `M key` will cycle the player through all egg
locations and allow them to collect all the eggs without having to search for them.

### Running

The DLL is pre-built in `./Debug/Dll1.dll`.

1. (Optional) Build the DLL using Visual Studio
2. Launch Pwn Adventure 3: Pwnie Island
3. Inject `Dll1.dll` using `Extreme Injector v3.7.3 by master131`
4. Check `dllmain.cpp` for all available features
