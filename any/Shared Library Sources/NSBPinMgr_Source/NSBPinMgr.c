//
//    NSBPinMgr.c
//

#define SHLIB_FORMAT_2

#define VERSION 1.0
// MANUFACTURER GlowkaWare
// INFHELP ""
// INFHELP "NSBPinMgr Shared Library"
// INFHELP ""
// INFHELP "This library provides a wrapper for the PIN Manager (Pen Input Manager)"
// INFHELP "for manipulating the Dynamic Input/virtual graffiti Area (DIA)."
// INFHELP ""
// INFHELP "Written by Ron Glowka, June 2004"
// INFHELP ""

#include <PalmOS.h>			// Includes all Palm OS headers
#include <PenInputMgr.h>
#include <PenInputMgrCommon.h>
#include <PenInputMgrSelectorNums.h>

#define BUILDING_THIS_LIB	// Defined in this file only...
#include "NSBPinMgr.h"		// Library public definitions

#if defined(__GNUC__)
    #include "NSBPinMgrDispatch.c"
#endif

#define THIS_REQUIRES_GLOBALS
#ifdef THIS_REQUIRES_GLOBALS
    #include "NSBPinMgrGlobals.h"
#else
    #define THIS_ALLOCATE_GLOBALS
    #define THIS_FREE_GLOBALS
    #define THIS_ATTACH_GLOBALS
    #define THIS_DETACH_GLOBALS
#endif

// SECTION Version Information
// DESC The version information functions provide information about
// DESC the version of the NSBPinMgr Shared Library.


/********************************************************************
 *                     T H I S _ V e r s i o n
 ********************************************************************/
Int32 THIS_Version(UInt16 refNum, double *version) 
// DESC Returns the version number of the NSBPinMgr Shared Library.
{	
	
    *version = VERSION;
    return 0;
}

/********************************************************************
 *                 T H I S _ C o m p i l e I n f o
 ********************************************************************/
Int32 THIS_CompileInfo(UInt16 refNum, char *compileDateTime) 
// DESC Returns the date and time that the NSBPinMgr was compiled.
{	
    char dateTime[21];
	
    StrCopy(dateTime, __DATE__);
    StrCat(dateTime, " ");
    StrCat(dateTime, __TIME__);
    StrCopy(compileDateTime, dateTime);

    return 0;
}

/********************************************************************
 *                 P e n M g r A v a i l a b l e
 ********************************************************************/
Int32 PenMgrAvailable(Int32 *penMgrAvailable)
{
    Err     err = 0;
    UInt32  penMgrAPIVersion;
    if (*penMgrAvailable > 0)
        return 1;
    else
        if (*penMgrAvailable < 0) {

			err = FtrGet(pinCreator, pinFtrAPIVersion, &penMgrAPIVersion);
		    if (err == 0)
		        *penMgrAvailable = 1;
		    else
		        *penMgrAvailable = 0;
		    return *penMgrAvailable;
	    }
        else
            return 0;
}

/********************************************************************
 *                           L i b I D
 ********************************************************************/
Int32 LibID(Int32 *libCard, Int32 *libID)
{
    UInt16  i;

    if (*libCard == -1 && *libID == -1) {
	    for (i = 0; i < MemNumCards(); i++)
	        if ((*libID = (Int32) DmFindDatabase(i, THIS_LibName)) != 0) {
	            *libCard = (Int32) i;
	            return 1;
	            break;
	        }
	    return 0;
	}
	else
		return 1;
}

/********************************************************************
 *           D I A N o t i f i c a t i o n C a l l b a c k
 ********************************************************************/
Err DIANotificationCallback(SysNotifyParamType *notifyParamsP)
{
    UInt16 refNum;
    EventType resizedEvent;
    EventType sizeEvent;
    
    THIS_LibGlobalsPtr  gP;
    
    refNum = (UInt16) notifyParamsP->userDataP;
    gP = PrvLockGlobals(refNum);
   
    resizedEvent.eType = 0;
    sizeEvent.eType = 0;
    if (notifyParamsP->notifyType == sysNotifyDisplayResizedEvent) {
        resizedEvent.eType = gP->displayResized;
        if (gP->DIAOpened != 0 || gP->DIAClosed != 0) {
            if (PINGetInputAreaState() == pinInputAreaOpen)
                sizeEvent.eType = gP->DIAOpened;
            else
                sizeEvent.eType = gP->DIAClosed;
        }
    }
    if (resizedEvent.eType != 0)
        EvtAddEventToQueue(&resizedEvent);
    if (sizeEvent.eType != 0)
        EvtAddEventToQueue(&sizeEvent);
        
    MemPtrUnlock(gP); 
    return 0;
}

/********************************************************************
 *           T H I S _ R e g i s t e r N o t i f y E v e n t s
 ********************************************************************/
Int32 THIS_RegisterNotifyEvents(UInt16 refNum, Int32 displayResized,
                                               Int32 DIAOpened,
                                               Int32 DIAClosed)
// DESC Register with the Notification Manager to be notified when
// DESC the sysNotifyDisplayResizedEvent notification occurs.  When it
// DESC does, an event for the specific type of notification will be 
// DESC generated.  The parameters for this function specify the event 
// DESC number to generate.  A value of 0 disables event notification for 
// DESCthat specific notification type.  Parameter values should typically
// DESC be in the custom event number range (24576 to 32767).  These events
// DESC can be "caught" with the NSBasic GetEventType() function.
// PARAM1 event number for sysNotifyDisplayResizedEvent - The display has
// PARAM1                                                 been resized.
// PARAM2 event number when the DIA has been opened
// PARAM3 event number when the DIA has been closed
// EXAMPLE Sub Form1223_After()
// EXAMPLE
// EXAMPLE     'Register to have the following events generated:
// EXAMPLE     '   event number 30001 - display resized
// EXAMPLE     '   event number 30002 - DIA has been opened
// EXAMPLE     '   event number 30003 - DIA has been closed
// EXAMPLE
// EXAMPLE     NPM.RegisterNotifyEvents 30001, 30002, 30003
// EXAMPLE End Sub
// EXAMPLE
// EXAMPLE
// EXAMPLE Sub Form1239_Event()
// EXAMPLE     Dim event as Integer
// EXAMPLE     
// EXAMPLE     event = GetEventType()
// EXAMPLE     Select Case event
// EXAMPLE         Case 30003
// EXAMPLE             Redraw
// EXAMPLE     End Select
// EXAMPLE End Sub
{
    THIS_ATTACH_GLOBALS
    
    if (LibID(&gP->thisLibCard, &gP->thisLibID)) {
	    if (gP->displayResized == 0 &&
	        gP->DIAOpened == 0 &&
	        gP->DIAClosed == 0) {
	        if (displayResized != 0 ||
	            DIAOpened != 0 ||
	            DIAClosed != 0)
	            SysNotifyRegister((UInt16) gP->thisLibCard, (LocalID) gP->thisLibID,
	                              sysNotifyDisplayResizedEvent,
	                              DIANotificationCallback,
	                              sysNotifyNormalPriority,
	                              (void *) refNum);
	    }
		else
	        if (displayResized == 0 &&
	            DIAOpened == 0 &&
	            DIAClosed == 0)
		        SysNotifyUnregister((UInt16) gP->thisLibCard, (LocalID) gP->thisLibID,
		                            sysNotifyDisplayResizedEvent,
		                            sysNotifyNormalPriority);
	    gP->displayResized = displayResized;
		gP->DIAOpened = DIAOpened;
		gP->DIAClosed = DIAClosed;
	}	                                
    THIS_DETACH_GLOBALS
    return 0;
}

/********************************************************************
 *           T H I S _ W i n G e t D i s p l a y E x t e n t
 ********************************************************************/
Int32 THIS_WinGetDisplayExtent(UInt16 refNum, Int32 *  x, Int32 *  y)
// DESC Return the current width and height of the display.
// RETURNS -1 - error.  The Pen Input Manager is not present on device.
{
    Coord extentX;
    Coord extentY;
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable)) {
	    WinGetDisplayExtent(&extentX, &extentY);
	    *x = (Int32) extentX;
	    *y = (Int32) extentY;
    }
    else {
    	*x = -1;
    	*y = -1;
    }
    
    THIS_DETACH_GLOBALS
    return 0;
}
/********************************************************************
 *           T H I S _ W i n S e t D i s p l a y E x t e n t
 ********************************************************************/
Int32 THIS_WinSetDisplayExtent(UInt16 refNum, Int32 size)
// DESC Sets the display width and height to either <<i>>normal<</i>> or
// DESC <<i>>large<</i>>.  If the size is <<i>>large<</i>>, the form's 
// DESC Dynamic Input Area (DIA) Policy, Input Trigger, and Dynamic
// DESC Input Area values will be set to <<i>>custom<</i>>, 
// DESC <<i>>enabled<</i>>, and <<i>>closed<</i>> respectively.
// PARAM1 0 = normal - 160 pixels wide x 160 pixels high
// PARAM1 1 = large - 160 pixels wide x 225 pixels high
{
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable)) {
	    if (size) {
			WinSetConstraintsSize(WinGetDisplayWindow(),
								  160, 160, 
								  pinMaxConstraintSize, 160, 
								  160, 160);
			FrmSetDIAPolicyAttr(FrmGetActiveForm(),
								frmDIAPolicyCustom);
			PINSetInputAreaState(pinInputAreaClosed);
			PINSetInputTriggerState(pinInputTriggerEnabled);
		}
		else {
			PINSetInputAreaState(pinInputAreaOpen);
			PINSetInputTriggerState(pinInputTriggerDisabled);
			FrmSetDIAPolicyAttr(FrmGetActiveForm(),
								frmDIAPolicyStayOpen);
			WinSetConstraintsSize(WinGetDisplayWindow(),
								  160, 160, 
								  160, 160, 
								  160, 160);
		}
	}

    THIS_DETACH_GLOBALS
	return 0;
}

/********************************************************************
 *           T H I S _ F r m G e t D I A P o l i c y
 ********************************************************************/
Int32 THIS_FrmGetDIAPolicy(UInt16 refNum, Int32 *value)
// DESC Returns the form's current Dynamic Input Area (DIA) Policy.
// DESC The DIA Policy controls whether the graffiti input area
// DESC may be collapsed or not.
// RETURNS 0 - frmDIAPolicyStayOpen.  The graffiti input area
// RETURNS                            is always open.  It cannot
// RETURNS                            be collapsed.
// RETURNS 1 - frmDIAPolicyCustom.  The graffiti input area is
// RETURNS                          collapsable.
// RETURNS -1 - error.  The Pen Input Manager is not present on device.
{
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable))
		*value = FrmGetDIAPolicyAttr(FrmGetActiveForm());
	else
		*value = -1;

    THIS_DETACH_GLOBALS
    return 0;
}

/********************************************************************
 *           T H I S _ F r m S e t D I A P o l i c y
 ********************************************************************/
Int32 THIS_FrmSetDIAPolicy(UInt16 refNum, Int32 value)
// DESC Sets the form's Dynamic Input Area (DIA) Policy. 
// DESC The DIA Policy controls whether the graffiti input area
// DESC may be collapsed or not.
// PARAM1 0 - frmDIAPolicyStayOpen.  The graffiti input area
// PARAM1                            is always open.  It cannot
// PARAM1                            be collapsed.
// PARAM1 1 - frmDIAPolicyCustom.  The graffiti input area is
// PARAM1                          collapsable.
{
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable)) {
	    if (value)
			FrmSetDIAPolicyAttr(FrmGetActiveForm(),
								frmDIAPolicyCustom);
		else
			FrmSetDIAPolicyAttr(FrmGetActiveForm(),
								frmDIAPolicyStayOpen);
	}

    THIS_DETACH_GLOBALS
	return 0;
}

/********************************************************************
 *           T H I S _ P I N G e t D I A S t a t e
 ********************************************************************/
Int32 THIS_PINGetDIAState(UInt16 refNum, Int32 *value)
// DESC Returns the current Dynamic Input Area (DIA) state. 
// RETURNS 0 - pinInputAreaOpen.  The Dynamic Input Area is displayed.
// RETURNS 1 - pinInputAreaClosed.  The Dynamic Input Area is not
// RETURNS                          being displayed.
// RETURNS 2 - pinInputAreaNone.  There is no Dynamic Input Area.
// RETURNS -1 - error.  The Pen Input Manager is not present on device.
{
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable))
		*value = (Int32) PINGetInputAreaState();
	else
		*value = -1;

    THIS_DETACH_GLOBALS
	return 0;
}

/********************************************************************
 *           T H I S _ P I N S e t D I A S t a t e
 ********************************************************************/
Int32 THIS_PINSetDIAState(UInt16 refNum, Int32 value)
// DESC Sets the current Dynamic Input Area (DIA) state. 
// PARAM1 0 - pinInputAreaOpen.  The Dynamic Input Area will be displayed.
// PARAM1 1 - pinInputAreaClosed.  The Dynamic Input Area will not
// PARAM1                          be displayed.
// PARAM1 2 - pinInputAreaNone.  There is no Dynamic Input Area.
{
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable))
		PINSetInputAreaState((PinInputTriggerStateT16) value);

    THIS_DETACH_GLOBALS
	return 0;
}

/********************************************************************
 *     T H I S _ P I N G e t I n p u t T r i g g e r S t a t e
 ********************************************************************/
Int32 THIS_PINGetInputTriggerState(UInt16 refNum, Int32 *value)
// DESC Returns the current Input Trigger state. 
// RETURNS 0 - pinInputTriggerEnabled.   The status bar icon is enabled,
// RETURNS                               meaning that the user is allowed
// RETURNS                               to open and close the Dynamic
// RETURNS                               Input Area.
// RETURNS 1 - pinInputTriggerDisabled.  The status bar icon is disabled,
// RETURNS                               meaning that the user is not
// RETURNS                               allowed to open and close the
// RETURNS                               Dynamic Input Area.
// RETURNS 2 - pinInputTriggerNone.   There is no Dynamic Input Area.
// RETURNS -1 - error.  The Pen Input Manager is not present on device.
{
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable))
		*value = (Int32) PINGetInputTriggerState();
	else
		*value = -1;

    THIS_DETACH_GLOBALS
	return 0;
}

/********************************************************************
 *     T H I S _ P I N S e t I n p u t T r i g g e r S t a t e
 ********************************************************************/
Int32 THIS_PINSetInputTriggerState(UInt16 refNum, Int32 value)
// DESC Sets the current Input Trigger state. 
// PARAM1 0 - pinInputTriggerEnabled.   Enable the status bar icon,
// PARAM1                               meaning that the user is allowed
// PARAM1                               to open and close the Dynamic
// PARAM1                               Input Area.
// PARAM1 1 - pinInputTriggerDisabled.  Disable the status bar icon,
// PARAM1                               meaning that the user is not
// PARAM1                               allowed to open and close the
// PARAM1                               Dynamic Input Area.
// PARAM1 2 - pinInputTriggerNone.   There is no Dynamic Input Area.
{
    THIS_ATTACH_GLOBALS
    
    if (PenMgrAvailable(&gP->penMgrAvailable))
		PINSetInputTriggerState((PinInputTriggerStateT16) value);

    THIS_DETACH_GLOBALS
	return 0;
}

/*###################################################################
 #       S t a n d a r d ,  R E Q U I R E D    F u n c t i o n s
 #*/

Err THIS_LibOpen(UInt16 refNum) {
    Err err = 0;
    THIS_ALLOCATE_GLOBALS	// Define local variables before this            
	
    return err;
}

Err THIS_LibClose(UInt16 refNum) {
    Err err = 0;
    
    THIS_FREE_GLOBALS	// Define local variables before this

    return err;
}

Err THIS_LibSleep(UInt16 refNum) {
    return 0;
}

Err THIS_LibWake(UInt16 refNum) {
    return 0;
}

/*###################################################################*/
