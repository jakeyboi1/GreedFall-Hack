#pragma once
#include "stdafx.h"
using namespace std;

class MemHacks {
public:
    helperFunctions helpers;

    //Add experience hack
    bool AddExperience(DWORD pId, HANDLE pHandle) {
        const auto modBase = reinterpret_cast<uintptr_t>(GetModuleHandle("Game.dll")); //This gets the base address of our game.dll module
        DWORD currentExperienceAddr = 0x00E4FCC0;
        vector<unsigned int> currentExperienceOffsets = { 0x38, 0x18, 0x0, 0x38, 0x40, 0x108, 0x50 };
        uintptr_t readAddr = modBase + currentExperienceAddr;
        uintptr_t addr = helpers.FindDMAAddy(pHandle, readAddr, currentExperienceOffsets);
        if (addr == readAddr) { return FALSE; }

        //Getting the current experience value from memory
        float currentVal = 0;
        if (!ReadProcessMemory(pHandle, (LPVOID)addr, &currentVal, sizeof(currentVal), nullptr)) { //This reads the value that is currently stored at the address and outputs it to currentVal variable
            printf("Failed to read current experience level!");
        }

        //Writing our new value to memory
        return writingFloatsToMem(pHandle, addr, currentVal + 100);
    }

    //Infinite carry weight hack
    bool InfiniteCarryWeight(DWORD pId, HANDLE pHandle) {
        const auto gameDllBaseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle("Game.dll"));
        DWORD currentCarryWeightAddr = 0x00D801B0; //This is the offset from Game.dll where the weight is stored
        vector<unsigned int> currentCarryWeightOffsets = { 0x2E0, 0xB0, 0x20, 0x80, 0x60, 0x80, 0x58 }; //These are all the offsets from the main offset where weight is stored
        uintptr_t readAddr = (uintptr_t)gameDllBaseAddr + currentCarryWeightAddr; //This is getting our final address by combining game.dll base addy and our offset to the cur weight together
        uintptr_t addr = helpers.FindDMAAddy(pHandle, readAddr, currentCarryWeightOffsets); //This is getting calculating our final address by calculating through our currentCarryWeightOffsets vector (the addy this returns is what we right our new data too)
        if (addr == readAddr) { return FALSE; }
        return writingFloatsToMem(pHandle, addr, 0);
    }

    //All shop items free hack (done by editing the asm of the game)
    bool AllShopItemsFree(DWORD pId, HANDLE pHandle) {
        const auto gameDllBaseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle("Game.dll"));
        uintptr_t readAddr = (uintptr_t)gameDllBaseAddr + 0x3F7E48; //Calculating where this instruction is by adding its offset to the baseaddress of game.dll
        //Note all removal of items or money passes through the address game.dll + 3F7E4A (3F7E48 when disabled will mean no items remove from your inventory bullets potions etc because it disabled the remove assembly code) more testing required on this
        BYTE nop[] = { 0x90, 0x90 }; //Since in cheat engine we can see the addr we are nop has 2 bytes 2B and C7 we have to do 0x90 or nop in 2 bytes so we must do it like this. If it was just one byte we can replace the &nop with 0x90 and sizeof(nop) with 1
        if (WriteProcessMemory(pHandle, (LPVOID)readAddr, &nop, sizeof(nop), NULL)) { //the addr specified here is the address specified to the specific instruction which removes the item from the vendor and money from you, to find addr go into cheat engine find your money (4 byte integer) right click debug find what writes to this address then go to that instruction in memory view/assembler view/decompiler and find the instruction nearby that is bytes 2B C7 instruction sub eax, the address that is assigned to that instruction is the addr needed to write too we write nop as the instruction is 2 bytes 2B and C7 so writing a single 0x90 would not work
            return TRUE;
        }
        else {
            return FALSE;
        }
    }

    //Infinite health hack
    bool InfiniteHealth(DWORD pId, HANDLE pHandle) {
        const auto gameDllBaseAddr = reinterpret_cast<uintptr_t>(GetModuleHandle("Game.dll"));
        DWORD currentHealthAddr = 0x00E4FCC0;
        vector<unsigned int> currentHealthOffsets = { 0x38, 0x28, 0x30, 0x38, 0xB0, 0x70, 0x428 };
        uintptr_t readAddr = (uintptr_t)gameDllBaseAddr + currentHealthAddr;
        uintptr_t addr = helpers.FindDMAAddy(pHandle, readAddr, currentHealthOffsets);
        if (addr == readAddr) { return FALSE; }
        return writingFloatsToMem(pHandle, addr, 500);
    }
private:
    bool writingFloatsToMem(HANDLE pHandle, uintptr_t addrToWriteToo, float newVal) {
        if (WriteProcessMemory(pHandle, (LPVOID)addrToWriteToo, &newVal, sizeof(newVal), nullptr)) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
};