# Sonar using two HC-SR04 and Arduino UNO R3

This project was made by two students from RTU (Riga Technical University) in 2026 as a course project.
The repo contains all the information needed to assemble a similar unit yourself.




https://github.com/user-attachments/assets/edb18fed-01fb-47a9-b66d-83d6e0534f34




Project was inspired by Alex Gyver "[Aim-Fan (MOS DUAL VERSION)](https://github.com/AlexGyver/Aim-Fan/tree/master)"
As it is under the MIT license, we took the original code and modified it for our project.


# Part list
| Part name  | Quantity |
|     :---:      |     :---:      |
| Arduino UNO R3 (or analog)  | 1  |
| Red LED  | 1  |
| HC-SR04  | 2  |
| Switch | 1 |
| 1 kOhm Resistor | 1 |
| Breadboard | 1 |
| Holder for 4x AA batteries | 1 |
| Servo motor 18,3kg/cm (6V) 180° | 1 |
| Jumper wires | many |


# Shematics

<img width="1167" height="815" alt="image" src="https://github.com/user-attachments/assets/5bd891da-57b9-4ea0-ad4b-bf8c2423a0aa" />


The provided schematic shows how to assemble all parts together.
In this modification you need to power the Arduino separately using a 5V USB power supply.
It can be changed to use only batteries, but it may cause possible MCU instability.

If you want to use the second variant, here is the schematic:


<img width="1181" height="797" alt="image" src="https://github.com/user-attachments/assets/fe4eadb4-b63f-47b1-81bf-21bc068e8ade" />

# Instructions

The main part for assembly is the main platform. It can be 3D printed or cut from wood.
Other parts are mainly made for aesthetics and can be replaced with simpler constructions.
By default, your device should look something like this:


<img width="1280" height="1022" alt="image" src="https://github.com/user-attachments/assets/d5b31ae6-e94d-43d3-b381-cca5dbd11c8f" />


<img width="1151" height="1280" alt="image" src="https://github.com/user-attachments/assets/10ac3cef-c4d1-49e9-90bb-d19e60157e4d" />



**Note:** This is a reference design only, so in your variant the part layout may be different.

# Main variables

This is the only place where you should change any settings.
You can try many combinations to get the desired output reliability and distance.

```
// ------------- SETTINGS --------------
#define STEP_DELAY 25       // SPEED OF SERVO
#define TIMEOUT 650        // TIME OUT BEFORE NEW SEEK
#define MAX_ANGLE 180       // MAXIMAL ROTATION ANGLE
#define MIN_ANGLE 0        // MINIMAL ROTATION ANGLE
#define DIST_MAX 200        // MAX DISTANCE CAP
#define DEADZONE 20         // MINIMAL DIFFERENCE WITH CALIBRATION
#define MIN_CATCH 15         // MIN POINTS TO COUNT AS TARGET
#define MISTAKES 5          // MISTAKES THAT SISTEM IGNORES DURING SCAN
// ------------- SETTINGS --------------

// ---------- PINS ----------
#define TRIG 4
#define ECHO 5
#define TRIG2 7
#define ECHO2 6

#define SERVO 9
#define MOS 10
// ---------- PINS ----------
```
At this point, if everything is assembled correctly and the preferences are set properly, you should have a working device.


# Contributions


This project is licensed under the MIT license, which means you can use, modify, sell, etc., without any fees.

Feel free to share your opinion in the Issues section or create pull requests.
If there are really good improvements, we will merge them with proper credit.
