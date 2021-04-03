int TrgPin = 4;
int EcoPin = A2;
float dist;
void setup()
{   
Serial.begin(9600);
//设置TrgPin为输出状态
pinMode(TrgPin, OUTPUT);
// 设置EcoPin为输入状态
pinMode(EcoPin, INPUT);
}
void loop()
{
digitalWrite(TrgPin, LOW);
delayMicroseconds(8);
digitalWrite(TrgPin, HIGH);
// 维持10毫秒高电平用来产生一个脉冲
delayMicroseconds(10);
digitalWrite(TrgPin, LOW);
// 读取脉冲的宽度并换算成距离
dist = pulseIn(EcoPin, HIGH) / 58.00;
Serial.print("Distance:");
Serial.print(dist);
Serial.println("cm");
delay(300);
}
