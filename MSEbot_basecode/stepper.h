
 
Servo topServo;  // create servo object to control a servo
Servo bottomServo;
// 16 servo objects can be created on the ESP32
 
int topPos = 0;    // variable to store the servo position
int bottomPos = 180;
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoTop = 15;
int servoBottom = 2;

int LeftState = 0;
int RightState = 0;

const int LimitSwitchLeft = 16;
const int LimitSwitchRight = 27;
 
void setupStepper() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(6);
  ESP32PWM::allocateTimer(7);
  ESP32PWM::allocateTimer(8);
  ESP32PWM::allocateTimer(9);
  
  topServo.setPeriodHertz(50);    // standard 50 hz servo
  topServo.attach(servoTop, 500, 2400); // attaches the servo on pin 18 to the servo object

  bottomServo.setPeriodHertz(50);
  bottomServo.attach(servoBottom, 500, 2400);
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep

  pinMode(LimitSwitchLeft, INPUT_PULLUP);
  pinMode(LimitSwitchRight, INPUT_PULLUP);
}


void incrementStepper() {
  LeftState = digitalRead(LimitSwitchLeft);
  RightState = digitalRead(LimitSwitchRight);
  

  if(LeftState == LOW && RightState == HIGH) {
    // left motor stops, right keeps going
  } else if (LeftState == HIGH && RightState == LOW) {
    // right motor stops, left keeps going
  } else if ((LeftState == LOW && RightState == LOW) && (topPos <= 180 && bottomPos >= 0) ){

        topPos += 1;
        bottomPos -= 1;
        topServo.write(topPos);
        bottomServo.write(bottomPos);

      ENC_btLeftMotorRunningFlag = false;
      ENC_btRightMotorRunningFlag = false;
      digitalWrite(ciMotorLeftA,HIGH);
      digitalWrite(ciMotorLeftB,HIGH);
      digitalWrite(ciMotorRightA,HIGH);
      digitalWrite(ciMotorRightB,HIGH);
      ledcWrite(2,255);
      ledcWrite(1,255);  //stop with braking Left motor 
      ledcWrite(3,255);
      ledcWrite(4,255);  //stop with braking Right motor 

      ucNextMotorStateIndex = 2;
     
  }

  if((topPos >= 180 && bottomPos <= 0)){
    ucNextMotorStateIndex = 0;
    SmartLEDs.setPixelColor(0,15,15,0);
  }
}



 long DegreesToDutyCycle(int deg) 
 {
  const long minDutyCycle = 1850;            // duty cycle for 0 degrees
  const long maxDutyCycle = 8050;            // duty cycle for 180 degrees

  long dutyCycle = map(deg, 0, 180, minDutyCycle, maxDutyCycle);  // convert to duty cycle

  return dutyCycle;
 }



 
