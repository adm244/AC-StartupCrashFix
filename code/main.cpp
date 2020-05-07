/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

//IMPORTANT(adm244): SCRATCH VERSION JUST TO GET IT UP WORKING

#include <windows.h>
#include <Shlobj.h>
#include <assert.h>
#include <stdio.h>

#include "types.h"
#include "detours.cpp"
#include "utils/win32_missingapi.cpp"

//TODO(adm244): support for DX9 version
//TODO(adm244): support for STEAM version

// ------------------ Actual Code -----------------
typedef void (STDCALL *Engine_Init_t)(void *engineSettings);
internal Engine_Init_t Engine_Init = 0;

typedef void (CDECL *ParseCommandLineOptions_t)(int argc, char **argv, void *engineSettings);
internal ParseCommandLineOptions_t ParseCommandLineOptions = (ParseCommandLineOptions_t)0x004089F0;

internal void *g_HookAddress = (void *)0x00405410;

internal void STDCALL Hook_Engine_Init(void *engineSettings)
{
  int argc = 0;
  char **argv = CommandLineToArgvA(GetCommandLineA(), &argc);
  ParseCommandLineOptions(argc, argv, engineSettings);
  LocalFree(argv);
  
  Engine_Init(engineSettings);
}

internal bool HookEngineInit()
{
  // 00405410 | E8 DBDE4F00 | call Engine::Init
  Engine_Init = (Engine_Init_t)RIPRel32(g_HookAddress, 1);
  assert(Engine_Init != 0);
  
  return WriteCallRel32(g_HookAddress, Hook_Engine_Init);
}
// ----------------- #Actual Code -----------------

// -------------------- Loader --------------------
typedef HRESULT __stdcall DInput8CreateFunc(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);
internal HMODULE dinput8 = 0;

internal HMODULE LoadDInput8()
{
  const char *libname = "\\dinput8.dll";
  
  char libpath[MAX_PATH];
  HRESULT result = SHGetFolderPathA(0, CSIDL_SYSTEM, 0, SHGFP_TYPE_CURRENT, libpath);
  assert(result == S_OK);
  
  strcat_s(libpath, MAX_PATH, libname);
  
  return LoadLibraryA(libpath);
}

extern "C" HRESULT __stdcall
FakeDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter)
{
  if( !dinput8 ) {
    dinput8 = LoadDInput8();
    assert(dinput8 != 0);
  }
  
  DInput8CreateFunc *DInput8Create = (DInput8CreateFunc *)GetProcAddress(dinput8, "DirectInput8Create");
  return DInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

internal BOOL WINAPI DllMain(HMODULE loader, DWORD reason, LPVOID reserved)
{
  if (reason == DLL_PROCESS_ATTACH) {
    assert(HookEngineInit() == true);
  }
  
  return TRUE;
}
// ------------------- #Loader --------------------
