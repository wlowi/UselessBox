/*
 * UselessBox - A useless box.
 * 
 */

#include <Servo.h>

#define DEBUG_ON

/* Arduino pins */
#define DOOR_PIN    ((byte) 9)
#define FINGER_PIN  ((byte)10)
#define LED_PIN     ((byte)13)
#define SWITCH_PIN  ((byte) 8)

#define CONFIG_PIN  ((byte) 7)
#define UP_PIN      ((byte) 6)
#define DOWN_PIN    ((byte) 5)

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

static int speedIndex[4] { 3, 10, 25, 40 };

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
static byte moveSeq1[] = { WAIT( 10), DOOR( 15, NORMAL), FINGER( 15, FAST), WAIT( 2), FINGER( 0, FAST), DOOR( 0, HYPER), END };
static byte moveSeq2[] = { WAIT( 10), FINGER( 15, FAST), FINGER( 0, FAST), END };
static byte moveSeq3[] = { WAIT( 10), DOOR( 7, SLOW), WAIT( 15), DOOR( 0, FAST), WAIT( 5), FINGER( 15, FAST), FINGER( 0, HYPER), END };

typedef uint16_t checksum_t;

typedef struct Config {
    
  int door_minPos;
  int door_maxPos;
  int finger_minPos;
  int finger_maxPos;
  
  checksum_t checksum;

} Config;

#define EEPROM_START_LOC    ((uint8_t*)0)
#define CONFIG_SIZE_BYTES   (sizeof(Config) - sizeof(checksum_t))

/* */
typedef struct ServoStruct {

  int minPos;
  int maxPos;
  int curPos;

  Servo servo;
  
} ServoStruct;

#define SERVO_MID_POS       ((int)1500)
#define SERVO_FRAME_MSEC    ((int)18)

ServoStruct door;
ServoStruct finger;
Config configuration;

void setup() {

#ifdef DEBUG_ON
  Serial.begin(19200);
#endif

  pinMode( LED_PIN, OUTPUT);
  pinMode( SWITCH_PIN, INPUT_PULLUP);
  pinMode( CONFIG_PIN, INPUT_PULLUP);
  pinMode( UP_PIN, INPUT_PULLUP);
  pinMode( DOWN_PIN, INPUT_PULLUP);

  initServo( door, DOOR_PIN);
  initServo( finger, FINGER_PIN);
    
  readConfig( configuration);
  if( !isConfigValid(configuration)) {
    flash( 5, 100);
    doConfig( configuration);
  }

  configServo( door, 
    configuration.door_minPos,
    configuration.door_maxPos);

  configServo( finger, 
    configuration.finger_minPos,
    configuration.finger_maxPos);

  randomSeed(analogRead(0));
}

void loop() {

  if( keyPressed( CONFIG_PIN)) {
    
    doConfig( configuration);

    configServo( door, 
      configuration.door_minPos,
      configuration.door_maxPos);

    configServo( finger, 
      configuration.finger_minPos,
      configuration.finger_maxPos);
  }

  if( !digitalRead( SWITCH_PIN)) {
    byte r = (byte)random(10);

    if( r < 4) {
      react( moveSeq1);
    } else if( r < 8) {
      react( moveSeq2);
    } else {
      react( moveSeq3);
    }
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
    delay( SERVO_FRAME_MSEC);
  }
  
  s.curPos = endPos;
  s.servo.writeMicroseconds( s.curPos);
  delay( SERVO_FRAME_MSEC);
}

static void initServo( ServoStruct &s, byte pin) {
  
  s.minPos = SERVO_MID_POS;
  s.maxPos = SERVO_MID_POS;
  s.curPos = SERVO_MID_POS;
  s.servo.attach(pin);
}

static void configServo( ServoStruct &s, int minP, int maxP) {
  
  s.minPos = minP;
  s.maxPos = maxP;
  s.curPos = minP;
  s.servo.writeMicroseconds(s.curPos);
  delay( SERVO_FRAME_MSEC);
}

static void flash( byte howOften, int howFast) {
  
  for( byte i=0; i<howOften; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(howFast);
    digitalWrite(LED_PIN, LOW);
    delay(howFast);
  }
}

static boolean keyPressed( byte pin) {

  if( !digitalRead(pin)) {
    delay( 50);
    while( !digitalRead(pin)) {
      delay( 10);
    }
    delay( 50);
    return true;
  }

  return false;
}

static void doConfig( Config &cfg) {
  
  while( !keyPressed( CONFIG_PIN)) {
    door.servo.writeMicroseconds(SERVO_MID_POS);
    finger.servo.writeMicroseconds(SERVO_MID_POS);
    delay( SERVO_FRAME_MSEC);
  }
  
  flash( 1, 200);
  cfg.door_minPos = kbdMove( door, cfg.door_minPos);
  
  flash( 2, 200);
  cfg.door_maxPos = kbdMove( door, cfg.door_maxPos);

  flash( 3, 200);
  cfg.finger_minPos = kbdMove( finger, cfg.finger_minPos);

  flash( 4, 200);
  cfg.finger_maxPos = kbdMove( finger, cfg.finger_maxPos);

  flash( 1, 500);
  writeConfig( cfg);
}

static int kbdMove( ServoStruct &s, int pos) {
  
  s.servo.writeMicroseconds(pos);
  delay( SERVO_FRAME_MSEC);

  while( !keyPressed( CONFIG_PIN)) {
    
    if( !digitalRead(DOWN_PIN)) {    
      pos -= 1;
      s.servo.writeMicroseconds(pos);
      delay( SERVO_FRAME_MSEC);
      
    } else if( !digitalRead(UP_PIN)) {   
      pos += 1;
      s.servo.writeMicroseconds(pos);
      delay( SERVO_FRAME_MSEC);
    }
  }

  return pos;
}

static void readConfig( Config &cfg) {

  void *configStart;

  configStart = (void*)(EEPROM_START_LOC);
  eeprom_read_block( &cfg, configStart, sizeof(Config));

#ifdef DEBUG_ON
  Serial.println("read");
  Serial.println(cfg.door_minPos);
  Serial.println(cfg.door_maxPos);
  Serial.println(cfg.finger_minPos);
  Serial.println(cfg.finger_maxPos);
  Serial.println(cfg.checksum);
#endif
}

static void writeConfig(Config &cfg) {
    
  void *configStart;

  configStart = (void*)(EEPROM_START_LOC);
  cfg.checksum = computeChecksum( cfg);

#ifdef DEBUG_ON
  Serial.println("write");
  Serial.println(cfg.door_minPos);
  Serial.println(cfg.door_maxPos);
  Serial.println(cfg.finger_minPos);
  Serial.println(cfg.finger_maxPos);
  Serial.println(cfg.checksum);
#endif

  eeprom_update_block( &cfg, configStart, sizeof(Config));
}

static checksum_t computeChecksum( Config &cfg)
{
    checksum_t checksum = 0;
    byte *p = (byte*)(&cfg);

    for( checksum_t i = 0; i < CONFIG_SIZE_BYTES; i++)
    {
        checksum = rotate(checksum);
        checksum ^= (i ^ ((byte)*(p+i)));
    }

    return checksum;
}

/* Left rotate unsigned int value.
 */
static checksum_t rotate( checksum_t v)
{
    return ((v >> 15) & 1) | (v << 1);
}

static boolean isConfigValid( Config &cfg)
{
    return cfg.checksum == computeChecksum( cfg);
}
