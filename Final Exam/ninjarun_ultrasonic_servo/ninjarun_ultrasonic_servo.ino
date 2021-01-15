#include <LiquidCrystal.h>
#include <Servo.h>
Servo myservo;

#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP 3
#define SPRITE_JUMP_UPPER 4
#define SPRITE_JUMP_LOWER 5
#define SPRITE_HOUSE_EMPTY ' '      // User the ' ' character
#define SPRITE_HOUSE_SOLID 6
#define SPRITE_HOUSE_SOLID_RIGHT 7
#define SPRITE_HOUSE_SOLID_LEFT 8

#define NINJA_HORIZONTAL_POSITION 1    // Horizontal position of ninja on screen

#define HOUSE_WIDTH 16
#define HOUSE_EMPTY 0
#define HOUSE_LOWER_BLOCK 1
#define HOUSE_UPPER_BLOCK 2

#define NINJA_POSITION_OFF 0          // Ninja is invisible
#define NINJA_POSITION_RUN_LOWER_1 1  // Ninja is running on lower row (pose 1)
#define NINJA_POSITION_RUN_LOWER_2 2  //                              (pose 2)

#define NINJA_POSITION_JUMP_1 3       // Starting a jump
#define NINJA_POSITION_JUMP_2 4       // Half-way up
#define NINJA_POSITION_JUMP_3 5       // Jump is on upper row
#define NINJA_POSITION_JUMP_4 6       // Jump is on upper row
#define NINJA_POSITION_JUMP_5 7       // Jump is on upper row
#define NINJA_POSITION_JUMP_6 8       // Jump is on upper row
#define NINJA_POSITION_JUMP_7 9       // Half-way down
#define NINJA_POSITION_JUMP_8 10      // About to land

#define NINJA_POSITION_RUN_UPPER_1 11 // Ninja is running on upper row (pose 1)
#define NINJA_POSITION_RUN_UPPER_2 12 //                              (pose 2)

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int trigPin = 2; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 3;

static char houseUpper[HOUSE_WIDTH + 1];
static char houseLower[HOUSE_WIDTH + 1];
static bool pushbuttonPushed = false;
static int melodyPin = 3;

void initializeGraphics() {
  static byte graphics[] = {
    // Run position 1
    B10000,
    B01111,
    B00111,
    B00111,
    B11110,
    B01100,
    B11110,
    B00010,
    // Run position 2
    B10000,
    B01111,
    B00111,
    B00111,
    B11110,
    B01100,
    B01100,
    B01000,
    // Jump
    B10000,
    B01111,
    B00111,
    B00111,
    B11110,
    B01101,
    B11111,
    B10000,
    // Jump upper
    B00000,
    B00000,
    B00000,
    B00000,
    B10000,
    B01111,
    B00111,
    B00111,
    // Jump lower
    B11110,
    B01101,
    B11111,
    B10000,
    B00000,
    B00000,
    B00000,
    B00000,
    // Ground
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    // Ground right
    B00001,
    B00011,
    B00111,
    B01111,
    B00011,
    B00011,
    B00011,
    B00011,
    // Ground left
    B10000,
    B11000,
    B11100,
    B11110,
    B11000,
    B11000,
    B11000,
    B11000,
  };
  int i;
  // Skip using character 0, this allows lcd.print() to be used to
  // quickly draw multiple characters
  for (i = 0; i < 8; ++i) {
    lcd.createChar(i + 1, &graphics[i * 8]);
  }
  for (i = 0; i < HOUSE_WIDTH; ++i) {
    houseUpper[i] = SPRITE_HOUSE_EMPTY;
    houseLower[i] = SPRITE_HOUSE_EMPTY;
  }
}

// Slide the house to the left in half-character increments
//
void advanceHouse(char* house, byte newHouse) {
  for (int i = 0; i < HOUSE_WIDTH; ++i) {
    char current = house[i];
    char next = (i == HOUSE_WIDTH - 1) ? newHouse : house[i + 1];
    switch (current) {
      case SPRITE_HOUSE_EMPTY:
        house[i] = (next == SPRITE_HOUSE_SOLID) ? SPRITE_HOUSE_SOLID_RIGHT : SPRITE_HOUSE_EMPTY;
        break;
      case SPRITE_HOUSE_SOLID:
        house[i] = (next == SPRITE_HOUSE_EMPTY) ? SPRITE_HOUSE_SOLID_LEFT : SPRITE_HOUSE_SOLID;
        break;
      case SPRITE_HOUSE_SOLID_RIGHT:
        house[i] = SPRITE_HOUSE_SOLID;
        break;
      case SPRITE_HOUSE_SOLID_LEFT:
        house[i] = SPRITE_HOUSE_EMPTY;
        break;
    }
  }
}

bool drawNinja(byte position, char* houseUpper, char* houseLower, unsigned int score) {
  bool collide = false;
  char upperSave = houseUpper[NINJA_HORIZONTAL_POSITION];
  char lowerSave = houseLower[NINJA_HORIZONTAL_POSITION];
  byte upper, lower;
  switch (position) {
    case NINJA_POSITION_OFF:
      upper = lower = SPRITE_HOUSE_EMPTY;
      break;
    case NINJA_POSITION_RUN_LOWER_1:
      upper = SPRITE_HOUSE_EMPTY;
      lower = SPRITE_RUN1;
      break;
    case NINJA_POSITION_RUN_LOWER_2:
      upper = SPRITE_HOUSE_EMPTY;
      lower = SPRITE_RUN2;
      break;
    case NINJA_POSITION_JUMP_1:
    case NINJA_POSITION_JUMP_8:
      upper = SPRITE_HOUSE_EMPTY;
      lower = SPRITE_JUMP;
      break;
    case NINJA_POSITION_JUMP_2:
    case NINJA_POSITION_JUMP_7:
      upper = SPRITE_JUMP_UPPER;
      lower = SPRITE_JUMP_LOWER;
      break;
    case NINJA_POSITION_JUMP_3:
    case NINJA_POSITION_JUMP_4:
    case NINJA_POSITION_JUMP_5:
    case NINJA_POSITION_JUMP_6:
      upper = SPRITE_JUMP;
      lower = SPRITE_HOUSE_EMPTY;
      break;
    case NINJA_POSITION_RUN_UPPER_1:
      upper = SPRITE_RUN1;
      lower = SPRITE_HOUSE_EMPTY;
      break;
    case NINJA_POSITION_RUN_UPPER_2:
      upper = SPRITE_RUN2;
      lower = SPRITE_HOUSE_EMPTY;
      break;
  }
  if (upper != ' ') {
    houseUpper[NINJA_HORIZONTAL_POSITION] = upper;
    collide = (upperSave == SPRITE_HOUSE_EMPTY) ? false : true;
  }
  if (lower != ' ') {
    houseLower[NINJA_HORIZONTAL_POSITION] = lower;
    collide |= (lowerSave == SPRITE_HOUSE_EMPTY) ? false : true;
  }

  byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;

  // Draw the scene
  houseUpper[HOUSE_WIDTH] = '\0';
  houseLower[HOUSE_WIDTH] = '\0';
  char temp = houseUpper[16 - digits];
  houseUpper[16 - digits] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(houseUpper);
  houseUpper[16 - digits] = temp;
  lcd.setCursor(0, 1);
  lcd.print(houseLower);

  lcd.setCursor(16 - digits, 0);
  lcd.print(score);

  houseUpper[NINJA_HORIZONTAL_POSITION] = upperSave;
  houseLower[NINJA_HORIZONTAL_POSITION] = lowerSave;
  return collide;
}

// Handle the pushbutton push as an interrupt
void pushbuttonPush() {
  pushbuttonPushed = true;
}

void setup() {
  myservo.attach(12);
  initializeGraphics();
  lcd.begin(16, 2);
}

void loop() {
  pushbuttonCheck();
  static byte ninjaPos = NINJA_POSITION_RUN_LOWER_1;
  static byte newHouseType = HOUSE_EMPTY;
  static byte newHouseDuration = 1;
  static bool playing = false;
  static bool blink = false;
  static unsigned int distance = 0;

  if (!playing) {
    drawNinja((blink) ? NINJA_POSITION_OFF : ninjaPos, houseUpper, houseLower, distance >> 3);
    if (blink) {
      lcd.setCursor(0, 0);
      lcd.print("Ninja Run");
      myservo.write(90);
    }
    delay(250);
    blink = !blink;
    if (pushbuttonPushed) {
      initializeGraphics();
      ninjaPos = NINJA_POSITION_RUN_LOWER_1;
      playing = true;
      pushbuttonPushed = false;
      distance = 0;
    }
    return;
  }
  myservo.write(180);
  // Shift the house to the left
  advanceHouse(houseLower, newHouseType == HOUSE_LOWER_BLOCK ? SPRITE_HOUSE_SOLID : SPRITE_HOUSE_EMPTY);
  advanceHouse(houseUpper, newHouseType == HOUSE_UPPER_BLOCK ? SPRITE_HOUSE_SOLID : SPRITE_HOUSE_EMPTY);

  // Make new house to enter on the right
  if (--newHouseDuration == 0) {
    if (newHouseType == HOUSE_EMPTY) {
      newHouseType = (random(3) == 0) ? HOUSE_UPPER_BLOCK : HOUSE_LOWER_BLOCK;
      newHouseDuration = 2 + random(10);
    } else {
      newHouseType = HOUSE_EMPTY;
      newHouseDuration = 10 + random(10);
    }
  }

  if (pushbuttonPushed) {
    if (ninjaPos <= NINJA_POSITION_RUN_LOWER_2) ninjaPos = NINJA_POSITION_JUMP_1;
    pushbuttonPushed = false;
  }

  if (drawNinja(ninjaPos, houseUpper, houseLower, distance >> 3)) {
    playing = false; // The ninja collided with something. Too bad.
    myservo.write(0);
    delay(5000);
  } else {
    if (ninjaPos == NINJA_POSITION_RUN_LOWER_2 || ninjaPos == NINJA_POSITION_JUMP_8) {
      ninjaPos = NINJA_POSITION_RUN_LOWER_1;
    } else if ((ninjaPos >= NINJA_POSITION_JUMP_3 && ninjaPos <= NINJA_POSITION_JUMP_5) && houseLower[NINJA_HORIZONTAL_POSITION] != SPRITE_HOUSE_EMPTY) {
      ninjaPos = NINJA_POSITION_RUN_UPPER_1;
    } else if (ninjaPos >= NINJA_POSITION_RUN_UPPER_1 && houseLower[NINJA_HORIZONTAL_POSITION] == SPRITE_HOUSE_EMPTY) {
      ninjaPos = NINJA_POSITION_JUMP_5;
    } else if (ninjaPos == NINJA_POSITION_RUN_UPPER_2) {
      ninjaPos = NINJA_POSITION_RUN_UPPER_1;
    } else {
      ++ninjaPos;
    }
    ++distance;
  }
  delay(100);
}

//Get input from push button OR ultrasonic sensor
void pushbuttonCheck() {
  long duration, cm;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/29) / 2;
  delay(100);
  int b = analogRead(A0);
  if (b < 850 || cm <= 10) {
    pushbuttonPushed = true;
  }
}
