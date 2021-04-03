#include <Servo.h>

#define FOOT_PIN A0
#define BODY_PIN A1
#define ARM_PIN A4
#define HAND_PIN A5
#define LEFT1_GO_PIN 9
#define LEFT2_GO_PIN 10
#define RIGHT1_GO_PIN 5
#define RIGHT2_GO_PIN 6
#define LEFT_GREY_PIN A10
#define RIGHT_GREY_PIN A11
#define S0 8
#define S1 12
#define S2 7
#define S3 3
#define sensorOut 2
#define TrgPin 4
#define EcoPin A2

#define FOOT_MIN 400
#define FOOT_MAX 1900
#define BODY_MIN 950
#define BODY_MAX 1800
#define ARM_MIN 100
#define ARM_MAX 2600
#define HAND_MIN 500
#define HAND_MAX 1500

#define FOOT_DEFAULT 1200
#define BODY_DEFAULT 1200
#define ARM_DEFAULT 1150
#define HAND_DEFAULT 1500
#define GREY_LIMIT 500
#define CAT_LIMIT 23
#define COLOR_LIMIT 7
Servo foot;
Servo body;
Servo arm;
Servo hand;

int footPwm;
int bodyPwm;
int armPwm;
int handPwm;

int R;
int G;
int B;

char val;
float dist;

void doDefault(){
    footPwm = FOOT_DEFAULT;
    bodyPwm = BODY_DEFAULT;
    armPwm = ARM_DEFAULT;
    handPwm = HAND_DEFAULT;
    foot.writeMicroseconds(FOOT_DEFAULT);
    body.writeMicroseconds(BODY_DEFAULT);
    arm.writeMicroseconds(ARM_DEFAULT);
    hand.writeMicroseconds(HAND_DEFAULT);
}

void setup()
{
    doDefault();
    pinMode(LEFT1_GO_PIN, OUTPUT);
    pinMode(LEFT2_GO_PIN, OUTPUT);
    pinMode(RIGHT1_GO_PIN, OUTPUT);
    pinMode(RIGHT2_GO_PIN, OUTPUT);
    pinMode(S0,OUTPUT);
    pinMode(S1,OUTPUT);
    pinMode(S2,OUTPUT);
    pinMode(S3,OUTPUT);
    pinMode(sensorOut,INPUT);
    pinMode(TrgPin, OUTPUT);
    pinMode(EcoPin, INPUT);
    pinMode(RIGHT_GREY_PIN, INPUT); 
    pinMode(LEFT_GREY_PIN, INPUT); 
    digitalWrite(S0,HIGH);
    digitalWrite(S1,LOW);

    foot.attach(FOOT_PIN);
    body.attach(BODY_PIN);
    arm.attach(ARM_PIN);
    hand.attach(HAND_PIN);

   
    

    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);
    Serial.println("start up");
    Serial1.println("start up1");
    Serial2.println("start up2");
    digitalWrite(TrgPin, HIGH);
}

void doServo(Servo servo, int max, int min, int *pwm, int way)
{
    while ((*pwm <= max && *pwm >= min) || (*pwm > max && way < 0) || (*pwm < min && way > 0))
    {
        if (Serial.available() > 0)
        {
            if (Serial.read() == 's')
            {
                break;
            }
        }
        *pwm += way;
        servo.writeMicroseconds(*pwm);
        delay(10);
    }
}

bool getColor(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  R = pulseIn(sensorOut,LOW);
  delay(100);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  G = pulseIn(sensorOut,LOW);
  delay(100);
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  B = pulseIn(sensorOut,LOW);
  delay(100);
  return R>70||B>70||G>70;
}

void autogo()
{
    doDefault();
    while (Serial.read() != 's')
    {
        digitalWrite(TrgPin, LOW);
        delayMicroseconds(3);
        digitalWrite(TrgPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(TrgPin, LOW);
        dist = pulseIn(EcoPin, HIGH) / 58.00;
        if(dist<=COLOR_LIMIT)
        {
            bool color=getColor();
            while(dist<=CAT_LIMIT)
            {
              digitalWrite(LEFT1_GO_PIN, HIGH);
              digitalWrite(LEFT2_GO_PIN, LOW);
              digitalWrite(RIGHT1_GO_PIN, HIGH);
              digitalWrite(RIGHT2_GO_PIN, LOW);
              delay(50);
              digitalWrite(LEFT1_GO_PIN, LOW);
              digitalWrite(LEFT2_GO_PIN, LOW);
              digitalWrite(RIGHT1_GO_PIN, LOW);
              digitalWrite(RIGHT2_GO_PIN, LOW);
              digitalWrite(TrgPin, LOW);
              delay(1000);
              delayMicroseconds(3);
              digitalWrite(TrgPin, HIGH);
              delayMicroseconds(10);
              digitalWrite(TrgPin, LOW);
              dist = pulseIn(EcoPin, HIGH) / 58.00;
            }
            digitalWrite(LEFT1_GO_PIN, LOW);
            digitalWrite(LEFT2_GO_PIN, LOW);
            digitalWrite(RIGHT1_GO_PIN, LOW);
            digitalWrite(RIGHT2_GO_PIN, LOW);
            armPwm=ARM_MAX;
            arm.writeMicroseconds(armPwm);
            delay(1000);
            bodyPwm=BODY_MIN;
            body.writeMicroseconds(bodyPwm);
            delay(1000);
            handPwm=1200;
            hand.writeMicroseconds(handPwm);
            delay(1000);
            if(color){
              bodyPwm=BODY_MAX;
              body.writeMicroseconds(bodyPwm);
              delay(1000);
            }else{
              footPwm=FOOT_MAX;
              foot.writeMicroseconds(footPwm);
              delay(1000);
              bodyPwm=BODY_MIN;
              body.writeMicroseconds(bodyPwm);
              delay(1000);
            }
            handPwm=HAND_MAX;
            hand.writeMicroseconds(handPwm);
            delay(1000);
            doDefault();
            
        }
        else if (analogRead(LEFT_GREY_PIN) > GREY_LIMIT && analogRead(RIGHT_GREY_PIN) > GREY_LIMIT) //前进
        {
            digitalWrite(LEFT1_GO_PIN, LOW);
            digitalWrite(LEFT2_GO_PIN, HIGH);
            digitalWrite(RIGHT1_GO_PIN, LOW);
            digitalWrite(RIGHT2_GO_PIN, HIGH);
        }
        else if (analogRead(LEFT_GREY_PIN) < GREY_LIMIT && analogRead(RIGHT_GREY_PIN) > GREY_LIMIT) //右
        {
            digitalWrite(LEFT1_GO_PIN, HIGH);
            digitalWrite(LEFT2_GO_PIN, LOW);
            digitalWrite(RIGHT1_GO_PIN, LOW);
            digitalWrite(RIGHT2_GO_PIN, HIGH);
        }
        else if (analogRead(LEFT_GREY_PIN) > GREY_LIMIT && analogRead(RIGHT_GREY_PIN) < GREY_LIMIT) //左
        {
            digitalWrite(LEFT1_GO_PIN, LOW);
            digitalWrite(LEFT2_GO_PIN, HIGH);
            digitalWrite(RIGHT1_GO_PIN, HIGH);
            digitalWrite(RIGHT2_GO_PIN, LOW);
        }

        delay(10);
    }
    digitalWrite(LEFT1_GO_PIN, LOW);
    digitalWrite(LEFT2_GO_PIN, LOW);
    digitalWrite(RIGHT1_GO_PIN, LOW);
    digitalWrite(RIGHT2_GO_PIN, LOW);
}

void loop()
{
    if (Serial.available() > 0)
    {
        switch (Serial.read())
        {
        case 'a':
            doServo(foot, FOOT_MAX, FOOT_MIN, &footPwm, 5);
            break;
        case 'b':
            doServo(foot, FOOT_MAX, FOOT_MIN, &footPwm, -5);
            break;
        case 'c':
            doServo(body, BODY_MAX, BODY_MIN, &bodyPwm, 5);
            break;
        case 'd':
            doServo(body,BODY_MAX,BODY_MIN,&bodyPwm,-5);
            break;
        case 'e':
            doServo(arm, ARM_MAX, ARM_MIN, &armPwm, 5);
            break;
        case 'f':
            doServo(arm, ARM_MAX, ARM_MIN, &armPwm, -5);
            break;
        case 'j':
            doServo(hand, HAND_MAX, HAND_MIN, &handPwm, 7);
            break;
        case 'k':
            doServo(hand, HAND_MAX, HAND_MIN, &handPwm, -7);
            break;
        case 'l': //停
            digitalWrite(LEFT1_GO_PIN, LOW);
            digitalWrite(LEFT2_GO_PIN, LOW);
            digitalWrite(RIGHT1_GO_PIN, LOW);
            digitalWrite(RIGHT2_GO_PIN, LOW);
            break;
        case 'm': //前
            digitalWrite(LEFT1_GO_PIN, LOW);
            digitalWrite(LEFT2_GO_PIN, HIGH);
            digitalWrite(RIGHT1_GO_PIN, LOW);
            digitalWrite(RIGHT2_GO_PIN, HIGH);
            break;
        case 'n'://you
            digitalWrite(LEFT1_GO_PIN, HIGH);
            digitalWrite(LEFT2_GO_PIN, LOW);
            digitalWrite(RIGHT1_GO_PIN, LOW);
            digitalWrite(RIGHT2_GO_PIN, HIGH);
            break;
        case 'o': //后
            digitalWrite(LEFT1_GO_PIN, HIGH);
            digitalWrite(LEFT2_GO_PIN, LOW);
            digitalWrite(RIGHT1_GO_PIN, HIGH);
            digitalWrite(RIGHT2_GO_PIN, LOW);
            break;
        case 'p'://zuo
            digitalWrite(LEFT1_GO_PIN, LOW);
            digitalWrite(LEFT2_GO_PIN, HIGH);
            digitalWrite(RIGHT1_GO_PIN, HIGH);
            digitalWrite(RIGHT2_GO_PIN, LOW);
            break;
        case 'q':
            autogo();
            break;
        }   
    }
    delay(10);
}
