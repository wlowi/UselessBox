/*
 * UselessBox - A useless box by WOLFIX.
 *
 * 19-12-2020 - V0.9.6, Low battery voltage monitor.
 * 18-12-2020 - V0.9.5, Switch power off after 60 sec.
 * 16-12-2020 - V0.9.4, Couple of changes to moves
 * 07-12-2020 - V0.9.3, Idle clap 
 * 02-12-2020 - V0.9.2, changed servo frame time handling
 * 20-11-2020 - V0.9.1, added immediate response
 * 29-11-2020 - V0.9, basically working
 */

#include <Servo.h>


/* Dump some stuff to Serial if this is defined. */
#undef DEBUG_ON

/* Arduino pins */
#define DOOR_PIN     ((byte) 9)
#define FINGER_PIN   ((byte)10)
#define LED_PIN      ((byte)13)
#define SWITCH_PIN   ((byte) 8)
#define POWER_PIN    ((byte) 4)

#define CONFIG_PIN   ((byte) 7)
#define UP_PIN       ((byte) 6)
#define DOWN_PIN     ((byte) 5)

/* All voltage values are *100 to 
 * retain 2 digits
 */
#define VOLTAGE_PIN  A0
/* ADC reference voltage 5.0 V */
#define ADC_REF_V    (500)
/* ADC resolution 0 - 1023 */
#define ADC_RES      (1024)

/* VDD Power low limit.
 * Below this value the battery is considered empty.
 * Default 7.2V
 */
#define POWER_LOW_V  (720)

/* Voltage divider resistors 18k and 9.9k 
 * Actually R2 is 10k but it is parallel with A0 input.
 * 
 * The actual values are irrelevant only the relation between 
 * them is relevant.
 */
#define V_DIVIDER_R1 (180)
#define V_DIVIDER_R2 ( 99)

/* Determine switch state */
#define IS_SWITCH_ON  (isSwitchOn( SWITCH_PIN))
#define IS_SWITCH_OFF (!IS_SWITCH_ON)

/* Power states */
#define ON          (true)
#define OFF         (false)

/* Response commands */
#define C_END       ((byte) 0)
#define C_WAIT      ((byte) 1)
#define C_DOOR      ((byte) 2)
#define C_FINGER    ((byte) 3)

/* Speed definitions for door and finger moves. */
#define SLOW        ((byte) 0)
#define NORMAL      ((byte) 1)
#define FAST        ((byte) 2)
#define WARP        ((byte) 3)

/* Speed index used for SLOW,NORMAL,FAST,WARP speed.
 * Microsecond increase/decrease per servo frame time (around 20msec).
 */
static int speedIndex[4] {
  3,  // SLOW
  8,  // NORMAL
  20, // FAST
  40  // WARP
};

/* Move sequence commands.
 * Every command is encoded in a single byte.
 * 
 * Parameters:
 *  tim     time in 100msec units (up to 63 = 6.3 seconds)
 *  pos     position 0 - 15
 *  spd     speed as defined above
 */
#define END               (C_END << 6)
#define WAIT( tim)       ((C_WAIT << 6)   | (tim & 0b00111111))
#define DOOR( pos, spd)  ((C_DOOR << 6)   | ((pos & 0b00001111) << 2) | spd)
#define FINGER( pos,spd) ((C_FINGER << 6) | ((pos & 0b00001111) << 2) | spd)

/* Macros to retrieve values from move sequence commands.
 */
#define GET_CMD( resp)   ((resp & 0b11000000) >> 6)
#define GET_TIM( resp)   (resp & 0b00111111)
#define GET_POS( resp)   ((resp & 0b00111100) >> 2)
#define GET_SPD( resp)   (resp & 0b00000011)


/* Move sequences.
 *
 * DO NOT FORGET TO TERMINATE EVERY SEQUENCE WITH "END"
 */
static byte moveAggressive[] = {
  DOOR( 10, FAST),
  FINGER( 15, FAST), WAIT( 1), FINGER( 0, FAST),
  DOOR( 0, WARP), END
};

static byte moveNormal[] = {
  DOOR( 15, NORMAL),
  FINGER( 15, NORMAL), WAIT( 2), FINGER( 0, FAST),
  DOOR( 0, WARP), END
};

static byte moveShy[] = {
  DOOR( 8, SLOW), WAIT( 15), DOOR( 0, FAST), WAIT( 5),
  DOOR( 10, FAST),
  FINGER( 15, FAST), WAIT( 1), FINGER( 0, WARP),
  DOOR( 0, WARP), END
};

static byte moveSeq4[] = {
  DOOR( 15, NORMAL),
  FINGER( 10, SLOW), WAIT( 5), FINGER( 15, WARP), WAIT( 1), FINGER( 10, WARP), WAIT( 10), FINGER(0, NORMAL),
  DOOR( 0, WARP), END
};

static byte moveSeq5[] = {
  DOOR( 15, NORMAL),
  FINGER( 10, SLOW), WAIT( 5), FINGER( 15, WARP), WAIT( 1), FINGER( 0, WARP),
  DOOR( 0, WARP), END
};

static byte moveSeq6[] = {
  DOOR( 15, NORMAL),
  FINGER( 10, NORMAL), WAIT( 3),
  FINGER( 12, WARP), WAIT( 1), FINGER( 10, WARP), WAIT( 1),
  FINGER( 12, WARP), WAIT( 1), FINGER( 10, WARP), WAIT( 1),
  FINGER( 12, WARP), WAIT( 1), FINGER( 10, WARP), WAIT( 3),
  FINGER( 15, WARP), WAIT( 1), FINGER( 0, WARP),
  DOOR( 0, WARP), END
};

/* Switch yourself off after this time */
#define IDLE_TIMEOUT_msec ((long)60000)
/* Do not clap right after movement */
#define QUIET_TIME_msec   ((long)10000)

static byte moveClap3[] = {
  DOOR( 5, FAST), DOOR( 0, FAST),
  DOOR( 5, FAST), DOOR( 0, FAST),
  DOOR( 5, FAST), DOOR( 0, FAST), END
};

/* This indicates low battery voltage */
static byte moveClap5[] = {
  DOOR( 5, FAST), DOOR( 0, FAST),
  DOOR( 5, FAST), DOOR( 0, FAST),
  DOOR( 5, FAST), DOOR( 0, FAST),
  DOOR( 5, FAST), DOOR( 0, FAST),
  DOOR( 5, FAST), DOOR( 0, FAST), END
};

/* The list of move sequences to be used in a random order. */
static byte *moveSequences[] = {
  moveAggressive,
  moveNormal,
  moveShy,
  moveSeq4,
  moveSeq5,
  moveSeq6
};


/* Configuration stored in EEPROM.
 * Stores start and end positions of door and finger.
 */
 
typedef uint16_t checksum_t;

typedef struct config_t {

  int door_minPos;
  int door_maxPos;
  int finger_minPos;
  int finger_maxPos;

  checksum_t checksum;

} config_t;

#define EEPROM_START_LOC    ((uint8_t*)0)
#define CONFIG_SIZE_BYTES   (sizeof(config_t) - sizeof(checksum_t))


/* */
typedef struct servostruct_t {

  int minPos;
  int maxPos;
  int curPos;

  unsigned long nextFrame_msec;
  
  Servo servo;

} servostruct_t;

#define SERVO_MID_POS_usec  ((int)1500)

/* May set this to 11 for fast digital servos */
#define SERVO_FRAME_msec    ((int)20)


/***** global variables ************************/

servostruct_t door;
servostruct_t finger;
config_t configuration;
unsigned long lastMove = 0L;
unsigned long nextClap = 0L;
unsigned long powerOff = 0L;

/***********************************************/

void setup() {

#ifdef DEBUG_ON
  Serial.begin(19200);
#endif

  pinMode( LED_PIN, OUTPUT);
  pinMode( POWER_PIN, OUTPUT);
  pinMode( SWITCH_PIN, INPUT_PULLUP);
  pinMode( CONFIG_PIN, INPUT_PULLUP);
  pinMode( UP_PIN, INPUT_PULLUP);
  pinMode( DOWN_PIN, INPUT_PULLUP);

  power( ON);

  randomSeed(analogRead(0));

  initServo( door, DOOR_PIN);
  initServo( finger, FINGER_PIN);

  /* Read configuration from EEPROM.
   * If it is invalid flash the LED 5 times and do a manual configuration.
   */
  readConfig( configuration);
  if ( !isConfigValid(configuration)) {
    flash( 5, 100);
    doConfig( configuration);
  }

  /* Configure end positions for door and finger moves.  
   */
  configServo( door,
               configuration.door_minPos,
               configuration.door_maxPos);

  configServo( finger,
               configuration.finger_minPos,
               configuration.finger_maxPos);

  lastMove = millis();
  nextClap = lastMove + QUIET_TIME_msec + random(IDLE_TIMEOUT_msec-QUIET_TIME_msec);
  powerOff = lastMove + IDLE_TIMEOUT_msec;

  /* Check for low battery and clap 5 times */
  if( voltageVdd() < POWER_LOW_V) {
    react( moveClap5);
  }
}

void loop() {
  
  /* If the config key is pressed force a manual configuration
   * and store it on EEPROM.
   */
  if ( keyPressed( CONFIG_PIN)) {

    doConfig( configuration);

    configServo( door,
                 configuration.door_minPos,
                 configuration.door_maxPos);

    configServo( finger,
                 configuration.finger_minPos,
                 configuration.finger_maxPos);
  }

#ifdef DEBUG_ON
  int vdd = voltageVdd();
  Serial.print("VDD ");
  Serial.println(vdd);
#endif

  if ( IS_SWITCH_ON) {

    byte rnd = (byte)random(255);

    byte sequenceCount = sizeof(moveSequences)/sizeof(byte*);
    rnd %= sequenceCount;

#ifdef DEBUG_ON
    Serial.print("cnt ");
    Serial.print(sequenceCount);
    Serial.print(" rnd ");
    Serial.println(rnd);
#endif

    react( moveSequences[rnd]);
    lastMove = millis();
    nextClap = lastMove + QUIET_TIME_msec + random(IDLE_TIMEOUT_msec-QUIET_TIME_msec);
    powerOff = lastMove + IDLE_TIMEOUT_msec;
    
  } else if( millis() > nextClap ){

    react( moveClap3);
    lastMove = millis();
    nextClap = lastMove + QUIET_TIME_msec + random(IDLE_TIMEOUT_msec-QUIET_TIME_msec);
    
  } else if( millis() > powerOff ) {

    power( OFF);
  }
}

/* React when the switch gets flipped to ON state 
 */
static void react( byte seq[]) {

  byte index = 0;
  byte command;

  /* immediateReact is set when the switch is flipped back to ON state
   * while the current react sequence is still processed.
   * In this case we want to immediately react with an aggressive sequence.
   */
  boolean immediateReact = false;

  while ( (command = seq[index++]) != END) {

    switch ( GET_CMD( command)) {
      case C_END:  // should never come here
        break;

      case C_WAIT: // wait for n * 100msec
        delay( 100 * GET_TIM( command));
        break;

      case C_DOOR: // move door
        immediateReact = moveServo( door, GET_POS( command), GET_SPD( command));
        break;

      case C_FINGER: // move finger
        immediateReact = moveServo( finger, GET_POS( command), GET_SPD( command));
        break;
    }

    if ( immediateReact) {
      index = 0;
      seq = moveAggressive;
    }
  }
}

/* Wait until the previous servo frame time has passed,
 * then set new servo timing value.
 */
static void writeServo( servostruct_t &s, int time_usec) {

  unsigned long now = millis();

  if( now < s.nextFrame_msec) {
    delay( s.nextFrame_msec - now);
  }
  
  s.servo.writeMicroseconds( time_usec);

  /* @TODO: This rolls over after about 50 days */
  s.nextFrame_msec = now + SERVO_FRAME_msec;
}

/* Move servo from current position to new position with a defined speed.
 */
static boolean moveServo( servostruct_t &s, byte newPos, byte howFast) {

  int endPos = s.minPos + ((s.maxPos - s.minPos) / 15) * newPos;
  int delta_usec = (endPos - s.curPos);
  int usecPerStep = (delta_usec < 0) ? -speedIndex[howFast] : speedIndex[howFast];
  int steps = delta_usec / usecPerStep;

  boolean switchWasOff = IS_SWITCH_OFF;

  for (int i = 0; i < steps; i++) {
    s.curPos += usecPerStep;
    writeServo( s, s.curPos);

    if ( switchWasOff && IS_SWITCH_ON) {
      return true;
    }
  }

  s.curPos = endPos;
  writeServo( s, s.curPos);

  return false;
}

static void initServo( servostruct_t &s, byte pin) {

  s.minPos = SERVO_MID_POS_usec;
  s.maxPos = SERVO_MID_POS_usec;
  s.curPos = SERVO_MID_POS_usec;
  s.nextFrame_msec = millis() + SERVO_FRAME_msec;

  s.servo.attach(pin);
}

static void configServo( servostruct_t &s, int minP, int maxP) {

  s.minPos = minP;
  s.maxPos = maxP;
  s.curPos = minP;

  writeServo( s, s.curPos);
}

/* Flash the LED a couple of times.
 */
static void flash( byte howOften, int howLong) {

  for ( byte i = 0; i < howOften; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(howLong);
    digitalWrite(LED_PIN, LOW);
    delay(howLong);
  }
}

static boolean keyPressed( byte pin) {

  if ( !digitalRead(pin)) {
    delay( 50);
    while ( !digitalRead(pin)) {
      delay( 10);
    }
    delay( 50);
    return true;
  }

  return false;
}

/* Determine switch state.
 * Wait until we see the same state 5 times in a row.
 * This function takes at least 5 milliseconds.
 */
static boolean isSwitchOn( byte pin) {

  boolean state = false;
  byte count = 0;

  while ( count < 5) {
    if ( !digitalRead(pin) == state) {
      delay( 1);
      count++;
    } else { // switch state
      state = !state;
      count = 0;
    }
  }

  return state;
}

/* Switch power relais on or off 
 */
static void power( boolean state) {

  digitalWrite( POWER_PIN, state);
}

/* Returns VDD Power-IN *100 to retain 2 digits.
 * 820 => 8.20 volt
 */
static int voltageVdd() {

  long v = ((long)analogRead( VOLTAGE_PIN));

  /* ADC value to voltage */
  v = v  * ADC_REF_V / ADC_RES;

  /* Compute original voltage bevore voltage divider */
  v = v * (V_DIVIDER_R1 + V_DIVIDER_R2) / V_DIVIDER_R2;

  return (int)v;
}

static void doConfig( config_t &cfg) {

  while ( !keyPressed( CONFIG_PIN)) {
    writeServo( door, SERVO_MID_POS_usec);
    writeServo( finger, SERVO_MID_POS_usec);
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

static int kbdMove( servostruct_t &s, int pos) {

  writeServo( s, pos);

  while ( !keyPressed( CONFIG_PIN)) {

    if ( !digitalRead( DOWN_PIN)) {
      pos -= 1;
      writeServo( s, pos);

    } else if ( !digitalRead( UP_PIN)) {
      pos += 1;
      writeServo( s, pos);
    }
  }

  return pos;
}

/* Read configuration from EEPROM.
 */
static void readConfig( config_t &cfg) {

  void *configStart;

  configStart = (void*)(EEPROM_START_LOC);
  eeprom_read_block( &cfg, configStart, sizeof(config_t));

#ifdef DEBUG_ON
  Serial.println("read");
  Serial.println(cfg.door_minPos);
  Serial.println(cfg.door_maxPos);
  Serial.println(cfg.finger_minPos);
  Serial.println(cfg.finger_maxPos);
  Serial.println(cfg.checksum);
#endif
}

/* Update the checksum and write configuration to EEPROM.
 */
static void writeConfig( config_t &cfg) {

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

  eeprom_update_block( &cfg, configStart, sizeof(config_t));
}

static checksum_t computeChecksum( config_t &cfg)
{
  checksum_t checksum = 0;
  byte *p = (byte*)(&cfg);

  for ( checksum_t i = 0; i < CONFIG_SIZE_BYTES; i++)
  {
    checksum = rotate(checksum);
    checksum ^= (i ^ ((byte) * (p + i)));
  }

  return checksum;
}

/* Left rotate unsigned int value.
 */
static checksum_t rotate( checksum_t v)
{
  return ((v >> 15) & 1) | (v << 1);
}

/* Return true if the configuration checksum is korrect.
 */
static boolean isConfigValid( config_t &cfg)
{
  return cfg.checksum == computeChecksum( cfg);
}
