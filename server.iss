
[Setup]
AppId={{CF2BBEBB-1CB7-46DE-BCB2-4D5EA6A796D4}
AppName=HTTP_messenger_server
AppVersion=1.0
;AppVerName=HTTP_messenger_server 1.0
AppPublisher=LV-617
AppPublisherURL=https://github.com/ita-social-projects/HTTP_Messenger_Server
AppSupportURL=https://github.com/ita-social-projects/HTTP_Messenger_Server
AppUpdatesURL=https://github.com/ita-social-projects/HTTP_Messenger_Server
DefaultDirName={autopf}\HTTP_messenger_server
ChangesAssociations=yes
DefaultGroupName=HTTP_messenger_server
AllowNoIcons=yes
OutputDir=C:\Jenkins_home\workspace\HTTP_Messenger_server\HTTP_Messenger_Server\out\Debug
OutputBaseFilename=HTTP_messenger_server_installer
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Jenkins_home\workspace\HTTP_Messenger_server\HTTP_Messenger_Server\out\Debug\HTTP_Messenger_Server.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Jenkins_home\workspace\HTTP_Messenger_server\HTTP_Messenger_Server\out\Debug\cpprest_2_10d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Jenkins_home\workspace\HTTP_Messenger_server\HTTP_Messenger_Server\out\Debug\libcrypto-1_1-x64.dll"; DestDir: "{app}"; Flags: ignoreversion

[Registry]
Root: HKA; Subkey: "Software\Classes\.myp\OpenWithProgids"; ValueType: string; ValueName: "HTTP_messenger_serverFile.myp"; ValueData: ""; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\HTTP_messenger_serverFile.myp"; ValueType: string; ValueName: ""; ValueData: "HTTP_messenger_server File"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\HTTP_messenger_serverFile.myp\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\HTTP_Messenger_Server.exe,0"
Root: HKA; Subkey: "Software\Classes\HTTP_messenger_serverFile.myp\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\HTTP_Messenger_Server.exe"" ""%1"""
Root: HKA; Subkey: "Software\Classes\Applications\HTTP_Messenger_Server.exe\SupportedTypes"; ValueType: string; ValueName: ".myp"; ValueData: ""

[Icons]
Name: "{group}\HTTP_messenger_server"; Filename: "{app}\HTTP_Messenger_Server.exe"
Name: "{group}\{cm:UninstallProgram,HTTP_messenger_server}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\HTTP_messenger_server"; Filename: "{app}\HTTP_Messenger_Server.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\HTTP_Messenger_Server.exe"; Description: "{cm:LaunchProgram,HTTP_messenger_server}"; Flags: nowait postinstall skipifsilent

