#include <AFMotor.h>
#include <ezButton.h>

#define VRX_PIN A0  // Arduino pin connected to VRX pin
#define VRY_PIN A1  // Arduino pin connected to VRY pin
#define SW_PIN 16

AF_DCMotor motor_1(1);
// AF_DCMotor motor_2(2);
// AF_DCMotor motor_3(3);
// AF_DCMotor motor_4(4);

ezButton button(SW_PIN);

int xValue = 0;  // To store value of the X axis
int yValue = 0;  // To store value of the Y axis
int bValue = 0;  // To store value of the button
int deadzone = 150;
int levelCеуontrol = 1; // which motors to controll if (1) M1 and M2, if (-1) M3 and M4

int xNeutral;
int yNeutral;
int xCenter;
int yCenter;

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(50);
  levelControl = 1;

  xNeutral = analogRead(VRX_PIN);
  yNeutral = analogRead(VRY_PIN);
  xCenter = xNeutral + deadzone;
  yCenter = yNeutral + deadzone;
  Serial.print(xNeutral);
  Serial.print(yNeutral);
  // motor_1.setSpeed(150);
  // motor_2.setSpeed(150);
  // motor_3.setSpeed(150);
  // motor_4.setSpeed(150);
}

int c = 0;

void loop() {
  button.loop();  // MUST call the loop() function first

  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  bValue = button.getState();

  if (button.isPressed()) {
    // Serial.println("INFO: Level control changed");
  }
  if (button.isReleased()) {
    // Serial.println("Control M3 - Y and M4 - X");
    levelControl = -levelControl;
  }

  if (levelControl == 1) {
    // Serial.println("Control M1 - Y and M2 - X");
  } else {
    // Serial.println("Control M3 - Y and M4 - X");
  }

  // Serial.println(xNeutral);
  // Serial.println(yNeutral);
  if (xValue >= xNeutral + deadzone) {
    moveMotor("LEFT", MLR);
  } else if (xValue <= xNeutral - deadzone) {
    moveMotor("RIGHT", MLR);
  } else {
    moveMotor("center", MLR);
  }

  if (yValue >= yNeutral + deadzone) {
    moveMotor("DOWN", mUD);
  } else if (yValue <= yValue - deadzone) {
    moveMotor("UP", mUD);
  } else {
    moveMotor("center", mUD);
  }

  // print data to Serial Monitor on Arduino IDE
  // // if (c++ % 100 == 0) {
  // Serial.print("x = ");
  // Serial.println(xValue);
  // Serial.print(", y = ");
  // Serial.println(yValue);
  //   Serial.print(" : button = ");
  //   Serial.println(bValue);
  // }
}

void moveMotorsIfNeeded(xValue, AF_DCMotor mLR, AF_DCMotor mUD) {
  if (xValue >= xNeutral + deadzone) {
    moveMotor("LEFT", MLR);
  } else if (xValue <= xNeutral - deadzone) {
    moveMotor("RIGHT", mLR);
  } else {
    moveMotor("center", mLR);
  }

  if (yValue >= yNeutral + deadzone) {
    moveMotor("DOWN", mUD);
  } else if (yValue <= yValue - deadzone) {
    moveMotor("UP", mUD);
  } else {
    moveMotor("center", mUD);
  }
}

void moveMotor(String side, AF_DCMotor m) {
  Serial.println(side);
}

// void moveArmUp() {
//   // Activate motors to move the arm upward
//   motor_1.run(FORWARD);
//   motor_2.run(FORWARD);
//   motor_3.run(FORWARD);
//   motor_4.run(FORWARD);
// }

// void moveArmDown() {
//   // Activate motors to move the arm downward
//   motor_1.run(BACKWARD);
//   motor_2.run(BACKWARD);
//   motor_3.run(BACKWARD);
//   motor_4.run(BACKWARD);
// }

// void stopArm() {
//   // Stop all motors
//   motor_1.run(RELEASE);
//   motor_2.run(RELEASE);
//   motor_3.run(RELEASE);
//   motor_4.run(RELEASE);
// }