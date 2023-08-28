// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <BlackBone/Process/Process.h>
#include <BlackBone/Patterns/PatternSearch.h>
#include <BlackBone/Process/RPC/RemoteFunction.hpp>
#include <BlackBone/Syscalls/Syscall.h>
#define PYTHONIMPORT extern "C" __declspec(dllexport)
using namespace blackbone;
using namespace std;
namespace InjectorMain
{
    Process proc;
    std::wstring ctow(const char* src)
    {
        return std::wstring(src, src + strlen(src));
    }
    NTSTATUS AttachCurrentProccess(const char* process) 
    {
        return proc.Attach((const wchar_t*)process, 3451UL);
    }
    void InjectviaLoadLibrary(const char* pr, const char* path_to_dll)
    {
        const wstring& string_x = ctow(path_to_dll);
        auto mainThread = proc.threads().getMain();
        if (auto pLoadLibrary = MakeRemoteFunction<decltype(&LoadLibraryW)>(proc, L"Kernel32.dll", "LoadLibraryW"); pLoadLibrary && mainThread)
        {
            auto result = pLoadLibrary.Call({ string_x.c_str() }, mainThread);
            if (*result)
                MessageBoxA(0, "SUCCESSFULLY ATTACHED!!!", "LibBlackBoneInjector-Python", 0);
            else
            {
                exit(554);
            }
        }
    }
}

PYTHONIMPORT void AttachProcess(const char* this_current_proc) {
    NTSTATUS status = InjectorMain::AttachCurrentProccess(this_current_proc);
    if (status) {
        cout << "[PyBlackBoneInjector] Failed To Find This Process" << endl;
    }
    else {
        cout << "[PyBlackBoneInjector] Success!!!" << endl;
    }
}
PYTHONIMPORT void LoadLibraryInject(const char* process, const char* path) {
    InjectorMain::InjectviaLoadLibrary(process, path);
}
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved) 
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "[PyBlackBone] DLL has Been Attached!!!" << endl;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
