#include <AFMotor.h>
#include <ezButton.h>

#define VRX_PIN A0  // Arduino pin connected to VRX pin
#define VRY_PIN A1  // Arduino pin connected to VRY pin
#define SW_PIN 16

AF_DCMotor motor_1(1);
AF_DCMotor motor_2(2);
AF_DCMotor motor_3(3);
AF_DCMotor motor_4(4);

ezButton button(SW_PIN);

int xValue = 0;  // To store value of the X axis
int yValue = 0;  // To store value of the Y axis
int bValue = 0;  // To store value of the button
int deadzone = 150;
int levelControl = 1;  // which motors to controll if (1) M1 and M2, if (-1) M3 and M4

int xNeutral;
int yNeutral;

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50);
  levelControl = 1;

  xNeutral = analogRead(VRX_PIN);
  yNeutral = analogRead(VRY_PIN);
  Serial.print(xNeutral);
  Serial.print(yNeutral);
}

int c = 0;

void loop() {
  button.loop();  // MUST call the loop() function first

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  bValue = button.getState();

  if (button.isPressed()) {
    Serial.println("INFO: Level control changed");
  }
  if (button.isReleased()) {
    levelControl = -levelControl;
    stopAllMotors();
  }

  if (levelControl == 1) {
    // Serial.println("Control M1 - Y and M2 - X");
    moveMotorsIfNeeded(xValue, yValue, motor_1, motor_2);
  } else {
    // Serial.println("Control M3 - Y and M4 - X");
    moveMotorsIfNeeded(xValue, yValue, motor_3, motor_4);
  }
}

void moveMotorsIfNeeded(int xValue, int yValue, AF_DCMotor mLR, AF_DCMotor mUD) {
  int mUDSpeed = 0;
  int mLRSpeed = 0;

  if (xValue >= xNeutral + deadzone) {
    // Serial.println("LEFT");
    mLRSpeed = map(xValue, xNeutral, 1023, 0, 255);
    mLR.setSpeed(mLRSpeed);
    mLR.run(FORWARD);
  } else if (xValue <= xNeutral - deadzone) {
    // Serial.println("RIGHT");
    mLRSpeed = map(xValue, xNeutral, 0, 0, 255);
    mLR.setSpeed(mLRSpeed);
    mLR.run(BACKWARD);
  } else {
    // Serial.println("stop");
    mLR.run(RELEASE);
    mLR.setSpeed(0);
  }

  if (yValue >= yNeutral + deadzone) {
    // Serial.println("DOWN");
    mUDSpeed = map(yValue, yNeutral, 1023, 0, 255);
    mUD.setSpeed(mUDSpeed);
    mUD.run(BACKWARD);
  } else if (yValue <= yNeutral - deadzone) {
    // Serial.println("UP");
    mUDSpeed = map(yValue, yNeutral, 0, 0, 255);
    mUD.setSpeed(mUDSpeed);
    mUD.run(FORWARD);
  } else {
    // Serial.println("stop");
    mUD.setSpeed(0);
    mUD.run(RELEASE);
  }
}

void moveMotor(String side, AF_DCMotor m) {
  Serial.println(side);
}

void stopAllMotors() {
  motor_1.run(RELEASE);
  motor_2.run(RELEASE);
  motor_3.run(RELEASE);
  motor_4.run(RELEASE);
}