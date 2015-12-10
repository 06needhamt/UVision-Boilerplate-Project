#define USER_BUTTON 0
#define JOYSTICK_SELECT 1
#define JOYSTICK_UP 2
#define JOYSTICK_DOWN 3
#define JOYSTICK_LEFT 4
#define JOYSTICK_RIGHT 5

void userButton_Init(void); //initialise the user button as input

void joyStick_Init(void); //initialise the joystick switches as inputs

void userButton_IntrEnable(void); //enables user button interrupt handling

void joyStick_IntrEnable_PG15_13(void); //enables joystick pins PG13, PG14, and PG15 for interrupts handling

void joyStick_IntrEnable_PG7(void); //enables joystick pin PG7 for interrupt handling

void joyStick_IntrEnable_PD3(void); //enables joystick pin PD3 for interrupt handling
