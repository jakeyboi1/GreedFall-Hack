// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "stdafx.h"
using namespace std; //Using the std namespace so we do not have to type std:: everytime

//Variables
bool infCarryWeightEnabled = FALSE;
MemHacks memhack; //Creating a variable which opens the MemHacks class for us to use in this code
helperFunctions helpers;

// Your thread function
DWORD WINAPI mainThread(LPVOID lpParam) {
    AllocConsole(); //Allocating a console to our program
    FILE* fp; //Setting a file pointer to the var fp
    freopen_s(&fp, "CONOUT$", "w", stdout); //opens a console on our variable fp
    DWORD pId = NULL;
    GetWindowThreadProcessId(FindWindow(NULL, "GreedFall"), &pId); //Getting the process id of the GreedFall window, and setting it the var pId
    if (pId == NULL) { printf("Failed to get proc id\n"); } //Making sure it worked
    HANDLE pHandle = NULL;
    pHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pId); //Getting our process handle for our process id
    if (pHandle == NULL) { printf("Failed to get process Handler\n"); } //Making sure it worked
    printf("Press [NUMPAD 1] for Infinite Carry Weight!\nPress [NUMPAD 2] to add 100 experience!\nPress [NUMPAD 3] to make buying items from a shop take no money!(Once enabled the only way to disable is by restarting your game)\n");

    while (true) {
        Sleep(10); //Waits 10ms
        if (infCarryWeightEnabled) {
            bool retval = memhack.InfiniteCarryWeight(pId, pHandle);
            if (!retval) { printf("Mem hack Inf carry weight FAILED!\n"); }
        }
        if (GetAsyncKeyState(VK_DELETE)) { //if the back key is pressed
            bool shutdownRetval = helpers.shutdown(fp); //Calls our helper and gets the retval
            if (shutdownRetval) { //if retval true
                break; //breaks loop
            }
            else {
                printf("Shutdown Failed\n");
            }
        }
        if (GetAsyncKeyState(VK_NUMPAD1)) { //If numpad one is pressed
            if (infCarryWeightEnabled == FALSE) {
                bool retval = memhack.InfiniteCarryWeight(pId, pHandle);
                if (retval) {
                    infCarryWeightEnabled = TRUE;
                    printf("Enabled Infinite Carry Weight\n");
                    Sleep(1000);
                }
                else {
                    printf("Error failed to enable Infinite Carry Weight\n");
                }
                bool consoleCleared = helpers.clearConsole();
            }
            else {
                infCarryWeightEnabled = FALSE;
                printf("Infinite Carry Weight Disabled\n");
                Sleep(1000);
                bool consoleCleared = helpers.clearConsole();
            }
        }
        if (GetAsyncKeyState(VK_NUMPAD2)) {
            bool retval = memhack.AddExperience(pId, pHandle);
            if (retval == FALSE) {
                printf("Error failed to add experience");
            }
            Sleep(200);
            bool consoleCleared = helpers.clearConsole();
        }
        if (GetAsyncKeyState(VK_NUMPAD3)) {
            bool retval = memhack.AllShopItemsFree(pId, pHandle);
            if (retval == FALSE) {
                printf("failed to enabled");
            }
            else {
                printf("Enabled");
            }
            Sleep(1000);
            bool consoleCleared = helpers.clearConsole();
        }
    }

    return 0; //returns zero closing this thread
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mainThread, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}