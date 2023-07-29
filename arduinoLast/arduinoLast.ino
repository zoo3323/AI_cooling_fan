#include <Servo.h>
#include <TM1637TinyDisplay.h>
#include "DHT.h"
#include <Adafruit_MLX90614.h>

#define LED_RED 5
#define LED_GREEN 6
#define LED_BLUE  11
#define SERVO_PIN 8
#define PIEZO_BUZZER  3
#define CLK 9
#define DIO 10
#define BUTTON_1 4
#define BUTTON_2 7
#define VR_PIN  A0
#define BRIGHT_PIN  A1
#define DHTPIN 2
#define DC_PIN 12

Servo myservo;
TM1637TinyDisplay display(CLK, DIO);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  myservo.attach(SERVO_PIN);
  display.setBrightness(BRIGHT_7);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  mlx.begin();
  dht.begin();
}
int dc = 0;
int DCSpeed = 0;
void loop() {
  if (dc == 1)
  {
    if (DCSpeed == 0)
    {
      digitalWrite(DC_PIN,0);
      dc = 0;
    }
    else if (DCSpeed == 1)
    {
      digitalWrite(DC_PIN,1); //ledPin핀을 5V로 출력합니다.
      delayMicroseconds(50);
      digitalWrite(DC_PIN,0);  //ledPin핀을 0V로 출력합니다. 
      delayMicroseconds(50);
    }
    else if (DCSpeed == 2)
    {
      digitalWrite(DC_PIN,1); //ledPin핀을 5V로 출력합니다.
      delayMicroseconds(100);
      digitalWrite(DC_PIN,0);  //ledPin핀을 0V로 출력합니다. 
      delayMicroseconds(50);
    }
    else if (DCSpeed == 3)
    {
      digitalWrite(DC_PIN,1); //ledPin핀을 5V로 출력합니다.
      delayMicroseconds(170);
      digitalWrite(DC_PIN,0);  //ledPin핀을 0V로 출력합니다. 
      delayMicroseconds(50);
    }
    else if (DCSpeed == 4)
    {
      digitalWrite(DC_PIN,1);
    }
  }
  if (Serial.available() > 0)
  {
    String strRead = Serial.readStringUntil('\n');
    if (strRead.indexOf("RGB=") != -1)
    {
      int commaIndex1 = strRead.indexOf(",");
      int commaIndex2 = strRead.indexOf(",", commaIndex1 + 1);

      int redValue = strRead.substring(4, commaIndex1).toInt();
      int greenValue = strRead.substring(commaIndex1 + 1, commaIndex2).toInt();
      int blueValue = strRead.substring(commaIndex2 + 1, strRead.length()).toInt();

      redLedSet(redValue, greenValue, blueValue);
      Serial.println("OKRGB");
    }
    else if (strRead.indexOf("DC=") != -1)
    {
      DCSpeed = strRead.substring(3, strRead.length()).toInt();
      dc = 1;
      Serial.println("OKDC");
    }
    else if (strRead.indexOf("SERVO=") != -1)
    {
      int servoDigree = strRead.substring(6, strRead.length()).toInt();
      if (servoDigree <= 180)
      {
        myservo.write(servoDigree);
        Serial.println("OKSERVO");
      }
      else Serial.println("error digree");
    }
    else if (strRead.indexOf("BUZZER=") != -1)
    {
      float bzFreq = strRead.substring(7, strRead.length()).toFloat();
      setBuzzer(bzFreq);
    }
    else if (strRead.indexOf("FND=") != -1)
    {
      float fndValue = strRead.substring(4, strRead.length()).toFloat();
      display.showNumber(fndValue);
      Serial.println("OKFND");
    }
    else if (strRead.indexOf("VR=?") != -1)
    {
      Serial.print("VR=");
      Serial.println(analogRead(VR_PIN));
    }
    else if (strRead.indexOf("BRIGHT=?") != -1)
    {
      Serial.print("BRIGHT=");
      Serial.println(analogRead(BRIGHT_PIN));
    }
    else if (strRead.indexOf("TEMPERATURE=?") != -1)
    {
      sendTemperature();
    }
    else if (strRead.indexOf("HUMIDITY=?") != -1)
    {
      sendHumidity();
    }
    else if (strRead.indexOf("OBJECT=?") != -1)
    {
      float objectTemp = mlx.readObjectTempC();
      Serial.print("OBJECT=");
      Serial.println(objectTemp);
    }
    else if (strRead.indexOf("AMBIENT=?") != -1)
    {
      float ambientTemp = mlx.readAmbientTempC();
      Serial.print("AMBIENT=");
      Serial.println(ambientTemp);
    }
  }

  if (btn1() == 1) Serial.println("BUTTON1=CLICK");
  if (btn2() == 1) Serial.println("BUTTON2=CLICK");
  
}

void redLedSet(int red, int green, int blue)
{
  analogWrite(LED_RED, red);
  analogWrite(LED_GREEN, green);
  analogWrite(LED_BLUE, blue);
}

void setBuzzer(int freq)
{
  if (freq > 31)
  {
    tone(PIEZO_BUZZER, freq);
    Serial.println("OKBUZZER");
  }
  else
  {
    noTone(PIEZO_BUZZER);
    Serial.println("OKBUZZER");
  }
}

int btn1()
{
  static int currBtn = 0;
  static int prevBtn = 0;

  currBtn = digitalRead(BUTTON_1);

  if (currBtn != prevBtn)
  {
    prevBtn = currBtn;
    if (currBtn == 1)
    {
      return 1;
    }
    delay(50);
  }

  return 0;
}

int btn2()
{
  static int currBtn = 0;
  static int prevBtn = 0;

  currBtn = digitalRead(BUTTON_2);

  if (currBtn != prevBtn)
  {
    prevBtn = currBtn;
    if (currBtn == 1)
    {
      return 1;
    }
    delay(50);
  }

  return 0;
}

void sendTemperature()
{
  float temperature = dht.readTemperature();
  if (!isnan(temperature))
  {
    Serial.print("TEMPERATURE=");
    Serial.println(temperature);
  }
  else
  {
    Serial.print("TEMPERATURE=");
    Serial.println(0);
  }
}

void sendHumidity()
{
  float humidity = dht.readHumidity();
  if (!isnan(humidity))
  {
    Serial.print("HUMIDITY=");
    Serial.println(humidity);
  }
  else
  {
    Serial.print("HUMIDITY=");
    Serial.println(0);
  }
}
