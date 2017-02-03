////http://api.openweathermap.org/data/2.5/weather?q=stockholm,SE&units=metric&appid = <app-id here>
////"particle webhook create webhook.json " Create the webhook
#include <math.h>
int led = D7; // This one is the built-in tiny one to the right of the USB jack

//ABCDEFG,dp is the order of the bits
const int numeral[10]= {
B01100000, //1
B11011010, //2
B11110010, //3
B01100110, //4
B10110110, //5
B00111110, //6
B11100000, //7
B11111110, //8
B11100110, //9
};
const int scode[10]= {
B11111100, //0
B01100000, //1
B11011010, //2
B11110010, //3
B01100110, //4
B10110110, //5
B00111110, //6
B11100000, //7
B11111110, //8
B11100110, //9
};

void displayWeather(bool minus, int temp, int status) {
  Serial.println(minus);
  Serial.println(temp);
  Serial.println(status);
}

void processValues(const char *temperature, const char *id) {
  Serial.println(temperature);
  Serial.println(id);
  //double ftemperature = strtod(temperature,NULL);
  int intemperature =(int)(roundf(strtod(temperature,NULL)));
  int intid = atoi(id);
  int status=0;  //unknown U
  if((int)(intid/100)==2)
  {
    status=1; //thunders t
  }else if (intid==800)
  {
    status=2;  //Clear Sky G G for good
  }else if (intid==801|| intid==802)
  {
    status=3;  //Light Clouds c
  }else if (intid==804|| intid==803)
  {
    status=4;   //Clouds C <Caps>
  }else if ((int)(intid/100)==3)
  {
    status=5;   //Drizzle d
  }else if ((int)(intid/100)==5)
  {
    status=6;   //Rain r
  }else if ((int)(intid/100)==6)
  {
    status=7;   //Snow s
  }else if ((int)(intid/100)==7)
  {
    status=8;   //Fog F
  }else if (intid>=900 && intid <910)
  {
    status=9;   //Extreme E
  }

  displayWeather(intemperature<0, intemperature<0?intemperature*-1:intemperature, status);
}


void processWeather(const char *event, const char *data) {
Serial.println("Handling Weather: ");
// Handle the webhook response
//Particle.publish("Handling weather",data);
int stringPos = strlen(data);
//Serial.println(stringPos);
char w_temp[7] = {""};
char w_id[4] = {""};
int itemCounter = 0;
int tempStringLoc=0;
memset(&w_temp,0,7);
memset(&w_id,0,4);
for (int i=0; i<stringPos; i++){
    if(data[i]=='~'){
          itemCounter++;
          tempStringLoc=0;
    }else
    {
        switch(itemCounter){
        case 0:
          w_id[tempStringLoc++] = data[i];
          break;
        case 1:
          w_temp[tempStringLoc++] = data[i];
          break;
        }
    }
}
processValues(w_temp,w_id);
digitalWrite(led, LOW);

}
void setup() {
pinMode(led, OUTPUT);
Serial.begin(9600);
Serial.print("Started: ");
// Subscribe to the webhook response event
Particle.subscribe("hook-response/weather_hook", processWeather , MY_DEVICES);
}


void loop() {
// Get some data
String data = String(10);
digitalWrite(led, HIGH);
// Trigger the webhook
Particle.publish("weather_hook", data, PRIVATE);
// Wait 60 seconds
delay(30000);
}