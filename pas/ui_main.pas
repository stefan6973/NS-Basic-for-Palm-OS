unit ui_main;

{$mode objfpc}{$H+}

interface

uses
   Classes, SysUtils, Forms, Controls, Graphics, Dialogs, Menus, StdCtrls,
   ExtCtrls, ActnList, Buttons, ComCtrls;

type

   { TfrmMain }

   TfrmMain = class(TForm)
      acl: TActionList;
      BitBtn1: TBitBtn;
      Button1: TButton;
      CheckBox1: TCheckBox;
      chb1: TCheckBox;
      chb2: TCheckBox;
      chg1: TCheckGroup;
      cb1: TComboBox;
      Edit1: TEdit;
      iml: TImageList;
      Label1: TLabel;
      mm: TMainMenu;
      Memo1: TMemo;
      pc: TPageControl;
      Panel1: TPanel;
      pm: TPopupMenu;
      ProgressBar1: TProgressBar;
      RadioButton1: TRadioButton;
      rb1: TRadioButton;
      rb2: TRadioButton;
      rgr1: TRadioGroup;
      sb: TStatusBar;
      ts1: TTabSheet;
      ts2: TTabSheet;
      ts3: TTabSheet;
      TrackBar1: TTrackBar;
   private

   public

   end;

var
   frmMain: TfrmMain;

implementation

{$R *.lfm}

end.

