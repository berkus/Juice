//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// Juice.cpp, tk 1996
//
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#include "windows.h"

#ifndef WIN32
    #include "string.h"
#endif

#ifndef _NPAPI_H_
#include "npapi.h"
#endif


//------------------------------------------------------------------------------------
// Upcall Mechanism
//------------------------------------------------------------------------------------

void 		(*ucNPP_Shutdown)(void);
NPError		(*ucNPP_New)(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char* argn[], char* argv[], NPSavedData* saved);
NPError		(*ucNPP_Destroy)(NPP instance, NPSavedData** save);
NPError		(*ucNPP_SetWindow)(NPP instance, NPWindow* window);
NPError		(*ucNPP_NewStream)(NPP instance, NPMIMEType type, NPStream *stream, NPBool seekable, uint16 *stype);
int32		(*ucNPP_WriteReady)(NPP instance, NPStream *stream);
int32		(*ucNPP_Write)(NPP instance, NPStream *stream, int32 offset, int32 len, void *buffer);
NPError	 	(*ucNPP_DestroyStream)(NPP instance, NPStream *stream, NPError reason);
void		(*ucNPP_StreamAsFile)(NPP instance, NPStream *stream, const char* fname);
int16 		(*ucNPP_HandleEvent)(NPP instance, void* event);
void		(*ucNPP_Print)(NPP instance, NPPrint* printInfo);
LONG		(*ucNPP_WindowHandler)(HWND hWnd, WORD Message, WORD wParam, LONG lParam);

//------------------------------------------------------------------------------------
// Oberon Definitions and Globals
//------------------------------------------------------------------------------------

#include <Math.h>
#include <stdio.h>
#include <signal.h>

#define heapSize 1*1024*1024

typedef void(*Proc)(void);
typedef long int LONGINT;

HFILE	bootFile;
LONGINT	bootResource;
BOOL	fromResource;
LPVOID	memoryAdr;
LONGINT	heapAdr;
char	Inifile[128] = "Juice.ini";

typedef void (*TrapHandler)(LPEXCEPTION_POINTERS p);
TrapHandler HandleTrap = (TrapHandler)NULL;     // address of Oberon trap handler

BOOL	console;
HANDLE	consOut, consIn;
LONGINT	regESI, regEBP, regEDI, regEBX, regESP, regEDX;

#define SetupOberonCall()	{ __asm mov regEDX,edx __asm mov regESP,esp  __asm mov regEBP,ebp  __asm mov regEBX,ebx  __asm mov regEDI,edi  __asm mov regESI,esi }
#define RestoreOberonCall()	{ __asm mov esi,regESI  __asm mov edi,regEDI  __asm mov ebx,regEBX  __asm mov ebp,regEBP  __asm mov esp,regESP __asm mov edx,regEDX }

HINSTANCE DLLInstance;

#ifdef WIN32

BOOL WINAPI
DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    DLLInstance = hDLL;
    return 1;
}

#else

int CALLBACK
LibMain(HINSTANCE hinst, WORD wDataSeg, WORD cbHeap, LPSTR lpszCmdLine)
{
    DLLInstance = hinst;
    return 1;
}

#endif

void OpenConsole(void)
{
        if (console = AllocConsole()) {
                SetConsoleTitle("Juice Console");
                consOut = GetStdHandle(STD_OUTPUT_HANDLE);
                consIn = GetStdHandle(STD_INPUT_HANDLE);
        }
}

void print (char *format, ...) {
        static char buf[128];
        DWORD       nofChars;
        DWORD       written;
        va_list     arguments;

#ifdef _DEBUG
        if (console) {
                va_start(arguments, format);
                nofChars = _vsnprintf(buf, 128, format, arguments);
                if (nofChars > 128) nofChars = 128;
                WriteConsole(consOut, buf, nofChars, &written, NULL);
        }
#endif
}

void __stdcall write(LONGINT data)
{
	print("%s", data);
}

int MyFilterFunction (LPEXCEPTION_POINTERS p)
{
        //print("Exception\n");
        if (NULL != HandleTrap) HandleTrap(p);
        return (EXCEPTION_EXECUTE_HANDLER);
}


static void __stdcall InstallUpCall(char* name, LONGINT *proc)
{
	print("Upcalls %s : %d\n", name, *proc);
	if (strcmp(name, "NPP_Shutdown")==0)      
		{ ucNPP_Shutdown = (void (*)(void))*proc; return; }
	if (strcmp(name, "NPP_New")==0)           
		{ ucNPP_New = (NPError (*)(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char* argn[], char* argv[], NPSavedData* saved))*proc; return; }
	if (strcmp(name, "NPP_Destroy")==0)       
		{ ucNPP_Destroy = (NPError (*)(NPP instance, NPSavedData** save))*proc; return; }
	if (strcmp(name, "NPP_SetWindow")==0)     
		{ ucNPP_SetWindow = (NPError (*)(NPP instance, NPWindow* window))*proc; return; }
	if (strcmp(name, "NPP_NewStream")==0)     
		{ ucNPP_NewStream = (NPError (*)(NPP instance, NPMIMEType type, NPStream *stream, NPBool seekable, uint16 *stype))*proc; return; }
	if (strcmp(name, "NPP_WriteReady")==0)    
		{ ucNPP_WriteReady = (int32 (*)(NPP instance, NPStream *stream))*proc; return; }
	if (strcmp(name, "NPP_Write")==0)         
		{ ucNPP_Write = (int32 (*)(NPP instance, NPStream *stream, int32 offset, int32 len, void *buffer))*proc; return; }
	if (strcmp(name, "NPP_DestroyStream")==0) 
		{ ucNPP_DestroyStream = (NPError (*)(NPP instance, NPStream *stream, NPError reason))*proc; return; }
	if (strcmp(name, "NPP_StreamAsFile")==0)  
		{ ucNPP_StreamAsFile = (void (*)(NPP instance, NPStream *stream, const char* fname))*proc; return; }
	if (strcmp(name, "NPP_HandleEvent")==0)   
		{ ucNPP_HandleEvent = (int16 (*)(NPP instance, void* event))*proc; return; }
	if (strcmp(name, "NPP_Print")==0)         
		{ ucNPP_Print = (void (*)(NPP instance, NPPrint* printInfo))*proc; return; }
	if (strcmp(name, "NPP_WindowHandler")==0)
		{ ucNPP_WindowHandler = (LONG (*)(HWND hWnd, WORD Message, WORD wParam, LONG lParam))*proc; return; }
}


//----------------------------------------------------------------------------
// Windows Window Handler:.
//----------------------------------------------------------------------------

LONG NP_LOADDS WINAPI WindowHandler(HWND hWnd, WORD Message, WORD wParam, LONG lParam)
{
	LONG res;
__try {
	if (ucNPP_WindowHandler != NULL) res = ucNPP_WindowHandler(hWnd, Message, wParam, lParam);
	else res = 0;
}
__except (MyFilterFunction(GetExceptionInformation())) {};

print("%ld\n", res);
return res;
}


void __stdcall GetAdr (LONGINT *adr, char *ident, LONGINT handle)
{
	if (handle != 0) {
		*adr = (LONGINT)GetProcAddress((HMODULE)handle, ident);
    }
    else {
		if (strcmp("heapAdr", ident) == 0) { *adr = (LONGINT)heapAdr; return; }
        else if (strcmp("heapSize", ident) == 0) { *adr = (LONGINT)heapSize; return; }
        else if (strcmp("Inifile", ident) == 0) { *adr = (LONGINT)Inifile; return; }
        else if (strcmp("system", ident) == 0) { *adr = (LONGINT)system; return; }
		else if (strcmp("Debug", ident) == 0) { *adr = (LONGINT)write; return; }
		else if (strcmp("HandleTrap", ident) == 0) { *adr = (LONGINT)&(HandleTrap); return; }

        else if (strcmp("sqrt", ident) == 0) { *adr = (LONGINT)sqrt; return; }
        else if (strcmp("exp", ident) == 0) { *adr = (LONGINT)exp; return; }
        else if (strcmp("ln", ident) == 0) { *adr = (LONGINT)log; return; }
        else if (strcmp("sin", ident) == 0) { *adr = (LONGINT)sin; return; }
        else if (strcmp("cos", ident) == 0) { *adr = (LONGINT)cos; return; }
        else if (strcmp("arctan", ident) == 0) { *adr = (LONGINT)atan; return; } //atan2

        else if (strcmp("LoadLibrary", ident) == 0) { *adr = (LONGINT)LoadLibraryA; return; }
        else if (strcmp("FreeLibrary", ident) == 0) { *adr = (LONGINT)FreeLibrary; return; }
        //else if (strcmp("GetModuleHandle", ident) == 0) { *adr = (LONGINT)LoadLibraryA; return; }
        //GetModuleHandleA;

		else if (strcmp(ident, "NPN_Version") == 0) { *adr = (LONGINT)NPN_Version; return; }
		else if (strcmp(ident, "NPN_GetURL") == 0) { *adr = (LONGINT)NPN_GetURL; return; }
		else if (strcmp(ident, "NPN_PostURL") == 0) { *adr = (LONGINT)NPN_PostURL; return; }
		else if (strcmp(ident, "NPN_RequestRead") == 0) { *adr = (LONGINT)NPN_RequestRead; return; }
		else if (strcmp(ident, "NPN_NewStream") == 0) { *adr = (LONGINT)NPN_NewStream; return; }
		else if (strcmp(ident, "NPN_Write") == 0) { *adr = (LONGINT)NPN_Write; return; }
		else if (strcmp(ident, "NPN_DestroyStream") == 0) { *adr = (LONGINT)NPN_DestroyStream; return; }
		else if (strcmp(ident, "NPN_Status") == 0) { *adr = (LONGINT)NPN_Status; return; }
		else if (strcmp(ident, "NPN_MemAlloc") == 0) { *adr = (LONGINT)NPN_MemAlloc; return; }
		else if (strcmp(ident, "NPN_MemFree") == 0) { *adr = (LONGINT)NPN_MemFree; return; }
		else if (strcmp(ident, "NPN_MemFlush") == 0) { *adr = NULL; return; } // (LONGINT)NPN_MemFlush;
		else if (strcmp(ident, "NPN_WindowHandler") == 0) { *adr = (LONGINT)WindowHandler; 
			return; 
		}

		else if (strcmp(ident, "NPP_UpCall")==0) { *adr = (LONGINT)InstallUpCall; return; }

		else { print("%s not found", ident); /* not found */ };
    };
}

static void ReadLong (LONGINT *x)
{
	if (fromResource) {
		*x = *((long*)bootResource);
		bootResource += 4;
	}
	else _hread(bootFile, (LPVOID)x, 4);
}

void ReadNum (LONGINT *x)
{
	LONGINT n, shift;
    unsigned char ch;

    shift = 0; n = 0; 
	if (fromResource) ch = *((unsigned char*)bootResource++);
	else _hread(bootFile, &ch, 1);
    while (ch >= 128) {
        n += (ch & 0x7F) << shift;
        shift += 7;
		if (fromResource) ch = *((unsigned char*)bootResource++);
		else _hread(bootFile, &ch, 1);
    }
    *x = n + (((ch & 0x3f) - ((ch >> 6) << 6)) << shift);
}

void Relocate (LONGINT heapAdr, LONGINT shift)
{
	LONGINT len, adr;

    ReadNum(&len);
    while (len != 0) {
        ReadNum(&adr);
        adr += heapAdr;
        *((LONGINT *)adr) += shift;
        len--;
    }
}

static NPError Boot(void)
{
    LONGINT	adr, len, shift, d, i, fileHeapAdr, fileHeapSize;
    LONGINT	dlsymAdr;
    Proc    body;
    OFSTRUCT  dummy;
	HANDLE	hDLL;
	HRSRC	hRes;
	HRSRC	hResLoad;

    bootFile = OpenFile("Juice.hex", &dummy, OF_READ);
    if (bootFile == HFILE_ERROR) { 
		print("bootfile not found\n");
		hDLL = LoadLibrary("npJuice.dll");
		if (hDLL == NULL) return NPERR_INVALID_PLUGIN_ERROR;

		hRes = FindResource(hDLL, "#102", "#102");
		if (hRes == NULL) return NPERR_INVALID_PLUGIN_ERROR;

		hResLoad = LoadResource(hDLL, hRes);
		if (hResLoad == NULL) return NPERR_INVALID_PLUGIN_ERROR;

		bootResource = (LONGINT)LockResource(hResLoad);
		fromResource = TRUE;

	}
	else fromResource = FALSE;

    ReadLong(&fileHeapAdr); ReadLong(&fileHeapSize);

    // memoryAdr = VirtualAlloc(NULL, heapSize, MEM_COMMIT, PAGE_READWRITE);
	memoryAdr = NPN_MemAlloc(heapSize);
    if (memoryAdr == NULL) return NPERR_OUT_OF_MEMORY_ERROR; 

    heapAdr = (LONGINT)memoryAdr;
    heapAdr += (-heapAdr) & 0x1F;

    d = heapAdr; i = fileHeapSize + 32;
    while (i > 0) {
		*((LONGINT *) d) = 0;
        i -= 4; d += 4;
    }

    shift = heapAdr - fileHeapAdr;
    ReadLong(&adr); ReadLong(&len);

    while (len != 0) {
		adr += shift;
        if (fromResource) {
			memcpy((LPVOID)adr, (LPVOID)bootResource, len);
			bootResource += len;
		}
		else _hread(bootFile, (LPVOID)adr, len);
        ReadLong(&adr); ReadLong(&len);
    }

    body = (Proc)(adr + shift);
    Relocate(heapAdr, shift);
    ReadLong(&dlsymAdr);

    *((LONGINT *)(heapAdr + dlsymAdr)) = (LONGINT)GetAdr;
    if (!fromResource) _lclose(bootFile);

	print("Call\n");
	(*body)();
	print("done\n");

	return NPERR_NO_ERROR;
}

//----------------------------------------------------------------------------
// NPP_Initialize:
//----------------------------------------------------------------------------
NPError NPP_Initialize(void)
{
	NPError	res;

#ifdef _DEBUG
	OpenConsole();
#endif
	print("Initialize\n");

	res = Boot();
	return res;
}


//----------------------------------------------------------------------------
// NPP_Shutdown:
//----------------------------------------------------------------------------
void NPP_Shutdown(void)
{
__try {
	print("Shutdown\n");
	SetupOberonCall();
	if (ucNPP_Shutdown != NULL) ucNPP_Shutdown();
	RestoreOberonCall();
    // if (memoryAdr != 0) VirtualFree(memoryAdr, 0, MEM_RELEASE);
	if (memoryAdr != 0) NPN_MemFree(memoryAdr);
}
__except (MyFilterFunction(GetExceptionInformation())) {};
#ifdef _DEBUG
	FreeConsole();
#endif
}

//----------------------------------------------------------------------------
// NPP_GetJavaClass:
//----------------------------------------------------------------------------
jref 
NPP_GetJavaClass(void)
{
	return NULL;
}

//----------------------------------------------------------------------------
// NPP_New:
//----------------------------------------------------------------------------
NPError NP_LOADDS
NPP_New(NPMIMEType pluginType,
                NPP instance,
                uint16 mode,
                int16 argc,
                char* argn[],
                char* argv[],
                NPSavedData* saved)
{
	NPError	res;

__try {
	print("\nNew %d %d %d\n", instance, mode, ucNPP_New);
	SetupOberonCall();
	if (ucNPP_New != NULL) res = ucNPP_New(pluginType, instance, mode, argc, argn, argv, saved);
	else res = NPERR_MODULE_LOAD_FAILED_ERROR;
	RestoreOberonCall();
	print("Done %d\n", res);
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//-----------------------------------------------------------------------------
// NPP_Destroy:
//-----------------------------------------------------------------------------
NPError NP_LOADDS
NPP_Destroy(NPP instance, NPSavedData** save)
{
	NPError	res;

__try {
	print("Destroy\n");
	SetupOberonCall();
	if (ucNPP_Destroy != NULL) res = ucNPP_Destroy(instance, save);
	else res = NPERR_MODULE_LOAD_FAILED_ERROR;
	RestoreOberonCall();
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//----------------------------------------------------------------------------
// NPP_SetWindow:
//----------------------------------------------------------------------------
NPError NP_LOADDS
NPP_SetWindow(NPP instance, NPWindow* window)
{
	NPError	res;

__try {
	print("set window\n");
	SetupOberonCall();
	if (ucNPP_SetWindow != NULL) res = ucNPP_SetWindow(instance, window);
	else res = NPERR_MODULE_LOAD_FAILED_ERROR;
	RestoreOberonCall();
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//----------------------------------------------------------------------------
// NPP_NewStream:
//----------------------------------------------------------------------------
NPError NP_LOADDS
NPP_NewStream(NPP instance,
              NPMIMEType type,
              NPStream *stream, 
              NPBool seekable,
              uint16 *stype)
{
	NPError	res;

__try {
	print("new stream\n");
	SetupOberonCall();
	if (ucNPP_NewStream != NULL) res = ucNPP_NewStream(instance, type, stream, seekable, stype);
	else res = NPERR_MODULE_LOAD_FAILED_ERROR;
	RestoreOberonCall();
	print("new stream done\n");
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}



//----------------------------------------------------------------------------
// NPP_WriteReady:
//----------------------------------------------------------------------------
int32 NP_LOADDS
NPP_WriteReady(NPP instance, NPStream *stream)
{
	int32	res;

__try {
	print("write ready\n");
	SetupOberonCall();
	if (ucNPP_WriteReady != NULL) res = ucNPP_WriteReady(instance, stream);
	else res = 0;
	RestoreOberonCall();
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//----------------------------------------------------------------------------
// NPP_Write:
//----------------------------------------------------------------------------
int32 NP_LOADDS
NPP_Write(NPP instance, NPStream *stream,
          int32 offset, int32 len, void *buffer)
{
	int32	res;

__try {
	print("write\n");
	SetupOberonCall();
	if (ucNPP_Write != NULL) res = ucNPP_Write(instance, stream, offset, len, buffer);
	else res = 0;
	RestoreOberonCall();
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//----------------------------------------------------------------------------
// NPP_DestroyStream:
//----------------------------------------------------------------------------
NPError NP_LOADDS
NPP_DestroyStream(NPP instance, NPStream *stream, NPError reason)
{
	NPError	res;

__try {
	print("destroy\n");
	SetupOberonCall();
	if (ucNPP_DestroyStream != NULL) res = ucNPP_DestroyStream(instance, stream, reason);
	else res = NPERR_MODULE_LOAD_FAILED_ERROR;
	RestoreOberonCall();
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//----------------------------------------------------------------------------
// NPP_StreamAsFile:
//----------------------------------------------------------------------------
void NP_LOADDS 
NPP_StreamAsFile(NPP instance, NPStream *stream, const char* fname)
{
__try {
	SetupOberonCall();
	if (ucNPP_StreamAsFile != NULL) ucNPP_StreamAsFile(instance, stream, fname);
	RestoreOberonCall();
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//----------------------------------------------------------------------------
// NPP_Print:
//----------------------------------------------------------------------------
void NP_LOADDS 
NPP_Print(NPP instance, NPPrint* printInfo)
{
  print("print\n");
__try {
	SetupOberonCall();
	if (ucNPP_Print != NULL) ucNPP_Print(instance, printInfo);
	RestoreOberonCall();
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}


//----------------------------------------------------------------------------
// NPP_HandleEvent:
// Mac-only.
//----------------------------------------------------------------------------
int16 NPP_HandleEvent(NPP instance, void* event)
{
	int16	res;

__try {
	SetupOberonCall();
	if (ucNPP_HandleEvent != NULL) res = ucNPP_HandleEvent(instance, event);
	else res = 0;
	RestoreOberonCall();
	return res;
}
__except (MyFilterFunction(GetExceptionInformation())) {};
}






