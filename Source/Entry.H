#ifndef _ENTRY_H
#define _ENTRY_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../../../@Libraries/Debug/Debug.H"
#include "../../../../@Libraries/NSWFL/NSWFL.H"

#include "../../../../@Libraries/CTrayIcon/CTrayIcon.H"
#include "../../../../@Libraries/CSocketServer/CSocketServer.H"

#include "../Resources/Resource.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern HINSTANCE ghAppInstance;

extern char gsAppCopyRight[64];
extern char gsFileVersion[10];
extern char gsTitleCaption[64];
extern char gsRegistryKey[255];

extern CTrayIcon gTrayIcon;
extern CSocketServer gSock;

extern bool bMachineName;
extern bool bUserName;
extern bool bLocalDate;
extern bool bUTCDate;
extern bool bHitCount;

extern SYSTEMTIME gLastConnect;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

