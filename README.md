# Microwave-Oven-Implementation

This project simulates a **microwave oven** using the **PIC16F877A microcontroller**, implemented in **Embedded C**. The system controls cooking time, power levels, and safety mechanisms using various peripherals, and is tested via simulation. The microcontroller handles,
    
    Input scanning
    Data storage in EEPROM
    Communication over UART
    Peripheral control. 

All functionalities are simulated and tested using PICSIM Lab and MPLAB IDE.

Technologies Used:

  **MPLAB IDE**
 
  **PICSIM Lab** (for simulation)
  
  **Embedded C**
  
  **Network Communication Tools**

Peripherals Interfaced:

  **CLCD (Character LCD)**
  
  **Matrix Keypad**
  
  **EEPROM**
  
  **UART**
  
  **Interrupts**
  
  **Buzzer**
  
  **Fan**

power on/reset--->POWER_ON

POWER_ON:

Powering on Microwave oven along with progress bar display for 3sec after that it should enter to MENU_SCREEN

MENU_SCREEN:
    
    1.Micro
    2.Grill
    3.Convection
    4.Start
MICRO_MODE:

if key 1 pressed, it enters to micro mode.

1.power = 900W

2.After 1sec delay,set_timer screen need to display

3.set time:
	Pattern:
	SET TIME <MM:SS>
	TIME:00:00//MM:SS
		//condition for setting time:
		1.second field 00 should be blinking initially
		2.based on key press that key(0 to 9) is considered as time
		3.Once both digits filled in second field, then minute field 00 should start blinking and based on key press minute considered
	*:CLEAR #:ENTER
		//condition for *,#:
		1.if *--> current blinking field should become 00
		2.if#--->enter into timer screen

4.display time (timer screen)
	Pattern:
	TIME:00:00//MM:SS
	4.START/RESUME
	5.PAUSE
	STOP
the time we set starts decrementing once it reaches 00:00, task done displayed
		start/resume:
		30 is added to time but max time for second is 60,based on that 30 added for each time key 4 pressed
		key 5 it should pause
		key 6 timer stops

5.Task done:
	once timer reaches 00,TASK DONE!!Enjoy your meal should be displayed

GRILL_MODE:

if key 2 pressed, it enters to grill_mode. Like previous mode(micro_mode),in grill_mode also it should display set_time and then timer screen need to display

CONVECTION_MODE:

if key 3 pressed, it enters to convection mode. Temperature is set in this mode and timer set and starts to run like previous mode

START_MODE:

if key 4 pressed, it enters to start mode. 30sec timer is run in this mode


