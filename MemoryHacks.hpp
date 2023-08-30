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
    bool AllShopItemsFree(DWORD pId, HANDLE pHandle) {
        uintptr_t gameDllBaseAddr = helpers.GetModuleBaseAddress(pId, (char*)"Game.dll");
        if (gameDllBaseAddr == 0) {
            return FALSE;
        }
        uintptr_t readAddr = gameDllBaseAddr + 0x3F7E48; //Calculating where this instruction is by adding its offset to the baseaddress of game.dll
        BYTE nop[] = { 0x90, 0x90 }; //Since in cheat engine we can see the addr we are nop has 2 bytes 2B and C7 we have to do 0x90 or nop in 2 bytes so we must do it like this. If it was just one byte we can replace the &nop with 0x90 and sizeof(nop) with 1
        if (WriteProcessMemory(pHandle, (LPVOID)readAddr, &nop, sizeof(nop), NULL)) { //the addr specified here is the address specified to the specific instruction which removes the item from the vendor and money from you, to find addr go into cheat engine find your money (4 byte integer) right click debug find what writes to this address then go to that instruction in memory view/assembler view/decompiler and find the instruction nearby that is bytes 2B C7 instruction sub eax, the address that is assigned to that instruction is the addr needed to write too we write nop as the instruction is 2 bytes 2B and C7 so writing a single 0x90 would not work
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
};