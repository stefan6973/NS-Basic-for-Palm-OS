Attribute VB_Name = "dbgUtils"
Option Explicit

Private msg_active As Boolean
Private trace_avtive  As Boolean
Private trace_prefix As String

'------------------------------------------------------------
'
'------------------------------------------------------------
Public Property Get isTracingEnabled() As Boolean
    isTracingEnabled = trace_avtive
End Property

'------------------------------------------------------------
'
'------------------------------------------------------------
Public Property Get areTraceMsgEnabled() As Boolean
    areTraceMsgEnabled = msg_active
End Property

'------------------------------------------------------------
'
'------------------------------------------------------------
Private Property Let Prefix(value As String)
    If value <> "???" Then
        trace_prefix = value & ": "
    End If
End Property

'------------------------------------------------------------
' enables or disables tracing of debug messages
'------------------------------------------------------------
Public Sub enableTracing(onConsole As Boolean, onGUI As Boolean, Optional ByVal newPrefix As String = "???")
    trace_avtive = onConsole
    msg_active = onGUI
    Prefix = newPrefix
End Sub

'------------------------------------------------------------
'
'------------------------------------------------------------
Public Sub disableTracing()
    trace_avtive = False
    trace_prefix = "???"
    msg_active = False
End Sub

'------------------------------------------------------------
' prints a trace message in the debug console if tracing is enabled
'------------------------------------------------------------
Public Sub trace(ByVal msg As String, Optional ByVal newPrefix As String = "???")
    Prefix = newPrefix
    If trace_avtive Then
        Debug.Print trace_prefix & msg
    End If
End Sub

'------------------------------------------------------------
' Show trace message box and deactivates further showing
' of a trace message box in the cancel button was pressed
'------------------------------------------------------------
Public Sub traceMsg(ByVal msg As String, Optional ByVal newPrefix As String = "???")
    trace msg, newPrefix
    If msg_active Then
        If MsgBox(trace_prefix & msg, vbOKCancel) = vbCancel Then
            msg_active = False
        End If
    End If
End Sub

'------------------------------------------------------------
' Show trace message box and deactivates further showing
' of a trace message box in the cancel button was pressed
'------------------------------------------------------------
Public Sub traceStep(ByVal step As Single, Optional ByVal msg As String = "", Optional ByVal newPrefix As String = "???")
    traceMsg "#" & Trim(str(step)) & " " & msg, newPrefix
End Sub

'------------------------------------------------------------
' Show current error
'------------------------------------------------------------
Public Sub traceError(Optional ByVal msg As String = "")
   traceMsg msg & " -> Error: " & Error
End Sub
