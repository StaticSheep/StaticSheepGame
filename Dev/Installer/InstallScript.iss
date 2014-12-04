; TCR compliant Sample install script.
; Basically put this file in the root for what you want included in the installer,
;   then put everything into the subdirectories as listed in the installer below.
;   Remember to change the AppId
;   Thanks to Dan Weiss (dweiss@digipen.edu) for the orignal version.

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{99612BF2-3DA1-45E8-9086-75D4E4760CBF}
; Standard app data stuff
; Change GameTestInstall to your game name
AppName=Giga Gravity Games
AppVerName=Giga Gravity Games
AppPublisher=DigiPen Institute of Technology
AppPublisherURL=http://www.digipen.edu/
AppSupportURL=http://www.digipen.edu/

; Default path to the file storage directory.
; {pf} is the default program files directory set by Windows
DefaultDirName={pf}\DigiPen\GigaGravityGames
; Start menu directory
DefaultGroupName=DigiPen\GigaGravityGames
; Output directory for the installer.
OutputDir=.\INSTALLER
; Setup executable installer
OutputBaseFilename=GigaGravityGames_Setup
; Path to the DigiPen EULA (Needed to pass TCRs)
LicenseFile=EULA\DigiPen_EULA.txt
; Compression scheme for the installer. Check Inno Setup help files for more options.
Compression=lzma
SolidCompression=yes
; Path to the icon for the installer (TCR check requires custom icon)
SetupIconFile=.\FILES\icon.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
; Creates an installer option to allow/disallow desktop shortcut
; Checked by default
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "editoricon"; Description: "{cm:AdditionalIcons} Editor"; GroupDescription: "{cm:AdditionalIcons}"

; This is the list of files that the installer should grab and install.
;
; Destination Directory Notes
;   {app} is the root directory that you will be installing to.
;   {temp} is a temporary directory that Windows sets that gets deleted after the
;      installer quits.
;   {userdocs} is the directory for My Documents/Documents on Windows XP, Vista, and 7.
;
; For more information on default paths to install to, check the "Constants" article
;   in the Inno Setup 5 documentation.
;
; I recommend placing any installers for required stuff (DirectX, PhysX, etc)
;   in the general structure below to keep things neat for you.
[Files]
; The game directoy is exaclty what you want your install directory in program files to look like
Source: .\GAMEDIRECTORY\*; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs
; Include the redistributable programs and install them to the temp directory
Source: .\REDIST\*; DestDir: {tmp}; Flags: ignoreversion
;Source: .\REDIST\dxwebsetup.exe; DestDir: {tmp}; Flags: ignoreversion
;Source: .\REDIST\DXSETUP.exe; DestDir: {tmp}; Flags: ignoreversion

; This is the list of shortcuts that the installer will setup for you.
; Of note, this will create the uninstaller automatically.
;
; Directory Notes
;   {group} is the start menu location that the game will install shortcuts to.
;   {commondesktop} is your Windows desktop directory.
[Icons]
Name: {group}\GigaGravityGames; Filename: {app}\GigaGravityGames.exe; WorkingDir: {app}; IconFilename: "{app}\icon.ico"
Name: {group}\SheepEngineEditor; Filename: {app}\GigaGravityGames.exe; Parameters: "-editor"; WorkingDir: {app}; IconFilename: "{app}\icon.ico"
Name: {group}\{cm:UninstallProgram,SheepEngine}; Filename: {uninstallexe}
Name: {commondesktop}\GigaGravityGames; Filename: {app}\GigaGravityGames.exe; Tasks: desktopicon; WorkingDir: {app}; IconFilename: "{app}\icon.ico"
Name: {commondesktop}\SheepEngineEditor; Filename: {app}\GigaGravityGames.exe; Parameters: "-editor"; Tasks: editoricon; WorkingDir: {app}; IconFilename: "{app}\icon.ico"

; List of items to execute in the installer.
; Note that this will run all executables in their silent versions as required by the TCRs.
;
; The last item being run is the installer option to automatically launch the game after
;   the installer exits as required by the TCRs.
[Run]
Filename: {tmp}\vcredist_x86.exe; Parameters: /q; StatusMsg: Installing Visual C++ x86 Redistributable...
;Filename: {tmp}\dxwebsetup.exe; Parameters: /q; StatusMsg: Installing DirectX...
Filename: {app}\GigaGravityGames.exe; Description: {cm:LaunchProgram,Giga Gravity Games}; Flags: nowait postinstall skipifsilent
Filename: {app}\GigaGravityGames.exe; Parameters: "-editor"; Description: {cm:LaunchProgram,Sheep Engine Editor}; Flags: postinstall unchecked
