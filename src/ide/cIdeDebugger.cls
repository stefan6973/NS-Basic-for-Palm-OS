VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cIdeDebugger"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"No"
Attribute VB_Ext_KEY = "Member0" ,"cIdeDebugger"

Option Explicit

Public Enum enConnectionState
   unknown = 0
   disconnected
   connected
End Enum

Private WithEvents m_tcp As MSWinsockLib.Winsock
Attribute m_tcp.VB_VarHelpID = -1

Private m_connectionState As enConnectionState 'lokale Kopie

'Um dieses Ereignis auszul�sen, verwenden Sie RaiseEvent mit der folgenden Syntax:
'RaiseEvent afterConnected[(arg1, arg2, ... , argn)]
Public Event afterConnected(ByVal state As enConnectionState)
Public Event afterDisconnected(ByVal state As enConnectionState)

Public Property Let Status(newState As enConnectionState)
Attribute Status.VB_Description = "Connection status of debugger"
    If m_connectionState <> newState Then
      
      m_connectionState = newState
      
      If m_connectionState = connected Then
         RaiseEvent afterConnected(m_connectionState)
      ElseIf m_connectionState = disconnected Then
         RaiseEvent afterDisconnected(m_connectionState)
      End If
    End If
    
End Property

Public Property Get Status() As enConnectionState
    Status = m_connectionState
End Property

'------------------------------------------------------------
' initialization method
'------------------------------------------------------------
Public Function Initialize() As cIdeDebugger
   If Me Is Nothing Then
      Set Initialize = New cIdeDebugger
   End If
   Set Initialize = Me
   
End Function

'------------------------------------------------------------
' constructor
'------------------------------------------------------------
Private Sub Class_Initialize()
   'Set m_tcp = New Winsock
End Sub
