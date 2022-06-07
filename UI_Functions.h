//	Filename:  UI_Functions.h
//	Author:  Paul Wittibschlager
//	Date Created:  08/27/2018
//	Company:  Valtronic, USA
//	Project:   Lumitex OraGlo Control Board
//	Project number:  DD400004
//	Description:  This file contains functions to support the user interface.

#ifndef UI_FUNCTIONS_H
#define	UI_FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

/* States within the UI state machine
   Their values MUST start at 0 and increment contiguously
     so that we can iterate through them with a for loop  */  
typedef enum {
    POWER_UP_STATE = 0,
    STANDBY_STATE,
    READY_STATE,
    OPERATION_STATE,
    THERAPY_COMPLETE_STATE,
    PAUSED_STATE,
    ERROR_STATE,            
    INSERT_MCA_STATE,
    READING_MCA_STATE,
    VERIFY_SN_STATE,
    MCA_DETACHED_STATE,
    MCA_READING_ERROR_STATE,
    MCA_EXPIRED_STATE,
    MCA_PERIOD_ERROR_STATE,
    MCA_RESUME_CONFIRM_STATE,
    NUM_UI_STATES // This is not an actual state.
                  // It just reports the number of states in use.
} ui_state_t;


typedef enum {
    IMAGE_1 = 0,
    IMAGE_1_TIME,
    IMAGE_2,
    IMAGE_2_TIME,
    IMAGE_3,
    IMAGE_3_TIME,
    IMAGE_4,
    IMAGE_4_TIME,
    COLOR_1_R,
    COLOR_1_G,
    COLOR_1_B,
    COLOR_1_TIME,
    COLOR_2_R,
    COLOR_2_G,
    COLOR_2_B,
    COLOR_2_TIME,
    SPARE0,
    SPARE1,
    SPARE2,
    SPARE3 
} ui_setting_t;

typedef enum IMAGE_ENUMERATION {
    PAUSED_TEXT_IMAGE = 2008,
    PRESS_TO_RESUME_IMAGE = 2012
} IMAGE_ENUM;
void InitializeUserInterface(void);

void UserInterfaceSubmenu(void);
void UserInterfacePrintNotes(void);

uint16_t GetCurrentlyUsedLEDCurrent(void);

uint32_t ReadUISetting(ui_state_t phase, ui_setting_t setting); 

// Function for implementing the User Interface state machine
void UpdateUIStateMachine(void);

void GenerateCountdownDisplay(void);


#ifdef	__cplusplus
}
#endif

#endif	/* UI_FUNCTIONS_H */

