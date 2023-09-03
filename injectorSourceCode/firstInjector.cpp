#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

using namespace std;

void get_proc_id(const char* window_title, DWORD &process_id) { /*Getting the process Id*/
    GetWindowThreadProcessId(FindWindow(NULL, window_title), &process_id); /*Passing by reference aka the &symbol process id in this funct has to be reference*/
}

void error(const char* error_title, const char* error_message) { /*Sows an error then exits the program*/
    MessageBox(NULL, error_message, error_title, NULL); /*Shows an error message*/
    exit(-1); /*Closes the program with a code of negative 1*/
}

bool file_exists(string file_name) { /*Ensures file exists*/
    struct stat buffer; /*Creating random buffer of the stat structure, making sure if this func is successful gets information about the given file*/
    return (stat(file_name.c_str(), &buffer) == 0); /*Returns if the function worked if 0 worked else did not*/
}

int main() {
    DWORD proc_id = NULL;
    TCHAR dll_path[MAX_PATH]; /*Max size a file path can be aka 260 characters*/
    const char* dll_name = "internalGreedFallHack.dll";
    const char* window_title = "GreedFall"; /*This has to be the window title of the application you want to inject too*/

    if (!file_exists(dll_name)) {
        error("file_exists", "File does not exist");
    }

    if (!GetFullPathName(dll_name, MAX_PATH, dll_path, nullptr)) {
        error("GetFullPathName", "Failed to get full path");
    }

    get_proc_id(window_title, proc_id);
    if (proc_id == NULL) {
        error("get_proc_id", "Failed to get process id");
    }

    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc_id);
    if (!h_process) {
        error("OpenProcess", "Failed to open a handle to process");
    }

    void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); /*Calling this function passing our handle to the process, then reserving the memory and commiting to it then allowed readwrite access*/
    if (!allocated_memory) {
        error("VirtualAllocex", "Failed to allocate memory");
    }

    if (!WriteProcessMemory(h_process, allocated_memory, dll_path, MAX_PATH, nullptr)) {
        error("WriteProcessMemory", "Write process memory failed to process memory");
    }

    HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_memory, NULL, nullptr);
    if (!h_thread) {
        error("CreateRemoteThread", "Failed to create remote thread");
    }

    CloseHandle(h_process);
    VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
    MessageBox(0, "Sucessfully Injected!", "Sucess!", 0);
}