VERSION 5.00
Begin VB.Form ideFormDebugger 
   Caption         =   "Debugger"
   ClientHeight    =   5310
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7695
   BeginProperty Font 
      Name            =   "Comic Sans MS"
      Size            =   9.75
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5310
   ScaleWidth      =   7695
   StartUpPosition =   1  'Fenstermitte
   Begin VB.CommandButton btnTest 
      Caption         =   "&Test"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1455
   End
End
Attribute VB_Name = "ideFormDebugger"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents m_debugger As cIdeDebugger

Public Property Get Debugger() As cIdeDebugger
   If m_debugger Is Nothing Then
      Set m_debugger = New cIdeDebugger
      m_debugger.Initialize
   End If
   Set Debugger = m_debugger
End Property

Private Sub btnTest_Click()
   With Debugger
      If .Status = connected Then
         .Status = disconnected
      End If
   End With
End Sub

Private Sub Form_Initialize()
   If Not Debugger.Status = connected Then
      Debugger.Status = connected
   End If
End Sub

Private Sub m_debugger_afterConnected(ByVal state As enConnectionState)
   MsgBox "after connected"
End Sub

Private Sub m_debugger_afterDisconnected(ByVal state As enConnectionState)
   MsgBox "after disconnected"
End Sub
