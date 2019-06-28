

// CH4_Temperature_Humidity_Fan_Controller_System v0.78 Beta

 
#include <DHT.h>


// Pin configuration
#define DHTPIN 2
#define DHTTYPE DHT11
#define buzzer_pin 3
#define relay_1_1_pin 4      // Minimum value of the fan speed
#define relay_1_2_pin 5     // Medium value of the fan speed
//#define relay_2_pin 6   // Maximum value of the fan speed
#define MQ4_pin A0

// MQ4 constants
#define preheat_time 10000
#define MQ4_threshold_0 0
#define MQ4_threshold_1 100
#define MQ4_threshold_2 250
#define MQ4_threshold_3 400

// DHT11 constants
#define HUM_threshold_0 0
#define HUM_threshold_1 60
#define HUM_threshold_2 65
#define HUM_threshold_3 70

#define TEMP_threshold_0 0
#define TEMP_threshold_1 43
#define TEMP_threshold_2 55
#define TEMP_threshold_3 70

int hum_fan_speed_level = 0;
int temp_fan_speed_level = 0;
int MQ4_fan_speed_level = 0;
int fan_speed_level = 0;
int alarm_level = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
pinMode(buzzer_pin, OUTPUT);

pinMode(relay_1_1_pin, OUTPUT);
pinMode(relay_1_2_pin, OUTPUT);
//pinMode(relay_3_pin, OUTPUT);

digitalWrite(relay_1_1_pin, HIGH); // Relay initial is off
digitalWrite(relay_1_2_pin, HIGH);
//digitalWrite(relay_3_pin, HIGH);


Serial.begin(9600);
dht.begin();
delay(preheat_time);
buzzer (100,50,2);
}

void loop()
{
temperature_check();
humidity_check();
MQ4_CH4_check();
int hum_temp_max = max(temp_fan_speed_level, hum_fan_speed_level);
fan_speed_level = max(hum_temp_max, MQ4_fan_speed_level);
//Serial.print("hum_fan_speed_level ");
//Serial.println(hum_fan_speed_level);
//Serial.print("MQ4_fan_speed_level ");
//Serial.println(MQ4_fan_speed_level);
fan_speed_relay();
alarm();
Serial.println(fan_speed_level);

}

void humidity_check()
{
  //int read_hum = DHT.read(DHTPIN);
  float hum = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(hum);
  
  if (hum >= HUM_threshold_0 && hum < HUM_threshold_1)
  {
    hum_fan_speed_level = 0;
    //delay(1000);
    return;
  }
  else if (hum >= HUM_threshold_1 && hum < HUM_threshold_2)
  {
    hum_fan_speed_level = 1;
    //delay(1000);
    return;   
  }
  else if (hum >= HUM_threshold_2 && hum < HUM_threshold_3)
  {
    hum_fan_speed_level = 2;
    //delay(1000);
    return;   
  }  
    else if (hum >= HUM_threshold_3 && hum < 100.00)
  {
    hum_fan_speed_level = 3;
    //delay(1000);
    return;   
  }  
}

void temperature_check()
{
  float temp = dht.readTemperature();
  Serial.print("Temperature :");
  Serial.println(temp);
  
  if (temp >= TEMP_threshold_0 && temp < TEMP_threshold_1)
  {
    temp_fan_speed_level = 0;
    //delay(1000);
    return;
  }
  else if (temp >= TEMP_threshold_1 && temp < TEMP_threshold_2)
  {
    temp_fan_speed_level = 1;
    //delay(1000);
    return;   
  }
  else if (temp >= TEMP_threshold_2 && temp < TEMP_threshold_3)
  {
    temp_fan_speed_level = 2;
    //delay(1000);
    return;   
  }  
    else if (temp >= TEMP_threshold_3 && temp < 100.00)
  {
    temp_fan_speed_level = 3;
    //delay(1000);
    return;   
  }   
}



void MQ4_CH4_check()
{
int MQ4_value = analogRead(MQ4_pin);
//Serial.print("MQ4: ");
//Serial.println(MQ4_value);
  if (MQ4_value >= MQ4_threshold_0 && MQ4_value < MQ4_threshold_1)
  {
    MQ4_fan_speed_level = 0;
    alarm_level = 0;
    return;
  }
  else if (MQ4_value >= MQ4_threshold_1 && MQ4_value < MQ4_threshold_2)
  {
    MQ4_fan_speed_level = 1;
    alarm_level = 1;
    return;   
  }
  else if (MQ4_value >= MQ4_threshold_2 && MQ4_value < MQ4_threshold_3)
  {
    MQ4_fan_speed_level = 2;
    alarm_level = 2;
    return;
  }
  else if (MQ4_value >= MQ4_threshold_3 && MQ4_value < 1024)
  {
    MQ4_fan_speed_level = 3;
    alarm_level = 3;
    return;   
  } 

}

void buzzer (int runtime, int delaytime, int repeat_count)
{
   for (int i = 1; i <= repeat_count; i++)
    {
    digitalWrite(buzzer_pin, HIGH);
    delay(runtime);
    digitalWrite(buzzer_pin, LOW);
    delay(delaytime);
    }
}

void alarm()
{
  if (alarm_level ==0)
  {return;}
  else if (alarm_level==1)
  {
    buzzer(850,150,1);
    return;
  }
  else if (alarm_level==2)
  {
    buzzer(240,10,4);
    return;
  }
  else if (alarm_level==3)
  {
    buzzer(120,5,8);
    buzzer(1000,0,1);
    return;
  }  
  
}

void fan_speed_relay ()
{


if (fan_speed_level == 0)
  {
   digitalWrite(relay_1_1_pin, HIGH); // OFF
   digitalWrite(relay_1_2_pin, HIGH); // OFF
   //digitalWrite(relay_3_pin, HIGH); // OFF   
   return;
  }
  
else if (fan_speed_level == 1)  
 {
  digitalWrite(relay_1_1_pin, LOW);   // ON
  digitalWrite(relay_1_2_pin, LOW); // ON
  //digitalWrite(relay_3_pin, HIGH); // OFF 
  return;
 }
else if (fan_speed_level == 2)
 {
  digitalWrite(relay_1_1_pin, LOW);  // ON
  digitalWrite(relay_1_2_pin, LOW); // ON
  //digitalWrite(relay_3_pin, HIGH); // OFF 
  return;
 }
 
else if (fan_speed_level == 3)
 {
  digitalWrite(relay_1_1_pin, LOW);  // ON
  digitalWrite(relay_1_2_pin, LOW); // ON
  //digitalWrite(relay_3_pin, HIGH); // OFF 
  return;
 }

}
