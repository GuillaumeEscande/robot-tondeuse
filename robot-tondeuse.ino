
//Const

#define MOTOR_FRONT_LEFT_UP 2
#define MOTOR_FRONT_LEFT_DOWN 3

#define MOTOR_FRONT_RIGHT_UP 4
#define MOTOR_FRONT_RIGHT_DOWN 5

#define MOTOR_BACK_LEFT_UP 13
#define MOTOR_BACK_LEFT_DOWN A0

#define MOTOR_BACK_RIGHT_UP 8
#define MOTOR_BACK_RIGHT_DOWN 9
#define CUT_COMMAND A1

#define RECEPT_THROTTLE 10
#define RECEPT_ANGLE 11
#define RECEPT_CUT 12

void convert_command(int throttle, int angle, int & left_motor, int & right_motor){

    if ( throttle > -5 && throttle < 5 ) {
      throttle = 0;
    }

    float angle_rad = ((float)angle) * PI / 180.0;
    
    if ( angle < -5 ) {
      left_motor = cos(angle_rad) * throttle;
      right_motor = throttle;
      return;
    }
    if ( angle > 5 ) {
      left_motor = throttle;
      right_motor = cos(angle_rad) * throttle;
      return;
    }
    
    left_motor = throttle;
    right_motor = throttle;
}

void convert_command_simple(int throttle, int angle, int & left_motor, int & right_motor){
  left_motor = map(throttle, -100, 100, -255, 255);
  right_motor = left_motor;
}


int ch1; // Throttle
int ch2; // Angle
int ch3; // Cut machine (On/Off)

int throttle; // Forward/Back speed (%)
int angle; // Turning Factor (degree)
bool cut; // Cut param (bool)

void setup() {

  pinMode(RECEPT_THROTTLE, INPUT);
  pinMode(RECEPT_ANGLE, INPUT);
  pinMode(RECEPT_CUT, INPUT);

  Serial.begin(9600); // Pour a bowl of Serial (for debugging)

  pinMode(MOTOR_FRONT_LEFT_UP, OUTPUT);
  pinMode(MOTOR_FRONT_LEFT_DOWN, OUTPUT);
  pinMode(MOTOR_FRONT_RIGHT_UP, OUTPUT);
  pinMode(MOTOR_FRONT_RIGHT_DOWN, OUTPUT);
  pinMode(MOTOR_BACK_LEFT_UP, OUTPUT);
  pinMode(MOTOR_BACK_LEFT_DOWN, OUTPUT);
  pinMode(MOTOR_BACK_RIGHT_UP, OUTPUT);
  pinMode(MOTOR_BACK_RIGHT_DOWN, OUTPUT);
  pinMode(CUT_COMMAND, OUTPUT);

  analogWrite(MOTOR_FRONT_LEFT_UP, 0);
  analogWrite(MOTOR_FRONT_LEFT_DOWN, 0);
  analogWrite(MOTOR_FRONT_RIGHT_UP, 0);
  analogWrite(MOTOR_FRONT_RIGHT_DOWN, 0);
  analogWrite(MOTOR_BACK_LEFT_UP, 0);
  analogWrite(MOTOR_BACK_LEFT_DOWN, 0);
  analogWrite(MOTOR_BACK_RIGHT_UP, 0);
  analogWrite(MOTOR_BACK_RIGHT_DOWN, 0);
  digitalWrite(CUT_COMMAND, 0);
}

void loop() {

  ch1 = pulseIn(RECEPT_THROTTLE, HIGH, 25000); // Read the pulse width of
  ch2 = pulseIn(RECEPT_ANGLE, HIGH, 25000); // each channel
  ch3 = pulseIn(RECEPT_CUT, HIGH, 25000);


  throttle = map(ch1, 1000, 2000, -100, 100);
  angle = map(ch2, 1000, 2000, -180, 180);
  cut = LOW;
  
  if (ch3 > 1500)
    cut = HIGH;
/*
  Serial.print("INPUTS : ");
  Serial.print(" - throttle : ");
  Serial.print(throttle);
  Serial.print(" - angle : ");
  Serial.print(angle);
  Serial.print(" - cut : ");
  Serial.println(cut);
*/
  
  int left_motor = 0;
  int right_motor = 0;
  int left_motor_command = 0;
  int right_motor_command = 0;

  convert_command(throttle, angle, left_motor, right_motor);
  
  left_motor_command = map(abs(left_motor), 0, 100, 0, 255);
  right_motor_command = map(abs(right_motor), 0, 100, 0, 255);
  
  if (left_motor >= 0) {
    analogWrite(MOTOR_FRONT_LEFT_UP, left_motor_command);
    analogWrite(MOTOR_FRONT_LEFT_DOWN, 0);
    analogWrite(MOTOR_BACK_LEFT_UP, left_motor_command);
    analogWrite(MOTOR_BACK_LEFT_DOWN, 0);
  } else {
    analogWrite(MOTOR_FRONT_LEFT_UP, 0);
    analogWrite(MOTOR_FRONT_LEFT_DOWN, left_motor_command);
    analogWrite(MOTOR_BACK_LEFT_UP, 0);
    analogWrite(MOTOR_BACK_LEFT_DOWN, left_motor_command);
  }
  
  if (right_motor >= 0) {
    analogWrite(MOTOR_FRONT_RIGHT_UP, right_motor_command);
    analogWrite(MOTOR_FRONT_RIGHT_DOWN, 0);
    analogWrite(MOTOR_BACK_RIGHT_UP, right_motor_command);
    analogWrite(MOTOR_BACK_RIGHT_DOWN, 0);
  } else {
    analogWrite(MOTOR_FRONT_RIGHT_UP, 0);
    analogWrite(MOTOR_FRONT_RIGHT_DOWN, right_motor_command);
    analogWrite(MOTOR_BACK_RIGHT_UP, 0);
    analogWrite(MOTOR_BACK_RIGHT_DOWN, right_motor_command);
  }


  digitalWrite(CUT_COMMAND, cut);
  
  Serial.print("OUTPUT : ");
  Serial.print(" - throttle : ");
  Serial.print(throttle);
  Serial.print(" - angle : ");
  Serial.print(angle);
  Serial.print(" - left_motor : ");
  Serial.print(left_motor);
  Serial.print(" - right_motor : ");
  Serial.print(right_motor);
  Serial.print(" - left_motor_command : ");
  Serial.print(left_motor_command);
  Serial.print(" - right_motor_command : ");
  Serial.print(right_motor_command);
  Serial.print(" - cut : ");
  Serial.println(cut);

  
  delay(100);

}
