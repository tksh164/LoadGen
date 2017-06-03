//
// Build: cl.exe /Zi loadgen.cpp
//

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "winmm.lib")

#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wchar.h>
#include <wincrypt.h>

// Functions
VOID DoWorkload();
VOID BusyWorkload(DWORD busyMilliSec);
VOID SleepWorkload(DWORD sleepMilliSec);
BOOL GetRandomValue(PDWORD r, DWORD n);

// Const
const DWORD BUSY_MSEC_RANGE_MIN = 1000;
const DWORD BUSY_MSEC_RANGE_MAX = 11000;
const DWORD SLEEP_MSEC_RANGE_MIN = 1000;
const DWORD SLEEP_MSEC_RANGE_MAX = 11000;

INT wmain(INT argc, WCHAR *argv[])
{
    DoWorkload();
    return 0;
}

VOID DoWorkload()
{
    while (TRUE)
    {
        // Busy

        DWORD milliSec;
        if (!GetRandomValue(&milliSec, BUSY_MSEC_RANGE_MAX))
        {
            wprintf(L"Failed\n");
            return;
        }

        DWORD busyMilliSec = BUSY_MSEC_RANGE_MIN + milliSec;
        wprintf(L"Busy: %d\n", busyMilliSec);
        BusyWorkload(busyMilliSec);

        // Sleep

        if (!GetRandomValue(&milliSec, SLEEP_MSEC_RANGE_MAX))
        {
            wprintf(L"Failed\n");
            return;
        }

        DWORD sleepMilliSec = SLEEP_MSEC_RANGE_MIN + milliSec;
        wprintf(L"Wait: %d\n", sleepMilliSec);
        Sleep(sleepMilliSec);
    }
}

VOID BusyWorkload(DWORD busyMilliSec)
{
    INT tmp = 0;
    DWORD startMilliSec = timeGetTime();
    while (TRUE)
    {
        tmp = !tmp;
        if (timeGetTime() >= startMilliSec + busyMilliSec) break;
    }
}

VOID SleepWorkload(DWORD sleepMilliSec)
{
    Sleep(sleepMilliSec);
}

BOOL GetRandomValue(PDWORD r, DWORD n)
{
    HCRYPTPROV cspHandle = NULL;
    if (!CryptAcquireContext(&cspHandle, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        wprintf(L"Failed: CryptAcquireContext function with 0x%08X\n", GetLastError());
        return FALSE;
    }

    BOOL returnValue = TRUE;
    DWORD randomData;
    if (!CryptGenRandom(cspHandle, sizeof(randomData), (BYTE *)&randomData))
    {
        wprintf(L"Failed: CryptGenRandom function with 0x%08X\n", GetLastError());
        returnValue = FALSE;
    }

    if (!CryptReleaseContext(cspHandle, 0))
    {
        wprintf(L"Failed: CryptReleaseContext function with 0x%08X\n", GetLastError());
        returnValue = FALSE;
    }

    if (returnValue) *r = randomData % n;
    return returnValue;
}
