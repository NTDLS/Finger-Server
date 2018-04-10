#ifndef _ENTRY_CPP
#define _ENTRY_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <Stdio.H>
#include <Stdlib.H>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Entry.H"
#include "Init.H"

#include "../Dialogs/MainDlg.H"
#include "../../../../@Libraries/CStringBuilder/CStringBuilder.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::System;
using namespace NSWFL::String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HINSTANCE ghAppInstance = NULL;

char gsAppCopyRight[64];
char gsFileVersion[10];
char gsTitleCaption[64];
char gsRegistryKey[255];

CTrayIcon gTrayIcon;
CSocketServer gSock;
SYSTEMTIME gLastConnect;

bool bMachineName = true;
bool bUserName = true;
bool bLocalDate = true;
bool bUTCDate = true;
bool bHitCount = true;

#define ID_TRAY_DETAILS	WM_USER + 1
#define ID_TRAY_CANCEL	WM_USER + 2
#define ID_TRAY_CLOSE	WM_USER + 3

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ProgressTrayIconProc(CTrayIcon *lpTI, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CTRAYICON_INITIALIZE)
	{
	}
	else if(uMsg == WM_COMMAND)
    {
		if(LOWORD(wParam) == ID_TRAY_CLOSE)
        {
			gTrayIcon.Destroy();
		}
		else if(LOWORD(wParam) == ID_TRAY_DETAILS)
        {
			if(hMainDialog)
			{
				SetForegroundWindow(hMainDialog);
				SetFocus(hMainDialog);
			}
			else{
				DialogBox(ghAppInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDialog);
			}
        }
    }
    else if(uMsg == WM_SHELLNOTIFY)
    {
		if(lParam == WM_LBUTTONDBLCLK)
		{
			if(hMainDialog)
			{
				SetForegroundWindow(hMainDialog);
				SetFocus(hMainDialog);
			}
			else{
				DialogBox(ghAppInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDialog);
			}
		}
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SockOnError(CSocketServer *pSock, CSocketClient *pClient, int iErrorNumber, const char *sErrorMsg)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SockClientHandlerThread(CSocketServer *pSock, CSocketClient *pClient, LPBASICHUNK pChunk)
{
	int iLoopCount = 0; //Rudimentary DOS prevention, will only stay connected for 1 minute.

	pSock->Lock();
	memset(&gLastConnect, 0, sizeof(gLastConnect));
	GetLocalTime(&gLastConnect);
	pSock->Unlock();

	while(pClient->IsConnected() && iLoopCount++ < 6000)
	{
		if(pClient->GetClientData(pChunk))
		{
			CStringBuilder string;
			int iRPos = InStrI("/W", (char *)pChunk->pBuffer, pChunk->iLength, 0);
			if(iRPos >= 0)
			{
				iRPos += 2; //Skip "/W"
				SkipWhiteSpaces((char *)pChunk->pBuffer, pChunk->iLength, &iRPos);

				int iWPos = 0;
				char sAskedFor[1024];

				for(;iRPos < pChunk->iLength && iWPos < sizeof(sAskedFor) && ((char *)pChunk->pBuffer)[iRPos] != '\r' && ((char *)pChunk->pBuffer)[iRPos] != '\n'; iRPos++)
				{
					sAskedFor[iWPos++] = ((char *)pChunk->pBuffer)[iRPos];
				}
				sAskedFor[iWPos] = '\0';

				if(iWPos > 0)
				{
					string.AppendF("Query: %s\r\n", sAskedFor);
				}
			}

			if(bMachineName)
			{
				char sMachineName[1024];
				if(Get_MachineName(sMachineName, sizeof(sMachineName)))
				{
					string.AppendF("Machine: %s\r\n", sMachineName);
				}
			}

			if(bUserName)
			{
				char sUsername[1024];
				if(Get_UserName(sUsername, sizeof(sUsername)))
				{
					string.AppendF("Username: %s\r\n", sUsername);
				}
			}

			if(bLocalDate)
			{
				SYSTEMTIME st;
				memset(&st, 0, sizeof(st));
				GetLocalTime(&st);

				char sDate[128];
				char sTime[128];

				if(GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, sDate, sizeof(sDate)) &&
					GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, sTime, sizeof(sTime)))
				{
					string.AppendF("LocalTime: %s %s\r\n", sDate, sTime);
				}
			}

			if(bUTCDate)
			{
				SYSTEMTIME st;
				memset(&st, 0, sizeof(st));
				GetSystemTime(&st);

				char sDate[128];
				char sTime[128];

				if(GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, sDate, sizeof(sDate)) &&
					GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, sTime, sizeof(sTime)))
				{
					string.AppendF("UTC: %s %s\r\n", sDate, sTime);
				}

			}

			if(bHitCount)
			{
				string.AppendF("HitCount: %d\r\n", pSock->NextPeerID());
			}

			if(string.Length == 0)
			{
				string.AppendF("NetworkDLS %s version %s\r\n", gsTitleCaption, gsFileVersion);
			}

			pClient->SetNextSendData(string.ToString());
			pClient->WaitOnDataToBeSent();

			return;
		}
		Sleep(10);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int AppEntry(HINSTANCE hInstance)
{
	HANDLE hMutex = NULL;

	if( (hMutex = OpenMutex(SYNCHRONIZE, FALSE, gsTitleCaption)) )
	{
		MessageBox(NULL, "Only one instance can run at a time!",
			gsTitleCaption, MB_ICONASTERISK | MB_SYSTEMMODAL);

		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
		return 0;
	}
	else hMutex = CreateMutex(0, 0, gsTitleCaption);

	//--------------------------------------------------------------------------

	if(gSock.Initialize(79, 10, &SockClientHandlerThread, &SockOnError))
	{
		gSock.PacketMode(false);

		if(gSock.Start())
		{
			HMENU hPopupMenu = CreateMenu();
			AppendMenu(hPopupMenu, MF_STRING, ID_TRAY_DETAILS, "&Details");
			AppendMenu(hPopupMenu, MF_STRING, ID_TRAY_CANCEL, "Ca&ncel");
			AppendMenu(hPopupMenu, MF_SEPARATOR, MF_SEPARATOR, "");
			AppendMenu(hPopupMenu, MF_STRING, ID_TRAY_CLOSE, "&Shutdown");

			gTrayIcon.Create(IDI_MAIN, gsTitleCaption, hPopupMenu, &ProgressTrayIconProc);

			gSock.Stop(true);
			gSock.Destroy();
		}
	}

	//--------------------------------------------------------------------------

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
int main(int argc, char *argv[])
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR CmdLine, int CmdShow)
#endif
{
    #ifdef _DEBUG
	ghAppInstance = GetModuleHandle(NULL);
    #else
	ghAppInstance = hInstance;
    #endif

	int iResult = 0;

	if(InitializeApp())
    {
		iResult = AppEntry(ghAppInstance);
    }

    UninitializeApp();

	return iResult;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

