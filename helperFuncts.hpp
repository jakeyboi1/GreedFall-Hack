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
            cout << "Module Name: " << fileName << endl;
        }
    }
    bool shutdown(FILE* fp) { //File "shuts down" the process.
        Sleep(1000);
        if (fp != nullptr) {
            fclose(fp); //Closes the file pointer
            FreeConsole(); //closes the console
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
    bool clearConsole() { //clears the console and reprints starter vals
        system("cls"); //clears the console
        printf("Press [NUMPAD 1] for Infinite Carry Weight!\nPress [NUMPAD 2] to add 100 experience!\nPress [NUMPAD 3] to make buying items from a shop take no money!(Once enabled the only way to disable is by restarting your game)\nnPress [NUMPAD 4] for infinite health!\n");
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
    MODULEENTRY32 GetModuleInfo(DWORD dwProcId, char* szModuleName) {
        MODULEENTRY32 ModuleInfo{};
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcId);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 ModuleEntry32;
            ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
            if (Module32First(hSnapshot, &ModuleEntry32)) {
                do {
                    if (strcmp(ModuleEntry32.szModule, szModuleName) == 0) {
                        ModuleInfo = ModuleEntry32;
                        break;
                    }
                } while (Module32Next(hSnapshot, &ModuleEntry32));
            }
            CloseHandle(hSnapshot);
        }
        return ModuleInfo;
    }
    uintptr_t find_signature(char* module, const char* pattern_, const char* mask, DWORD pId, HANDLE pHandle) {
	    const auto compare = [](const uint8_t* data, const uint8_t* pattern, const char* mask_) {
		    for (; *mask_; ++mask_, ++data, ++pattern)
			    if (*mask_ == 'x' && *data != *pattern)
				    return false;

		    return (*mask_) == 0;
		};
    
        MODULEENTRY32 moduleInfo = GetModuleInfo(pId, module);
	    MODULEINFO module_info = {};
        GetModuleInformation(pHandle, moduleInfo.hModule, &module_info, sizeof MODULEINFO);

	    auto module_start = uintptr_t(module_info.lpBaseOfDll);
	    const uint8_t* pattern = reinterpret_cast<const uint8_t*>(pattern_);
	    for (size_t i = 0; i < module_info.SizeOfImage; i++)
		    if (compare(reinterpret_cast<uint8_t*>(module_start + i), pattern, mask))
			    return module_start + i;

	    return 0;
    }
    bool checkIfMemAddrIsProtected(uintptr_t addr) {
        //This function will check if the memory addr is protected or not (useful for debugging access violation errors returns true if not protected false if it is)
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery((void*)addr, &mbi, sizeof(mbi));
        bool no_rights = (mbi.Protect & PAGE_GUARD) || (mbi.Protect & PAGE_NOACCESS);
        if (!no_rights) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
};