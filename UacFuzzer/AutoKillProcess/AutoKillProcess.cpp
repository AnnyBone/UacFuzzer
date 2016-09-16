#include "stdafx.h"

#include <windows.h>

#define DATA_SIZE 1024
#define  AUTOELEVATE_COUNT 57

int _tmain(int argc, _TCHAR* argv[])
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
	CHAR* lpStr = NULL;
	
	while (true)
	{
		Sleep(1);

		for (int i = 0; i < AUTOELEVATE_COUNT; i++)
		{
			memset(szSysCmd, 0, DATA_SIZE);
			lpStr = strstr(szAutoElevateExe[i], "\\");

			if (lpStr == NULL)
			{
				lpStr = szAutoElevateExe[i];
			}
			else
			{
				lpStr = lpStr + 1;	
			}
			
			sprintf_s(szSysCmd, DATA_SIZE,"taskkill /f /im %s", lpStr);

			system(szSysCmd);
		}
	}
	
	return 0;
}

