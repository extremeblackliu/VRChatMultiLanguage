// dllmain.cpp : 定义 DLL 应用程序的入口点。
/*

        VRChat Multi Language Loader
    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
        Author:ExtremeBlackLiu
    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
        Librarys:
            nlohmann/json (nlohmann)
            sneakyevilSK/IL2CPP_Resolver (sneakyevil)
            sneakyevilSK/VersionHijack (sneakyevil)
    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
        this repo:
            ExtremeBlackLiu/VRChatMultiLanguage (me)

*/


#include "Main.hpp"
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "VersionHijack.hpp"

using namespace nlohmann;

void SetText(Unity::CComponent* ptext,const wchar_t* newText)
{
    Unity::System_String* pString = ptext->GetMemberValue<Unity::System_String*>("text");
    if (!pString)
    {
        return;
    }
    memset(&pString->m_wString, 0x00, (size_t)((size_t)pString->m_iLength * 2 + 1));
    memcpy(&pString->m_wString, newText, static_cast<size_t>(lstrlenW(newText)) * 2 + 1);
    ptext->SetMemberValue<Unity::System_String*>("text", pString);
}

void SetupText(const char* path , const char* component ,const wchar_t* wchar)
{
    Unity::CGameObject* pTitleText = Unity::GameObject::Find(path);
    if (!pTitleText)
    {
        return;
    }
    Unity::CComponent* pText = pTitleText->GetComponent(component);
    if (!pText)
    {
        return;
    }
    SetText(pText, wchar);
}

inline std::wstring String2WString(std::string& str)
{
    int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length() + 1, 0, 0);
    wchar_t* wchar = new wchar_t[length + 1];
    memset(wchar, 0x00, (size_t)((length * 2) + 1));
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wchar, length);
    std::wstring ws(wchar);
    delete[] wchar;
    return ws;
}

void throwError(HMODULE hModule,const char* errorText)
{
    MessageBoxA(0, errorText, "VRChat - MultiLanguage - Error", 0);
    FreeLibraryAndExitThread(hModule, 0x0);
    return;
}

void ThreadInit(HMODULE hModule)
{
    while (!GetModuleHandleA("GameAssembly.dll"))
    {
        Sleep(10000);
    }
    
    IL2CPP::Initialize();

    Unity::CGameObject* pTestObj = Unity::GameObject::Find("UserInterface/Canvas_QuickMenu(Clone)");
    while (!pTestObj)
    {
        pTestObj = Unity::GameObject::Find("UserInterface/Canvas_QuickMenu(Clone)");
        Sleep(5000);
    }

    std::ifstream in("Language.json");
    json parser;
    if (!in.is_open())
    {
        throwError(hModule, "Unable to Open the Language.json");
        return;
    }
    if (!in.good())
    {
        throwError(hModule, "Bad Reading Language.json");
        return;
    }
    in >> parser;

    size_t object_count = parser["Objects"].size();
    if (object_count == 0)
    {
        throwError(hModule, "Objects Count == 0");
        return;
    }
    for (size_t i = 0; i < object_count ; i++)
    {
        std::string str = parser["Objects"][i]["lang"].get<std::string>();
        std::string path = parser["Objects"][i]["path"];
        std::string component = parser["Objects"][i]["component"];
        SetupText(path.c_str(), component.c_str(), String2WString(str).c_str());
    }
    //SetupText("UserInterface/MenuContent/Screens/Settings/TitlePanel/VersionText", "Text", (wchar_t*)L"https://github.com/ExtremeBlackLiu/VRChatMultiLanguage");
    in.close();
    FreeLibraryAndExitThread(hModule, 0x0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
         VersionHijack::Initialize();
         DisableThreadLibraryCalls(hModule);
         HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadInit, hModule, 0, 0);
         break;
    }
    return TRUE;
}
