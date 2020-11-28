/*
 * UselessBox - A useless box.
 * 
 */

#include <Servo.h>

/* Arduino pins */
#define DOOR_PIN    ((byte) 9)
#define FINGER_PIN  ((byte)10)
#define LED_PIN     ((byte)13)
#define SWITCH_PIN  ((byte) 8)

/* Response commands */
#define C_END       ((byte) 0)
#define C_WAIT      ((byte) 1)
#define C_DOOR      ((byte) 2)
#define C_FINGER    ((byte) 3)

/* Speed definitions */
#define SLOW        ((byte) 0)
#define NORMAL      ((byte) 1)
#define FAST        ((byte) 2)
#define HYPER       ((byte) 3)

static int speedIndex[4] { 5, 20, 40, 80 };

/*
 * tim     time in 100msec units (up to 63 = 6.3 seconds)
 * pos     position 0 - 15
 * spd     speed as defined above
 */
#define END               (C_END << 6)
#define WAIT( tim)       ((C_WAIT << 6)   | (tim & 0b00111111))
#define DOOR( pos, spd)  ((C_DOOR << 6)   | ((pos & 0b00001111) << 2) | spd)
#define FINGER( pos,spd) ((C_FINGER << 6) | ((pos & 0b00001111) << 2) | spd)

#define GET_CMD( resp)   ((resp & 0b11000000) >> 6)
#define GET_TIM( resp)   (resp & 0b00111111)
#define GET_POS( resp)   ((resp & 0b00111100) >> 2)
#define GET_SPD( resp)   (resp & 0b00000011)

/* DO NOT FORGET TO TERMINATE EVERY SEQUENCE WITH "END" */
static byte moveSeq[] = { WAIT( 10), DOOR( 15, NORMAL), FINGER( 15, NORMAL), WAIT( 2), FINGER( 0, FAST), DOOR( 0, NORMAL), END };

/* */
typedef struct ServoStruct {

  int minPos;
  int maxPos;
  int curPos;

  Servo servo;
  
} ServoStruct;

ServoStruct door;
ServoStruct finger;

void setup() {

  // Serial.begin(19200);
  
  pinMode( LED_PIN, OUTPUT);

  pinMode( SWITCH_PIN, INPUT_PULLUP);

  initServo( door, DOOR_PIN, 1800, 1200);
  initServo( finger, FINGER_PIN, 1800, 1200);

  flash( 2, 200);
}

void loop() {

  if( digitalRead( SWITCH_PIN)) {
    react( moveSeq);
  }
}

static void react( byte seq[]) {

  byte index = 0;
  byte command;
  
  while( (command = seq[index++]) != END) {
    
    switch( GET_CMD( command)) {
      case C_END:  // should never come here
        break;

      case C_WAIT: // wait for n * 100msec
        delay( 100 * GET_TIM( command));
        break;

      case C_DOOR: // move door
        moveServo( door, GET_POS( command), GET_SPD( command));
        break;

      case C_FINGER: // move finger
        moveServo( finger, GET_POS( command), GET_SPD( command));
        break;
    }    
  }    
}

static void moveServo( ServoStruct &s, byte pos, byte spd) {

  int endPos = s.minPos + ((s.maxPos - s.minPos) / 15) * pos;
  int delta = (endPos - s.curPos);
  int deltaPerStep = speedIndex[spd];
  int steps;

  if( delta < 0) deltaPerStep = -deltaPerStep;

  steps = delta / deltaPerStep;

  for(int i=0; i < steps; i++) {
    s.curPos += deltaPerStep;
    s.servo.writeMicroseconds( s.curPos);
    delay( 18);
  }
}

static void initServo( ServoStruct &s, byte pin, int minP, int maxP) {
  
  s.minPos = minP;
  s.maxPos = maxP;
  s.curPos = minP;
  s.servo.attach(pin);
  s.servo.writeMicroseconds( s.curPos);
}

static void flash( byte howOften, int howFast) {
  
  for( byte i=0; i<howOften; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(howFast);
    digitalWrite(LED_PIN, LOW);
    delay(howFast);
  }
}
