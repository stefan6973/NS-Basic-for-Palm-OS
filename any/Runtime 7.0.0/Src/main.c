#define   mainpgm yes


#define   ERROR_CHECK_LEVEL 1
#define   nil 0 
#include  <PalmOS.h>
#include  <PalmTypes.h>
#include  <PalmOSGlue.h>
#include  <PalmCompatibility.h>
#include  <crc.h>
#include  <day.h>
#include  <selday.h>
#include  <FatalAlert.h>
#include  <FloatMgr.h>
#include  <DataMgr.h>
#include  <StringMgr.h>
#include  <Launcher.h>
#include  <SystemMgr.h>
#include  <DateTime.h>
#include  <window.h>
#include  "copy.h"
#include  <mathlib.h>
#include <ErrorBase.h>                   //05212003 

extern void spawnFormUpdateEventCode();     //09192004
char** gridCellPtr(struct ObjectData* objPtr, int row, int col);   //inserted 02062004
void ScrollGrid(TableType* grid ,int idx, int lines);              //inserted 02062004
//void debugControls(char*);                                          //09242003
struct gridColInfo* locateGridColInfo(struct ObjectData* objPtr,  int col);               //09242003
void resetCustomDrawEditableProperty();                               //09242003                         ;
void ScrollTableLines(int noLines,TableType* tbl,ScrollBarType *scl); //09242003
extern void redrawGrid(struct ObjectData * objPtr);                   //09242003
double misc_int(double x);
extern Boolean  mathLibOpenSw;
extern void GetSignatureGadgetBounds(RectangleType* r);
extern void CaptureSignature();
void setupFuncTable();
int basc_dbGetInt(char* ptr);                              //added 5.21.2000
//09242003 void useOurOwnDisplayMemory(FormPtr frmPtr);               //added 5.21.2000
void SetFieldText( UInt nFieldID, const CharPtr pSrcText, Int nMaxSize, Boolean bRedraw );  //added 5.21.2000
//09242003 void useOurOwnDisplayMemory(FormPtr frmPtr);              //added 5.21.2000 
void executeFormRedraw();                                 //added 5.21.2000 
void FreeListValues(VoidPtr pold,int noItems );           //added 5.21.2000 
extern int integ_value( struct dataelem *  arg, double *    arg_ad);        /*---calc integ value of data elem__*/
extern VoidHand forHandle;
static void EventLoop(void);
void init_a_data_value(int de_no);
int dbug(char *s);
extern  updti( struct dataelem* ptr, unsigned int* dest, int* src );
extern  updtc( struct dataelem* ptr, char* dest, char*  src );
                                                       
extern updtd( struct dataelem* ptr, double* dest, double* src );

int misc_codeexec(Word objId,unsigned long offset);
void chkFldUpdate();       //5.30.2000
void ExitHousekeeping();   //5.30.2000
int launchPgm();
void UpdateScrollbar(FieldPtr field, ScrollBarPtr scroll);
static void ScrollLines( int numLinesToScroll, Boolean redraw, FieldPtr field,ScrollBarPtr scroll);
static  Boolean ApplicationHandleEvent(EventPtr event);
static Boolean ViewHandleEvent(EventPtr event);
void StrDouble (double val, CharPtr str,int dgts);
double StrToDouble(char *p);


//3/24/2002 int cntFldEnters;
int           runtimeVersionInteg = 700;
int           runtimeVersionFrac=0;
int           applVersionInteg;
int           applVersionFrac;
unsigned long myCreationDate;
int           LitePro;  //2=pro version,1=medium,0=light
extern long   moduleCntr;

char           oldName[33];

// Structure definition moved to copy.h
struct basc_pref bp;    // 12/18/2000 MMD

struct  cmd
{
     UInt cardNoP;
     LocalID dbIDP;
} ;
extern UInt     serRef;
extern Boolean	gOldSerialManager;						//02.27.02 GH
extern UInt8	gSerialState;							//03.06.02 GH		
extern void serialSetState(UInt8 state);				//03.06.02 GH

extern    VoidHand h3;
int firstPenDown;
FormPtr thePenDownForm;
int       StartApplication();
struct dataelem *de;
VoidHand  h2;
int       fldToUpdate;
char*     p2;
UInt      idx2;
UInt      len;
FormPtr   frm;
Word      crc;
int pendownCount;
char terminationAction;   //  'l'=launch the launcher app
                          //  'n'=no action req'd 
DWord startFreeDynamicMemory, endFreeDynamicMemory, junk;
struct ObjectData * locateObjectPtr(Word id);   //added 5.31.2000

int varsInDynamHeap;       //06012002  # of numer vars alloc in heap
int maxVarsInDynamHeap;    //06012002  max # of vars to alloc in dymam heap


//*****************************************************************************************
DWord PilotMain(Word cmd,Ptr cmdPBP, Word launchFlags)
{
   int  i;
   int  error=0;
   char wk[50];
    Err err;
   LocalID lid;
   UInt cdNo;
   DmSearchStateType st;
//SysFatalAlert("0");
   if (cmd==sysAppLaunchCmdNormalLaunch)
   {
      
//SysFatalAlert("starting");
       terminationAction='l';
       ErrTry                       //inserted 05212003
       {                            //inserted 05212003 
           error=launchPgm();       
       }                            //inserted 05212003
       ErrCatch (inErr)                    //inserted 05212003
       {                            //inserted 05212003
//SysFatalAlert("CAUGHT ERROR");
	         ErrTry                              //inserted 05282003
		     {                                   //inserted 05282003
//SysFatalAlert("to housekeeping");
		          ExitHousekeeping();            //inserted 05282003
//SysFatalAlert("back from exitHousekeeping");
	         }                                   //inserted 05282003
	         ErrCatch (inErr)                    //inserted 05282003
	         {                                   //inserted 05282003
//SysFatalAlert("caught2");	                                             //inserted 05282003
	         }  ErrEndCatch                      //inserted 05282003
	             
       } ErrEndCatch                //inserted 05212003
//SysFatalAlert("launcher");
       if((terminationAction =='l') || (terminationAction=='n')) 
       {     
            if((ULong)PrefGetPreference(prefLauncherAppCreator)==(ULong)'lnch')
            {
                //launch the standard applic launcher program
//SysFatalAlert("std launcher");
//StrPrintF(debug,"term error=%d",error);
//SysFatalAlert(debug);
//SysFatalAlert("exiting3");
                SysUIAppSwitch(0,DmFindDatabase(0,"Launcher"),sysAppLaunchCmdNormalLaunch,NULL);
            }
            else
            {
              //launch the custom applic launcher program
              err=DmGetNextDatabaseByTypeCreator( TRUE, &st,(ULong)'appl',
                  (ULong)PrefGetPreference(prefLauncherAppCreator),TRUE,&cdNo,&lid);
              if(err)
                return 0;
//SysFatalAlert("exiting2");
              SysUIAppSwitch(0,lid,sysAppLaunchCmdNormalLaunch,NULL);        
            }  
            
       }     
   }

//SysFatalAlert("exiting");    
  
   return 0;
     
}



//*****************************************************************************************
//--------------------------------------------------------------------
int launchPgm()
{
    int error;
//int z;
//char debug[100];
//z=sizeof(EventType);
//StrPrintF(debug,"event len=%d",z);
//SysFatalAlert(debug);
       
       
       error=StartApplication();
       
       
//SysFatalAlert("Launch");
       if (error)
       { 
       
//SysFatalAlert("going to exit housekeeping");
          if(error!=999)           //added 2/18/2003
          {                        //added 2/18/2003 
//StrPrintF(debug,"going to housekeeping err=%d",error);
//SysFatalAlert(debug);
              ExitHousekeeping();  //08142002
          }                           //added 2/18/2003
//SysFatalAlert("exiting");
          return error;
       }
       terminationAction='n';  //set no action req'd
       EventLoop();
       ExitHousekeeping();
       return(0);
       
       //MemHeapFreeBytes(0, &endFreeDynamicMemory, &junk);
       //if(startFreeDynamicMemory != endFreeDynamicMemory)
       //{
       //
       //     StrPrintF(debug,"MemLeak bytes=%d",endFreeDynamicMemory-startFreeDynamicMemory);
       //}
       //ErrNonFatalDisplayIf (startFreeDynamicMemory != endFreeDynamicMemory,
       //                      debug);
}

//*****************************************************************************************
//---------------------------------------------------------------
void EventLoop(void)
{
  EventType event;
  Word      error;
  Boolean firstTime = true;
  unsigned long timeToWait = 0;

  do
  {
    //insertions 08022005
    //if a timer is in effect, then calculate a time to wait
    //     Note..cannot use if scanner is enabled
    if (timerExpirationTime==0)  
    {
       timeToWait = -1;  //wait forever
    }
    else
    {
       timeToWait = (timerExpirationTime - TimGetTicks());
       if(timeToWait<0) timeToWait=1;
    }
    EvtGetEvent(&event, timeToWait);     
 
    //end insertions 08022005
    //08022005 EvtGetEvent(&event, evtWaitForever);
    firstTime=true;
//Trace("event %d\n",event.eType);

    if( (event.eType==keyDownEvent)&&(
                                      ((event.data.keyDown.chr >= vchrHardKeyMin ) && ( event.data.keyDown.chr <= vchrHardKeyMax))//2004.07.26 GWH
                                    ||((event.data.keyDown.chr >= 0x300 ) && ( event.data.keyDown.chr <= 0x3ff))//2004.01.05 GWH
                                    ||((event.data.keyDown.chr >= 0x500 ) && ( event.data.keyDown.chr <= 0x5ff))//2003.10.11 GWH
                                    ||(event.data.keyDown.chr==vchrLaunch )
                                    ||(event.data.keyDown.chr==vchrCommand )  //2005.02.08
                                    ||(event.data.keyDown.chr==chrCommandStroke )  //2005.02.08
                                    ||(event.data.keyDown.chr==0x006 )  //2005.02.21
                                    ||(event.data.keyDown.chr==vchrFind )
                                    ||(event.data.keyDown.chr==vchrCalc )
                                    ||(event.data.keyDown.chr==vchrKeyboardAlpha )
                                    ||(event.data.keyDown.chr==vchrKeyboardNumeric )
                                    ||(event.data.keyDown.chr==vchrPowerOff ) //2002.05.27 GH
                                    ||((event.data.keyDown.chr=='S') && (event.data.keyDown.keyCode==1) && (event.data.keyDown.modifiers==commandKeyMask)) //2004.10.07 GWH
                                    
                                    )) goto notSystem; 
                                   
letSystemTry:
    if (!SysHandleEvent(&event))
    {
notSystem:
        
        //03.16.2005 if(MenuGetActiveMenu() == NULL) goto noMenu;
       //02282005 if((MenuGetActiveMenu() == NULL) && (event.data.keyDown.chr != vchrCommand))goto noMenu;  //modofied 02252005
//Trace("at menu");
       if (! MenuHandleEvent(menuPtr,&event,&error))
       {
noMenu:
//Trace("Menu no");
          if (!ApplicationHandleEvent(&event))
          {
             

             FrmDispatchEvent(&event);
             if( (event.eType==keyDownEvent)&&(
                                      ((event.data.keyDown.chr >= vchrHardKeyMin ) && ( event.data.keyDown.chr <= vchrHardKeyMax))//2004.07.26 GWH
                                    ||((event.data.keyDown.chr >= 0x300 ) && ( event.data.keyDown.chr <= 0x3ff))//2004.01.05 GWH
                                    ||((event.data.keyDown.chr >= 0x500 ) && ( event.data.keyDown.chr <= 0x5ff))//2003.10.11 GWH
                                    ||(event.data.keyDown.chr==vchrCommand )   //2005.02.08
                                    ||(event.data.keyDown.chr==chrCommandStroke )   //2005.02.08
                                    ||(event.data.keyDown.chr==0x006 )  //2005.02.21
                                    ||(event.data.keyDown.chr==vchrLaunch )
                                    ||(event.data.keyDown.chr==vchrFind )
                                    ||(event.data.keyDown.chr==vchrCalc )
                                    ||(event.data.keyDown.chr==vchrKeyboardAlpha )
                                    ||(event.data.keyDown.chr==vchrKeyboardNumeric )
                                    ||(event.data.keyDown.chr==vchrPowerOff ) //2002.05.27 GH
                                    ||((event.data.keyDown.chr=='S') && (event.data.keyDown.keyCode==1) && (event.data.keyDown.modifiers==commandKeyMask)) //2004.10.07 GWH
                                    
                                    )&&(firstTime==true)&&(eventHandled==0))
             {
//dbug("after frmdispatch ev");
                firstTime=false;
                goto letSystemTry;
             }
          }
       }
    }   
  } while (event.eType != appStopEvent);
  if( (event.eType==appStopEvent) && (terminationAction=='l'))
  {
      terminationAction='o';
  }
  if(terminationAction=='n') terminationAction='o';  //must be o.s. stopping
//dbug("going to termin");
  typeOfScript='z'  ;
  misc_codeexec (9915  ,4l);  //exec pgm termination code
  
//dbug("back from termin");
//SysFatalAlert("got app stop event");
}
//*****************************************************************************************
//---------------------------------------------------------------------
static Boolean ApplicationHandleEvent(EventPtr event)
{
  Int		formId;
  Boolean	handled = false;
  FormPtr frm;  //temp
  
  if (event->eType == frmLoadEvent)
  {
		// Load the form resource specified in the event then activate the form.
		
	    ignoreNextPenup=false;       //01212004
		
 
		formId = event->data.frmLoad.formID;

//StrPrintF(debug,"os6 init form=%d",formId);
//SysFatalAlert(debug);
          
        frm=FrmGetFormPtr(formId);       //added 5.21.2000
        if(romVersion<0x06000000)      //added 01222004 (os6)
        {                              //added 01222004 (os6)
             if(frm==NULL)               //added 5.21.2000 
             {                           //added 5.21.2000
                frm = FrmInitForm(formId); //added 5.21.2000
             }                           //added 5.21.2000
        }                             //added 01222004 (os6)     
        else                          //added 01222004 (os6)
        {                             //added 01222004 (os6)
             //note..looks like always need to do this on os6 //added 01222004 (os6)
             frm = FrmInitForm(formId);//added 01222004 (os6)
        }                             //added 01222004 (os6)
        // Set the event handler for the form.  The handler of the currently
	// active form is called by FrmDispatchEvent each time it receives an event.
		
//dbug("os6 set active form");

        FrmSetActiveForm(frm);
        FrmSetEventHandler(frm, ViewHandleEvent);
//dbug("os6 screen init done");
        handled=true;
  }
  return handled;
}
//*****************************************************************************************
//------------------------------------------------------------------------
static Boolean ViewHandleEvent(EventPtr event)
{
	 
	unsigned long freeMem;
    unsigned long largestMem;

    Word       idx;
	FieldPtr   fldp;
	FieldPtr   fldPtr;
	Boolean    handled=false;
	int        res;
	ControlPtr ptr;
	int        j;
	int        kind;
	FormPtr    frmPtr;
  
	UInt16       bmpID;    //added 07222003
	MemHandle  bmpH;       //added 07212003
	BitmapType * bmpPtr;   //added 07212003
	UInt32     winAttr;    //added 07212003
	UInt32     winVersion; //added 07212003
	Err      err;          //added 07212003
	int      xFactor;      //added 07212003
	int      yFactor;      //added 07212003
	Word fldHeight;
	Word textHeight;
	Word scrollPos;
	ListPtr lstPtr;
 	FieldAttrType attr;  //GHGlue
  
	RectangleType r; 
	Word saveFldToUpdate;  //added 5.20.2000  
	TableType* grid;             //added 09242003
	struct ObjectData * objPtr;  //added 09242003   
	if(event->data.fldChanged.fieldID==10503) return true; //2003.04.28 ignore spurious event from Find 
	
	if (event->eType==keyDownEvent)
	{	lastKeyDownKeyCode=0;
		lastKeyDownChr=0;
		lastKeyDownModifiers=0;
    };

	switch( event->eType )
	{
	case fldChangedEvent:
		fldToUpdate=event->data.fldChanged.fieldID;    //01152002
		frmPtr = FrmGetActiveForm();
   
		// see if any attached scrollbars that need updated
		idx = FrmGetObjectIndex( frmPtr, event->data.fldChanged.fieldID );
		if( idx < FrmGetNumberOfObjects( frmPtr ) - 1 )
		{
			if( FrmGetObjectType( frmPtr, idx + 1 ) == frmScrollBarObj )
			{
				// have an attached scrollbar
				UpdateScrollbar( (FieldType *)event->data.fldChanged.pField, (ScrollBarPtr)( FrmGetObjectPtr( frmPtr, idx + 1 ) ) );
			}
		}
		break;
				
	case frmObjectFocusLostEvent:	//Added 11.20.2006 GH for fnav
//StrPrintF(debug,"nav lost=%d",event->data.frmObjectFocusTake.objectID);
//SysFatalAlert(debug)
		frmPtr = FrmGetActiveForm();
		if( frmFieldObj == FrmGetObjectType(frmPtr, FrmGetObjectIndex(frmPtr,event-> data.frmObjectFocusTake.objectID)) )
		{
			fldToUpdate=event->data.frmObjectFocusTake.objectID; 
			chkFldUpdate();
		}
		break;

		
	//09242003 additions
	case tblSelectEvent:
          
         objPtr=locateObjectPtr(  event->data.tblSelect.tableID); 
         objPtr->currentRow=event->data.tblSelect.row+objPtr->firstRow;
         objPtr->currentCol=event->data.tblSelect.column;
    //09242003 end of additions       
	case ctlSelectEvent:
	case lstSelectEvent:
	case ctlRepeatEvent:
	case sclRepeatEvent:
         
//StrPrintF(debug,"ctl-select=%d",event->data.popSelect.controlID);
//SysFatalAlert(debug);

 
        chkFldUpdate();    //added 5.10.2000
        
		if((event->eType==ctlSelectEvent))
		{
			ptr=(ControlType*)(event->data.ctlSelect.pControl);

//StrPrintF(debug,"in ctlSelectEvent  fldToUpdate=%d ctlEntering=%d",fldToUpdate,event->data.ctlSelect.controlID);
//SysFatalAlert(debug);
			
			//if(ptr->style==3)  //if popup--ignore , will get popselect event
			if(CtlGlueGetControlStyle(ptr)==3)   //GHGlue: if popup--ignore , will get popselect event
				goto overAndOut;
		}
      
		typeOfScript='t'  ;
		res = misc_codeexec( event->data.ctlSelect.controlID, 4l );
		
		if( res==1)  handled=true;   //12172001

//dbug("back from codeexec");
//SysFatalAlert("back from codexec on event");
      
		if( event->eType == sclRepeatEvent )
		{
			//must determine if a standalone scrollbar or one attached to a field or grid
			idx=FrmGetObjectIndex(FrmGetActiveForm(),event->data.sclRepeat.scrollBarID);
			if(idx!=0)  //dont do if first obj on form
			{
				if(FrmGetObjectType(FrmGetActiveForm(),idx-1)==frmFieldObj) //must be a fld obj
				{
					//also must have id=scrollbar id -1
					fldp=(FieldType *)FrmGetObjectPtr(FrmGetActiveForm(),idx-1);
					
					//if(fldp->id==(event->data.sclRepeat.scrollBarID-1))
					if(FrmGetObjectId(FrmGetActiveForm(),idx-1)==(event->data.sclRepeat.scrollBarID-1)) //GHGlue
					{
						//its a scrollbar attached to a field obj
						ScrollLines(event->data.sclRepeat.newValue-event->data.sclRepeat.value,false,fldp,(ScrollBarType *)event->data.sclRepeat.pScrollBar);
					}
				}
				//09242003 additions
				else
                {
                  if(FrmGetObjectType(FrmGetActiveForm(),idx-1)==frmTableObj) //if prev obj is a grid
                  {
                     if(idx>0)
                     {
                         
                       if(FrmGetObjectId(FrmGetActiveForm(),idx-1)==(event->data.sclRepeat.scrollBarID-1))             
                       {
                          grid=(TableType *)FrmGetObjectPtr(FrmGetActiveForm(),idx-1); 
                          ScrollGrid(grid,idx-1,event->data.sclRepeat.newValue-event->data.sclRepeat.value);
                       
					}
				}
			}     
		}     
                //09242003 end of additions
			}     
		}     
overAndOut:
		
		handled=false;   //12172001
		break;
	case popSelectEvent:
//StrPrintF(debug,"popselect %d",event->data.popSelect.controlID);
//SysFatalAlert(debug);

		chkFldUpdate();
		typeOfScript='t';
		res = misc_codeexec( event->data.popSelect.controlID, 4l );
		
//SysFatalAlert("back from codexec on event");

		handled = false;
		break;
	case frmOpenEvent:
	    
alsoRedraw:


//dbug("opening form");
		fldToUpdate=0;   //01152002
		executeFormRedraw();           //added 5.21.2000
	
	
		//added 3/24/2002 
		//see if a field has focus and remember it
		   
         frmPtr=FrmGetActiveForm();
         fldPtr=FrmGlueGetActiveField(frmPtr);
         
         
         for(j=0;j<FrmGetNumberOfObjects(frmPtr);++j)
         {
            if(FrmGetObjectPtr(frmPtr,j)==fldPtr)
           {
               fldToUpdate=FrmGetObjectId(frmPtr,j);
//dbug("setting focus id");
               break;
           }
         
         }
         	
		//end of additions 3/24/2002
		handled = true;    //????suspicious
		underway = 1;
		break;
	case menuEvent:
//Trace("***got menu event*** %d",event->data.menu.itemID);

//03.16.2005 added -- cmdBar menu items seem to be in the range of 10001 & up
//let the system handle those
if(   (event->data.menu.itemID>=10000)   //03.16.2005 added
&&    (event->data.menu.itemID<=10010))  //03.16.2005 added
{                                        //03.16.2005 added
		handled=false;                   //03.16.2005 added 
		break;                           //03.16.2005 added
}                                        //03.16.2005 added
		chkFldUpdate();
		//deleted 05202003  MenuEraseStatus(NULL);
		MenuEraseStatus(MenuGetActiveMenu());       //added 05202003
		MenuDispose(MenuGetActiveMenu());           //added 05202003
		

		MenuSetActiveMenu(NULL);

		FrmSetMenu(FrmGetActiveForm(),0);

		menuPtr=(MenuBarType*)null_ptr;
      
//StrPrintF(debug,"going to codex exec %d",event->data.menu.itemID);
//SysFatalAlert(debug);

		typeOfScript='m';
		misc_codeexec(event->data.menu.itemID,4l);
//dbug("past codeexec");
		handled = true;
		break;
	case daySelectEvent:    
//dbug( "at dayselect event");    
		handled = true;
		break;
	case keyDownEvent:
//StrPrintF(debug,"in keydown event val=%d",event->data.keyDown.chr);
//SysFatalAlert(debug);
		eventHandled = 0;
		eventType    = 1;
		if( ( event->data.keyDown.chr >= 0x1700 ) && ( event->data.keyDown.chr <= 0x17ff ) )
			eventType = 4;  //Sony JogDial
		if( ( event->data.keyDown.chr == 259 ) || ( event->data.keyDown.chr==268 ) )
			eventType = 5;  //HandEra JogDial 5/16/01 GWH
		if( ( event->data.keyDown.chr >= 0x500 ) && ( event->data.keyDown.chr <= 0x5ff ) ) //2003.10.11 GWH
			eventType = 7;  //PalmOne event										//2003.10.11 GWH
        if( (event->data.keyDown.chr=='S') && (event->data.keyDown.keyCode==1) && (event->data.keyDown.modifiers==commandKeyMask)) { //2004.10.07 GWH
        	//magic values were set in serialWakeupHandler
        	serialSetState(kSerialDataWaiting);
			eventType = 6;}  //SerialReceived event         	
		
		switch( event->data.keyDown.chr )	// 03/29/2001 MMD
		{
		case vchrHard1:			lastKeyString[0] = 0x01; break;
		case vchrHard2:			lastKeyString[0] = 0x02; break;
		case vchrHard3:			lastKeyString[0] = 0x03; break;
		case vchrHard4:			lastKeyString[0] = 0x04; break;
		case vchrHardPower:		lastKeyString[0] = 0x0e; break;
		case vchrHardCradle:	lastKeyString[0] = 0x0f; break;
		case vchrLaunch:		lastKeyString[0] = 0x11; break;
		case vchrFind:			lastKeyString[0] = 0x07; break;
		case vchrCalc:			lastKeyString[0] = 0x10; break;
		case vchrPowerOff:		lastKeyString[0] = 0x12; break;   				//2002.05.27 GWH
		case vchrLowBattery:	lastKeyString[0] = 0x13; break;   				//2003.04.28 GWH
		case 0x0503:			lastKeyString[0] = 200+event->data.keyDown.keyCode; //2003.01.30 GWH vchrNavChange
								eventType = 7; break; 							//2003.01.30 GWH 
		default:				lastKeyString[0] = event->data.keyDown.chr;
		}
		lastKeyString[1] = 0x00;
		lastKeyDownKeyCode = event->data.keyDown.keyCode;
		lastKeyDownChr = event->data.keyDown.chr;
		lastKeyDownModifiers = event->data.keyDown.modifiers;
		frmPtr = FrmGetActiveForm();
//StrPrintF(debug,"in keydown event val=%d",lastKeyString[0]);
//SysFatalAlert(debug);
		frmNo=FrmGetActiveFormID();   //04022002
		typeOfScript='v';
		misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
        typeOfScript = ' ';   //10262004
		if( eventHandled == 0 )
		{
			//new 1/9/2001 if a toggle button down or up, see if have fld or scrollbar on form and scroll it
			if( (event->data.keyDown.chr==0x0b) || (event->data.keyDown.chr==0x0c) ||(event->data.keyDown.chr==0x1704)||(event->data.keyDown.chr==0x1703))
			{
				for(idx=0; idx<FrmGetNumberOfObjects(frmPtr);++idx)
				{
					if(FrmGetObjectType(frmPtr, idx)==frmFieldObj)
					{
						//if (idx != (FrmGetNumberOfObjects(frmPtr)-1) && (((FieldPtr)FrmGetObjectPtr(frmPtr,idx))->attr.singleLine==0))
						FldGetAttributes((FieldPtr)FrmGetObjectPtr(frmPtr,idx) ,&attr);		//GHGlue
						if (idx != (FrmGetNumberOfObjects(frmPtr)-1) && (attr.singleLine==0) && (attr.visible==1)) //GHGlue
						{
							if(FrmGetObjectType(frmPtr, idx+1)==frmScrollBarObj)
							{
								//have an attached scrollbar
								FldGetScrollValues((FieldPtr)FrmGetObjectPtr(frmPtr,idx),&scrollPos,&textHeight,&fldHeight);

								if((event->data.keyDown.chr==0x0c)||(event->data.keyDown.chr==0x1704))
								{
									//scroll down
									if ((scrollPos+fldHeight)<textHeight)
										FldScrollField((FieldPtr)FrmGetObjectPtr(frmPtr,idx),fldHeight,winDown);
								}
								else
								{
									//scroll up
//StrPrintF(debug,"pos=%d fldHt=%d txtHt=%d",scrollPos,fldHeight,textHeight);
//SysFatalAlert(debug);
									if ((scrollPos!=0)&&(scrollPos<fldHeight))
										FldScrollField((FieldPtr)FrmGetObjectPtr(frmPtr,idx),scrollPos,winUp);
									else
										//04032002if (scrollPos>fldHeight)
										if (scrollPos>=fldHeight)          //04032002
											FldScrollField((FieldPtr)FrmGetObjectPtr(frmPtr,idx),fldHeight,winUp);
								}
								UpdateScrollbar((FieldPtr) FrmGetObjectPtr(frmPtr, idx),
										(ScrollBarPtr)FrmGetObjectPtr(frmPtr, idx+1));
							}
						}
					}
					else
					{
						if( FrmGetObjectType(frmPtr, idx)==frmListObj)
						{
							//have a listbox
							if( idx == 0 ) goto skipPopupCheck;
							if( FrmGetObjectType( frmPtr, idx - 1) == frmPopupObj) goto dontUseListbox;  //belongs to popup ctl

							{
skipPopupCheck:
								lstPtr = FrmGetObjectPtr( frmPtr, idx );
//StrPrintF(debug,"pos=%d fldHt=%d txtHt=%d",lstPtr->topItem,LstGetVisibleItems(lstPtr),LstGetNumberOfItems(lstPtr));
//SysFatalAlert(debug);
								if(event->data.keyDown.chr==0x0c)
								{
									//scroll down
									//if ((lstPtr->topItem+LstGetVisibleItems(lstPtr))<LstGetNumberOfItems(lstPtr))
									if ((LstGlueGetTopItem(lstPtr)+LstGetVisibleItems(lstPtr))<LstGetNumberOfItems(lstPtr)) //GHGlue
									{
										//if ( (lstPtr->topItem+LstGetVisibleItems(lstPtr)*2)>LstGetNumberOfItems(lstPtr))
										if ( (LstGlueGetTopItem(lstPtr)+LstGetVisibleItems(lstPtr)*2)>LstGetNumberOfItems(lstPtr))  //GHGlue
											LstSetTopItem(lstPtr, LstGetNumberOfItems(lstPtr)-LstGetVisibleItems(lstPtr));
										else
											//LstSetTopItem(lstPtr,lstPtr->topItem+LstGetVisibleItems(lstPtr));
											LstSetTopItem(lstPtr,LstGlueGetTopItem(lstPtr)+LstGetVisibleItems(lstPtr)); //GHGlue
									}
								}
								else
								{
									//scroll up
									//if ((lstPtr->topItem!=0)&&(lstPtr->topItem<LstGetVisibleItems(lstPtr)))
									if ((LstGlueGetTopItem(lstPtr)!=0)&&(LstGlueGetTopItem(lstPtr)<LstGetVisibleItems(lstPtr))) //GHGlue
										LstSetTopItem(lstPtr,0);
									else
										//if (lstPtr->topItem>=LstGetVisibleItems(lstPtr))
										if (LstGlueGetTopItem(lstPtr)>=LstGetVisibleItems(lstPtr))  //GHGlue
											LstSetTopItem(lstPtr,LstGlueGetTopItem(lstPtr)-LstGetVisibleItems(lstPtr)); 
								}
								LstDrawList(lstPtr);
							}
						}
						//start additions 02062004
						else
						{
				 
						     if( FrmGetObjectType(frmPtr, idx)==frmTableObj)
						     {
						         
				 
						         grid=(TablePtr)FrmGetObjectPtr(frmPtr,idx);
				 
						         //03052004objPtr=locateObjectPtr(  event->data.tblSelect.tableID);
						         objPtr=locateObjectPtr( FrmGetObjectId(frmPtr,idx));   //03052004
				 
						         if(  (event->data.keyDown.chr==0x0c) ||  (event->data.keyDown.chr==0x1704) )
						             ScrollGrid(grid , idx, objPtr->scrollPagesize);  //modified 03052004
						         else
						             ScrollGrid(grid , idx, 0-objPtr->scrollPagesize); //modified 03052004
							}
						}
						//end additions 02062004
dontUseListbox:  
						;                  
					}
				}
			} 
			
			handled = false;
		}
		else
			handled = true;
		break;
	case penUpEvent:
//SysFatalAlert("PENUP");
//Trace("Penup 1");		
		if ((firstPenDown==0 )||(thePenDownForm!=FrmGetActiveForm())) break;  //sometimes the OS sends a penup as the first event of a program
//Trace("Penup 2");
		eventHandled	= 0;
		eventType		= 3;
		currentX			= event->screenX;
		currentY			= event->screenY;
		penDown			= event->penDown;  
		frmPtr			= FrmGetActiveForm();

		if(ignoreNextPenup==true)
		{
//Trace("Penup 3");
		   ignoreNextPenup=false;
		   goto skipIt;
		   
		}
		frmNo=FrmGetActiveFormID();   //04022002
		if( msgboxOrAlertUp == false )
		{
//Trace("Penup 4");
			typeOfScript='v';
			misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
		    typeOfScript = ' ';      //10262004
		}
		if( eventHandled == 0 ) 
		{
			if( msgboxOrAlertUp == false )
			{
//SysFatalAlert("msboxAlertUp is false");
				
//Trace("Penup 5");				
				

				//-----------end of additions 07212003----------------
				frmPtr = FrmGetActiveForm();
				//for( j = 0; j < FrmGetNumberOfObjects( frmPtr ); ++j )
				for( j = FrmGetNumberOfObjects( frmPtr )-1; j >=0 ; --j ) //01.21.2009 GH:find topmost, not bottommost, first
				{
					controlPtr=FrmGetObjectPtr(frmPtr,j);
					kind=FrmGetObjectType(frmPtr,j);

					if( ( kind == frmLabelObj ) || ( kind == frmGadgetObj ) || ( kind == frmBitmapObj ) )
					{             
						//added following 07212003
						xFactor=1;                 
						yFactor=1;                 
						if(kind == frmBitmapObj)   
						{						   
							//dbug("Bitmap Click 1");
						    err = FtrGet(sysFtrCreator, sysFtrNumWinVersion, &winVersion);
			                if(winVersion>=4)
			                {
			                        //Upon return, if version has a value of 4 or greater, the High-
									//Density Display feature set is present. Note that just because the
									//High-Density Display feature set is present, it isnt necessarily being
									//used. You may want to check the density of the screen, as follows:
							        WinScreenGetAttribute(winScreenDensity, &winAttr);
//StrPrintF(debug,"winDensity=%lu  ",winAttr);
//dbug(debug);
									if (winAttr == kDensityDouble) 
									{
					                    bmpID=FrmGetObjectId(frmPtr,j);
					                    bmpH=DmGetResource((unsigned long)'Tbmp',bmpID);
					                    bmpPtr=(BitmapType*)MemHandleLock(bmpH);
										FrmGetObjectBounds(frmPtr,j,&r);
//StrPrintF(debug,"bmpId=%d bmpDensity=%d x=%d y=%d origx=%d origy=%d w=%d h=%d" 
//,bmpID,BmpGetDensity(bmpPtr),event->screenX,event->screenY,r.topLeft.x,r.topLeft.y,r.extent.x,r.extent.y);
//dbug(debug);
					                    if(BmpGetDensity(bmpPtr)==kDensityDouble)
					                    {
//dbug("recognized high density");
					                            //the screen is high density
					                            //xFactor=2; //01.21.2009 GH Does not seem to be needed
					                            //yFactor=2; //01.21.2009 GH Does not seem to be needed
					                                         //01.21.2009 If indeed the case, this whole code block
					                                         //can be eliminated.
					                    }
					                    MemPtrUnlock(bmpPtr);
					                    DmReleaseResource( bmpH);       
				                    }
			                           
			                }
						}                          
						//StrPrintF(debug,"Bitmap Click 2 factor=%d,%d",xFactor,yFactor);
						//dbug(debug);
						
						//---end of additions 07212003------------------					
						FrmGetObjectBounds(frmPtr,j,&r);
						if( ( event->screenX >= r.topLeft.x )
							&& ( event->screenX <= ( r.topLeft.x + r.extent.x /xFactor) )   //modified 07212003
							&& ( event->screenY >= r.topLeft.y )
							&& ( event->screenY <= ( r.topLeft.y + r.extent.y/yFactor ) ) ) //modified 07212003
						{
//							if( ((kind==frmLabelObj) && (((FormLabelType*)controlPtr)->attr.usable==1) )  //GHGlue
//								|| ((kind==frmGadgetObj) && (((FormGadgetType*)controlPtr)->attr.usable==1) )
//								|| ((kind==frmBitmapObj) && (((FormBitmapType*)controlPtr)->attr.usable==1) ) )
							//dbug("Bitmap Click 3");
							if(FrmGlueGetObjectUsable(frmPtr,j))    //GHGlue
							{
//StrPrintF(debug,"looking for code for %d  x=%d y=%d",FrmGetObjectId(frmPtr,j),currentX,currentY);
//SysFatalAlert(debug);
								//dbug("Bitmap Click 4");
								chkFldUpdate();    //11-4-2001 added 
								typeOfScript='t';
								//dbug("Bitmap Click 5");
								res = misc_codeexec( FrmGetObjectId( frmPtr, j ), 4l );
								//11/4/2001 ignoreNextPenup=true;
								break;
							}     
						}              
					}          
				}
			}
			
		
		}
		else
			EvtFlushPenQueue();    //event handled by application so ignore the point
skipIt:
		if( eventHandled == 0 )	handled = false;
		else							handled = true;
		break;
	case penDownEvent:
//SysFatalAlert("PENDOWN");

//Trace("Pendown 1");
		firstPenDown	= 1;
		eventHandled	= 0;
		eventType		= 2;
		currentX			= event->screenX;
		currentY			= event->screenY;
		penDown			= event->penDown;  
		frmPtr			= FrmGetActiveForm();
        frmNo=FrmGetActiveFormID();   //04022002
		thePenDownForm = frmPtr;
		if( msgboxOrAlertUp == false )
		{
//Trace("Pendown 2");
			typeOfScript='v';
			misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
		    typeOfScript = ' ';    //10262004
		}
		if( eventHandled == 0 )	handled = false;
		else					handled = true;
		
		//see if the pendown within a signature capture gadget control
		if( (sigCaptureCtlId!=0)&&(sigCaptureSw==true))
		{
		     //there is a ctl active--see if on it
		     if(  FrmGetActiveFormID()==sigCaptureFormId) //must be on same form too
		     {
		         GetSignatureGadgetBounds(&r);
		         if( (currentX>=r.topLeft.x) && (currentX<=(r.topLeft.x+r.extent.x)) && (currentY>=r.topLeft.y) && (currentY<=(r.topLeft.y+r.extent.y)) )
		         {
		            CaptureSignature();
		         }
		     }
		}
		break;
	case penMoveEvent:

		handled=false;
		break;  
	case firstUserEvent:
//dbug("at user event");
		if (event->data.generic.datum[1]==1)
		{

			//nextscreen
			frmNo=event->data.generic.datum[0];
			//12172001 firstPenDown=0;
			
//StrPrintF(debug,"frmno=%d",frmNo);
//SysFatalAlert(debug);
			//aborts MemPtrFree(event);  //08032002
			if(event->data.generic.datum[2]==0)
			{
     	        resetToDesignState=false;
			}
			else
			{
			    //wants to set form back to design state
			    FrmDeleteForm(FrmGetFormPtr(frmNo));
		        FrmInitForm(frmNo);
		        resetToDesignState=true;
			}   

//MemHeapFreeBytes(0,&freeMem,&largestMem);
//StrPrintF(debug,"free bytes=%lu", freeMem);
//dbug(debug);
			FrmGotoForm(frmNo); 
			handled = true;
			break;
		}    
		else
			goto alsoRedraw;    //don't know if ever get here
	case firstUserEvent+1:
		goto alsoRedraw;        //can't find where this is used   
	case fldEnterEvent:
         //3/24/2002 cntFldEnters+=1;
         
         
//StrPrintF(debug,"in fldEnterEvent  fldToUpdate=%d   fldEntered=%d",fldToUpdate,event->data.fldEnter.fieldID);
//SysFatalAlert(debug);
		saveFldToUpdate=fldToUpdate;           //added 5.20.2000
		if(fldToUpdate!=event->data.fldEnter.fieldID)
		{

		    chkFldUpdate();   //added 5.10.2000 dont fire on same field as already in
		}    
       
		
		//added 5.20.2000 save field to update but not if chkFldUpdate fired the last field's validate code and it repositioned to a new field
		if((fldToUpdate==0)||(saveFldToUpdate==fldToUpdate)) 
		{
		    fldToUpdate=event->data.fldEnter.fieldID;  //added 5.20.2000 save which fld starting into
//dbug("saved new field ID");
		}
		
		ignoreNextPenup=true;  //01312002  so can have bitmaps under field
		handled = false;
		break;   
	case frmCloseEvent:	// 03/29/2001 MMD Commented this out because it does nothing
//dbug("in form close");
		//additions 08262004

		eventHandled = 0;
		eventType    = 9;
		frmPtr=FrmGetActiveForm();
		frmNo=FrmGetActiveFormID();    
//StrPrintF(debug,"firing close event fomid=%d",frmNo);
//SysFatalAlert(debug);
		typeOfScript='v';
		misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
		typeOfScript = ' ';      //10262004
		//end additions 08262004
		frmPtr=FrmGetActiveForm();  //01152002
		FrmEraseForm(frmPtr);       //01152002
		handled=true;   //12172001  keep from unloading
		
		break;
	case ctlEnterEvent:
	case ctlExitEvent:
	case fldHeightChangedEvent:
	case frmSaveEvent:
	    break;   //added 08262004
	case frmUpdateEvent:
	    //09192004 break;  //inserted 09172004 to avoid errors
//04132005	    if( typeOfScript== 'v' ) break;  //09172004 or else will get loop
 	     if( ( typeOfScript== 'v' )    //04132005
 	     ||  ( typeOfScript== 'b' )    //04132005
 	     ||  ( typeOfScript== 'a'))    //04132005
 	          break;                   //04132005 or else will get loop
 	          
 	    if(msgboxOrAlertUp)           //09192004
 	    {                             //09192004
 	        //remember that we have to process a frmUpdateEvent after script over  //09192004
	        pendingFormUpdateEventSw = true; //09192004
 	        break;                    //09192004
 	    }
	                                 //09192004
	    //additions 08262004
		eventHandled = 0;
		spawnFormUpdateEventCode();   //09192004
		
		if( eventHandled == 0 )	handled = false;
		else					handled = true;
 		break;
		//end additions 08262004
		
	//case frmTitleEvent:
	case frmTitleSelectEvent:
	case lstEnterEvent:
	case lstExitEvent:
	//case popSelectEvent:
	case sclEnterEvent:
	case sclExitEvent:
    //case nilEvent:				//03.06.02 GH
	case tblEnterEvent:
	case tblExitEvent:
	case winEnterEvent:
	case winExitEvent:
      handled = false;
      break;
    case nilEvent:
/*		if (gSerialState==kSerialWaitingForData) {			//03.06.02 GH
dbug("nilEvent waiting for data");
			serialSetState(kSerialDataWaiting);
			eventHandled = 0;
			eventType    = 6;
			frmNo=FrmGetActiveFormID();   //04022002
			typeOfScript='v';
			misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
			typeOfScript = ' ';     //10262004
			if( eventHandled == 0 )	handled = false;
			else					handled = true;
			break;
		} 
		else
			handled = false;
			break;  */ 
			
		//start insertions 08022005	
        if(timerExpirationTime != 0) 
        { 
          if( timerExpirationTime <= TimGetTicks()) 
          {
            //timer has expired
            timerExpirationTime=0;  //prevent from firing again
           
           //exec form event code
           eventType = 10;
          
           //lastKeyString[1] = 0x00;
		   //lastKeyDownKeyCode = event->data.keyDown.keyCode;
		   //lastKeyDownChr = event->data.keyDown.chr;
		   //lastKeyDownModifiers = event->data.keyDown.modifiers;
		   
		   frmPtr = FrmGetActiveForm();
//StrPrintF(debug,"in keydown event val=%d",lastKeyString[0]);
//SysFatalAlert(debug);
		   frmNo=FrmGetActiveFormID();    
		   typeOfScript='v';
		   misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
           typeOfScript = ' ';    
             
          }
        }
    
    
       handled=false;
       break;
        //end insertions 08022005
	default:	// 03/29/2001 MMD This case expanded to allow user events to pass
				// 05/16/2001 GHW added special case for HandEra displayExtentChangedEvent
		if( ( event->eType >= firstUserEvent + 0x0100 ) || (event->eType==firstUserEvent - 2) )
		{
			eventHandled	= 0;
			eventType		= event->eType;
			frmPtr			= FrmGetActiveForm();
			frmNo=FrmGetActiveFormID();   //04022002
			if( msgboxOrAlertUp == false )
			{
				typeOfScript='v';
				misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
			    typeOfScript = ' ';   //10262004
			}
			if( eventHandled == 0 )	handled = false;
			else					handled = true;
		}
		else
	      handled = false;
      break;


	}
	return handled;
} // ViewHandleEvent

//*****************************************************************************************
void chkFldUpdate()    //see if have left a field and need to fire code for that field
{
   ControlPtr ptr;
   FormPtr frmPtr ;
   int theFld;  //01152002
   int j;
   int res;
   if (fldToUpdate==0) return;
//StrPrintF(debug,"fldUpdate fld=%d",fldToUpdate);
//SysFatalAlert(debug);

//3/24/2002 cntFldEnters=0;
         frmPtr=FrmGetActiveForm();
         for(j=0;j<FrmGetNumberOfObjects(frmPtr);++j)
         {
            ptr=(ControlType*)FrmGetObjectPtr(frmPtr,j);
            //if(ptr->id==fldToUpdate)
            if(FrmGetObjectId(frmPtr,j)==fldToUpdate) //GHGlue
            {
               
               FldReleaseFocus((FieldPtr)ptr);
                  
               theFld=fldToUpdate;   //01152002
               fldToUpdate=0;        //01152002
               
               typeOfScript='t';
               res=misc_codeexec(theFld,4l);  //01152002
               
               return;                    
            }
         }        
   
}


//*****************************************************************************************
int dbGetInt(char* ptr)
{
  int i;
  char* p=ptr;
  *(char*)&i=*p;
  ++p;
  *(((char*)&i)+1)=*p;
  return(i);
}
//*****************************************************************************************
void init_a_work_var(int i)
{
	unsigned char a, b;
	unsigned long cl, dl;
	double wkdbl;
	int j,k,l;
	int wkid;
	int wki;
	Long wkl;
	int jjj;
	double d;
	char conn  = 'n';
	char cons  = 's';
	int con256 = 256;
	int con32k = 32767;   
	int con17  = 17;
	int con13  = 13;
	int con4   = 4;
	int con11  = 11;
	int con7   = 7;
	int con2   = 2;
	ULong triple;
	char hex80;
	char hex40;
	char hex00;
	double wkd;
	char theByte=0x00;


	hex80=0x80;
	hex40=0x40;
	hex00=0x00;
   

   wki=DELEN;
   wkl=wki;
//StrPrintF(debug,"init wk var %d",i+work_de_max*userFunctionSequentialNo);
//SysFatalAlert(debug);
   
   if (i<=string_work_de_max)       //06012002
   {                                           //05282003
         hData=DmNewResource(VbRscDB,(ULong)'WkVr',i+work_de_max*userFunctionSequentialNo,wkl);
         if(hData==0)  OutOfMemory(6005);      //05282003
   }                                           //05282003      
   else                            //06012002
   {                               //06012002
//dbug("allocating arith wk");
       hData=MemHandleNew(wkl+8);  //06012002
       if(hData==0)  OutOfMemory(6006);      //05282003
       theByte=0x02;  //memory alloc on dyn heap  //06012002
   }                               //06012002
   if(hData==0)
   {
      SysFatalAlert("Out of Memory 2");
      ErrThrow(-1);                 //inserted 05212003
      //05212003 return;
   }
   de=(struct dataelem *)MemHandleLock(hData);
   
//dbug("initwkvar");   
   DmWrite(deListPtr,i*4,&de,4);  //store handle in de list
   
   //06012002updtc((struct dataelem*)de,&de->isConstant,&theByte);
   updtc((struct dataelem*)de,&de->flags3,&theByte);   //06012002
   updti((struct dataelem*)de,&de->tbl_no,&i);
   
   updti((struct dataelem*)de,&de->max_no_values1,&con1);
   updti((struct dataelem*)de,&de->max_no_values2,&con1);
   updti((struct dataelem*)de,&de->max_no_values3,&con1);
   updti((struct dataelem*)de,&de->cur_no_values,&con1);  
//StrPrintF(debug,"setting kind=%d",*((char*)(subptr+10)) );
   if (i<=string_work_de_max)
   {
		
		updti((struct dataelem*)de,(unsigned int*)(&de->length), &con32k);   
		updti((struct dataelem*)de,(unsigned int*)(&de->dgt_bef),&con0);
		updti((struct dataelem*)de,(unsigned int*)(&de->dgt_aft),&con0); 
		updtc((struct dataelem*)de,&de->kind,&cons);
		updtc((struct dataelem*)de,&de->flags,&hex00);
   }
   else
   {
		updti((struct dataelem*)de,(unsigned int*)(&de->length), &con11);
		updti((struct dataelem*)de,(unsigned int*)(&de->dgt_bef),&con0);
		updti((struct dataelem*)de,(unsigned int*)(&de->dgt_aft),&con0); 
		updtc((struct dataelem*)de,&de->kind,&conn);
		updtc((struct dataelem*)de,&de->flags,&hex40);
   } 
 //SysFatalAlert("de switch");
	switch(de->kind)
	{
	case 's':
		updti((struct dataelem*)de,(unsigned int*)(&de->dgt_bef),&con0);  //allows to be resized
		updti((struct dataelem*)de,(unsigned int*)(&de->dgt_aft),&con0);  
		jjj=DELEN+2;
		updti((struct dataelem*)de,(unsigned int*)(&de->firstData),&jjj);
		wki=(int)(DELEN);
//StrPrintF(debug,"resize1 string to %d -- max#val=%d size=%d",wki,de->max_no_values1,(int)DELEN);
//SysFatalAlert(debug);
		
		wki=wki+(((struct dataelem*)de)->max_no_values1*((struct dataelem*)de)->max_no_values2*((struct dataelem*)de)->max_no_values3)*2;
//StrPrintF(debug,"resize2 string to %d -- max#val=%d size=%d",wki,de->max_no_values1,(int)DELEN);
//SysFatalAlert(debug);
		wkl=wki;
		MemHandleUnlock(hData);
		hData=DmResizeResource(hData,wkl);
		if(hData==0)  OutOfMemory(6008);      //05282003
		de=(struct dataelem *)MemHandleLock(hData);
//StrPrintF(debug,"string var setup = %d",i);
//SysFatalAlert(debug);				
		DmWrite(deListPtr,i*4,&de,4);  //store handle in de list
				
		wki=DELEN+(2);
		updti((struct dataelem*)de,(unsigned int*)(&de->firstData),&wki);
		updti((struct dataelem*)de,(unsigned int*)((char*)de+DELEN),&con0);  //offset=0 means no data
		break;
	default:
		jjj=DELEN;
		updti((struct dataelem*)de,(unsigned int*)(&de->firstData),&jjj);
		
		wki=DELEN+8;
		wkl=wki;
//StrPrintF(debug,"resize double to %d i=%d cnt=%d",wki,i,de_cnt);
//SysFatalAlert(debug);
		
		
		//06012002 MemHandleUnlock(hData);
		//06012002 hData=DmResizeResource(hData,wkl);
		//06012002 de=(struct dataelem*)MemHandleLock(hData);
		
//StrPrintF(debug,"numer var setup = %d",i);
//SysFatalAlert(debug);				
		DmWrite(deListPtr,i*4,&de,4);  //store handle in de list
		
		
		wki=DELEN;
		updti(de,&de->firstData,&wki);
		wkd=0;
		updtd(de,(double*)(((char*)de)+DELEN),&wkd);
		break;
	}
   
}


//*****************************************************************************************
void setupADataelem(int i)
{
	 char xx;
	unsigned char b;
	unsigned char a;
	unsigned long dl;
	unsigned long cl;
	double wkdbl;
	int j,k,l;
	int wkid;
	int wki;
	Long wkl;
	int jjj;
	double d;
	char *subptr;
	double dbl0=0;
	ULong triple;
	ULong offset;  //11-4-2001
	MemHandle hData;
	MemHandle h2;
	char *p2;
	char theByte=0x00;
	int tempi;
	int tempi2;

	if (i <=(string_work_de_max+arith_work_de_max))
	{
//StrPrintF(debug,"going to init_a_work_var #=%d",i);
//SysFatalAlert(debug);
		init_a_work_var(i);
		return;
	}
 //StrPrintF(debug,"setup reg var=%d",i);
 //SysFatalAlert(debug);
   h2=DmGetResource(rscTag,(ULong)9908);
   if(h2==0)
   {
		SysFatalAlert("Can't find program's dataelem record");
		ErrThrow(-1);                 //inserted 05212003
		//05212003 return;
   }

   p2=(char*)MemHandleLock(h2);
//dbug("zz1");

   
   offset=(i-string_work_de_max-arith_work_de_max-1);
   offset*=18l;  //11-4-2001
//StrPrintF(debug,"id=%d offset=%lu strmax=%d arithmax=%d  record=%lu",i,offset,string_work_de_max,arith_work_de_max,p2);
//SysFatalAlert(debug);
   subptr=(char*)p2+offset;                               //11-4-2001
   wkid=dbGetInt(subptr+0);  /*get tbl no*/
      
   wkl=DELEN;
   
//StrPrintF(debug,"de=%lu bytes id=%d size=%d",wkl,wkid,(int)DELEN);
//SysFatalAlert(debug);
  
   if((*((char*)subptr+14)=='s')                            //06012002
     || (*((int*)(subptr+8)) >1)							//06012002
     || (*((int*)(subptr+10)) > 1)							//06012002
     || (*((int*)(subptr+12))> 1)  						    //06012002
     || (varsInDynamHeap >= maxVarsInDynamHeap)) 			//06012002
    {
//dbug("zz2--resourc");
      hData=DmNewResource(VbRscDB,(ULong)'Data',wkid,wkl);
      if(hData==0)  OutOfMemory(6007);      //05282003
    }
    else													//06012002
    {                              						    //06012002
//StrPrintF(debug,"zz3--heap id=%d",i);
//dbug(debug);
       ++varsInDynamHeap;									//06012002
       hData=MemHandleNew(wkl+8);
       if(hData==0)   OutOfMemory(6040);  //05282003   						//06012002
       theByte=0x02;  //memory alloc on dyn heap  			//06012002
//tempi=theByte;
//StrPrintF(debug,"theByte=%d",tempi);
//SysFatalAlert(debug);
   }      
   if(hData==0)
   {
      SysFatalAlert("Out of Memory 2");
      ErrThrow(-1);                 //inserted 05212003
      //05212003 return;
   }
   de=(struct dataelem *)MemHandleLock(hData);
//StrPrintF(debug,"wkid=%d",wkid);
//SysFatalAlert(debug);
   
//StrPrintF(debug,"real var setup = %d  delistPtr=%lu de=%lu  ",i,deListPtr,de);
//SysFatalAlert(debug);		
   DmWrite(deListPtr,(wkid)*4,&de,4);  //store handle in de list
   
   updti(de,(unsigned int*)(&de->tbl_no),(int*)(&wkid));
//StrPrintF(debug,"read id=%d from address=%lu",wkid,(int*)subptr);
//SysFatalAlert(debug);
   updti(de,(unsigned int*)(&de->length), (int*)(subptr+2));
   updti(de,(unsigned int*)(&de->dgt_bef),(int*)(subptr+4));
   updti(de,(unsigned int*)(&de->dgt_aft),(int*)(subptr+6)); 
   updti(de,(unsigned int*)(&de->max_no_values1),(int*)(subptr+8));  
   updti(de,(unsigned int*)(&de->max_no_values2),(int*)(subptr+10));  
   updti(de,(unsigned int*)(&de->max_no_values3),(int*)(subptr+12));  
//StrPrintF(debug,"id=%d len=%d bef=%d aft=%d max=%d max2=%d max3=%d",
//wkid,de->length,de->dgt_bef,de->dgt_aft,de->max_no_values1,de->max_no_values2,de->max_no_values3);
//SysFatalAlert(debug);
//dbug("zz4");
   if(de->max_no_values1==0)
   {
      updti(de,&de->max_no_values1,&con1);
   }

   if(de->max_no_values2==0)
   {
      updti(de,&de->max_no_values2,&con1);
   }
   if(de->max_no_values3==0)
   {
      updti(de,&de->max_no_values3,&con1);
   }
   triple=2*de->max_no_values1*de->max_no_values2*de->max_no_values3;
   updti(de,&de->cur_no_values,&con0);  
//StrPrintF(debug,"setting kind=%d",*((char*)(subptr+14)) );
//SysFatalAlert(debug);
   
   updtc(de,&de->kind,subptr+14);
   updtc(de,&de->flags,subptr+15);
   //06012002 updtc(de,&de->isConstant,subptr+16);	// constant flag
//StrPrintF(debug,"flags3=%d",*(subptr+16));
//dbug(debug);

   xx=*((char*)subptr+16) | theByte;                   //06012002
//tempi=xx;
//tempi2=theByte;
//StrPrintF(debug,"setting flag3=%d   when theByte=%d",tempi,tempi2);
//SysFatalAlert(debug);
   updtc(de,&de->flags3,(char*)(&xx));	// constant flag //06012002
   
   updtc(de, &de->flags2, subptr+17 );		// 02/20/2001  MMD
      
//SysFatalAlert("de switch");
   switch(de->kind)
   {
   case 's':
//dbug("zz5-string");
		jjj=DELEN+triple;
		updti(de,&de->firstData,&jjj);
		
		wki=(int)(DELEN);
//StrPrintF(debug,"resize1 string to %d -- max#val=%d size=%d",wki,de->max_no_values1,(int)DELEN);
//SysFatalAlert(debug);
		wki=wki+triple;
		wkl=wki;
		MemHandleUnlock(hData);
		hData=DmResizeResource(hData,wkl);
		if(hData==0)
		{
		      SysFatalAlert("out of memory");
		      ErrThrow(-1);                 //inserted 05212003
		}
		de=(struct dataelem*)MemHandleLock(hData);
//StrPrintF(debug,"string var setup = %d de=%lu",i,de);
//SysFatalAlert(debug);				
		
		DmWrite(deListPtr,wkid*4,&de,4);  //store handle in de list
		
		wki=DELEN+(triple);
		updti(de,&de->firstData,&wki);
		DmSet(de,DELEN,2*de->max_no_values1*de->max_no_values2*de->max_no_values3,0x00);
		
		break;
	default:
//dbug("zz5-numeric");
		jjj=DELEN;
		updti(de,&de->firstData,&jjj);
		
        wkl=DELEN+4*triple;   //because triple is already 2*sub1*sub2*sub3  
//StrPrintF(debug,"resize double to %lu i=%d cnt=%d",wkl,i,de_cnt);
//dbug(debug);

//tempi=de->flags3;
//StrPrintF(debug,"& 0x02=%d",tempi);
//SysFatalAlert(debug);
		if((de->flags3 & 0x02)==0x02)   						//06012002
		{
//dbug("heap--no resize eeded");	 
	    }
		else											//06012002
        {												//06012002
//dbug("resource resized");
        		MemHandleUnlock(hData);
				hData=DmResizeResource(hData,wkl);
				if(hData==0)
				{
		      		SysFatalAlert("out of memory");
		      		ErrThrow(-1);                 //inserted 05212003
				}
		 		de=(struct dataelem *)MemHandleLock(hData);
		}												//06012002
		//dont know why need//de=(struct dataelem *)MemHandleLock(MemHandleNew(MemHandleSize(hData)));  //03012002 remove
		//dont know why need//MemMove(de,MemHandleLock(hData),MemHandleSize(hData));                    //03012002 remove this too
//StrPrintF(debug,"numer var setup = %d  de=%lu",i,de);
//SysFatalAlert(debug);				
		DmWrite(deListPtr,wkid*4,&de,4);  //store handle in de list
//SysFatalAlert("yyy");		
		wki=DELEN;
		updti(de,&de->firstData,&wki);
//StrPrintF(debug,"before dmset de=%lu  #elems=%d  delen=%d",de,de->max_no_values1*de->max_no_values2*de->max_no_values3,DELEN);
//SysFatalAlert(debug);		

		DmSet(de,DELEN,8*de->max_no_values1*de->max_no_values2*de->max_no_values3,0x00);
//StrPrintF(debug,"after dmset de=%lu  #elems=%d",de,de->max_no_values1*de->max_no_values2*de->max_no_values3);
//SysFatalAlert(debug);		
		break;
   }
    

	MemHandleUnlock(h2);
	DmReleaseResource(h2);
//StrPrintF(debug,"going to initADataElement  de=%lu",de);
//dbug(debug);
	init_a_data_value(i);  //give the var its init value (if any)
//dbug("back from init");
}

//*****************************************************************************************
void init_a_data_value(int de_no)
{
  unsigned char b;
  unsigned char a;
  unsigned long dl;
  unsigned long cl;
  double wkdbl;
  int j,k,l;
  int wkid;
  int wki;
  Long wkl;
  int jjj;
  double d;
  double z;
  MemHandle h2;
  MemHandle hData;
  char* p2;
  
  ULong triple;
//StrPrintF(debug,"init a var's value wkid=%d",de_no);
//SysFatalAlert(debug);    //init data elem value
   h2=DmGetResource(rscTag,(ULong)9909);
   if(h2==0)
   {
       SysFatalAlert("Can't find init values");
       ErrThrow(-1);                 //inserted 05212003
       //05212003 return;

   }

   p2=(char*)MemHandleLock(h2);
   
loop:
    wkid=dbGetInt(p2);



    p2=(char*)p2+2;
   if( wkid==0) goto loopEnd;
   if(wkid!=de_no)
   {
       len=dbGetInt(p2); //length of value string
       p2=(char*)p2+2+len;
       goto loop;    
   }
//StrPrintF(debug,"got value = %s len=%u deListPtr=%lu wkid=%d",(char*)p2+2,len,deListPtr,wkid);
//SysFatalAlert(debug);  
   len=dbGetInt(p2); //length of value string
   p2=(char*)p2+2;
   
   de=*((VoidPtr*)((char*)deListPtr+wkid*4));
   
//StrPrintF(debug,"init wkid=%d len=%u de=%lu",wkid,len,de);
//SysFatalAlert(debug);
   hData=MemPtrRecoverHandle(de);  //??
//SysFatalAlert("1");
   switch(de->kind)
   {
     case 's':

         len=len+MemHandleSize(hData);

         MemHandleUnlock(hData);
//StrPrintF(debug,"resizing string to %u (desize=%d)",len,MemHandleSize(hData));
//SysFatalAlert(debug);
         hData=DmResizeResource(hData,len);
         if(hData==0)  OutOfMemory(6009);      //05282003
         de=(struct dataelem*)MemHandleLock(hData);
         //stores in subscr 1
//StrPrintF(debug,"de resized=%lu",de);
//SysFatalAlert(debug);
         DmStrCopy(de,de->firstData,(char*)p2);
         
         updti(de,(unsigned int*)((char*)de+DELEN),(int*)(&de->firstData));  //ptr to entry 's string
//StrPrintF(debug,"delist = %d",wkid);
//SysFatalAlert(debug);		         
         DmWrite(deListPtr,wkid*4,&de,4);  //store handle in de list
         
         break;
     default:
//StrPrintF(debug,"init dbl=%s",(char*)p2);
//SysFatalAlert(debug);
         wkdbl=StrToDouble((char*)p2);     
         //stores in subscr 1

//StrPrintF(debug,"storing dbl %s  at offset %d",debug2,de->firstData);
//SysFatalAlert(debug);
        /*03092002
          if(de->kind=='i')          //02022002 added
          {                         //02022002 added
            if(wkdbl<0)    z=wkdbl-0.5;      //02022002 added
            else           z=wkdbl+0.5;      //02022002 added
            z=misc_int(z);      //02022002 added
          }                         //02022002 added
          //02102002 else              ;
          else   z=wkdbl;       //02102002
          DmWrite(de,de->firstData,&z,8);
        */  
//StrPrintF(debug,"numer var value = %d",wkid);
//SysFatalAlert(debug);		
        DmWrite(de,de->firstData,&wkdbl,8);
//SysFatalAlert("store ok");
         
   }
 
  
loopEnd:
//SysFatalAlert("init var loop end");
   MemHandleUnlock(h2);
   DmReleaseResource(h2);
}

//*****************************************************************************************
static void ScrollLines( int numLinesToScroll, Boolean redraw, FieldPtr field,ScrollBarPtr scroll)
{
  FormPtr frm=FrmGetActiveForm();
  if(numLinesToScroll<0)
     FldScrollField(field, -numLinesToScroll, winUp);
  else
     FldScrollField(field, numLinesToScroll, winDown);
  //if blank lines at end & scroll up, fldscrollfield makes disappear.
  //so update scrollbar
  if ((FldGetNumberOfBlankLines(field) && numLinesToScroll<0) || redraw )
       UpdateScrollbar(field,scroll);
}
//*****************************************************************************************
static void PageScroll( WinDirectionType direction, FieldPtr field)
{
   FormPtr frm=FrmGetActiveForm();
   if (FldScrollable(field, direction)) 
   {
       int linesToScroll = FldGetVisibleLines(field)-1;
       if (direction==winUp)
           linesToScroll = -linesToScroll;
       //if use need scrollptr ---ScrollLines( linesToScroll, true,field,??);
   }
}                      
//*****************************************************************************************
void UpdateScrollbar(FieldPtr field, ScrollBarPtr scroll)
{
  FormPtr frm=FrmGetActiveForm();
  Word currentPosition;
  Word textHeight;
  Word fieldHeight;
  Word maxValue;
  int i;
  
  
  FldGetScrollValues( field, &currentPosition, &textHeight, &fieldHeight);
//StrPrintF(debug,"cur=%d    textHt=%d fldHt=%d", currentPosition, textHeight, fieldHeight);
//SysFatalAlert(debug);
  
  if(textHeight<=fieldHeight) //don't need scrollbar
  {

      for(i=0;i<FrmGetNumberOfObjects(FrmGetActiveForm());++i)
      {
        if (FrmGetObjectPtr(FrmGetActiveForm(),i)==scroll)
        {
            FrmHideObject(FrmGetActiveForm(),i);
            break;
        }
      }
      return;
  }
  else
  {
//		  scroll->attr.usable=1;  //GHGlue This and next 8 lines
	  for(i=0;i<FrmGetNumberOfObjects(FrmGetActiveForm());++i)
      {
        if (FrmGetObjectPtr(FrmGetActiveForm(),i)==scroll)
        {
            FrmShowObject(FrmGetActiveForm(),i);
            break;
        }
      }

      SclDrawScrollBar(scroll);
  }
  if( textHeight>fieldHeight)
    maxValue = textHeight - fieldHeight;
  else if (currentPosition)
    maxValue = currentPosition;
  else
  
    maxValue=10;    //added 4.26.2000
   
   
   
//StrPrintF(debug,"cur=%d, textH=%d fldH=%d maxv=%d",currentPosition, textHeight,fieldHeight, maxValue);
//SysFatalAlert(debug);
   SclSetScrollBar(scroll, currentPosition, 0, maxValue, fieldHeight);  //added 4.26.2000
   
   
} 
/***********************************************************************
 *
 * FUNCTION:    GetObjectPtr
 *
 * DESCRIPTION: This routine returns a pointer to an object in the current
 *              form.
 *
 * PARAMETERS:  objectID (in) - The resource ID of the control to get.
 *
 * RETURNED:    Pointer to the control.
 *
 ***********************************************************************/
VoidPtr GetObjectPtr (Word objectID)
{
	FormPtr frm;
	
	frm = FrmGetActiveForm ();
	if( frm )
		return (FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, objectID)));
	else
		return NULL;
}


/***********************************************************************
 *
 * FUNCTION:    SetFieldText
 *
 * DESCRIPTION:Perform all necessary actions to set a Field control's
 *					text and redraw, if necesary.  Allocates a text handle.
 *              
 *
 * PARAMETERS: nFieldID(in) - The resource ID number of the field .
 *					pSrcText(in) - The text to copy into field.
 *					nMaxSize(in) - Max size that the field can grow.
 *					bRedraw(in)  - Should the text be redrawn now? 
 *
 * RETURNED:   None
 *
 ***********************************************************************/
void SetFieldText( UInt nFieldID, const CharPtr pSrcText, Int nMaxSize, Boolean bRedraw )
{
	struct ObjectData *objectPtr;   //added 5.31.2000
	VoidHand hFieldText;
	CharPtr  pFieldText;
	FieldPtr pField;
	Err  err;       //05282003
//StrPrintF(debug,"in setfieldtext id=%d field=%s max=%d",nFieldID,pSrcText,nMaxSize);
//SysFatalAlert(debug);

	pField = (FieldType*)GetObjectPtr( nFieldID );
	if( !pField )
		return;


		
	hFieldText = FldGetTextHandle( pField );
	if( !hFieldText )
	{
		hFieldText = MiscHandleNew( nMaxSize ,"setFld");
    }
	// If already allocated, make sure it can handle nMaxSize already.
	// If not, realloc that buffer
	else
	{
		ULong curSize = MemHandleSize( hFieldText );
		if( curSize < nMaxSize )
		{                                               //05282003
			err=MemHandleResize(hFieldText, nMaxSize ) ;//05282003
			if(err==0)  OutOfMemory(6005);      //05282003
		}                                         //05282003		
	}

	if( hFieldText )
	{
		int len = StrLen(pSrcText);
		
		pFieldText = (char*)MemHandleLock( hFieldText );
		
		if (len >= nMaxSize)
		{
			StrNCopy( pFieldText, pSrcText, nMaxSize-1);
			pFieldText[nMaxSize-1] = '\0';
		}
		else
			StrCopy( pFieldText, pSrcText );

		MiscHandleUnlock( hFieldText );

		FldSetTextHandle( pField, (Handle)hFieldText );
		FldSetTextAllocatedSize( pField, nMaxSize );
		
		FldSetMaxChars( pField, nMaxSize-1);

		FldRecalculateField( pField, true );

		if( bRedraw )
		{

			FldDrawField( pField );
	    }

	   objectPtr=locateObjectPtr(nFieldID);

       if( objectPtr->dataPtr!=0)
       {

           MiscHandleFree((VoidHand)objectPtr->dataPtr);
       }

       objectPtr->dataPtr=(VoidPtr)(MiscPtrRecoverHandle(pSrcText));  //note:field uses handle, not pointer
       
	}
}

 
//*****************************************************************************************

//use our own record of the last display values for some form objects to set the display values
/* made section comments 09242003
void useOurOwnDisplayMemory(FormPtr frmPtr)
{
   int oldNoItems;
   
   struct ViewData * viewPtr;
   VoidPtr objectsPtr;
   struct ObjectData * thisObjPtr;
   int j;
   int i;
   int cnt;
   VoidPtr p;   //added 5.31.2000
   int mmm;
   
//dbug("in useOwnMemory");
   
   
   for(i=0;i<view_cnt;++i)
   {
     viewPtr=(struct ViewData*)((char*)viewElemsPtr+i*sizeof(viewExample));  //point to this view's data 
     if(viewPtr->cnt==0) goto noObjs;
     if(viewPtr->id==FrmGetFormId(frmPtr))
     {
       
           FrmDrawForm(frmPtr);  //temp
       
       objectsPtr=viewPtr->objectsPtr;

       for(j=0;j<viewPtr->cnt;++j)
       {
          thisObjPtr=(struct ObjectData*)((char*)objectsPtr+j*sizeof(objectExample));  //point to this object's data
          if(thisObjPtr->dataPtr!=0)
          {
              switch(thisObjPtr->type)
              {
                  
                  case frmFieldObj:
                     if(resetToDesignState==true) break;
                     p=MiscHandleLock((VoidHand)(thisObjPtr->dataPtr));                                           //added 5.31.2000
//StrPrintF(debug,"setting fields fld=%s length=%d",p,StrLen(p));
//SysFatalAlert(debug);
                    SetFieldText(thisObjPtr->id,p,FldGetMaxChars((FieldPtr)(FrmGetObjectPtr(frmPtr,j)))+1,false);//added 5.31.2000
                    MiscPtrUnlock(p);                                                                            //added 5.31.2000  
                    break;
	              case frmControlObj:
                    if(resetToDesignState==true) break;
//StrPrintF(debug,"setting controls=%s",(char*)(thisObjPtr->dataPtr));
//SysFatalAlert(debug);
	                
	                CtlSetLabel((ControlType*)(FrmGetObjectPtr(frmPtr,j)),(char*)(thisObjPtr->dataPtr));
	                break;

	              case frmListObj:
                    if(resetToDesignState==true) break;
	                cnt=MemPtrSize(thisObjPtr->dataPtr)/4;
//StrPrintF(debug,"setting lists  #choices=%d",cnt);
//SysFatalAlert(debug);
//for(mmm=0;mmm<cnt;++mmm)a

//{
//StrPrintF(debug,"setting new list elem to*%s* len=%d",*((char**)((char*)(thisObjPtr->dataPtr)+4*mmm)),StrLen(*((char**)((char*)(thisObjPtr->dataPtr)+4*mmm))));  
//SysFatalAlert(debug);
//}
	                
	                  LstSetListChoices( (ListPtr)(FrmGetObjectPtr(frmPtr,j)),(char**)(thisObjPtr->dataPtr), cnt);
	                
	                break;
	              
	              
	              case frmLabelObj:
	                if(resetToDesignState==true) break;
	                FrmCopyLabel(frmPtr,thisObjPtr->id,(char*)(thisObjPtr->dataPtr));
                    break;
	              case frmLineObj:
	              case frmRectangleObj:
	              case frmBitmapObj:
	              case frmTableObj:
	              case frmTitleObj:
	              case frmPopupObj:
	              case frmGraffitiStateObj:
	              case frmGadgetObj:
	              case frmScrollBarObj:
	                      break;
                  default:
                          break;     
              }
          }
dontDoIt:
         ;
       }
       
       goto endTheLoop;  //only process the one matching form
     }
noObjs:
     ;
   }
endTheLoop:
     ;
//dbug("leaving useOwnMemory");   
    
       
}

---09242003   */


//*****************************************************************************************
void executeFormRedraw()
{
      Word res;
      FormPtr frmPtr;
      Word idx;
      RectangleType r; 
      RectangleType theBounds;
      struct ObjectData * objPtr;
      VoidHand h;
     
//dbug("os6 in form redraw");  
      frmPtr= FrmGetActiveForm();
      frmNo=FrmGetActiveFormID();   //04022002
//StrPrintF(debug,"os6 form#=%d",frmNo);
//dbug(debug);      
      //10272002FrmEraseForm(frmPtr);         //10202002  to stop old popups fromstaying on screen
      FrmDrawForm(frmPtr);                    //10272002
 
       
//StrPrintF(debug,"at form redraw frmptr=%lu form=%d",(Long)frmPtr,FrmGetFormId(frmPtr));
//SysFatalAlert(debug);   
      //10092002 useOurOwnDisplayMemory(frmPtr);  //put in the last known display values/lists etc

//StrPrintF(debug,"exec beforedraw=%d",frmNo-1);
//SysFatalAlert(debug);
      
      typeOfScript='b';
      misc_codeexec(frmNo-1,4l);       //exec form startup code if there
//dbug("os6 back from before draw");
      frmPtr= FrmGetActiveForm();
      frmNo=FrmGetActiveFormID();   //04022002
       //see if any attached scrollbars that need initialized
       for(idx=0; idx<FrmGetNumberOfObjects(frmPtr);++idx)
       {
           if(FrmGetObjectType(frmPtr, idx)==frmFieldObj)
           {
              if (idx != (FrmGetNumberOfObjects(frmPtr)-1))
              {
                  if(FrmGetObjectType(frmPtr, idx+1)==frmScrollBarObj)
                  {
                     //have an attached scrollbar
                     UpdateScrollbar((FieldPtr) FrmGetObjectPtr(frmPtr, idx),(ScrollBarPtr)FrmGetObjectPtr(frmPtr, idx+1));
                  }
              }
            }   
       }
//dbug("drawing form");
       
           FrmDrawForm(frmPtr);
       //added 09242003
       for(idx=0; idx<FrmGetNumberOfObjects(frmPtr);++idx)
       {
              if(FrmGetObjectType(frmPtr, idx)==frmTableObj)
              {
//SysFatalAlert("drawing table");
                  objPtr=locateObjectPtr( FrmGetObjectId (frmPtr,idx));
//StrPrintF(debug,"redrawing grid id=%d   id2=%d", FrmGetObjectId (frmPtr,idx),objPtr->id);
//SysFatalAlert(debug);
                  redrawGrid(objPtr);  //02062004 modified
              }
       }       
              
      //end of additions 09242003     
       
//StrPrintF(debug,"exec after draw=%d",frmNo);
//SysFatalAlert(debug);
       typeOfScript='a';
       misc_codeexec(frmNo,4l);      //code after form drawn 

}
//--------------------------------------------------------------------------------------------------------------------------
//*****************************************************************************************
void ExitHousekeeping()
{
       VoidPtr tempWinPtr;
       VoidPtr temp;
       VoidHand tempH;
       //struct data_elem* de; 
       int i,j;                         
       int col;                       //added 09242003
       Word   rscId;                  //09242003
       struct dbType* db ;            //added 09242003
	   struct dbField* dbFld;         //added 09242003   	         
       struct gridColInfo* cptr;      //added 09242003
       struct ViewData * viewPtr;       
       VoidPtr objectsPtr;              
       struct ObjectData * thisObjPtr;  
       FormPtr frm;                     
       UInt useCnt;
       //
       VoidHand newHand;
	   VoidHand oldHand;
	   VoidPtr oldPtr;
	   VoidPtr newPtr;
	   Int ix;
	   int id;
	   //Int i;
	   //int kk;
	   int res;
	   //UInt finalInstrCtr=instr_ctr;
	   //struct data_elem* de;
	   
	   VoidPtr p3;
	   VoidHand h3;
	   struct codeBlock* tempPtr;
	   int schCnt=0;
	   
	   void * textP;     //inserted 02062004
	   int    row;       //inserted 02062004 
	   
	   //
//SysFatalAlert("in exit housekeeping");

#ifdef ORG_NSB    // 12/08/2000 MMD
   NSBTerm();     // 12/08/2000 MMD
#endif            // 12/08/2000 MMD
    
   
     
    //free up view and view objects memory
     
    for(i=0;i<view_cnt;++i)
    {

       if(viewElemsPtr==0) goto noViewsYet;    //01272003
       viewPtr=(struct ViewData*)((char*)viewElemsPtr+i*sizeof(viewExample));  //point to this view's data 
       frm=FrmGetFormPtr(viewPtr->id);
       FrmEraseForm(frm);     //08032002
       FrmDeleteForm(frm);    //08032002 to clear memory for form  
       
//SysFatalAlert("past the form delete");       
       if(viewPtr->cnt==0) goto skipView;    //added 5.24.2000
       
       objectsPtr=viewPtr->objectsPtr;
       
       
//StrPrintF(debug,"housekeeping form=%lu",(long unsigned)frm);
//SysFatalAlert(debug);
       for(j=0;j<viewPtr->cnt;++j)
       {

          thisObjPtr=(struct ObjectData*)((char*)objectsPtr+j*sizeof(objectExample));  //point to this object's data
//StrPrintF(debug,"freeing object type=%d",thisObjPtr->type);
//SysFatalAlert(debug);
          if(thisObjPtr->dataPtr!=0)
          {
              if(thisObjPtr->type == frmFieldObj)
              {
//dbug("freeing field");
                
                  MemHandleFree((VoidHand)(thisObjPtr->dataPtr));
                  
              }
              else
              {
                 if(thisObjPtr->type == frmListObj)
                 {
                    
//dbug("freeing list");
	                FreeListValues(thisObjPtr->dataPtr,thisObjPtr->numItems);
                 }
                 else
                 {
//dbug("freeing other obj");
                     MemPtrFree( thisObjPtr->dataPtr);
                 }
              }   
          }
          //start additions 09242003
          if(thisObjPtr->type == frmTableObj)
          {
//SysFatalAlert("freeing grid");
             //start additions 02062004
             //free cell text ptrs
			   if(thisObjPtr->gridContent)  
			   {
			        for(row=0;row<thisObjPtr->numItems;++row)
			        {
			           for(col=thisObjPtr->howManyColsInBind;col<thisObjPtr->cols;++col)
			           {
			             
			             textP=*( gridCellPtr(thisObjPtr,row,col));
			             if(textP)      MemPtrFree( textP);
			           }
			        }
			      
			        
			              
			   }
             //end additions 02062004
             
             for(col=0;col<thisObjPtr->cols;++col)
             {
//SysFatalAlert("freeing grid col");
                 cptr=locateGridColInfo(thisObjPtr, col);
                 if(cptr!=0)
                 {
                     if(cptr->format!=0) MemPtrFree(cptr->format);
                 }    
             }
             if(thisObjPtr->colInfo!=0)     MemPtrFree(thisObjPtr->colInfo);
             if(thisObjPtr->gridContent!=0) MemPtrFree(thisObjPtr->gridContent);
             if(thisObjPtr->itemData!=0)    MemPtrFree(thisObjPtr->itemData);
             
          }
          //end of additions 09242003 
       }
       MemHandleUnlock(viewPtr->objectsHand);
       MemHandleFree(viewPtr->objectsHand);       
skipView:                                    //added 5.24.2000
        ;                                    //added 5.24.2000 
    }
    if(viewElems) MemHandleUnlock(viewElems);  //mod 05282003
    if(viewElems) MemHandleFree(viewElems);    //mod 05282003
//-----------------------------------------------------------------------------------------------------------------------
noViewsYet:              //01272003       


    while(1)
    {  
//SysFatalAlert("in free vars loop again");
       //free vars allocated from heap //08032002
//StrPrintF(debug,"2a   codeBlkPtr=%lu id=%d deListPtr=%lu",codeBlockPtr,codeBlockPtr->id,deListPtr);
//SysFatalAlert(debug);
       if(codeBlockPtr==0) break;                       //sequence 05282003
       //commented out 01122004 if(codeBlockPtr->id==50000) goto elimCodeBlock;  //sequence 05282003
       //its the first  one
//SysFatalAlert("releasing vars");       
       for(i=1;i<(de_cnt+string_work_de_max+arith_work_de_max+2);++i)
       {
          de=*((VoidPtr*)((char*)deListPtr+i*4));
          if(de)
          {
//char buffer[100];
 //if((i>50)&&(i<60))
 //{
 //StrPrintF(buffer,"wkvar=%d flags=%2.2x",de->flags3);
 //SysFatalAlert(buffer);
 //}
              	if((de->flags3 & 0x02)==0x02)   						
				{
//StrPrintF(debug,"freeing delistptr for var=%d",i);
//SysFatalAlert(debug);
				    MemPtrFree(de);
				}
          }
       }
       if(codeBlockPtr->id==50000) goto elimCodeBlock;  //added 01122004
       //now--move back thru nested sub/func calls and do the same again
       //05282003 if(codeBlockPtr==0) break;                       
       //05282003 if(codeBlockPtr->id==50000) goto elimCodeBlock;  
       id=codeBlockPtr->functionLinkNo;
       while(1)
       {
//StrPrintF(debug,"searching for rec=%d offset=&%d",codeBlockPtr->functionLinkNo, codeBlockPtr->offset);
//SysFatalAlert(debug);
          schCnt+=1;
          if(schCnt==10) goto noMore;
          ix=DmFindResource(VbRscDB,(ULong)'fwrk',id,NIL); 
          if(ix!= -1)
          {
//SysFatalAlert("got old func rec");
             oldHand=MiscDmGetResourceIndex(VbRscDB,ix,"rescwk"); 
             if(oldHand)  //inserted 05282003
             {
		             oldPtr=MiscHandleLock(oldHand);
		//StrPrintF(debug,"got id=%d offset=%d",*((int*)oldPtr),*((unsigned int*)((char*)oldPtr+2)));
		//SysFatalAlert(debug);

		             if( (*((int*)oldPtr) != codeBlockPtr->functionLinkNo) || (*((unsigned int*)((char*)oldPtr+2)) != codeBlockPtr->offset))
		             {
		                MiscHandleUnlock(oldHand);
		                MiscDmReleaseResource(oldHand);
		                id+=50;                        //increm recno by 50 to try again
		             }
		             else
		             {
		               break;               //found the record we want
		             }
		     }   //inserted 05282003         
          }
          else
          {
//SysFatalAlert("no wkvars rec found");
              goto noMore;
              //MiscFatalAlert("internal error exiting a function.");
          }
       }   
//StrPrintF(debug,"find linkNo=%d rsc=%d ",id,ix);
//SysFatalAlert(debug);

//SysFatalAlert("ZZ");
       if(deListPtr) MemMove((char*)deListPtr+4,(char*)oldPtr+4,work_de_max*4);  //05282003      //and copy the handles
       if(oldHand)  MemHandleUnlock(oldHand);   //05282003
       MiscDmReleaseResource(oldHand);
   
       userFunctionSequentialNo-=1;  //reduce count of nested user function calls
       //move back to prev code block ptr
//SysFatalAlert("deleting codeBlockPtr");
      //h3=codeBlockPtr->newh3;
      //p3=(char*)codeBlockPtr->newp3;
      //MemPtrUnlock(p3);
      //DmReleaseResource(h3);
elimCodeBlock:
      tempPtr=codeBlockPtr;
      //p3=(char*)codeBlockPtr->p3;
      //h3=codeBlockPtr->h3;

      //numberParas=codeBlockPtr->numberParas;
      //paragTbl=(char*)codeBlockPtr->paragTbl;
      //theCodeOffset=codeBlockPtr->oldOffset;
      //codePtr=(char*)codeBlockPtr->codePtr;
      //instr_ctr=codeBlockPtr->instr_ctr;

      codeBlockPtr=(struct codeBlock*)codeBlockPtr->prevCodePtr;
      MemPtrUnlock(tempPtr);
      MemPtrFree(tempPtr);
//StrPrintF(debug,"done deleting codeBlockPtr  new=%lu",(unsigned long)codeBlockPtr);
//SysFatalAlert(debug);  
      if(codeBlockPtr==0)
      {
//SysFatalAlert("found 0 codeBlockPtr");
         break;          
      }     
     }  
noMore:      

//SysFatalAlert("past codeBlock deletes totally");
       //free for loop strucs that may have been left open
       while(1)
       {
          if( forptr==0) break;
          temp=forptr->prevForPtr;
          MemPtrFree(forptr);
          forptr=temp;
       }
       //free drawing windows
       while(1)
       {
          if( winRoot==0) break;
          tempWinPtr=(struct DrawWinStruct*)(MemHandleLock(winRoot));
          tempH=(VoidHand)(((struct DrawWinStruct*)(tempWinPtr))->next);
//SysFatalAlert("destroying window name");
          MemHandleFree((VoidHand)(((struct DrawWinStruct*)(tempWinPtr))->nameHandle));
//SysFatalAlert("destroying os window");
          //MemHandleFree((VoidHand)(((struct DrawWinStruct*)(tempWinPtr))->win));
          WinDeleteWindow((WinHandle)(((struct DrawWinStruct*)(tempWinPtr))->win),false);
//SysFatalAlert("destroying swb window");
          MemHandleUnlock(winRoot);
          MemHandleFree(winRoot);
          winRoot=tempH;
       }
       

       
       /*commented out 09242003
       if(oldName[0]!=0x00)
       {
         DmResetRecordStates( ApplPrcDB );
         DmCloseDatabase( ApplPrcDB );
       }
       end commented out 09242003 */
       
       
//SysFatalAlert("before dbElems");
       if(dbElemsPtr!=0)
       {
          //start insertions 09242003
           for (i=1;i<=dbCnt;++i)
	       {
//StrPrintF(debug,"looking for db#=%d",i );
//SysFatalAlert(debug);
	         db=(struct dbType*)((char*)dbElemsPtr + (i-1)*sizeof(db_example));    //modified 06142005
	          
	         db->openSw='n';
	         
	         //look for field def record
	         rscId=i;
	         h2=MiscDmGetResource((long)'Fldd',rscId,"fldd");  //read the field definition record
		     if(h2!=0)
		     {
//StrPrintF(debug,"freeing db=%d",i);
//SysFatalAlert(debug);
		         db=(struct dbType*)((char*)dbElemsPtr+(i-1)*sizeof(db_example));   //modified 06142005
     	         dbFld=(struct dbField*)db->ptrFields;  //pt to 1st field
 
		         for(j=0;j<db->noFields;++j)
		         {
//SysFatalAlert("freeing dbFld");
		             MemHandleFree(dbFld->hName);
		             dbFld = (struct dbField*)((char*)dbFld + sizeof(dbFldExample)); 
		         }
//SysFatalAlert("past dbFields");
		         MemHandleFree(db->hCreator);
		         MemHandleFree(db->hName);
		         MemPtrFree(db->ptrFields);
		     
		         DmReleaseResource(  h2);
		     }
           }
           
          //end of additions 09242003
          MemPtrFree(dbElemsPtr);
          dbElems=NULL;
       }    
       //start additions 09242003
        if(oldName[0]!=0x00)
       {
         DmResetRecordStates( ApplPrcDB );
         DmCloseDatabase( ApplPrcDB );
       }
       //end additions 09242003

       if(deListHand!=NULL)
       {
          MemHandleFree(deListHand);
          deListHand=NULL;
       }    
       if(VbRscLocalID != 0)
       {
          DmResetRecordStates( VbRscDB );
          DmCloseDatabase(VbRscDB);
          DmDeleteDatabase(bp.cardNo,VbRscLocalID);
          VbRscLocalID=0;
       }
       //while(1)
       //{
       //    if (codeBlockPtr==0) break;
       //    if(h3!=0)  MemHandleUnlock(h3);
       //    temp=codeBlockPtr->prevCodePtr;
       //    h3=codeBlockPtr->h3;
       //    MemPtrFree(codeBlockPtr);
       //    codeBlockPtr=(struct codeBlock*)temp;
       //}
//SysFatalAlert("closing forms");
       FrmCloseAllForms() ;
       //added 11/17/2001  remove Mathlib ref library
       if(mathLibOpenSw==true)
       {
           MathLibClose(MathLibRef,&useCnt);
           SysLibRemove(MathLibRef);
       }      
//SysFatalAlert("Leaving housekeeping");
}

//*****************************************************************************************
int StartApplication()
{
  char*p2;   //06102002
  unsigned char  b;
  unsigned char  a;
  unsigned long  dl;
  unsigned long  cl;
  VoidPtr        ptr; 
  unsigned char *uptr;
  char *         to;
  double         wkdbl;
  int            i,j,k,l;
  Word           prefSize;
  
  char           debug2[20];
  VoidPtr        para;
  int            err;
  Int            idx;
 
  UInt           index;
  int            wkid;
  int            modId;
  double         wkd;
  int            wki;
  Long           wkl;
  UInt           attrib;
  ULong          theType;
  
  ULong          theCreator;
  UInt           offset;
  struct dbType  db_example;
  struct dbType* db ;            //added 09242003
  VoidPtr        p;              //added 09242003
  int            col;            //added 09242003
  struct dbField* dbFld;         //added 09242003
  ULong          rscId;          //added 09242003 
  char *         subptr;
  
  int            jjj;
 Long            diff;
//
  LocalID        appl;
  UInt           attr;
  UInt           version;
  ULong          crDate;
  ULong          modDate;
  ULong          bkupDate;
  ULong          modNum;
  LocalID        appInfoID;
  LocalID        sortInfo;
  ULong          type;
  ULong          creator; 
  DateTimeType   timptr1;
  DateTimeType   timptr2;
  char           asterisk='c';
  char           percent='%';
  unsigned char  secByte[8];
  
union  {
  FlpDouble q;
  double b;
  int e;
 } x;

double      d;
char        work[100];
char        hex80;
char        hex40;
char        hex00;

ULong       tim1;
ULong       tim2;
ULong       tim3;
ULong       tim4;
ULong       tim5;
ULong       tim6;
ULong       tim7;
Long        longcon0=0l;
ULong       triple;
FormPtr     frmPtr ;
  
  struct ViewData * viewPtr;       //added 5.21.2000
  VoidPtr objectsPtr;              //added 5.21.2000
  struct ObjectData * thisObjPtr;  //added 5.21.2000
  VoidPtr   bldPtr;                //added 5.21.2000
  VoidHand  bldHand;               //added 5.21.2000
  VoidPtr   oldPtr;                //added 5.21.2000
  VoidPtr   newListPtr;            //added 5.21.2000
  int cnt;                         //added 5.21.2000
UInt32		value;				   //02.27.02 GH
  
#ifdef ORG_SWB
   rscTag = 'Swba';    //resource identifier used for code and other resource records
#endif
#ifdef ORG_NSB
   rscTag = 'nsba';    //resource identifier used for code and other resource records
#endif 

//SysFatalAlert("start appl");
  
  
  aNullStringConstant=0x00;
  setupFuncTable();
  FtrGet(sysFtrCreator,sysFtrNumROMVersion,&romVersion);   //get the dvc per system ROM version
  gOldSerialManager=!(FtrGet(sysFileCSerialMgr, sysFtrNewSerialPresent, &value)==0 && value !=0);	//02.27.02 GH
  userFunctionSequentialNo=0;
  msgboxOrAlertUp=false;
  ignoreNextPenup=false;
  
  firstPenDown=1;  //12172001
 
  timerExpirationTime = 0;   //08022005
  winRoot=0;
  tim1=TimGetSeconds();
  hex80=0x80;
  hex40=0x40;
  hex00=0x00;
  serRef=0;
  con0=0;
  con1=1;
  viewElemsPtr=0;         //01272003
  stop_op_code=0x00;
 
  null_ptr=(void*)(0l);
  init_first_time=0;
  codeBlockPtr=0;
  forHandle=0;
  perfHand=0;
  moduleCntr=0;
  pendownCount=0;
  menuPtr=(MenuBarType*)null_ptr;

  EvtFlushPenQueue();     //12172001

     prefSize=36;
#ifdef ORG_SWB
      PrefGetAppPreferences((DWord)'basc',0,&bp,&prefSize,0);
#endif
#ifdef ORG_NSB
     PrefGetAppPreferences((DWord)'NSBp',0,&bp,&prefSize,0);
#endif
  StrCopy(oldName,bp.name);   
  //MMD DEBUG
  StrCopy(bp.name,"");        //10.10.2000 remove comment if live
#ifdef ORG_SWB
     PrefSetAppPreferences( (DWord)'basc',0,0,&bp,36,0);
#endif
#ifdef ORG_NSB
     PrefSetAppPreferences( (DWord)'NSBp',0,0,&bp,36,0);
#endif
  
if(oldName[0]!=0x00)
{
  //ApplPrcLocalID=DmFindDatabase(bp.cardNo,bp.name);
  ApplPrcLocalID=DmFindDatabase(bp.cardNo,oldName);     //10.10.2000
  if (ApplPrcLocalID==0)
  {
     SysFatalAlert("Can't find program's PRC file.");
     ErrThrow(-1);                 //inserted 05212003
     return(1);
  }
 
  ApplPrcDB=0;
  ApplPrcDB=DmOpenDatabase(bp.cardNo, ApplPrcLocalID, dmModeReadOnly);
  if (ApplPrcDB==0)
  {
     SysFatalAlert("Can't open PRC file!");
     ErrThrow(-1);                 //inserted 05212003
     return(1);
  } 
  StrCat(oldName,"-bsc");
}
else
{
//SysFatalAlert("aborting");
    //determine if a fat app or a restart of thin app    //03172003
    if(DmGet1Resource((unsigned long)'Talt',1001)==0)    //03172003
    {                                                    //03172003
      //it's a thin app , so terminate                   //03172003
      terminationAction='l';                             //03172003  //want Palm launcher  
      return(999);                                       //03172003
    }                                                    //03172003
} 
  tim2=TimGetSeconds();

  //find basic code db for this app



#ifdef ORG_SWB
    //delete work db if there                                          //05282003
    VbRscLocalID=DmFindDatabase(bp.cardNo,"StylusBasic..pdb");         //05282003
    //VbRscDB=DmOpenDatabase(bp.cardNo, VbRscLocalID, dmModeReadWrite);  //05282003
    if (VbRscLocalID!=0)                                                    //05282003
    {                                                                  //05282003
        //DmCloseDatabase(VbRscDB);                                              //05282003
         //get rid of old work database                                //05282003
        err=DmDeleteDatabase (bp.cardNo, VbRscLocalID);         //05282003
    }                                                                  //05282003
                                                                    
    //create work database                                             //05282003
    err=DmCreateDatabase(bp.cardNo,"StylusBasic..pdb",(ULong)'SWBa',(ULong)'Rsrc',true);
#endif
#ifdef ORG_NSB
    //delete work db if there                                          //05282003
    VbRscLocalID=DmFindDatabase(bp.cardNo,"NSBasic..pdb");             //05282003
    //VbRscDB=DmOpenDatabase(bp.cardNo, VbRscLocalID, dmModeReadWrite);  //05282003
    if (VbRscLocalID!=0)                                                    //05282003
    {                                                                  //05282003
        //DmCloseDatabase(VbRscDB);                                      //05282003
         //get rid of old work database                                //05282003
        err=DmDeleteDatabase (bp.cardNo, VbRscLocalID);                //05282003
    }                                                                  //05282003
                                                                    
    //create work database                                             //05282003
    err=DmCreateDatabase(bp.cardNo,"NSBasic..pdb",(ULong)'nsba',(ULong)'Rsrc',true);
#endif
  
#ifdef ORG_SWB
  VbRscLocalID=DmFindDatabase(bp.cardNo,"StylusBasic..pdb");
#endif
#ifdef ORG_NSB
  VbRscLocalID=DmFindDatabase(bp.cardNo,"NSBasic..pdb");
#endif  
  
  if(VbRscLocalID==0) goto noRscDb;   //05282003  may be because no memory to create
  VbRscDB=DmOpenDatabase(bp.cardNo, VbRscLocalID, dmModeReadWrite);
  if (VbRscDB==0)
  {
noRscDb:                                     //05282003
     SysFatalAlert("Can't open rsc file.");
     ErrThrow(-1);                 //inserted 05212003
     return(1);
  } 

//StrPrintF(debug," #recs=%d",DmNumRecords(VbRscDB));
//SysFatalAlert(debug);
  
  
  while(DmNumRecords(VbRscDB)>0)
  {
      index=0;
      DmRemoveResource(VbRscDB,index);
  }

  tim3=TimGetSeconds(); 
 
   h2=DmGetResource(rscTag,(ULong)9905);
   if(h2==0)
   {
       SysFatalAlert("Can't find program's base record");
       ErrThrow(-1);                 //inserted 05212003
       return(1);
   }
   p2=(char *)MemHandleLock(h2);
   frmNo=((struct base_rec*)p2)->def_screen_no;

   first_string_work =((struct base_rec*)p2)->first_string_work;
   first_arith_work  =((struct base_rec*)p2)->first_arith_work;
   de_cnt            =((struct base_rec*)p2)->de_cnt;
   parag_cnt         =((struct base_rec*)p2)->parag_cnt;
   codemod_cnt       =((struct base_rec*)p2)->codemod_cnt;
   view_cnt          =((struct base_rec*)p2)->view_cnt;
   string_work_de_max=((struct base_rec*)p2)->string_wk_max;
   arith_work_de_max =((struct base_rec*)p2)->arith_wk_max;
   work_de_max=string_work_de_max+arith_work_de_max;
   
   dbCnt             =((struct base_rec*)p2)->dbCnt;
   applVersionInteg  =((struct base_rec*)p2)->versionInteg;

   applVersionFrac   =((struct base_rec*)p2)->versionFrac;
   LitePro           =((struct base_rec*)p2)->LitePro;
   secByte[0]        =((struct base_rec*)p2)->secByte1;
   secByte[1]        =((struct base_rec*)p2)->secByte2;
   secByte[2]        =((struct base_rec*)p2)->secByte3;
   secByte[3]        =((struct base_rec*)p2)->secByte4;
   secByte[4]        =((struct base_rec*)p2)->secByte5;
   secByte[5]        =((struct base_rec*)p2)->secByte6;
   secByte[6]        =((struct base_rec*)p2)->secByte7;
   secByte[7]        =((struct base_rec*)p2)->secByte8;
   myCreationDate    =((struct base_rec*)p2)->createDate;
   dbElems =MemHandleNew((dbCnt+1)*sizeof(db_example));
   if(dbElems==0) OutOfMemory(6041);    //05282003
   ptr     =MemHandleLock(dbElems);
   dbElemsPtr=ptr;
   
   MemHandleUnlock(h2);
   DmReleaseResource(h2);
//StrPrintF(debug,"got base rec--form=%d decnt=%d",frmNo,de_cnt);
//SysFatalAlert(debug);

   
   //in version 3 , added way to alloc vars from heap //06102002
  if(applVersionInteg>=300)  maxVarsInDynamHeap=200;    //06012002
  else 					     maxVarsInDynamHeap=0;      //06012002	 
   
   //alloc list of de handles
   wkl=(de_cnt+
                string_work_de_max+
                arith_work_de_max
                +2)
                *4;
   deListHand=MemHandleNew( wkl);
   if(deListHand==0)
   {
        StrPrintF(debug,"Not enough memory to allocate program variables(%d)",de_cnt+string_work_de_max+arith_work_de_max +2);
        SysFatalAlert(debug);
        ErrThrow(-1);                 //inserted 05212003
   }             
   deListPtr=(char*)MemHandleLock(deListHand);
//StrPrintF(debug,"delist setup cnt=%d  mem=%lu  ",de_cnt,wkl);
//SysFatalAlert(debug);		              
   DmSet(deListPtr,0,wkl,0x00);  //03242002
//SysFatalAlert("past delist setup");
   //03242002for (i=0;i<(de_cnt+string_work_de_max+arith_work_de_max+2);++i) 
   //03242002{
   //03242002    DmWrite(deListPtr,i*4,&longcon0,4);  //indicate all are missing(not yet init'd)
   //03242002}
//StrPrintF(debug,"beyondbase");
//SysFatalAlert(debug);


//check if demo pgm exceeded 5 day-limit
//BUG!! Fat apps do not get checked!
//DmDatabaseInfo does not get anything for fat apps.
  tim6=TimGetSeconds();
if(oldName[0]!=0x00)
{
  DmDatabaseInfo(bp.cardNo,ApplPrcLocalID,oldName,
                &attr,&version,&crDate,&modDate,
                &bkupDate,&modNum,&appInfoID,&sortInfo,&type,&creator);
                
  if (secByte[6] !=(255-secByte[7])) goto IllegalSecBytes;
  
  b=*((unsigned char*)(&myCreationDate)+3);
  a=b;
  while(a>5) a=a-6;
  cl=0;
  
  cl=secByte[a];
  dl=((b+913)*11314l);
  if((dl % 255)==cl) goto maybeDemo;
  dl=((b+123)*98765l);
  if((dl%255)==cl) goto maybeLicensed;
IllegalSecBytes:
//StrPrintF(debug,"c=%lu,d=%lu b=%u a=%u sec4=%u",cl,dl,b,a,secByte[4]);
//SysFatalAlert(debug);
  SysFatalAlert("Sorry--the program file being executed is corrupted. Please replace.");
  ErrThrow(-1);                 //inserted 05212003
maybeLicensed:
  if (secByte[6]==(dl % 128)) goto beyondChecks;
  else goto maybeDemo;
    
maybeDemo:
  if( ( secByte[6] - 128 ) != ( dl % 128 ) )
   goto maybeLicensed;
ItsADemo:
  if(secByte[6]<128) goto IllegalSecBytes;        

      wkl=5l*24l*60l*60l;
      diff=(TimGetSeconds()-myCreationDate);
//StrPrintF(debug,"at diff=2   crdate=%lu  today=%lu diff=%ld 5days=%ld",myCreationDate,TimGetSeconds(),diff,wkl);
//SysFatalAlert(debug);

      if( (diff-wkl)>0l)   
      {
         TimSecondsToDateTime(TimGetSeconds(),&timptr2);
         TimSecondsToDateTime(myCreationDate,&timptr1);
         StrPrintF(debug,
           "Sorry--programs generated with the free demo version expire after 5 days and must be recompiled.(created %d/%d/%d)",
           timptr1.month,timptr1.day,timptr1.year );
         SysFatalAlert(debug);
         ErrThrow(-1);                 //inserted 05212003
         return(1);
         
      }
}
  
beyondChecks:

//StrPrintF(debug,"beyondchecks");
//SysFatalAlert(debug);
     tim7=TimGetSeconds();
//additions start here 09242003
//****************************************************************
//    look for any 'Fldd' resources that define database record 
//  layouts needed for bind operations
//
//****************************************************************
//StrPrintF(debug,"# dbs=%d",dbCnt);
//SysFatalAlert(debug);
    for (i=1;i<=dbCnt;++i)
    {

         db=(struct dbType*)((char*)dbElemsPtr + (i-1)*sizeof(db_example));   //modified 06112005
//StrPrintF(debug,"looking for db#=%d dbMemPtr=%lu",i,db );
//SysFatalAlert(debug);

         db->noFields=0;
         db->ptrFields=0;
         db->openSw='n';
         
         //look for field def record
         rscId=i;
         h2=MiscDmGetResource((long)'Fldd',rscId,"fldd");  //read the field definition record
	     if(h2!=0)
	     {
 //StrPrintF(debug,"found fldd for dbNo=%d",rscId);
 //SysFatalAlert(debug);
            p2=MemHandleLock(h2);
            
            db->hName=MemHandleNew(StrLen(p2)+1);   //phys name of database on device
            p=MemHandleLock(db->hName);
 
            MemMove(p,p2,StrLen(p2)+1);
//StrPrintF(debug,"found dbname=%s",p2);
//SysFatalAlert(debug);
            MemHandleUnlock(db->hName);
            p2+=StrLen(p2)+1;
            
            db->hCreator=MemHandleNew(StrLen(p2)+1);   //creator name
            p=MemHandleLock(db->hCreator);
            MemMove(p,p2,StrLen(p2)+1);
            MemHandleUnlock(db->hCreator);
            p2+=StrLen(p2)+1;
            //get keyFieldNo (may be zero)           //added 02042004
            db->keyFldNo=basc_dbGetInt( (char*)p2);  //added 02042004
            p2+=2;                                  //added 02042004 
            //get no fields
            db->noFields=basc_dbGetInt( (char*)p2);
            p2+=2; 
            
            
            db->ptrFields=MemPtrNew(db->noFields * sizeof(dbFldExample));
            dbFld=(struct dbField*)db->ptrFields;  //pt to 1st field
            for( col=0;col<db->noFields;++col)
            {
               
//SysFatalAlert("blding column");                
                dbFld->tblNo=basc_dbGetInt( (char*)p2);  //data element #
                p2+=2;
                dbFld->colNo=basc_dbGetInt( (char*)p2);
                p2+=2;
                dbFld->colNo=col;  //override
                
                dbFld->type=basc_dbGetInt( (char*)p2);
                p2+=2;;
//StrPrintF(debug,"Assigning col type =%d to col#=%d",  dbFld->type,dbFld->colNo);
//SysFatalAlert(debug);              
                dbFld->hName=MemHandleNew(StrLen(p2)+1);
                p=MemHandleLock(dbFld->hName);
                MemMove(p,p2,StrLen(p2)+1);
                MemHandleUnlock(dbFld->hName);
                p2+=StrLen(p2)+1;
                
                if(*p2==0x00) dbFld->keyFlag=false;
                else          dbFld->keyFlag=true;
                ++p2;
                
                dbFld = (struct dbField*)((char*)dbFld + sizeof(dbFldExample)); 
                    
            }
            
            MemHandleUnlock(h2);
         }
    }

//end of additions 09242003

 //*****************************************************************
//  process the view table of contents record
//     build a view table and object table for each view
//****************************************************************
//SysFatalAlert("proc the vtoc");
    h2=MiscDmGetResource(rscTag,(ULong)9907,"vtoc");  //rread the view tbl-of-contents record
    if(h2==0)
    {
       SysFatalAlert("Can't find view TOC record");
       ErrThrow(-1);                 //inserted 05212003
       return(1);
    }
    p2=MiscHandleLock(h2);
//StrPrintF(debug,"-4  view_cnt=%d",view_cnt);
//SysFatalAlert(debug);
    viewElems=MiscHandleNew((view_cnt)*sizeof(viewExample),"vwElems");
    viewElemsPtr=MiscHandleLock(viewElems);

    for(i=0;i<view_cnt;++i)  //for each view
    {

      
      viewPtr=(struct ViewData*)((char*)viewElemsPtr+i*sizeof(viewExample));  //point to this view's data
      
      //06102002 wkid=basc_dbGetInt( (char*)p2+i*2);  //get id of view from the vtoc record
      wkid=basc_dbGetInt( (char*)p2);  //get id of view from the vtoc record
      viewPtr->id=wkid;                    //save id of view
//StrPrintF(debug,"got form=%d ",wkid);
//SysFatalAlert(debug);
      p2+=2;   //06102002
      if (applVersionInteg >= 300)    //06102002
      {								//06102002
			//skip over form name	//06102002
			while(1)				//06102002
   			{						//06102002
   				if((*p2)==0x00) break; //06102002
  				++p2;				//06102002
  			}						//06102002
       	    ++p2;							//06102002
      }								//06102002
      frmPtr=FrmGetFormPtr(wkid);   //pt to form
      if(frmPtr==NULL) 
      {
//dbug("loading form   in vtoc processing:");
          frmPtr=FrmInitForm(wkid);  //if not loaded, cause it to be
      }
      //temp FrmDrawForm(frmPtr); //temp
//StrPrintF(debug,"got form=%d #objs=%d ptr=%lu",wkid,  FrmGetNumberOfObjects(frmPtr),(long)frmPtr);
//SysFatalAlert(debug);
      //create a data area for all controls

      //07142005 if(FrmGetNumberOfObjects((FormType*)frmPtr)==0) MiscHandleNew(1*sizeof(objectExample),"vwObjs");
      //07142005 else viewPtr->objectsHand=MiscHandleNew((FrmGetNumberOfObjects((FormType*)frmPtr))*sizeof(objectExample),"vwObjs");
      if(FrmGetNumberOfObjects((FormType*)frmPtr)!=0)                                     //inserted 07142005
          viewPtr->objectsHand=MiscHandleNew((FrmGetNumberOfObjects((FormType*)frmPtr))*sizeof(objectExample),"vwObjs");  //inserted 07142005
      viewPtr->cnt=FrmGetNumberOfObjects((FormType*)frmPtr);
      if(viewPtr->cnt==0) goto nextView;       
      objectsPtr=MiscHandleLock(viewPtr->objectsHand);
      viewPtr->objectsPtr=objectsPtr;      

//StrPrintF(debug,"view has %d objs",viewPtr->cnt);
//SysFatalAlert(debug);
      for(j=0;j<viewPtr->cnt;++j)  //for each object in the form
      {
//StrPrintF(debug,"compare j=%d ctl=%d obj=%d",j,controlid,FrmGetObjectId(frmPtr,j)   );
//SysFatalAlert(debug);


        thisObjPtr=(struct ObjectData*)((char*)objectsPtr+j*sizeof(objectExample));  //point to this objectsbject's data


        thisObjPtr->id=FrmGetObjectId(frmPtr,j);
//StrPrintF(debug,"obj-id=%d",thisObjPtr->id);//
//SysFatalAlert(debug);
        thisObjPtr->type=FrmGetObjectType(frmPtr,j); //save object type in data area
        thisObjPtr->numItems=0;                      
        thisObjPtr->dataPtr = 0;                     //have not yet allocated area for label/list
        //start of additions 09242003
        thisObjPtr->frmId=wkid;
        thisObjPtr->index=j;
        thisObjPtr->hidden=false;
//SysFatalAlert("not hidden");
        //doesn't work --if(!FrmGlueGetObjectUsable(frmPtr,j))    //inserted 02062004
		//doesn't work --{                                        //inserted 02062004
//SysFatalAlert("obj is unusable");
		//doesn't work --	thisObjPtr->hidden=true;			 //inserted 02062004
        //doesn't work --}                                        //inserted 02062004
        //end of additions 09242003

              switch(thisObjPtr->type)
              {
                  case frmFieldObj:
//dbug("setting fields");
                    oldPtr=FldGetTextPtr( (FieldType*)FrmGetObjectPtr(frmPtr,j));  //get init text of the field
                    if(oldPtr==NULL) ;  //may not have one, so ignore
                    else
                    {
                        bldHand=MiscHandleNew(StrLen(oldPtr)+1,"FldTxt");  //has one, so alloc storage
                        bldPtr=MiscHandleLock(bldHand);
                        MemMove(bldPtr,oldPtr,StrLen(oldPtr)+1);            //and copy to our own storage
                        thisObjPtr->dataPtr=(VoidPtr)bldHand;                //and save handle for later
                    }
                    break;
	              case frmControlObj:
//StrPrintF(debug,"setting controls=%s", CtlGetLabel((ControlType*)FrmGetObjectPtr(frmPtr,j)));
//SysFatalAlert(debug);
	                oldPtr=(VoidPtr)CtlGetLabel( (ControlType*)FrmGetObjectPtr(frmPtr,j));  //get init text of the control
                    if(oldPtr==NULL) ;  //may not have one, so ignore
                    else
                    {
                        bldHand=MiscHandleNew(StrLen(oldPtr)+1,"CtlTxt");  //has one, so alloc storage
                        bldPtr=MiscHandleLock(bldHand);
                        MemMove(bldPtr,oldPtr,StrLen(oldPtr)+1);            //and copy to our own storage
                        thisObjPtr->dataPtr=bldPtr;                //and save ptr for later
                    }
                    break;
 
	              case frmListObj:

	                //pold= ( (ListPtr)controlPtr)->itemsText;
	                cnt=LstGetNumberOfItems((ListType*)FrmGetObjectPtr(frmPtr,j));
	                thisObjPtr->numItems=0;
//StrPrintF(debug,"init setting lists  #choices=%d",cnt);
//SysFatalAlert(debug);
	                newListPtr=MiscPtrNew((cnt)*4,"LstPtr");  //alloc a new list of string ptrs
	                thisObjPtr->dataPtr=newListPtr;      //and save ptr to list entries for later
	                for(k=0;k<cnt;++k)
	                {
	                    oldPtr=LstGetSelectionText( (ListType*)FrmGetObjectPtr(frmPtr,j),k);  //get init text of the item
                        if(oldPtr==NULL)  *( (char**)((char*)newListPtr+4*k)) =0;          //put ptr to string in list of ptrs 
                        else
                        {
                          thisObjPtr->numItems+=1;
                          bldHand=MiscHandleNew(StrLen(oldPtr)+1,"LstTxt");  //has one, so alloc storage
                          bldPtr=MiscHandleLock(bldHand);
                          MemMove(bldPtr,oldPtr,StrLen(oldPtr)+1);            //and copy to our own storage
//StrPrintF(debug,"setting  elem %d to *%s*  len=%d",k,bldPtr,StrLen(bldPtr));
//SysFatalAlert(debug);
                          *( (char**)((char*)newListPtr+4*k)) =bldPtr;          //put ptr to string in list of ptrs  
                        }
                    }
                    break;
	              //09242003 additions
	              case frmTableObj:
//StrPrintF(debug,"init a table id=%d",thisObjPtr->id);
//SysFatalAlert(debug);
	                   
     
	                 initializeGrid(thisObjPtr,frmPtr);
	                 break;   
	              //09242003 end of additions
	              
	              case frmLabelObj:
	              case frmLineObj:
	              case frmRectangleObj:
	              case frmBitmapObj:
	              //removed 09242003 case frmTableObj:
	              case frmTitleObj:
	              case frmPopupObj:
	              case frmGraffitiStateObj:
	              case frmGadgetObj:
	              case frmScrollBarObj:
	                      break;
                  default:
                          break;     
              }
        
outOfHere:
          ;
      }

         
nextView:
      ;
    }   
    
    MemHandleUnlock(h2);      //7/28/2001
    DmReleaseResource(h2);     //7/28/2001
#ifdef ORG_NSB    // 12/08/2000 MMD
   NSBInit();     // 12/08/2000 MMD
#endif            // 12/08/2000 MMD
    
     
//StrPrintF(debug,"%d  %lu %lu %lu %lu %lu %lu %lu",de_cnt,tim1,tim2,tim3,tim4,tim5,tim6,tim7);
//SysFatalAlert(debug);     

#ifdef  _DEBUGGER 
     	debuggerConnect();  //delays until make commun with IDE
#endif
//dbug("os6 going to startup cod");
     typeOfScript='i';
     misc_codeexec (9906  ,4l);  //if startup code--go execute
//dbug("after proj code");

justGetForm:

//StrPrintF(debug,"os6 going to initial form=%d",frmNo);
//dbug(debug);

     resetToDesignState=true; 
     FrmGotoForm(frmNo);
    
//dbug("os6 after init form");

   return(0);
}

/*
//09242003 degugging
void debugControls(char *s)
{
FormType * frm=FrmGetActiveForm();
int i,j;
StrPrintF(debug,"frm#objects=%d   at   s=%s",FrmGetNumberOfObjects (frm),s);
SysFatalAlert(debug);
for(i=0;i<FrmGetNumberOfObjects (frm);++i)
{
    StrPrintF(debug,"obj type=%d ",FrmGetObjectType(frm,i));
    SysFatalAlert(debug); 
}

}
*/
extern void spawnFormUpdateEventCode()     //09192004
{                                          //09192004 

       FormPtr frmPtr;
       int frmNo;
       
        eventType    = 8;
		frmPtr=FrmGetActiveForm();
		frmNo=FrmGetActiveFormID();    
		executeFormRedraw();          //10262004 ldm    
//SysFatalAlert("firing update");
		typeOfScript='v';
		misc_codeexec( 10800 - frmNo, 4l );  //exec form event code if there
		typeOfScript = ' ';  //10262004
		pendingFormUpdateEventSw = false;          //09192004
}                                           //09192004   
