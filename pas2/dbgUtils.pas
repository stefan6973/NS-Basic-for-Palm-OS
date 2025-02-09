
unit dbgutils;

{$mode ObjFPC}{$H+}

interface
// code
// '------------------------------------------------------------
// '
// '------------------------------------------------------------
//# Public Property Get isTracingEnabled() As Boolean
function isTracingEnabled()(): boolean;
begin //#Public Property Get isTracingEnabled() As Boolean
//# isTracingEnabled = trace_avtive
isTracingEnabled := trace_avtive
end;
// '------------------------------------------------------------
// '
// '------------------------------------------------------------
//# Public Property Get areTraceMsgEnabled() As Boolean
function areTraceMsgEnabled()(): boolean;
begin //#Public Property Get areTraceMsgEnabled() As Boolean
//# areTraceMsgEnabled = msg_active
areTraceMsgEnabled := msg_active
end;

uses
   Classes, SysUtils;

implementation
var
msg_active: boolean;
trace_avtive: boolean;
trace_prefix: string;





end.
