; Script generated with the Venis Install Wizard

; Define your application name
!define APPVERSION "1.0.0"
!define APPNAME "Chew Broadcaster"
!define APPNAMEANDVERSION "Chew Broadcaster ${APPVERSION}"

; Additional script dependencies
!include WinVer.nsh
!include x64.nsh

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES32\chew-broadcaster"
InstallDirRegKey HKLM "Software\${APPNAME}" ""
OutFile "Chew-Broadcaster-${APPVERSION}-Installer.exe"

; Use compression
SetCompressor LZMA

; Need Admin
RequestExecutionLevel admin

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\bin\32bit\chew32.exe"

!define MUI_PAGE_CUSTOMFUNCTION_LEAVE PreReqCheck

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "data\obs-studio\license\gplv2.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

;!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_COMPONENTS
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Function PreReqCheck
	; Abort on XP or lower
	${If} ${AtMostWinXP}
		MessageBox MB_OK|MB_ICONSTOP "Due to extensive use of DirectX 10 features, ${APPNAME} requires Windows Vista SP2 or higher and cannot be installed on this version of Windows."
		Quit
	${EndIf}

	; Vista specific checks
	${If} ${IsWinVista}
		; Check Vista SP2
		${If} ${AtMostServicePack} 1
			MessageBox MB_YESNO|MB_ICONEXCLAMATION "${APPNAME} requires Service Pack 2 when running on Vista. Would you like to download it?" IDYES sptrue IDNO spfalse
			sptrue:
				ExecShell "open" "http://windows.microsoft.com/en-US/windows-vista/Learn-how-to-install-Windows-Vista-Service-Pack-2-SP2"
			spfalse:
			Quit
		${EndIf}

		; Check Vista Platform Update
		nsexec::exectostack "$SYSDIR\wbem\wmic.exe qfe where HotFixID='KB971512' get HotFixID /Format:list"
		pop $0
		pop $0
		strcpy $1 $0 17 6
		strcmps $1 "HotFixID=KB971512" gotPatch
			MessageBox MB_YESNO|MB_ICONEXCLAMATION "${APPNAME} requires the Windows Vista Platform Update. Would you like to download it?" IDYES putrue IDNO pufalse
			putrue:
				${If} ${RunningX64}
					; 64 bit
					ExecShell "open" "http://www.microsoft.com/en-us/download/details.aspx?id=4390"
				${Else}
					; 32 bit
					ExecShell "open" "http://www.microsoft.com/en-us/download/details.aspx?id=3274"
				${EndIf}
			pufalse:
			Quit
		gotPatch:
	${EndIf}

	ClearErrors
	GetDLLVersion "MSVCR120.DLL" $R0 $R1
	IfErrors vs2013Missing vs2013OK
	vs2013Missing:
		MessageBox MB_YESNO|MB_ICONEXCLAMATION "Your system is missing runtime components that ${APPNAME} requires. Would you like to download them?" IDYES vs2013true IDNO vs2013false
		vs2013true:
			ExecShell "open" "http://www.microsoft.com/en-us/download/details.aspx?id=40784"
		vs2013false:
		Quit
	vs2013OK:
	ClearErrors

	; DirectX Version Check
	ClearErrors
	GetDLLVersion "D3DCompiler_33.dll" $R0 $R1
	IfErrors dxMissing dxOK
	dxMissing:
		MessageBox MB_YESNO|MB_ICONEXCLAMATION "Your system is missing DirectX components that ${APPNAME} requires. Would you like to download them?" IDYES dxtrue IDNO dxfalse
		dxtrue:
			ExecShell "open" "https://obsproject.com/go/dxwebsetup"
		dxfalse:
		Quit
	dxOK:
	ClearErrors

	; Check previous instance
	; System::Call 'kernel32::OpenMutexW(i 0x100000, b 0, w "OBSMutex") i .R0'
	; IntCmp $R0 0 notRunning
	; System::Call 'kernel32::CloseHandle(i $R0)'
	; MessageBox MB_OK|MB_ICONEXCLAMATION "${APPNAME} is already running. Please close it first before installing a new version." /SD IDOK
	; Quit
notRunning:

FunctionEnd

Function filesInUse
	MessageBox MB_OK|MB_ICONEXCLAMATION "Some files were not able to be installed. If this is the first time you are installing Chew Broadcaster, please disable any anti-virus or other security software and try again. If you are re-installing or updating Chew Broadcaster, close any applications that may be have been hooked, or reboot and try again."  /SD IDOK
FunctionEnd

Var outputErrors

Section "Chew Broadcaster" Section1

	; Set Section properties
	SetOverwrite on

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR"
	File /r "data"
	SetOutPath "$INSTDIR\bin"
	File /r "bin\32bit"
	SetOutPath "$INSTDIR\obs-plugins"
	File /r "obs-plugins\32bit"

	${if} ${RunningX64}
		SetOutPath "$INSTDIR\bin"
		File /r "bin\64bit"
		SetOutPath "$INSTDIR\obs-plugins"
		File /r "obs-plugins\64bit"
	${endif}

	ClearErrors

	IfErrors 0 +2
		StrCpy $outputErrors "yes"

	WriteUninstaller "$INSTDIR\uninstall.exe"

	SetOutPath "$INSTDIR\bin\32bit"
	CreateShortCut "$DESKTOP\Chew Broadcaster.lnk" "$INSTDIR\bin\32bit\chew32.exe"
	CreateDirectory "$SMPROGRAMS\Chew.tv"
	CreateShortCut "$SMPROGRAMS\Chew.tv\Chew Broadcaster (32bit).lnk" "$INSTDIR\bin\32bit\chew32.exe"
	CreateShortCut "$SMPROGRAMS\Chew.tv\Uninstall.lnk" "$INSTDIR\uninstall.exe"

	${if} ${RunningX64}
		SetOutPath "$INSTDIR\bin\64bit"
		CreateShortCut "$SMPROGRAMS\Chew.tv\Chew Broadcaster (64bit).lnk" "$INSTDIR\bin\64bit\chew64.exe"
	${endif}

	SetOutPath "$INSTDIR\bin\32bit"

	StrCmp $outputErrors "yes" 0 +2
		Call filesInUse
SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section "un.chew-broadcaster Program Files"

	SectionIn RO

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\Chew Broadcaster.lnk"
	Delete "$SMPROGRAMS\Chew.tv\Chew Broadcaster (32bit).lnk"
	Delete "$SMPROGRAMS\Chew.tv\Uninstall.lnk"
	${if} ${RunningX64}
		Delete "$SMPROGRAMS\Chew.tv\Chew Broadcaster (64bit).lnk"
	${endif}

	; Clean up Chew Broadcaster
	RMDir /r "$INSTDIR\bin"
	RMDir /r "$INSTDIR\data"
	RMDir /r "$INSTDIR\obs-plugins"
	RMDir "$INSTDIR"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\Chew.tv"
	RMDir "$INSTDIR\Chew Broadcaster"
SectionEnd

Section /o "un.User Settings" Section2
	RMDir /R "$APPDATA\chew-broadcaster"
SectionEnd

!insertmacro MUI_UNFUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} "Remove the Chew Broadcaster program files."
	!insertmacro MUI_DESCRIPTION_TEXT ${Section2} "Removes all settings, plugins, scenes and sources, profiles, log files and other application data."
!insertmacro MUI_UNFUNCTION_DESCRIPTION_END

; eof
