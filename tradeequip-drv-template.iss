; Inno Setup Installation Script.
; (c) Leader InfoTech, 2004
; (c) Valery Grazdankin ( leader ), 2004

[Languages]
Name: Russian; MessagesFile: "compiler:languages\Russian.isl"
Name: English; MessagesFile: "compiler:default.isl"

[Setup]
SourceDir=..\..\
AppName=Ananas
AppVerName=Ananas 0.4.2
AppPublisher=Ananas Team
AppPublisherURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
AppSupportURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
AppUpdatesURL=http://ananas.lrn.ru, http://www.leaderit.ru/page=ananas
DefaultDirName={pf}\Ananas
DefaultGroupName=Ananas
AppCopyright=Copyright © 2002-2004 Leader InfoTech, Ananas Team
DisableProgramGroupPage=yes
AllowNoIcons=yes
LicenseFile=COPYING.RU.cp1251
InfoBeforeFile=build\win32\README.CP1251
Compression=lzma
SolidCompression=yes
OutputDir=.\
OutputBaseFilename=AnanasSetup

[Components]
Name: "designer"; Description: "Ananas Designer"; Types: full compact custom
Name: "engine"; Description: "Ananas Engine"; Types: full compact custom
Name: "webengine"; Description: "Ananas WebEngine"; Types: full compact custom
Name: "doc"; Description: "Documentation"; Types: full compact custom
Name: "doc\user"; Description: "Руководство пользователя"; Types: full custom
Name: "doc\designer"; Description: "Описание дизайнера"; Types: full custom
Name: "doc\script"; Description: "Описание встроенного языка"; Types: full custom
;Name: "sources"; Description: "Source code"; Types: full custom
;;Name: "sqlservers"; Description: "Database SQL servers"; Types: full custom
;;Name: "sqlservers\mysql"; Description: "MySQL Database servers"; Types: full custom
;;Name: "sqlservers\postgres"; Description: "PostgreSQL Database servers"; Types: full custom
;Name: "acrobat"; Description: "Программа просмотра документации Acrobat Reader 6.0"; Types: full custom
Name: "applications"; Description: "Applications"; Types: full custom
Name: "applications\inventory"; Description: "Inventory"; Types: full custom

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "build\win32\setuplogo.bmp"; DestDir: {tmp}; Flags: dontcopy
Source: "doc\*"; DestDir: "{app}\doc"; Flags: ignoreversion
Source: "src\designer\ananas-designer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\designer\testwrite.cfg"; DestDir: "{app}\applications\test"; Flags: ignoreversion
Source: "src\engine\ananas-engine.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\administrator\ananas-administrator.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\plugins\ananasplugin.dll"; DestDir: "{app}\designer"; Flags: ignoreversion
;Source: "src\plugins\rcplugin.dll"; DestDir: "{app}\designer"; Flags: ignoreversion
Source: "src\plugins\mysql\qsqlmysqlu.dll"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion
Source: "src\webengine\ananas-webengine.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\engine\bkground.jpg"; DestDir: "{app}"; Flags: ignoreversion
;Source: "src\designer\my.rc"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\lib\ananas.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "COPYING.RU.cp1251"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\win32\README.CP1251"; DestDir: "{app}"; Flags: ignoreversion
Source: "translations\*.qm"; DestDir: "{app}\translations"; Flags: ignoreversion
Source: "src\designer\templates\*.*"; DestDir: "{app}\templates"; Flags: ignoreversion
Source: "src\webengine\tpl\*.*"; DestDir: "{app}\tpl"; Flags: ignoreversion
;;;Source: "*"; Excludes:"*.exe,*.dll,*.lib,*.~*,.qmake.cache,*.tds,*.obj,CVS\*,tmp\*"; DestDir: "{app}\sources"; Flags: ignoreversion recursesubdirs
;Source: "c:\mysql\*"; DestDir: "{app}\mysql\"; Flags: ignoreversion recursesubdirs

Source: "applications\inventory\*.*"; DestDir: "{app}\applications\inventory"; Flags: ignoreversion

Source: "c:\qt\3.3.4\lib\qtmt334.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\3.3.4\lib\qsa112.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\3.3.4\plugins\designer\qseditorplugin.dll"; DestDir: "{app}\designer"; Flags: ignoreversion
Source: "c:\qt\3.3.4\plugins\sqldrivers\*.dll"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion
Source: "c:\qt\3.3.4\bin\libmysql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\3.3.4\bin\pq.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\3.3.4\bin\cc3260.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\3.3.4\bin\cc3260mt.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "c:\qt\3.3.4\bin\linguist.exe"; DestDir: "{app}"; Flags: ignoreversion

; NOTE: Don't use "Flags: ignoreversion" on any shared system files
[INI]
Filename: "{app}\lit.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.leaderit.ru"

[Icons]
Name: "{group}\Ananas Engine"; Filename: "{app}\ananas-engine.exe"
Name: "{group}\Ananas Administrator"; Filename: "{app}\ananas-administrator.exe"
Name: "{group}\Ananas Designer"; Filename: "{app}\ananas-designer.exe"
Name: "{group}\Ananas Web Engine"; Filename: "{app}\ananas-webengine.exe"
Name: "{group}\{cm:UninstallProgram,Ananas}"; Filename: "{uninstallexe}"; AfterInstall: ananasAfterInstall
Name: "{userdesktop}\Ananas Engine"; Filename: "{app}\ananas-engine.exe"; Tasks: desktopicon
Name: "{userdesktop}\Ananas Administrator"; Filename: "{app}\ananas-administrator.exe"; Tasks: desktopicon
Name: "{userdesktop}\Ananas Designer"; Filename: "{app}\ananas-designer.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Ananas"; Filename: "{app}\ananas-designer.exe"; Tasks: quicklaunchicon
Name: "{group}\Ананас в интернете"; Filename: "{app}\lit.url"

[Run]
Filename: "{app}\ananas-designer.exe"; Description: "{cm:LaunchProgram,Ananas}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\lit.url"
Type: files; Name: "{app}\ananasplugin.dll"

[Code]
//
procedure InitializeWizard();
var
  SplashImage: TBitmapImage;
  SplashForm: TForm;
  SplashFileName: String;
begin

  SplashFileName := ExpandConstant('{tmp}\setuplogo.bmp');
  ExtractTemporaryFile(ExtractFileName(SplashFileName));

  SplashForm := TForm.create(nil);
  with SplashForm do
  begin
    BorderStyle := bsNone;
    Position := poScreenCenter;
    // Adjust the height and width of the SplashForm
    // to the size of our splash image
    ClientWidth := 400;
    ClientHeight := 120;
  end;

  SplashImage := TBitmapImage.Create(SplashForm);
  with SplashImage do
  begin
    Bitmap.LoadFromFile(SplashFileName);
    Stretch := true;
    Align := alClient;
    Parent := SplashForm;
  end;

  with SplashForm do
  begin
     Show;
     Repaint;
     Sleep(3000);
     Close;
     Free;
  end;
end;


procedure ananasAfterInstall();
begin
  { copy files }
  FileCopy( ExpandConstant('{app}\designer\ananasplugin.dll'), ExpandConstant('{app}\ananasplugin.dll'), false);
end;

