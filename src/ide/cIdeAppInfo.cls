VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "AppInfo"
Attribute VB_GlobalNameSpace = True
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_tempFolder As String
Private m_appFolder As String
Private m_fontsFolder As String
Private m_langFolder As String
Private m_imagesFolder As String

'------------------------------------------------------------
' checks whether a folder exist
'------------------------------------------------------------
Private Function FolderExist(ByVal foldername As String) As Boolean
   FolderExist = False
   
   If foldername = vbNullString Then Exit Function
   
   On Error GoTo exception
   FolderExist = Dir(foldername, vbDirectory) <> vbNullString
   
   traceMsg "FolderExist ? " & foldername & " => " & FolderExist
   Exit Function
   
exception:
   traceError "FolderExist"
   On Error GoTo 0
End Function

'------------------------------------------------------------
' gets the name of an app-local subfolder, if folder not exist
' then the folder will be created first
'------------------------------------------------------------
Public Property Get SubFolder(ByVal subFolderName As String) As String
   On Error GoTo exception
   
   Dim path As String
   path = Me.AppFolder & subFolderName & "\"
   
   If Not FolderExist(path) Then
      MkDir path
   End If
   
   SubFolder = path
   Exit Property
   
exception:
   traceError "SubFolder"
End Property

'------------------------------------------------------------
' Folder where the application at runtime is installed
'------------------------------------------------------------
Public Property Get AppFolder() As String
   If m_appFolder = "" Then
      m_appFolder = App.path
      
      '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      trace m_appFolder
      '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      
      If Right(m_appFolder, 3) = "src" Then ' app runs inside IDE
         m_appFolder = Replace(m_appFolder, "src", "bin\")
         '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         traceMsg "Running as VB build"
         '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      ElseIf Right(m_appFolder, 3) = "bin" Then ' app runs outside IDE
         m_appFolder = Replace(m_appFolder, "bin", "bin\")
         '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         traceMsg "Running as standalone"
         '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      Else
         MsgBox "this kind of app folder isn't supported yet: " & m_appFolder
         End
      End If
   End If
   AppFolder = m_appFolder
End Property

'------------------------------------------------------------
' Folder where the fonts at runtime are installed
'------------------------------------------------------------
Public Property Get FontsFolder() As String
   If m_fontsFolder = "" Then
      m_fontsFolder = SubFolder("Fonts")
   End If
   FontsFolder = m_fontsFolder
   traceMsg "FontsFolder => " & FontsFolder
End Property

'------------------------------------------------------------
' Folder where the language files at runtime are installed
'------------------------------------------------------------
Public Property Get LangFolder() As String
   If m_langFolder = "" Then
      m_langFolder = SubFolder("Lang")
   End If
   LangFolder = m_langFolder
   traceMsg "LangFolder => " & LangFolder
End Property

'------------------------------------------------------------
' Folder where the image files at runtime are installed
'------------------------------------------------------------
Public Property Get ImagesFolder() As String
   If m_imagesFolder = "" Then
      m_imagesFolder = SubFolder("Images")
   End If
   ImagesFolder = m_imagesFolder
   traceMsg "ImagesFolder => " & ImagesFolder
End Property

'------------------------------------------------------------
' Folder where the application at runtime is installed
'------------------------------------------------------------
Public Property Get TempFolder() As String
   If m_tempFolder = "" Then
      m_tempFolder = SubFolder("Temp")
   End If
   TempFolder = m_tempFolder
   traceMsg "TempFolder => " & TempFolder
End Property

'------------------------------------------------------------
' AppInfo constructor
'------------------------------------------------------------
Public Sub Class_Initialize()
   enableTracing True, False, "App-Info"
   
   m_tempFolder = ""
   m_appFolder = ""
   m_fontsFolder = ""
   m_langFolder = ""
   m_imagesFolder = ""
   
End Sub

Private Sub Class_Terminate()
   disableTracing
End Sub
