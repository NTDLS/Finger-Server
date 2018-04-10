#ifndef _MAINDLG_CPP
#define _MAINDLG_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <CommCTRL.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "../Source/Entry.H"

#include "../Dialogs/MainDlg.H"
#include "../Dialogs/NetLogoDlg.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::Windows;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------(Variable Declarations)
HWND hMainDialog = NULL;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK MainDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hTotalConnect = NULL;
	static HWND hLastConnect = NULL;

	//--------------------------------------------------------------------------

	if(uMsg == WM_INITDIALOG)
    {
        hMainDialog = hWnd;

        HMENU hMenu = GetSystemMenu(hWnd, FALSE);
        AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
        AppendMenu(hMenu, MF_STRING, MAINDIALOG_MENU_ABOUT, "About");

        SendMessage(hWnd, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(hWnd, WM_SETICON, TRUE, (LPARAM) LoadIcon(ghAppInstance, MAKEINTRESOURCE(IDI_MAIN)));

		hTotalConnect = GetDlgItem(hWnd, IDC_TOTALCONNECT);
		hLastConnect  = GetDlgItem(hWnd, IDC_LASTCONNECT);

		CheckDlgButton(hWnd, IDC_MACHINENAME, bMachineName);
		CheckDlgButton(hWnd, IDC_USERNAME, bUserName);
		CheckDlgButton(hWnd, IDC_LOCALDATE, bLocalDate);
		CheckDlgButton(hWnd, IDC_UTCDATE, bUTCDate);
		CheckDlgButton(hWnd, IDC_HITCOUNT, bHitCount);

		SendMessage(hWnd, WM_TIMER, 0, 0);

		SetTimer(hWnd, 0, 1000, NULL);

		CenterWindow(hWnd);

        return TRUE; // Return TRUE to set the keyboard focus, Otherwise return FALSE.
    }

	//--------------------------------------------------------------------------

    else if(uMsg == WM_TIMER)
    {
		gSock.Lock();
		int iTotalConnections = gSock.NextPeerID();
		if(iTotalConnections > 0)
		{
			char sDate[128];
			char sTime[128];

			if(GetTimeFormat(LOCALE_USER_DEFAULT, 0, &gLastConnect, NULL, sDate, sizeof(sDate)) &&
				GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &gLastConnect, NULL, sTime, sizeof(sTime)))
			{
				char sDateTime[256];
				sprintf_s(sDateTime, sizeof(sDateTime), "%s %s", sDate, sTime);
				Set_Text(hLastConnect, sDateTime);
			}
		}
		else {
			Set_Text(hLastConnect, "n/a");
		}

		Set_Long(hTotalConnect, iTotalConnections);
		gSock.Unlock();
	}

	//--------------------------------------------------------------------------

    else if(uMsg == WM_COMMAND)
    {
        if(wParam == IDC_OK)
        {
			bMachineName = IsDlgButtonChecked(hWnd, IDC_MACHINENAME) > 0;
			bUserName = IsDlgButtonChecked(hWnd, IDC_USERNAME) > 0;
			bLocalDate = IsDlgButtonChecked(hWnd, IDC_LOCALDATE) > 0;
			bUTCDate = IsDlgButtonChecked(hWnd, IDC_UTCDATE) > 0;
			bHitCount = IsDlgButtonChecked(hWnd, IDC_HITCOUNT) > 0;

			hMainDialog = NULL;
            EndDialog(hWnd, 0);
			return TRUE;
        }
        else if(wParam == IDC_CANCEL)
        {
			hMainDialog = NULL;
            EndDialog(hWnd, 0);
			return TRUE;
        }
        return FALSE;
    }

    //--------------------------------------------------------------------------

    else if(uMsg == WM_SYSCOMMAND) //- Received a system menu message.
    {
        if(LOWORD(wParam) == MAINDIALOG_MENU_ABOUT) //- About.
        {
            _AboutDialogInfo ADI;
    
            ADI.DisplayIcon  = LoadIcon(ghAppInstance, MAKEINTRESOURCE(IDI_MAIN));
            ADI.TitleCaption = gsTitleCaption;
            ADI.FileVersion  = gsFileVersion;
            ADI.BuildDate    = __DATE__;
            ADI.BuildTime    = __TIME__;
            ADI.CopyRight    = gsAppCopyRight;
            ADI.OwnerHandle  = hWnd;

            NetLogo(&ADI);

            return TRUE;
        }

        return FALSE;
    }

    //--------------------------------------------------------------------------

    else if(uMsg == WM_CLOSE) //- Received a close message.
    {
		hMainDialog = NULL;
		EndDialog(hWnd, 0);
		return TRUE;
    }

    //--------------------------------------------------------------------------

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
