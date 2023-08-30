#pragma once
#include "stdafx.h"
using namespace std;

class MemHacks {
public:
    helperFunctions helpers;
    bool AddExperience(DWORD pId, HANDLE pHandle) {
        uintptr_t gameDllBaseAddr = helpers.GetModuleBaseAddress(pId, (char*)"Game.dll");
        if (gameDllBaseAddr == 0) {
            return FALSE;
        }
        DWORD currentExperienceAddr = 0x00E4FCC0;
        vector<unsigned int> currentExperienceOffsets = { 0x38, 0x18, 0x0, 0x38, 0x40, 0x108, 0x50 };
        uintptr_t readAddr = gameDllBaseAddr + currentExperienceAddr;
        uintptr_t addr = helpers.FindDMAAddy(pHandle, readAddr, currentExperienceOffsets);
        if (addr == readAddr) {
            return FALSE;
        }

        //Getting the current experience value from memory
        float currentVal = 0;
        if (!ReadProcessMemory(pHandle, (LPVOID)addr, &currentVal, sizeof(currentVal), nullptr)) { //This reads the value that is currently stored at the address and outputs it to currentVal variable
            printf("Failed to read current experience level!");
        }

        //Writing our new value to memory
        float val = currentVal + 100;
        if (WriteProcessMemory(pHandle, (LPVOID)addr, &val, sizeof(val), nullptr)) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
    bool InfiniteCarryWeight(DWORD pId, HANDLE pHandle) {
        uintptr_t gameDllBaseAddr = helpers.GetModuleBaseAddress(pId, (char*)"Game.dll"); //Gets  the base address of our needed process/module (Only tested this with .dll files not exe)
        if (gameDllBaseAddr == 0) {
            return FALSE;
        }
        //cout << "enginedll base addr = " << gameDllBaseAddr << endl; //Prints our adddress
        DWORD currentCarryWeightAddr = 0x00D801B0; //This is the offset from Game.dll where the weight is stored
        vector<unsigned int> currentCarryWeightOffsets = { 0x2E0, 0xB0, 0x20, 0x80, 0x60, 0x80, 0x58 }; //These are all the offsets from the main offset where weight is stored
        uintptr_t readAddr = gameDllBaseAddr + currentCarryWeightAddr; //This is getting our final address by combining game.dll base addy and our offset to the cur weight together
        uintptr_t addr = helpers.FindDMAAddy(pHandle, readAddr, currentCarryWeightOffsets); //This is getting calculating our final address by calculating through our currentCarryWeightOffsets vector (the addy this returns is what we right our new data too)
        if (addr == readAddr) {
            return FALSE;
        }
        //cout << "Recieved addy: " << addr << endl; //Prints our recieved addy
        float val = 0; //Setting the value we want to write
        if (WriteProcessMemory(pHandle, (LPVOID)addr, &val, sizeof(val), nullptr)) { //Writing the value to the address weight is stored
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
};