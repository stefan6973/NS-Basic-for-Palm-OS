
unit toolbitmaps;

{$mode ObjFPC}{$H+}

interface
const
CF_BITMAP = 2;
LR_LOADMAP3DCOLORS = &h1000;
LR_LOADFROMFILE = &h10;
LR_LOADTRANSPARENT = &h20;
IMAGE_BITMAP = 0;// code
//# Declare Function LoadBitmap Lib "user32" Alias "LoadBitmapA" (ByVal hInstance As Long, ByVal lpBitmapName As String) As Long
function LoadBitmap(): long;
begin //#Declare Function LoadBitmap Lib "user32" Alias "LoadBitmapA" (ByVal hInstance As Long, ByVal lpBitmapName As String) As Long
//# Declare Function OpenClipboard Lib "user32" (ByVal hWnd As Long) As Long
function OpenClipboard(): long;
begin //#Declare Function OpenClipboard Lib "user32" (ByVal hWnd As Long) As Long
//# Declare Function CloseClipboard Lib "user32" () As Long
function CloseClipboard(): long;
begin //#Declare Function CloseClipboard Lib "user32" () As Long
//# Declare Function EmptyClipboard Lib "user32" () As Long
function EmptyClipboard(): long;
begin //#Declare Function EmptyClipboard Lib "user32" () As Long
//# Declare Function SetClipboardData Lib "user32" (ByVal wFormat As Long, ByVal hMem As Long) As Long
function SetClipboardData(): long;
begin //#Declare Function SetClipboardData Lib "user32" (ByVal wFormat As Long, ByVal hMem As Long) As Long
//# Declare Function LoadImage Lib "user32" Alias "LoadImageA" (ByVal hInst As Long, ByVal lpsz As String, ByVal un1 As Long, ByVal n1 As Long, ByVal n2 As Long, ByVal un2 As Long) As Long
function LoadImage(): long;
begin //#Declare Function LoadImage Lib "user32" Alias "LoadImageA" (ByVal hInst As Long, ByVal lpsz As String, ByVal un1 As Long, ByVal n1 As Long, ByVal n2 As Long, ByVal un2 As Long) As Long

uses
   Classes, SysUtils;

implementation






end.
