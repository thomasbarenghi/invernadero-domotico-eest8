
#define BLYNK_TEMPLATE_ID "TMPLq4XNRtK7"
#define BLYNK_DEVICE_NAME "Invernadero"
#define BLYNK_AUTH_TOKEN "v74XIKpqob2A_J40zQxlGU3X0Swub34q";
#define BLYNK_PRINT Serial
//#define DHTPIN 4

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>


//Blynk
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Proyecto";
char pass[] = "proyecto2021grupo";
int str;

//Comunicacion
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];     
char messageFromPC[numChars] = {0};
char message1FromPC[numChars] = {0};
int integerFromPC = 0;
int integer2FromPC = 0;
int integer3FromPC = 0;
int integer4FromPC = 0;
int integer5FromPC = 0;
int integer6FromPC = 0;
boolean newData = false;
int i1=99;
int i2=99;
int i3=99;
int i5=99;
String i4="x";
int start=0;
//============

void setup() {
  
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    Serial1.begin(115200);
    while (!Serial) {;}
    delay(2000);
}

//============

void loop() {

Blynk.run();
  
//Serial.println("Inicio completo.");

    
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
    
}

//Transmicion a Arduino
void send() {Serial.print("\nSe envió: ");Serial.print(str); Serial1.println(str);}

//Recepcion arduino: grabacion de datos
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

//Recepcion arduino: procesamiento de datos
void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    integer2FromPC = atoi(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    integer3FromPC = atoi(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    integer4FromPC = atoi(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    integer5FromPC = atoi(strtokIndx);     // convert this part to a float
}

//============

//Console log: Data recepcionada
void showParsedData() {

Serial.print("Modo: ");
Serial.println(integerFromPC);

Serial.print("Temp: ");
Serial.println(integer2FromPC);

Serial.print("Humedad: ");
 Serial.println(integer3FromPC);
 
Serial.print("Luz: ");
    Serial.println(integer4FromPC);
        Serial.print("Hora: ");
    Serial.println(integer5FromPC);
    myTimerEvent();
}

//Recepcion de Blynk
BLYNK_WRITE(V0) {
str = param.asInt();
int ver;
ver=str;
if ( (ver==1) || (ver==2) || (ver==3) ){
Serial.print("\nSe recibió: ");Serial.print(str);
send();}
}


//Transmision a Blynk
void myTimerEvent(){


int x1= 28; //modo de funcionamiento
int x2= 29; //humedad;TEMPERATURA
int x3= 30; //HUMEDAD
String x4= "Si";
int x5;

if (integer4FromPC==1) {x4= "Si";}
else  {x4= "No";}

x5= int(integer5FromPC);


if (i1 == integerFromPC) {;}else{Blynk.virtualWrite(V1,integerFromPC); i1=integerFromPC;Serial.print(" Sistemas:");Serial.print(i1);} //sistemas
if (i2 == integer2FromPC) {;}else{Blynk.virtualWrite(V2,integer2FromPC); i2=integer2FromPC;Serial.print(" Temp:");Serial.print(i2);} //temperatura
if (i3 == integer3FromPC) {;}else{Blynk.virtualWrite(V3,integer3FromPC); i3=integer3FromPC;Serial.print(" Humedad:");Serial.print(i3);} //humedad
if (i4 == x4) {;}else{Blynk.virtualWrite(V4,x4); i4=x4;Serial.print(" Luz:");Serial.print(i4);} //luz
if (i5 == integer5FromPC) {;}else{Blynk.virtualWrite(V5,integer5FromPC); i5=integer5FromPC;Serial.print(" Hora:");Serial.print(i5);} //hs

//if (i4 == integer4FromPC) {;}else{Blynk.virtualWrite(V4,integer4FromPC); i4=integer4FromPC;}  //luz
//Blynk.virtualWrite(V1,x1); //in3
//Blynk.virtualWrite(V2,x2); //in
//Blynk.virtualWrite(V3,x3); //in2
//Blynk.virtualWrite(V4,messageFromPC);
//Serial.print("Sended");
//delay(1000);
}
