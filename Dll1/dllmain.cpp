// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cstdint>

// This method defines a threat that will run concurrently with the game
DWORD WINAPI MyThread(HMODULE hModule)
{
	// The following 3 lines enable a writable console
	// We don't actually need a console here, but it is very useful to print debugging information to. 
	AllocConsole();
	FILE* f = new FILE;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Injection worked\n";
	std::cout << "Process ID is: " << GetCurrentProcessId() << std::endl;
	// We can see by looking at the process ID in process explorer that this code is being run by the process it was injected into. 

	// From cheat engine analysis we know that
	// z_coord is at memory address: [[[["PwnAdventure3-Win32-Shipping.exe"+018FCD60] + 20 ] + 238 ] + 280 ] + 98 
	// This code follows that pointer path
	//
	// NB this may lead trying to dereference null pointers or reading memory you don't have access to.
	// In other module you would be expected to catch and handle these possible errors 
	
	uintptr_t PwnAventAddr = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
	printf("PwnAventAddr: %p\n", PwnAventAddr);
	uintptr_t firstStep = *(uintptr_t*)(PwnAventAddr + 0x18FCD60);
	printf("PwnAventAddr + 0x18FCD60 = %p has value %p\n", PwnAventAddr + 0x18FCD60, firstStep);
	uintptr_t secondStep = *(uintptr_t*)(firstStep + 0x20);
	printf("firstStep + 0x20 = %p has value %p\n", firstStep + 0x20, secondStep);
	uintptr_t thirdStep = *(uintptr_t*)(secondStep + 0x238);
	printf("secondStep + 0x238 = %p has value %p\n", secondStep + 0x238, thirdStep);
	uintptr_t forthStep = *(uintptr_t*)(thirdStep + 0x280);
	printf("thirdStep + 0x280 = %p has value %p\n", thirdStep + 0x280, forthStep);
	

	/*
	uintptr_t GameLogicAddr = (uintptr_t)GetModuleHandle(L"GameLogic.dll");
	printf("GameLogicAddr = %p\n", GameLogicAddr);

	uint32_t GameLogic = (uint32_t)GameLogicAddr;
	GameLogic += 0x97D7C + 0x4 + 0x8 + 0x10 + 0xBC;

	printf("GameLogic = %p\n", (uintptr_t)GameLogic);

	uint32_t* slotAd = (uint32_t*)(GameLogic);
	printf("slotAd = %p\n", slotAd);
	uint32_t mana = *slotAd;
	printf("slot = %d\n", mana);
	*/

	uint32_t PwnAvent = (uint32_t)PwnAventAddr;
	printf("PwnAvent = %p\n", PwnAvent);
	PwnAvent += 0x82EA7C + 0x504 + 0x35C + 0x90 + 0x18 + 0x180;
	printf("PwnAvent = %p\n", (uintptr_t)PwnAvent);
	uint32_t* manaAd = (uint32_t*)((uintptr_t)PwnAvent);
	printf("manaAd = %p\n", manaAd);
	uint32_t mana = *manaAd;
	printf("mana = %d\n", mana);


	// coords x, y, z at 0x90, 0x94, 0x98
	float* x_coord_Address = (float*)(forthStep + 0x90);
	float* y_coord_Address = (float*)(forthStep + 0x94);
	float* z_coord_Address = (float*)(forthStep + 0x98);

	float x_coord;
	float y_coord;
	float z_coord;

	std::vector<std::vector<float>> egg_locations {
		{ -25045.0f, 18085.0f, 260.0f },
		{ -51570.0f, -61215.0f, 5020.0f },
		{ 24512.0f, 69682.0f, 2659.0f },
		{ 60453.0f, -17409.0f, 2939.0f },
		{ 1522.0f, 14966.0f, 7022.0f },
		{ 11604.0f, -13131.0f, 411.0f },
		{ -72667.0f, -53567.0f, 1645.0f },
		{ 48404.0f, 28117.0f, 704.0f },
		{ 65225.0f, -5740.0f, 4928.0f },
		{ -2778.0f, -11035.0f, 10504.0f }
	};

	std::vector<float> bear_chest_location { -7894.0f, 64482.0f, 4000.0f };
	std::vector<float> spawn_village_location { -39547.0f, -20335.0f, 3000.0f };

	float sink_amount = 300.0f;
	int egg_index = 0; //used to cycle between egg locations
	
	// This is the main loop that will run in the background
	while (true) {

		x_coord = *x_coord_Address;
		y_coord = *y_coord_Address;
		z_coord = *z_coord_Address;

		// Pressing B will print player coords
		if (GetAsyncKeyState('B') & 1) {
			std::cout << "coords: " << x_coord << " " << y_coord << " " << z_coord << std::endl;
			mana = *manaAd;
			printf("mana = %d\n", mana);
		}

		// Pressing N will sink the player into the ground
		if (GetAsyncKeyState('N') & 1) {
			std::cout << "sinking..." << std::endl;
			*z_coord_Address -= sink_amount;
		}

		// Pressing M will cycle the player between different egg locations
		if (GetAsyncKeyState('M') & 1) {
			std::cout << "cycling egg locations " << egg_index << "..." << std::endl;

			auto egg_location = egg_locations[egg_index];

			*x_coord_Address = egg_location[0];
			*y_coord_Address = egg_location[1];
			*z_coord_Address = egg_location[2];

			egg_index++;
			if (egg_index >= egg_locations.size())
				egg_index = 0;
		}

		if (GetAsyncKeyState('H') & 1) {
			std::cout << "teleporting to bear chest..." << std::endl;

			*x_coord_Address = bear_chest_location[0];
			*y_coord_Address = bear_chest_location[1];
			*z_coord_Address = bear_chest_location[2];
		}

		if (GetAsyncKeyState('J') & 1) {
			std::cout << "teleporting to spawn village..." << std::endl;

			*x_coord_Address = spawn_village_location[0];
			*y_coord_Address = spawn_village_location[1];
			*z_coord_Address = spawn_village_location[2];
		}

		if (GetAsyncKeyState('K') & 1) {
			std::cout << "teleporting up..." << std::endl;
			*z_coord_Address += 1000.0f;
		}

	}
	return 0;
}

// This is the main method that runs when the DLL is injected.
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		// We run the cheat code in a seperate thread to stop it interupting the game execution. 
		// Again we dont catch a possible NULL, if we are going down then we can go down in flames. 
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MyThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
