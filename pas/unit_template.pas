unit unit_template;

{$mode ObjFPC}{$H+}

interface

uses
   Classes, SysUtils;

type
	u8 = byte;
    u16 = word;
    u32 = longword;

    TTest = class(TStrings)

    end;

const
  a = '2';

var
  b: u8 = 12;
  bb: Boolean = True;
  ss: String = 'dg';

//--------------------------------------------------
//
//--------------------------------------------------
implementation

uses DateUtils;

type
	u64 = LongInt;

const
   c = 12;

var
   d: u16 = 1223;

procedure abc(arg1: Boolean);
begin

end;

function def(arg2: u16): u32;
begin

end;

initialization

finalization

end.

