#include "application.h"
#include <elapsedMillis.h>
#include <math.h>
int led = D7; // This one is the built-in tiny one to the right of the USB jack

//pins for decimal point and each segment
const int numberofSegments=8;

//A, B, C, D, E, F, G, dp
const int segmentPins[]= { A0, D4, D2, D1, D0, A1, D3, A2}; 

const int numberofDigits=4;

const int digitPins[numberofDigits] = { A3, A4, D5, D6}; //digits 1, 2, 3, 4

int outputSegmentValues[] = {0b00000010, 0b00000010, 0b00000010, 0b00000010}; // Initial values for Digits

// delay in milliseconds between Weather requests (10 minutes)
const unsigned long intervalAfterFirstUpdate = 10 * 60 * 1000;   // Set interval here 

unsigned long interval = 20 * 1000;      // Actual value changes after first update
// Elapsed time since update
elapsedMillis timeElapsed;

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
    0b10011100, //C
    0b00111010, //o
    0b11111100, //O
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
            digitalWrite(segmentPins[--segmentCounter], (int)(value & 1) ? LOW: HIGH);  // Display LSB
            value >>= 1;                                                                // Set LSB
        }
    }
}

void displayWeather() {
   for (int i = 0; i < numberofDigits; i++) {                               // Display Each Segment
         outputDigit(i, outputSegmentValues[i]);
   }
}
void setSegmentValues(bool minus, int temp, int status) {
    Serial.println(minus);
    Serial.println(temp);
    Serial.println(status);
    outputSegmentValues[0] = minus? 0b00000010: 0b00000000;                 // Set G segment for minus
    int digitCounter = (temp < 10 )? 1: 2;                                   // To format  || (temp < 100 && !minus)
    outputSegmentValues[2] = 0b00000000;                                    // To clear the 2nd digit in case of single digit Weather
    do {                                                                    // Run once to display 0
      outputSegmentValues[digitCounter] = numeral[temp%10];
      temp /= 10;
      digitCounter--;
    } while (temp);
    outputSegmentValues[3] = statusCode[status];
}

void processValues(const char *temperature, const char *id) {
    //Sample:  temperature: -4.81  id: 800
    Serial.println(temperature);
    Serial.println(id);
    int intemperature =(int)(roundf(strtod(temperature,NULL))); // Round the temperature
    int intid = atoi(id);
    int status=0;  // U unknown 
    if((int)(intid/100) == 2)
    {
        status = 1; //thunders t
    }else if (intid == 800)
    {
        status = 2;  //Clear Sky C
    }else if (intid == 801 || intid == 802)
    {
        status = 3;  //Partly Overcast o
    }else if (intid == 804 || intid == 803)
    {
        status = 4;   //Overcast O
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
    
    setSegmentValues(intemperature < 0, intemperature < 0 ? intemperature * -1: intemperature, status);
}


void processWeather(const char *event, const char *data) {
    //Sample data: 800~-4.81
    Serial.println("Handling Weather: ");
    // Handle the webhook response
    int stringPos = strlen(data);
    char w_temp[7] = {""};
    char w_id[4] = {""};
    int itemCounter = 0;
    int tempStringLoc = 0;
    memset(w_temp, 0, sizeof(w_temp));
    memset(w_id, 0, sizeof(w_id));
    for (int i = 0; i < stringPos; i++){
        if(data[i] == '~'){
                    itemCounter++;
                    tempStringLoc = 0;
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
    digitalWrite(led, LOW);                             // Turn off when the response is received
    if (interval < intervalAfterFirstUpdate) {          // Changes the actual interval value
        interval = intervalAfterFirstUpdate;   
        // removing this for block (or moving this if block to the top of this function)
        // is causing unwanted segments to turn on. I don't know why.
        for (int i = 0; i < numberofDigits; i++) {
            Serial.println();
        }
    }
}
void setupPins() {
    pinMode(led, OUTPUT);
    for (int i = 0; i < numberofDigits; i++) {
        pinMode( digitPins[i], OUTPUT);
    }
    for (int i = 0; i < numberofSegments; i++) {
        pinMode( segmentPins[i], OUTPUT);
    }
}

void setup() {
    setupPins();
    Serial.begin(9600);
    Serial.print("Started: ");
    // Subscribe to the webhook response event
    Particle.subscribe("hook-response/weather_hook", processWeather , MY_DEVICES);
}


void loop() {
    if (timeElapsed > interval) 
    {   
        // Get some data
        String data = String(10);
        digitalWrite(led, HIGH);    // Turn on when the request is sent
        // Trigger the webhook
        Particle.publish("weather_hook", data, PRIVATE);
        timeElapsed = 0;            // reset the counter to 0 so the counting starts over...
    }
    displayWeather();
}