program NSBasic;

{$mode objfpc}{$H+}

uses
   {$IFDEF UNIX}
   cthreads,
   {$ENDIF}
   {$IFDEF HASAMIGA}
   athreads,
   {$ENDIF}
   Interfaces, // this includes the LCL widgetset
   Forms, ui_main, unit_template
   { you can add units after this };

{$R *.res}

begin
   RequireDerivedFormResource:=True;
   Application.Scaled:=True;
   Application.Initialize;
   Application.CreateForm(TfrmMain, frmMain);
   Application.Run;
end.

