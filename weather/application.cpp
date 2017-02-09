////http://api.openweathermap.org/data/2.5/weather?q=stockholm,SE&units=metric&appid=<app-id here>
////"particle webhook create webhook.json " Create the webhook
#include "application.h"
#include <math.h>
int led = D7; // This one is the built-in tiny one to the right of the USB jack

//pins for decimal point and each segment
//A, B, C, D, E, F, G, dp
const int numberofSegments=8;

const int segmentPins[]= { A0, D4, D2, D1, D0, A1, D3, A2}; 

const int numberofDigits=4;

const int digitPins[numberofDigits] = { A3, A4, D5, D6}; //digits 1, 2, 3, 4

int outputSegmentValues[] = {0b00000010, 0b00000010, 0b00000010, 0b00000010}; // Initial values for Digits

// delay in milliseconds between Weatheer requests 
const long int intervalAfterFirstUpdate = 10 * 60 * 1000;   // Set interval here 

long int interval = 20 * 1000;      // Actual value changes after first update

//ABCDEFG,dp
const int numeral[10]= {
    0b11111100, //0
    0b01100000, //1
    0b11011010, //2
    0b11110010, //3
    0b01100110, //4
    0b10110110, //5
    0b10111110, //6
    0b11100000, //7
    0b11111110, //8
    0b11110110, //9
};
const int statusCode[10]= {
    0b01111100, //U
    0b00011110, //t
    0b10111100, //G
    0b00011010, //c
    0b10011100, //C
    0b01111010, //d
    0b00001010, //r
    0b10110110, //S
    0b10001110, //F
    0b10011110, //E
};

void selectDigit(int segment) {
    for (int i = 0; i < numberofSegments; i++) {
        if (segment == i) {
            digitalWrite(digitPins[i], HIGH);
        }
        else {
            digitalWrite(digitPins[i], LOW);
        }
    }
}

void clearDigit(int segment) {
    int segmentCounter = numberofSegments;
    while (segmentCounter) {
        digitalWrite(segmentPins[--segmentCounter],HIGH);
    }
}

void outputDigit(int segment, int value) {
    delay(segment == 0? 0: 4);              // Loop is taking some time causing visible brightness difference. Hence, Compensation.
    clearDigit(segment);                    // Clear the digit before moving. Overwriting is causing fade effect.
    selectDigit(segment);
    int segmentCounter = numberofSegments;
    if (value > 0)                          // only if there is something to turn on.
    {
        while (segmentCounter) {
            digitalWrite(segmentPins[--segmentCounter], (int)(value & 1) ? LOW: HIGH);
            value >>= 1;
        }
    }
}

void displayWeather() {
   for (int i = 0; i < numberofDigits; i++) {
         outputDigit(i, outputSegmentValues[i]);
   }
}

void displayWeather(bool minus, int temp, int status) {
    Serial.println(minus);
    Serial.println(temp);
    Serial.println(status);
    outputSegmentValues[0] = minus? 0b00000010: 0b00000000; // Set G segment for minus
    int digitCounter = temp > 9? 2: 1;                      //To remove preceeding Zero
    outputSegmentValues[2] = 0b00000000;                    // To clear the 2nd digit in case of single digit Weather
    while (digitCounter){
      outputSegmentValues[digitCounter] = numeral[temp%10];
      temp /= 10;
      digitCounter--;
    } 
    outputSegmentValues[3] = statusCode[status];
    displayWeather();
}

void processValues(const char *temperature, const char *id) {
    Serial.println(temperature);
    Serial.println(id);
    //double ftemperature = strtod(temperature,NULL);
    int intemperature =(int)(roundf(strtod(temperature,NULL))); // Round the temperature
    int intid = atoi(id);
    int status=0;  //unknown 
    if((int)(intid/100) == 2)
    {
        status = 1; //thunders t
    }else if (intid == 800)
    {
        status = 2;  //Clear Sky/Good G 
    }else if (intid == 801 || intid == 802)
    {
        status = 3;  //Light Clouds c
    }else if (intid == 804 || intid == 803)
    {
        status = 4;   //Clouds C
    }else if ((int)(intid / 100)  == 3)
    {
        status = 5;   //Drizzle d
    }else if ((int)(intid / 100) == 5)
    {
        status = 6;   //Rain r
    }else if ((int)(intid / 100) == 6)
    {
        status = 7;   //Snow s
    }else if ((int)(intid / 100) == 7)
    {
        status = 8;   //Fog F
    }else if (intid >= 900 && intid < 910)
    {
        status = 9;   //Extreme E
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
  delay(interval);
}