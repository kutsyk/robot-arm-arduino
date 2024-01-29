#include <AFMotor.h>

#define AILE_PIN 14
#define ELEV_PIN 15
#define THRO_PIN 16
#define RUDD_PIN 17
#define DEADZONE 150

AF_DCMotor motor_1(1);
AF_DCMotor motor_2(2);
AF_DCMotor motor_3(3);
AF_DCMotor motor_4(4);

int aile_neutral;
int elev_neutral;
int thro_neutral;
int rudd_neutral;

int aile_val;
int elev_val;
int thro_val;
int rudd_val;

enum ControlType {
  AILE,
  ELEV,
  THRO,
  RUDD,
};

class MotorOperator {
public:
  AF_DCMotor motor = NULL;
  ControlType controlType = AILE;
  int neutral; 
  int maxControlValue;
  int minControlValue;

public:
  void init(ControlType control_type, int neut, int minV, int maxV, AF_DCMotor m);
  void updateMotorState(int controlValue);
  void stop();
};

void MotorOperator::init(ControlType control_type, int neut, int minV, int maxV, AF_DCMotor m) {
  controlType = control_type;
  neutral = neut;
  maxControlValue = maxV;
  minControlValue = minV;
  motor = m;
}

void MotorOperator::updateMotorState(int controlValue) {
  int motorSpeed = 0;

  if (controlValue >= neutral + DEADZONE) {
    motorSpeed = map(controlValue, neutral, maxControlValue, 0, 255);
    motor.setSpeed(motorSpeed);
    motor.run(FORWARD);
  } else if (controlValue <= neutral - DEADZONE) {
    motorSpeed = map(controlValue, neutral, minControlValue, 0, 255);
    motor.setSpeed(motorSpeed);
    motor.run(BACKWARD);
  } else {
    motor.run(RELEASE);
    motor.setSpeed(0);
  }
} 

void MotorOperator::stop() {
  motor.run(RELEASE);
}

MotorOperator M_1;
MotorOperator M_2;
MotorOperator M_3;
MotorOperator M_4;

void setup() {
  Serial.begin(9600);

  pinMode(AILE_PIN, INPUT);
  pinMode(ELEV_PIN, INPUT);
  pinMode(THRO_PIN, INPUT);
  pinMode(RUDD_PIN, INPUT);

  aile_neutral = pulseIn(AILE_PIN, HIGH);
  elev_neutral = pulseIn(ELEV_PIN, HIGH);
  thro_neutral = pulseIn(THRO_PIN, HIGH);
  rudd_neutral = pulseIn(RUDD_PIN, HIGH);

  M_1.init(AILE, aile_neutral, 1000, 1910, motor_1);
  M_2.init(ELEV, elev_neutral, 1000, 1910, motor_2);
  M_3.init(THRO, thro_neutral, 1000, 1910, motor_3);
  M_4.init(RUDD, rudd_neutral, 1000, 1910, motor_4);
  stopAllMotors();
}

void loop() {
  aile_val = pulseIn(AILE_PIN, HIGH);
  elev_val = pulseIn(ELEV_PIN, HIGH);
  thro_val = pulseIn(THRO_PIN, HIGH);
  rudd_val = pulseIn(RUDD_PIN, HIGH);

  M_1.updateMotorState(aile_val);
  M_2.updateMotorState(elev_val);
  M_3.updateMotorState(thro_val);
  M_4.updateMotorState(rudd_val);
}

void stopAllMotors() {
  motor_1.run(RELEASE);
  motor_2.run(RELEASE);
  motor_3.run(RELEASE);
  motor_4.run(RELEASE);
}
