
unit mwinreg;

{$mode ObjFPC}{$H+}

interface
const
// '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// '%  VB4 Registry Editor Demo                                            %
// '%  MSVC 2.2 Registry Constants and API Prototypes                      %
// '%  Written by Roger Wynn                                               %
// '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// 'NOTE:
// 'Additional Registry Constants can be found in 'WINNT.H'.
// 'Additional Error Constants can be found in and 'WINERROR.H'.
// 'Additional Structures and Prototypes can be found in 'WIN32API.TXT',
// '  but may need to be modified.
// 'Registry Constants
HKEY_CLASSES_ROOT = &h80000000;
HKEY_CURRENT_USER = &h80000001;
HKEY_LOCAL_MACHINE = &h80000002;
HKEY_USERS = &h80000003;
// 'Registry Specific Access Rights
KEY_QUERY_VALUE = &h1;
KEY_SET_VALUE = &h2;
KEY_CREATE_SUB_KEY = &h4;
KEY_ENUMERATE_SUB_KEYS = &h8;
KEY_NOTIFY = &h10;
KEY_CREATE_LINK = &h20;
KEY_ALL_ACCESS = &h3f;
// 'Open/Create Options
REG_OPTION_NON_VOLATILE = 0&;
REG_OPTION_VOLATILE = &h1;
// 'Key creation/open disposition
REG_CREATED_NEW_KEY = &h1;
REG_OPENED_EXISTING_KEY = &h2;
// 'masks for the predefined standard access types
STANDARD_RIGHTS_ALL = &h1f0000;
SPECIFIC_RIGHTS_ALL = &hffff;
// 'Define severity codes
ERROR_SUCCESS = 0&;
ERROR_ACCESS_DENIED = 5;
ERROR_NO_MORE_ITEMS = 259;
// 'Predefined Value Types
REG_NONE = (0);
REG_SZ = (1);
REG_EXPAND_SZ = (2);
REG_BINARY = (3);
REG_DWORD = (4);
REG_DWORD_LITTLE_ENDIAN = (4);
REG_DWORD_BIG_ENDIAN = (5);
REG_LINK = (6);
REG_MULTI_SZ = (7);
REG_RESOURCE_LIST = (8);
REG_FULL_RESOURCE_DESCRIPTOR = (9);
REG_RESOURCE_REQUIREMENTS_LIST = (10);
// 'Global Variables
// 'Operation Flag Values
NOOPERATION = 0;
DELETEREGVALUE = 1;
DELETEREGKEY = 2;
GETREGVALUEDATA = 3;
SETREGVALUEDATA = 4;// code
// 'Structures Needed For Registry Prototypes
// '------------------------------------------------------------
// '
// '------------------------------------------------------------
// '------------------------------------------------------------
// ' Registry Function Prototypes
// '------------------------------------------------------------
//# Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" _
function RegOpenKeyEx(): ;
begin //#Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" _
//# (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, _
(ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, _
end;
//# ByVal samDesired As Long, phkResult As Long) As Long
ByVal samDesired As Long, phkResult As Long) As Long
end;
//# Declare Function RegSetValueEx Lib "advapi32" Alias "RegSetValueExA" _
function RegSetValueEx(): ;
begin //#Declare Function RegSetValueEx Lib "advapi32" Alias "RegSetValueExA" _
//# (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, _
(ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, _
end;
//# ByVal dwType As Long, ByVal szData As String, ByVal cbData As Long) As Long
ByVal dwType As Long, ByVal szData As String, ByVal cbData As Long) As Long
end;
//# Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long
function RegCloseKey(): long;
begin //#Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long
//# Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" _
function RegQueryValueEx(): ;
begin //#Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" _
//# (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, _
(ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, _
end;
//# ByRef lpType As Long, ByVal szData As String, ByRef lpcbData As Long) As Long
ByRef lpType As Long, ByVal szData As String, ByRef lpcbData As Long) As Long
end;
//# Declare Function RegCreateKeyEx Lib "advapi32" Alias "RegCreateKeyExA" _
function RegCreateKeyEx(): ;
begin //#Declare Function RegCreateKeyEx Lib "advapi32" Alias "RegCreateKeyExA" _
//# (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, _
(ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, _
end;
//# ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, _
ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, _
end;
//# lpSecurityAttributes As SECURITY_ATTRIBUTES, phkResult As Long, _
lpSecurityAttributes As SECURITY_ATTRIBUTES, phkResult As Long, _
end;
//# lpdwDisposition As Long) As Long
lpdwDisposition As Long) As Long
end;
//# Declare Function RegEnumKeyEx Lib "advapi32.dll" Alias "RegEnumKeyExA" _
function RegEnumKeyEx(): ;
begin //#Declare Function RegEnumKeyEx Lib "advapi32.dll" Alias "RegEnumKeyExA" _
//# (ByVal hKey As Long, ByVal dwIndex As Long, ByVal lpName As String, _
(ByVal hKey As Long, ByVal dwIndex As Long, ByVal lpName As String, _
end;
//# lpcbName As Long, ByVal lpReserved As Long, ByVal lpClass As String, _
lpcbName As Long, ByVal lpReserved As Long, ByVal lpClass As String, _
end;
//# lpcbClass As Long, lpftLastWriteTime As FILETIME) As Long
lpcbClass As Long, lpftLastWriteTime As FILETIME) As Long
end;
//# Declare Function RegEnumValue Lib "advapi32.dll" Alias "RegEnumValueA" _
function RegEnumValue(): ;
begin //#Declare Function RegEnumValue Lib "advapi32.dll" Alias "RegEnumValueA" _
//# (ByVal hKey As Long, ByVal dwIndex As Long, ByVal lpValueName As String, _
(ByVal hKey As Long, ByVal dwIndex As Long, ByVal lpValueName As String, _
end;
//# lpcbValueName As Long, ByVal lpReserved As Long, lpType As Long, _
lpcbValueName As Long, ByVal lpReserved As Long, lpType As Long, _
end;
//# ByVal lpData As String, lpcbData As Long) As Long
ByVal lpData As String, lpcbData As Long) As Long
end;
//# Declare Function RegDeleteKey Lib "advapi32.dll" Alias "RegDeleteKeyA" _
function RegDeleteKey(): ;
begin //#Declare Function RegDeleteKey Lib "advapi32.dll" Alias "RegDeleteKeyA" _
//# (ByVal hKey As Long, ByVal lpSubKey As String) As Long
(ByVal hKey As Long, ByVal lpSubKey As String) As Long
end;
//# Declare Function RegDeleteValue Lib "advapi32.dll" Alias "RegDeleteValueA" _
function RegDeleteValue(): ;
begin //#Declare Function RegDeleteValue Lib "advapi32.dll" Alias "RegDeleteValueA" _
//# (ByVal hKey As Long, ByVal lpValueName As String) As Long
(ByVal hKey As Long, ByVal lpValueName As String) As Long
end;

uses
   Classes, SysUtils;

implementation






end.
