#pragma once
#include "stdafx.h"
void PrintAllModules(HANDLE recProcHandle) {
    HMODULE hArray[MAX_PATH]; //creating an array with the maximum size allowed for file paths
    DWORD fak; //creating a var to store the processmodules in
    if (EnumProcessModules(recProcHandle, hArray, sizeof(hArray), &fak)) { printf("EnumProcessModules Completed"); }; //This is getting all processmodules that are associated with our process
    char fileName[10000]; //creating an array with a max value of 10000 so it can run until it finds it
    unsigned int which = 0; //creating a var to use in our for loop
    for (unsigned int i = 0; i < fak / sizeof(HMODULE); i++) { //for looping over the variable fak
        GetModuleFileNameEx(recProcHandle, hArray[i], fileName, 256); //Getting the file name of the module
        std::cout << "Module Name: " << fileName << std::endl; //printing the module
    }
}

bool shutdown(FILE* fp) {
    Sleep(1000);
    if (fp != nullptr) {
        fclose(fp);
        FreeConsole();
        return TRUE;
    }
    else {
        return FALSE;
    }
}

uintptr_t GetModuleBaseAddress(DWORD dwProcID, char* szModuleName)
{
    uintptr_t ModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 ModuleEntry32;
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &ModuleEntry32))
        {
            do
            {
                if (strcmp(ModuleEntry32.szModule, szModuleName) == 0)
                {
                    ModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &ModuleEntry32));
        }
        CloseHandle(hSnapshot);
    }
    return ModuleBaseAddress;
}

MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessID)
{
    MODULEENTRY32 modEntry = { 0 };

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 curr = { 0 };

        curr.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &curr))
        {
            do
            {
                if (!strcmp(curr.szModule, moduleName))
                {
                    modEntry = curr;
                    break;
                }
            } while (Module32Next(hSnapshot, &curr));
        }
        CloseHandle(hSnapshot);
    }
    return modEntry;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

bool clearConsole() {
    system("cls");
    printf("Press [NUMPAD 1] for Infinite Carry Weight!\nPress [NUMPAD 2] to add 100 experience!\n");
    return TRUE;
}