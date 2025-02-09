
unit mnsbasiccompile;

{$mode ObjFPC}{$H+}

interface
const
menusOn = false;
allowRegistration = true;
bookEdition = false;
defaultLanguage = "english";// code
//# Declare Function WinHelp Lib "USER32" Alias "WinHelpA" _
function WinHelp(): ;
begin //#Declare Function WinHelp Lib "USER32" Alias "WinHelpA" _
//# (ByVal hWnd As Long, ByVal lpHelpFile As String, _
(ByVal hWnd As Long, ByVal lpHelpFile As String, _
end;
//# ByVal wCommand As Long, ByVal dwData As Long) As Long
ByVal wCommand As Long, ByVal dwData As Long) As Long
end;

uses
   Classes, SysUtils;

implementation






end.
