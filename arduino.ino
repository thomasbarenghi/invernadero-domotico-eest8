#include <SoftwareSerial.h> 
#include <DHT.h>
#include "RTClib.h"
#define RELAY_ON 0    
#define RELAY_OFF 1

DHT dht(2, DHT11);
RTC_DS3231 rtc;

int P;
int dataNumber;
int dataFull= 1;
const byte numChars = 32;
char receivedChars[numChars];  
boolean newData = false;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
float t = 0;float tmax = 0;float tmin = 0;
float hum = 0;float huMax = 0;float huMin = 0;
float hs = 0;float hmax = 0;float hmin = 0;
int thresholdValue = 800;
int sensorValue;
int hss = A0;

String data;
String a="a";

int lightSend= 0;
int tempSend= 2;
int humSend= 3;
int estado;
int hourData;
int minuteData;

int rele3;
int rele4;
int rele5;
int rele6;

SoftwareSerial espSerial(8, 9);


void setup () {
  
digitalWrite (3, RELAY_OFF);
digitalWrite (4, RELAY_OFF);
digitalWrite (5, RELAY_OFF);
digitalWrite (6, RELAY_OFF);
pinMode (3, OUTPUT);
pinMode (4, OUTPUT);
pinMode (5, OUTPUT);
pinMode (6, OUTPUT); 
pinMode(13,OUTPUT);
pinMode(hss, INPUT);
pinMode(A0+1,INPUT);

Serial.begin(115200);
espSerial.begin(115200);

while (!Serial) {;}



dht.begin();
t = dht.readTemperature();;
hum = dht.readHumidity();;
sensorValue = analogRead(hss);
//delay(2000);

if (! rtc.begin()) {Serial.println("Couldn't find RTC");while (1);}
//if (rtc.lostPower()) {Serial.println("RTC lost power, lets set the time!");rtc.adjust(DateTime(2021, 6, 2, 16, 0, 0)); }
//rtc.adjust(DateTime(2021, 11, 1, 11, 48, 0));
consoleStart();

}

void loop () {


recvWithEndMarker();
showNewNumber();
sendData();
//configSens();
console();
configSens();
//delay(1000);
}


void configSens() {

DateTime now = rtc.now(); 

hourData=(now.hour(), DEC);
//Relay 3: Luz
//Relay 4: Calefaccion
//Relay 5: Ventiladores
//Relay 6: Agua

if((now.hour() > hmin)  &&  (now.hour() < hmax)){digitalWrite (3, RELAY_ON); lightSend=1; rele3=1;}
else{digitalWrite (3, RELAY_OFF);lightSend= 0;rele3=0;}

if(t < tmin){digitalWrite(4,RELAY_ON);rele4=1;}
if(t > tmax){digitalWrite(4,RELAY_OFF);digitalWrite(5,RELAY_ON);rele4=0;rele5=1;}
tempSend=dht.readTemperature();
t=dht.readTemperature();

if(hum > 80){digitalWrite(5,RELAY_ON);}
if(hum < huMin){digitalWrite(5,RELAY_OFF);rele5=0;}
humSend=dht.readHumidity();
hum=dht.readHumidity();

if(sensorValue > thresholdValue){digitalWrite (6, RELAY_ON);rele6=1;}
else {digitalWrite (6, RELAY_OFF);rele6=0;}
sensorValue = analogRead(hss);
//delay(3000);
//_loop();
}

//Recepcion

void recvWithEndMarker() {
static byte ndx = 0;
char endMarker = '\n';
char rc;

if (Serial.available() > 0) {
 
rc = Serial.read();

if (rc != endMarker) {receivedChars[ndx] = rc;ndx++;
if (ndx >= numChars) {ndx = numChars - 1;}}
else {receivedChars[ndx] = '\0';ndx = 0; newData = true; showNewNumber();}}

}


void showNewNumber() {
if (newData == true) {
dataNumber = 0;dataNumber = atoi(receivedChars);
Serial.print("This just in ... ");
Serial.println(receivedChars);
Serial.print("Data as Number ... ");
Serial.println(dataNumber);
if (dataNumber > 0) {estado=dataNumber;}
newData = false;
setS();
}}


//Transmision

void sendData(){


 DateTime now = rtc.now(); 
data= String("<")+String("a")+String(",")+int(estado)+String(",")+int(tempSend)+String(",")+int(humSend)+String(",")+int(lightSend)+String(",")+(now.hour())+String(">");
espSerial.println(data);
Serial.println(data);
//espSerial.println("<hola, 1, 2, 3, 1>");//sistemas, temp, humedad, luz
}

//Ejecucion

void setS(){

if (dataNumber > 0) {
dataFull=dataNumber;}

if(dataNumber == 0){//default
tmin= 20;tmax= 30;
hmin= 8;hmax= 18;
huMin= 20;huMax= 60;
}

if(dataNumber == 1){//rucula
tmin= 13;tmax= 18;
hmin= 8;hmax= 19;
huMin= 20;huMax= 60;
}

if(dataNumber == 2){//lechuga
tmin= 16;tmax= 20;
hmin= 8;hmax= 18;
huMin= 20;huMax= 60;
}

if(dataNumber == 3){//apio
tmin= 19;tmax= 23;
hmin= 8;hmax= 18;
huMin= 20;huMax= 60;
}

if(dataNumber == 4){//cebolla
tmin= 20;tmax= 25;
hmin= 7;hmax= 19;
huMin= 20;huMax= 60;
}}

//Inicio

void consoleStart(){
Serial.print("Iniciando sistemas...");
Serial.println();
}

//Consola

void console(){
DateTime now = rtc.now(); 
Serial.print(now.year(), DEC);Serial.print('/');Serial.print(now.month(), DEC);Serial.print('/');Serial.print(now.day(), DEC);
Serial.print(" (");Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);Serial.print(") ");
Serial.print(now.hour(), DEC);Serial.print(':');Serial.print(now.minute(), DEC);Serial.print(':');Serial.print(now.second(), DEC);
Serial.print(" Temp:");Serial.print(dht.readTemperature(), DEC);
Serial.print(" Humedad:");Serial.print(dht.readHumidity(), DEC);
Serial.print(" Humedad Suelo:");Serial.print(sensorValue);
Serial.print(" Tmax:");Serial.print(tmax);
Serial.print(" Tmin:");Serial.print(tmin);
Serial.println();
Serial.print(" rele3:");Serial.print(rele3);
Serial.print(" rele4:");Serial.print(rele4);
Serial.print(" rele5:");Serial.print(rele5);
Serial.print(" rele6:");Serial.print(rele6);
Serial.print(" hum:");Serial.print(hum);
Serial.print(" hum:");Serial.print(huMax);
Serial.print(" t:");Serial.print(t);
//delay(5000);
}
