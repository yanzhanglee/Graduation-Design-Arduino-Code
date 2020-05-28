#include <Bounce2.h> //按键消抖


//输出的数字口：HIGH触发，LOW关闭
#define LED_Right  10  //面对按键，左侧的LED灯（5VDC）
#define LED_Mid  11  //面对按键，中间的LED灯（5VDC）
#define LED_Left  12  //面对按键，右侧的LED灯（5VDC）
#define BAZZ_Bii  5   //蜂鸣器，喇叭（5VDC）
#define FAN_Fast  8   //风扇，高速（12VDC）
#define FAN_Slow  9   //风扇低速（5VDC）
#define NEG_IonG  6   //负离子发生器（220VAC）
#define UV_Light  7   //紫外线灯管（220VAC）

//输入的数字口：LOW为被触发，HIGH为未触发，已做上拉电阻和硬件消抖
//按钮和倒地开关可以做为外部中断触发，建议使用状态机编程。
#define Switch  2   //按钮
#define FALL_Det  3   //倒地检测
#define MAGN_Det  4   //干簧管，磁性开关

//模拟输入输出预留口：直接接在IO上，
#define INS_Left  A4  //面对按键，左侧的接口
#define INS_Left  A5  //面对按键，右侧的接口

int flagState = 0;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;


Bounce debouncer = Bounce();
int buttonState;
unsigned long buttonPressTimeStamp;

void setup() 
{
  
  Serial.begin(57600);
  Serial.println("Hello World");
  
  pinMode(LED_Left,OUTPUT);
  pinMode(LED_Mid,OUTPUT);
  pinMode(LED_Right,OUTPUT);
  pinMode(BAZZ_Bii,OUTPUT);
  pinMode(FAN_Fast,OUTPUT);
  pinMode(FAN_Slow,OUTPUT);
  pinMode(NEG_IonG,OUTPUT);
  pinMode(UV_Light,OUTPUT);

  pinMode(FALL_Det,INPUT);
  pinMode(MAGN_Det,INPUT);
  pinMode(Switch,INPUT);
  
  shutDown();
  digitalWrite(BAZZ_Bii,LOW);

  debouncer.attach(Switch,INPUT_PULLUP);
  debouncer.interval(25);
  
  Serial.println("Finish Setup");
}

void loop(){
  boolean changed = debouncer.update();
  switch(flagState){
    
    case 0:
      Serial.println("Here is S1_0");
      shutDown();
      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 1;
          previousMillis1 = millis();
          Serial.println("Set state to S1_1");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == true){
        flagState = 4;
        previousMillis1 = millis();
        Serial.println("Set state to S2_0");
        break;
      }
      break;
      
    case 1://蜂鸣器短鸣，LED_Left亮，紫外线持续运行，风力12V和负离子运行20分钟，风力5V运行40分钟。
      Serial.println("Here is S1_1");

      if(millis() - previousMillis1 <=300){
        digitalWrite(BAZZ_Bii,HIGH);
      }else{
        digitalWrite(BAZZ_Bii,LOW);
      }
      shutDown();
      digitalWrite(LED_Left,HIGH);
      digitalWrite(LED_Mid,LOW);
      digitalWrite(LED_Right,LOW);
      digitalWrite(UV_Light,HIGH);

      if(millis() - previousMillis1 <= 200000){
        digitalWrite(NEG_IonG,HIGH);
        digitalWrite(FAN_Fast,HIGH);
      }else{
        digitalWrite(NEG_IonG,LOW);
        digitalWrite(FAN_Fast,LOW);
        digitalWrite(FAN_Slow,HIGH);
      }

      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          flagState = 2;
          previousMillis1 = millis();
          Serial.println("Set state to S1_2");
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 0;
          previousMillis1 = millis();
          Serial.println("Set state to S1_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == true){
        flagState = 4;
        previousMillis1 = millis();
        Serial.println("Set state to S2_0");
        break;
      }
      break;
      
     case 2://蜂鸣器短鸣，LED_Mid亮，紫外线持续运行，风力5V持续运行，负离子运行40分钟，停止20分钟。
      Serial.println("Here is S1_2");

      if(millis() - previousMillis1 <=300){
        digitalWrite(BAZZ_Bii,HIGH);
      }else{
        digitalWrite(BAZZ_Bii,LOW);
      }
      shutDown();
      digitalWrite(LED_Left,LOW);
      digitalWrite(LED_Mid,HIGH);
      digitalWrite(LED_Right,LOW);
      digitalWrite(UV_Light,HIGH);

      if(millis() - previousMillis1 <= 200000){
        digitalWrite(NEG_IonG,HIGH);
        digitalWrite(FAN_Slow,HIGH);
      }else{
        digitalWrite(NEG_IonG,LOW);        
      }
     
      
      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          flagState = 3;
          previousMillis1 = millis();
          Serial.println("Set state to S1_3");
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 0;
          previousMillis1 = millis();
          Serial.println("Set state to S1_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == true){
        flagState = 4;
        previousMillis1 = millis();
        Serial.println("Set state to S2_0");
        break;
      }
      break;
      
     case 3://蜂鸣器短鸣，LED_Right亮，紫外线持续运行，风力12V持续运行，负离子持续运行，运行60分钟切换至s1_1。
      Serial.println("Here is S1_3");

      if(millis() - previousMillis1 <=300){
        digitalWrite(BAZZ_Bii,HIGH);
      }else{
        digitalWrite(BAZZ_Bii,LOW);
      }
      
      shutDown();
      digitalWrite(LED_Left,LOW);
      digitalWrite(LED_Mid,LOW);
      digitalWrite(LED_Right,HIGH);
      digitalWrite(UV_Light,HIGH);

      if(millis() - previousMillis1 <= 300000){
        digitalWrite(NEG_IonG,HIGH);
        digitalWrite(FAN_Fast,HIGH);
      }else{
        flagState = 1;
        previousMillis1 = millis();
        break;
      }
      
      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          flagState = 1;
          previousMillis1 = millis();
          Serial.println("Set state to S1_1");
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 0;
          previousMillis1 = millis();
          Serial.println("Set state to S1_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == true){
        flagState = 4;
        previousMillis1 = millis();
        Serial.println("Set state to S2_0");
        break;
      }
      break;
      
     case 4:
      Serial.println("Here is S2_0");
      if(millis() - previousMillis1 <=1000){
        digitalWrite(BAZZ_Bii,HIGH);
      }else{
        digitalWrite(BAZZ_Bii,LOW);
      }

      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          flagState = 5;
          previousMillis1 = millis();
          Serial.println("Set state to S2_1");
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      
      if(digitalRead(MAGN_Det) == false){
        flagState = 0;
        previousMillis1 = millis();
        Serial.println("Set state to S1_0");
        break;
      }

      break;

      case 5:
      Serial.println("Here is S2_1");

      if(millis() - previousMillis1 <=300){
        digitalWrite(BAZZ_Bii,HIGH);
      }else{
        digitalWrite(BAZZ_Bii,LOW);
      }
      
      if(millis() - previousMillis1 <= 3000){
        LED_Left_Blink(500);
      }
      else{
        digitalWrite(LED_Left,HIGH);
        digitalWrite(LED_Mid,LOW);
        digitalWrite(LED_Right,LOW);
        flagState = 8;
        previousMillis1 = millis();
        Serial.println("Set state to S3_1");
        break;
      }

      if( changed ){
        digitalWrite(LED_Left,LOW);
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          flagState = 6;
          previousMillis1 = millis();
          Serial.println("Set state to S2_2");
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 4;
          previousMillis1 = millis();
          Serial.println("Set state to S2_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == false){
        flagState = 0;
        previousMillis1 = millis();
        Serial.println("Set state to S1_0");
        break;
      }

      break;

     case 6:
      Serial.println("Here is S2_2");

      if(millis() - previousMillis1 <=300){
        digitalWrite(BAZZ_Bii,HIGH);
      }else{
        digitalWrite(BAZZ_Bii,LOW);
      }
      if(millis() - previousMillis1 <= 3000){
        LED_Mid_Blink(500);
      }
      else{
        digitalWrite(LED_Mid,HIGH);
        digitalWrite(LED_Left,LOW);
        digitalWrite(LED_Right,LOW);
        flagState = 9;
        previousMillis1 = millis();
        Serial.println("Set state to S3_2");
        break;
      }

      if( changed ){
        digitalWrite(LED_Mid,LOW);
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          flagState = 7;
          previousMillis1 = millis();
          Serial.println("Set state to S2_3");
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 4;
          previousMillis1 = millis();
          Serial.println("Set state to S2_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == false){
        flagState = 0;
        previousMillis1 = millis();
        Serial.println("Set state to S1_0");
        break;
      }

      break;
      
     case 7:
      Serial.println("Here is S2_3");

      if(millis() - previousMillis1 <=300){
        digitalWrite(BAZZ_Bii,HIGH);
      }else{
        digitalWrite(BAZZ_Bii,LOW);
      }
      if(millis() - previousMillis1 <= 3000){
        LED_Right_Blink(500);
      }
      else{
        digitalWrite(LED_Right,HIGH);
        digitalWrite(LED_Mid,LOW);
        digitalWrite(LED_Left,LOW);
        flagState = 10;
        previousMillis1 = millis();
        Serial.println("Set state to S3_3");
        break;
      }

      if( changed ){
        digitalWrite(LED_Right,LOW);
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          flagState = 5;
          previousMillis1 = millis();
          Serial.println("Set state to S2_1");
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 4;
          previousMillis1 = millis();
          Serial.println("Set state to S2_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == false){
        flagState = 0;
        previousMillis1 = millis();
        Serial.println("Set state to S1_0");
        break;
      }

      break;

     case 8://蜂鸣器倒计时30s，紫外线运行30分钟，切换至s2_0
      Serial.println("Here is S3_1");
      if(millis() - previousMillis1 <=10000){
        Bazz_Blink(250);
      }
      else if(millis() - previousMillis1 <=20000 && millis() - previousMillis1>10000){
        Bazz_Blink(200);
      }
      else if(millis() - previousMillis1 <=30000 && millis() - previousMillis1>20000){
        Bazz_Blink(150);
      }
      else{
        digitalWrite(BAZZ_Bii,LOW);
        digitalWrite(UV_Light,HIGH);
      }

      if(millis() - previousMillis1 >= 1800000){
        digitalWrite(UV_Light,LOW);
      }

      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 4;
          previousMillis1 = millis();
          Serial.println("Set state to S2_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == false){
        flagState = 0;
        previousMillis1 = millis();
        Serial.println("Set state to S1_0");
        break;
      }

      break;

     case 9://蜂鸣器倒计时30s，紫外线运行60分钟，切换至s2_0
      Serial.println("Here is S3_2");
      if(millis() - previousMillis1 <=10000){
        Bazz_Blink(250);
      }
      else if(millis() - previousMillis1 <=20000 && millis() - previousMillis1>10000){
        Bazz_Blink(200);
      }
      else if(millis() - previousMillis1 <=30000 && millis() - previousMillis1>20000){
        Bazz_Blink(150);
      }
      else{
        digitalWrite(BAZZ_Bii,LOW);
        digitalWrite(UV_Light,HIGH);
      }

      if(millis() - previousMillis1 >= 3600000){
        digitalWrite(UV_Light,LOW);
      }

      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 4;
          previousMillis1 = millis();
          Serial.println("Set state to S2_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == false){
        flagState = 0;
        previousMillis1 = millis();
        Serial.println("Set state to S1_0");
        break;
      }

      break;

     case 10://蜂鸣器倒计时30s，紫外线运行90分钟，切换至s2_0
      Serial.println("Here is S3_3");
      if(millis() - previousMillis1 <=10000){
        Bazz_Blink(250);
      }
      else if(millis() - previousMillis1 <=20000 && millis() - previousMillis1>10000){
        Bazz_Blink(200);
      }
      else if(millis() - previousMillis1 <=30000 && millis() - previousMillis1>20000){
        Bazz_Blink(150);
      }
      else{
        digitalWrite(BAZZ_Bii,LOW);
        digitalWrite(UV_Light,HIGH);
      }

      if(millis() - previousMillis1 >= 5400000){
        digitalWrite(UV_Light,LOW);
      }

      if( changed ){
        int value = debouncer.read();
        if( value == HIGH){
          Serial.println("Button released (state 0)");
          buttonState = 0;
          break;
        }
        else{
          buttonState = 1;
          Serial.println("Button pressed (state 1)");
          buttonPressTimeStamp = millis();
        }   
      }
      if (buttonState == 1){
        if (millis() - buttonPressTimeStamp >= 1000){
          buttonPressTimeStamp = millis();
          flagState = 4;
          previousMillis1 = millis();
          Serial.println("Set state to S2_0");
          break;
        }
      }
      if(digitalRead(MAGN_Det) == false){
        flagState = 0;
        previousMillis1 = millis();
        Serial.println("Set state to S1_0");
        break;
      }

      break;
  }
}

void LED_Left_Blink (unsigned int time_ms)
{
  static unsigned long int n = 0;
  static int stateLed = LOW;
  if (millis()-n>=time_ms)
  {
     stateLed=!stateLed;
     digitalWrite(LED_Left, stateLed);
     n=millis();
  }
}

void LED_Mid_Blink (unsigned int time_ms)
{
  static unsigned long int n = 0;
  static int stateLed = LOW;
  if (millis()-n>=time_ms)
  {
     stateLed=!stateLed;
     digitalWrite(LED_Mid, stateLed);
     n=millis();
  }
}

void LED_Right_Blink (unsigned int time_ms)
{
  static unsigned long int n = 0;
  static int stateLed = LOW;
  if (millis()-n>=time_ms)
  {
     stateLed=!stateLed;
     digitalWrite(LED_Right, stateLed);
     n=millis();
  }
}

void Bazz_Blink (unsigned int time_ms){
  Serial.println("Bazz_Blink");
  Serial.println(time_ms);
  static unsigned long int n=0;
  static int stateBazz = HIGH;
  if(millis() - n >= time_ms){
    stateBazz = !stateBazz;
    digitalWrite(BAZZ_Bii,stateBazz);
    n = millis();
  }
}

void shutDown(){
  //Serial.println("shutDown");
  digitalWrite(LED_Left,LOW);
  digitalWrite(LED_Mid,LOW);
  digitalWrite(LED_Right,LOW);
  //digitalWrite(BAZZ_Bii,LOW);

  digitalWrite(FAN_Fast,LOW);
  digitalWrite(FAN_Slow,LOW);
  digitalWrite(NEG_IonG,LOW);
  digitalWrite(UV_Light,LOW);
}
