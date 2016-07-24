#ifndef TAN_DISPLAY_H
#define TAN_DISPLAY_H

void Set_LW_PID_Param(void);
void Set_Height_PID_Param(void);
void Set_YAW_PID_Param(void);
void Display_Menu(void);
void Choose_Menu(void);

extern char CMD_FLAG;
extern char START_FLAG;
extern int value;
#endif // TAN_DISPLAY_H
