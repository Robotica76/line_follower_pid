// 6 optic sensor bar
int s1 = A0;
int s2 = A1;
int s3 = A2;
int s4 = A3;
int s5 = A4;
int s6 = A5;


int s1_v = 0;  // variable to store the value read
int s2_v = 0;  // variable to store the value read
int s3_v = 0;  // variable to store the value read
int s4_v = 0;  // variable to store the value read
int s5_v = 0;  // variable to store the value read
int s6_v = 0;  // variable to store the value read

// to store the values in a integer value

int threshold = 400;
int velocidad = 125;

double error = 0;

// parameters and variables for non linear PID
const int vmin=80;
const int vmax=125;
const float kp=.05;
const float ki=0.0003;
const float kd=0.3;
const float kv=0.07;
int p,d,u,vbase;
long i=0;
int p_old=0;


void setup() {
  Serial.begin(9600);           //  setup serial

  pinMode(5,OUTPUT); // D5 --- PWMA
  pinMode(9,OUTPUT); // D9 --- AIN1
  pinMode(4,OUTPUT); // D4 -- AIN2

  pinMode(6,OUTPUT); // D6 --- PWMB
  pinMode(7,OUTPUT); // D7 --- BIN1
  pinMode(8,OUTPUT); // D8 --- BIN2

}

void loop() {
  // Pin organization from the back
  s1_v = analogRead(s1);  // read the input pin -- right
  s2_v = analogRead(s2);  // read the input pin
  s3_v = analogRead(s3);  // read the input pin
  s4_v = analogRead(s4);  // read the input pin
  s5_v = analogRead(s5);  // read the input pin
  s6_v = analogRead(s6);  // read the input pin -- left

  p = -3*s6_v -2*s5_v -1*s4_v + s3_v + 2*s2_v + 3*s1_v;

  i=i+p;
  d=p-p_old;
  p_old=p;
  if ((p*i)<0) i=0;  // integral windup

  u=kp*p+ki*i+kd*d;
  vbase=vmin+(vmax-vmin)*exp(-kv*abs(kp*p));
  drive(vbase+u,vbase-u);

  Serial.println(p);
}


void drive(int L, int R) // speed for wheels Left and Right, positive is forward
{
  L=constrain(L,-255,255); // avoid PWM overflow
  R=constrain(R,-255,255);
  

  digitalWrite(4, L<0); // switch < and >= if left wheel doesnt spin as expected
  digitalWrite(9, L>=0);
  analogWrite(5, abs(L));
  
  digitalWrite(8, R<0); // switch < and >= if left wheel doesnt spin as expected
  digitalWrite(7, R>=0);
  analogWrite(6, abs(R));
}
