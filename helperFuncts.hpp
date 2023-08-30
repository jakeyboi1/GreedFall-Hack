#pragma once
#include "stdafx.h"
using namespace std;

class helperFunctions {
public:
    void PrintAllModules(HANDLE recProcHandle) { //Note visual studio debugger does this for you but its a cool function (THis function just prints all modules the recieved process is using
        HMODULE hArray[MAX_PATH]; //setting the variable to an array with the arrays max val being MAX_PATH
        DWORD fak; //Used to store the file name in
        if (EnumProcessModules(recProcHandle, hArray, sizeof(hArray), &fak)) { printf("EnumProcessModules Completed"); } //Gets all modules for our process and stores them in the var fak
        char fileName[10000]; //char array max size of 10000 (could likely work with MAX_PATH)
        for (unsigned int i = 0; i < fak / sizeof(HMODULE); i++) { //For loop over the length of fak
            GetModuleFileNameEx(recProcHandle, hArray[i], fileName, 256); //Gets the file name of the current module
            cout << "Module Name: " << fileName << endl; //Prints it to the console
        }
    }
    bool shutdown(FILE* fp) { //File "shuts down" the process.
        Sleep(1000);
        if (fp != nullptr) {
            fclose(fp); //Closes the file pointer
            FreeConsole(); //closes the console
            return TRUE; //returns true
        }
        else {
            return FALSE;
        }
    }
    bool clearConsole() { //clears the console and reprints starter vals
        system("cls"); //clears the console
        printf("Press [NUMPAD 1] for Infinite Carry Weight!\nPress [NUMPAD 2] to add 100 experience!\n");
        return TRUE;
    }
    uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, vector<unsigned int> offsets) {
        uintptr_t addr = ptr;
        for (unsigned int i = 0; i < offsets.size(); ++i) {
            ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
            addr += offsets[i];
        }
        return addr;
    }
    MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessId) {
        MODULEENTRY32 modEntry = { 0 };
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessId);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 curr = { 0 };
            curr.dwSize = sizeof(MODULEENTRY32);
            if (Module32First(hSnapshot, &curr)) {
                do {
                    if (!strcmp(curr.szModule, moduleName)) {
                        modEntry = curr;
                        break;
                    }                    
                }
                while (Module32Next(hSnapshot, &curr));
            }
            CloseHandle(hSnapshot);
        }
        return modEntry;
    }
    uintptr_t GetModuleBaseAddress(DWORD dwProcId, char* szModuleName) {
        uintptr_t ModuleBaseAddress = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcId);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 ModuleEntry32;
            ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
            if (Module32First(hSnapshot, &ModuleEntry32)) {
                do {
                    if (strcmp(ModuleEntry32.szModule, szModuleName) == 0) {
                        ModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
                        break;
                    }
                } 
                while (Module32Next(hSnapshot, &ModuleEntry32));
            }
            CloseHandle(hSnapshot);
        }
        return ModuleBaseAddress;
    }
};

/*void PrintAllModules(HANDLE recProcHandle) {
    HMODULE hArray[MAX_PATH]; //creating an array with the maximum size allowed for file paths
    DWORD fak; //creating a var to store the processmodules in
    if (EnumProcessModules(recProcHandle, hArray, sizeof(hArray), &fak)) { printf("EnumProcessModules Completed"); }; //This is getting all processmodules that are associated with our process
    char fileName[10000]; //creating an array with a max value of 10000 so it can run until it finds it
    unsigned int which = 0; //creating a var to use in our for loop
    for (unsigned int i = 0; i < fak / sizeof(HMODULE); i++) { //for looping over the variable fak
        GetModuleFileNameEx(recProcHandle, hArray[i], fileName, 256); //Getting the file name of the module
        std::cout << "Module Name: " << fileName << std::endl; //printing the module
    }
}*/

/*bool shutdown(FILE* fp) {
    Sleep(1000);
    if (fp != nullptr) {
        fclose(fp);
        FreeConsole();
        return TRUE;
    }
    else {
        return FALSE;
    }
}*/

/*uintptr_t GetModuleBaseAddress(DWORD dwProcID, char* szModuleName)
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
}*/