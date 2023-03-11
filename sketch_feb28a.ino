//4, 5, 6, 7
#define pin4 4
#define pin5 5
#define pin6 6
#define pin7 7

const int IRpin = A0;          // аналоговый пин для подключения выхода Vo сенсора
int value1;                    // для хранения аналогового значения
#define model 1080

int distance_l;
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;
int constl1 = 20;
int constf = 25;
int constl = 30;
int status = 1;

void setup() {
  Serial.begin(9600);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}

void loop() {
  fdist();
  ldist();
  delay(100);  
  update();
  do_action();
  delay(250); 
  
}

void fdist(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
}

void ldist(){
  float volts = analogRead(IRpin)*0.0048828125;
  distance_l =32*pow(volts,-1.10);    
}

void stop(){
  analogWrite(pin6, 0);
  analogWrite(pin5, 0);
}
void forward(){
  digitalWrite(pin7, HIGH);
  analogWrite(pin6, 250);
  digitalWrite(pin4, HIGH);
  analogWrite(pin5, 250);
}
void right(){
  analogWrite(pin6, 250);
  digitalWrite(pin7, HIGH);
  analogWrite(pin5, 250);
  digitalWrite(pin4, LOW);
}
void left(){ 
  analogWrite(pin6, 250);
  digitalWrite(pin7, LOW);
  analogWrite(pin5, 250);
  digitalWrite(pin4, HIGH); 
}

void update(){
  if(status == 0){
    if(distance_l > constl){
        status = 3;
      }
    else if(distance > constf){
      if(distance_l < constl){
        status = 1;
      }
    }
    else if(distance < constf){
      if(distance_l < constl){
        status = 2;
      }
    } 
  }
  else if(status == 1){
    if(distance < constf || distance_l > constl){
      status = 0;
    }
  }
  else if(status == 2){
    if(distance > constf){
      if(distance_l < constl){//?
        status = 0;
      }
    }
  }
  else if(status == 3){//?
    if(distance_l < constl1){
      status = 0;
    }
  }
}

void do_action(){
  if(status == 0){
    stop();
  }
  if(status == 1){
    forward();
  }
  if(status == 2){
    right();
  }
  if(status == 3){
    left();
  }
}