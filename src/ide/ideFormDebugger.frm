VERSION 5.00
Begin VB.Form ideFormDebugger 
   Caption         =   "Debugger"
   ClientWidth     =   7500
   ClientHeight    =   500
   ClientTop       =   350
   ClientLeft      =   100
   BeginProperty Font 
      Name            =   "Comic Sans MS"
      Size            =   10
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleWidth      =   7500
   ScaleHeight     =   5000
   StartUpPosition =   1  'Fenstermitte
   Begin VB.CommandButton btnConnect 
      Caption         =   "&Connect"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   10
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Width           =   1500
      Height          =   500
      Left            =   250
      Top             =   250
      TabIndex        =   1
   End
   Begin VB.CommandButton btnDisconnect 
      Caption         =   "&Disconnect"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   10
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Width           =   1500
      Height          =   500
      Left            =   250
      Top             =   500
      TabIndex        =   2
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
      Set Debugger = m_debugger.Initialize()
   Else
      Set Debugger = m_debugger
   End If
End Property

Private Sub btnConnect_Click()
   With Debugger
      If .Status = connected Then
         .Status = disconnected
      End If
   End With
End Sub

Private Sub btnDisconnect_Click()
   With Debugger
      If Not .Status = connected Then
         .Status = connected
      End If
   End With
End Sub

Private Sub m_debugger_afterConnected(ByVal state As enConnectionState)
   MsgBox "after connected"
End Sub

Private Sub m_debugger_afterDisconnected(ByVal state As enConnectionState)
   MsgBox "after disconnected"
End Sub
