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
// ---------- pins ----------


#include "Servo.h"
Servo servo;

#include "GyverHacks.h"
GTimer stepTimer(STEP_DELAY);
GTimer sonarTimer(50);
GTimer timeoutTimer(TIMEOUT);

#include <NewPing.h>
NewPing sonar1(TRIG, ECHO, DIST_MAX);
NewPing sonar2(TRIG2, ECHO2, DIST_MAX);

typedef enum {SEEK, HOLD};
boolean direct;
boolean next;
const byte steps_num = (MAX_ANGLE - MIN_ANGLE);
int angle = MIN_ANGLE;
int distance1[steps_num + 1], distance2[steps_num + 1];
boolean catch_flag, catched_flag, hold_flag = false;
byte catch_num;
byte mistakes;
byte mode = 0;
byte catch_pos;
int hold_signal;
byte direct_seek = 2;
byte lost_flag = 0;


void setup() {
  Serial.begin(115200);
  servo.attach(SERVO);
  servo.write(MIN_ANGLE);
  pinMode(MOS, OUTPUT);
  delay(5000);
  digitalWrite(MOS, 0);
  delay(3000);            
  calibration();          
}

void loop() {
  switch (mode) {
    case SEEK: seek();  
      break;
    case HOLD: hold();  
      break;
  }
}

void seek() {
  if (direct) {             
    if (angle < MAX_ANGLE)
      turn_to(MAX_ANGLE);   
    else {
      direct = false;       
      delay(50);            
    }
  }
  else {                    
    if (angle > MIN_ANGLE)
      turn_to(MIN_ANGLE);   
    else {
      direct = true;        
      delay(50);            
    }
  }
  search();                 
}

void hold() {
  if (!hold_flag) {       
    turn_to(catch_pos);     
  } else {                    
    digitalWrite(MOS, 1);
    if (sonarTimer.isReady()) {                   
      byte pos = (angle - MIN_ANGLE);         
      int curr_dist1, curr_dist2;
      int diff1, diff2;

      curr_dist1 = sonar2.ping_cm(); 
      if (curr_dist1 == 0) curr_dist1 = DIST_MAX;   
      diff1 = distance1[pos] - curr_dist1;          
      delay(50);
      curr_dist2 = sonar1.ping_cm();   
      if (curr_dist2 == 0) curr_dist2 = DIST_MAX;   
      diff2 = distance2[pos] - curr_dist2;          

      if (lost_flag == 0) {
        if (diff1 < DEADZONE) {
          direct_seek = 1;
          lost_flag = 1;
        }
        if (diff2 < DEADZONE) {
          direct_seek = 0;
          lost_flag = 1;
        }
      } else if (lost_flag == 1) {
        if (diff1 < DEADZONE && diff2 < DEADZONE) lost_flag = 2;
      }

      if (diff1 > DEADZONE && diff2 > DEADZONE) {
        lost_flag = 0;
      }

      if (lost_flag == 2) {   
        if (timeoutTimer.isReady()) {             
          mode = SEEK;                            
          hold_flag = false;
          direct = direct_seek;
          lost_flag = 0;
          digitalWrite(MOS, 0);
        }
      } else {                                    
        timeoutTimer.reset();                     
      }
    }
  }
}

void search() {
  if (/*angle % 2 == 0 &&*/ next) {                 
    
    next = false;

    byte pos = (angle - MIN_ANGLE);         
    int curr_dist;
    int diff;

    if (direct) curr_dist = sonar2.ping_cm(); 
    else curr_dist = sonar1.ping_cm();

    if (curr_dist == 0) curr_dist = DIST_MAX;   

    if (direct) diff = distance1[pos] - curr_dist;         
    else diff = distance2[pos] - curr_dist;

    if (diff > DEADZONE) {                      
      if (!catch_flag) {
        catch_flag = true;        
        catch_pos = angle;        
      }
      catch_num++;                
      if (catch_num > MIN_CATCH)  
        catched_flag = true;      
    } else {                                    
      if (catch_flag) {               
        if (mistakes > MISTAKES) {    
          
          catch_flag = false;
          catched_flag = false;
          catch_num = 0;
          mistakes = 0;
        } else {
          mistakes++;                 
        }
      }

      if (catched_flag) {             
        mode = HOLD;                  
        
        if (direct) catch_pos += (catch_num / 2);
        else catch_pos -= (catch_num / 2);

        hold_flag = false;
        catch_flag = false;
        catched_flag = false;
        catch_num = 0;
        mistakes = 0;
      }
    }
  }
}

void calibration() {
  for (angle = MIN_ANGLE; angle <= MAX_ANGLE; angle++) {
    servo.write(angle);
    byte pos = (angle - MIN_ANGLE);
    int curr_dist = sonar2.ping_cm();
    if (curr_dist == 0) distance1[pos] = DIST_MAX;
    else distance1[pos] = curr_dist;
    delay(STEP_DELAY);
  }
  for (angle = MAX_ANGLE; angle >= MIN_ANGLE; angle--) {
    servo.write(angle);
    byte pos = (angle - MIN_ANGLE);
    int curr_dist = sonar1.ping_cm();
    if (curr_dist == 0) distance2[pos] = DIST_MAX;
    else distance2[pos] = curr_dist;
    delay(STEP_DELAY);
  }
}

void turn_to(byte to_angle) {
  if (stepTimer.isReady()) {
    if (angle < to_angle) angle++;
    else if (angle > to_angle) angle--;
    else {
      hold_flag = true;
    }
    servo.write(angle);
    next = true;
    if (angle > MAX_ANGLE) direct = false;
    if (angle < MIN_ANGLE) direct = true;
  }
}
