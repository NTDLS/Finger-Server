#ifndef _INIT_CPP
#define _INIT_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <WindowsX.H>
#include <ShellAPI.H>
#include <Stdio.H>
#include <Stdlib.H>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Init.H"
#include "Entry.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::Registry;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool InitializeApp(void)
{
	LoadString(ghAppInstance, IDS_APPCOPYRIGHT, gsAppCopyRight, sizeof(gsAppCopyRight));
    LoadString(ghAppInstance, IDS_FILEVERSION, gsFileVersion, sizeof(gsFileVersion));
    LoadString(ghAppInstance, IDS_TITLECAPTION, gsTitleCaption, sizeof(gsTitleCaption));
    LoadString(ghAppInstance, IDS_REGISTRYKEY, gsRegistryKey, sizeof(gsRegistryKey));

	bMachineName = Get_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "MachineName");
	bUserName = Get_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "UserName");
	bLocalDate = Get_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "LocalDate");
	bUTCDate = Get_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "UTCDate");
	bHitCount = Get_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "HitCount");

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool UninitializeApp(void)
{
	Set_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "MachineName", bMachineName);
	Set_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "UserName", bUserName);
	Set_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "LocalDate", bLocalDate);
	Set_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "UTCDate", bUTCDate);
	Set_BOOLRegistryValue(HKEY_CURRENT_USER, gsRegistryKey, "HitCount", bHitCount);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

