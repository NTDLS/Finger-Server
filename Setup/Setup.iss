[Setup]
;-- Main Setup Information
 AppName                = Finger Server
 AppVerName             = Finger Server 1.0.0.1
 AppCopyright           = Copyright © 1995-2010 NetworkDLS.
 DefaultDirName         = {pf}\NetworkDLS\Finger Server
 DefaultGroupName       = NetworkDLS\Finger Server
 UninstallDisplayIcon   = {app}\Finger.Exe
 WizardImageFile        = ..\..\..\..\@Resources\Setup\LgSetup.bmp
 WizardSmallImageFile   = ..\..\..\..\@Resources\Setup\SmSetup.bmp
 PrivilegesRequired     = PowerUser
 Uninstallable          = Yes
 AppMutex               = Finger Server
 LicenseFile            = ..\..\..\..\@Resources\Setup\EULA.txt
 Compression            = ZIP/9
 OutputBaseFilename     = Setup
 MinVersion             = 0.0,5.0

;-- Windows 2000 & XP (Support Dialog)
 AppPublisher    = NetworkDLS
 AppPublisherURL = http://www.NetworkDLS.com/
 AppUpdatesURL   = http://www.NetworkDLS.com/
 AppVersion      = 1.0.0.1

[Tasks]
 Name: RunAtStart;        Description: "Run at &startup?"; GroupDescription: "Additional icons:"; Flags: unchecked
 Name: RunAtStart\common; Description: "For all &users";   GroupDescription: "Additional icons:"; Flags: unchecked exclusive
 Name: RunAtStart\user;   Description: "For me &only";     GroupDescription: "Additional icons:"; Flags: unchecked exclusive

[Files]
 Source: "..\Release\Finger.Exe"; DestDir: "{app}";

[Icons]
 Name: "{group}\Finger Server";         Filename: "{app}\Finger.Exe";
 Name: "{commonstartup}\Finger Server"; Filename: "{app}\Finger.Exe"; Tasks: RunAtStart\common
 Name: "{userstartup}\Finger Server";   Filename: "{app}\Finger.Exe"; Tasks: RunAtStart\User

[Registry]
 Root: HKLM; Subkey: "Software\NetworkDLS\Finger Server"; Flags: uninsdeletekey noerror;

 Root: HKCU; Subkey: "Software\NetworkDLS\Finger Server"; ValueType: Dword; ValueName: "MachineName"; ValueData:"1"; Flags: CreateValueIfDoesntExist;
 Root: HKCU; Subkey: "Software\NetworkDLS\Finger Server"; ValueType: Dword; ValueName: "UserName";    ValueData:"1"; Flags: CreateValueIfDoesntExist;
 Root: HKCU; Subkey: "Software\NetworkDLS\Finger Server"; ValueType: Dword; ValueName: "LocalDate";   ValueData:"1"; Flags: CreateValueIfDoesntExist;
 Root: HKCU; Subkey: "Software\NetworkDLS\Finger Server"; ValueType: Dword; ValueName: "UTCDate";     ValueData:"1"; Flags: CreateValueIfDoesntExist;
 Root: HKCU; Subkey: "Software\NetworkDLS\Finger Server"; ValueType: Dword; ValueName: "HitCount";    ValueData:"1"; Flags: CreateValueIfDoesntExist;

[Run]
 Filename: "{app}\Finger.Exe"; Description: "Launch application"; Flags: postinstall nowait skipifsilent unchecked

