#include <SPI.h> //for the SD card module                                                       //For JOE = Learn how Time Change and Sleep Systems work
#include <SD.h>  //for the SD card module
#include <Wire.h> //for the RTC
#include <TimeLib.h>
#include <DS3232RTC.h>
#include <LowPower.h>
const byte rxPIN = 0;  //indicate rx pin on arduino board
const byte txPIN = 1;  //indicate tx pine on arduino board
const byte rtcAlarmPin = 3; //indicate pin D3 for external interrupt
int chipSelect = 8;  //indicate pin where CS port for SD Card Module is connected
String distance = String("");
double distanceSum = 0; //
long distance_reading = 0;
double distanceAverage = 0;
char inChar = '0';
int i = 0;

int DistArray[50];    //JWP: Variables for the Median Test
int median = 0;
int number = 0;

tmElements_t tm;

// Create a file to store the TEST
File myFile;

 time_t compileTime()
{
    const time_t FUDGE(10);    //fudge factor to allow for upload time, etc. (seconds, YMMV)
    const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char compMon[3], *m;
    strncpy(compMon, compDate, 3);
    compMon[3] = '\0';
    m = strstr(months, compMon);
    tmElements_t tm;
    tm.Month = ((m - months) / 3 + 1);
    tm.Day = atoi(compDate + 4);
    tm.Year = atoi(compDate + 7) - 1970;
    tm.Hour = atoi(compTime);
    tm.Minute = atoi(compTime + 3);
    tm.Second = atoi(compTime + 6);
    time_t t = makeTime(tm);
    return t + FUDGE;        //add fudge factor to allow for compile time
}

void setup() {
  pinMode(rxPIN, INPUT); //set the rxPIN as an input coming from the sensor
  pinMode(txPIN, OUTPUT);//set the txPIN as a output to the sensor
  pinMode(chipSelect, OUTPUT);
  Serial.begin(9600); //initialize the serial monitor
  
  Wire.begin();
  SPI.begin();
  Serial.print("Initializing SD card...");
  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");//if there is no information going from the SD card module into the indicated chip select pin on the arduino board, this message will be printed to the serial monitor
    return;
  }
  Serial.println("initialization done.");//if there is information going into the chip select pin, this message will be printed to the serial monitor
  
  //open file
  myFile=SD.open("TEST.txt", FILE_WRITE);
  // if the file opened ok, write to it:
  if (myFile) {
    Serial.println("File opened ok");
    // print the headings for our TEST
    myFile.println("Date,Time,Distance mm");
  }
  myFile.close();
  pinMode(rtcAlarmPin, INPUT_PULLUP); //Set pin to an interrupt using INPUT_PULLUP
  RTC.squareWave(SQWAVE_NONE); //Disable the default square wave coming from the RTC
  RTC.setAlarm(ALM1_MATCH_SECONDS, 0, 0, 0, 0 ); // setAlarm Syntax (RTC.setAlarm(alarmType, seconds, minutes, hours, dayOrDate);)
  RTC.alarm(ALARM_1);
  RTC.alarmInterrupt(ALARM_1, true);  // Enable alarm 1 interrupt A1IE
}
void loggingDateTime(){
    setSyncProvider(RTC.get); //Get the current date and time from the RTC
    myFile = SD.open("TEST.txt", FILE_WRITE); //open the file
  if (myFile) {
    //print the current date and time in the TEST.txt file
    myFile.print(year(), DEC);
    myFile.print('/');
    myFile.print(month(), DEC);
    myFile.print('/');
    myFile.print(day(), DEC);
    myFile.print(',');
    myFile.print(hour(), DEC);
    myFile.print(':');
    myFile.print(minute(), DEC);
    myFile.print(':');
    myFile.print(second(), DEC);
    myFile.print(",");
  }
    myFile.close(); //close the file
    //also print date and time to serial monitor
    Serial.print(year(), DEC);
    Serial.print('/');
    Serial.print(month(), DEC);
    Serial.print('/');
    Serial.print(day(), DEC);
    Serial.print(' ');
    Serial.print(hour(), DEC);
    Serial.print(':');
    Serial.print(minute(), DEC);
    Serial.print(':');
    Serial.print(second(), DEC);
    Serial.println();
  
  
}
void loggingDistance(){
  myFile = SD.open("TEST.txt", FILE_WRITE); //open the file
  if (myFile) {
    Serial.println("open with success");
    myFile.println(median); //print the average of 50 distance readings into the TEST.txt file
  }
  myFile.close(); //close the file
  //print the logged distance to the screen
  Serial.print("distance median is: ");
  Serial.println(median);
  Serial.print('\n');
}
// Interrupt Service Routine (ISR) function

void loop() {

 while (number < 50) {        //JWP: New Median Code
  digitalWrite(txPIN, HIGH);
  if(Serial.available()){
    inChar = Serial.read();
  if(isDigit(inChar)){
    distance += inChar;
  }
  if(inChar == '\r'){
    distance_reading = distance.toInt();
    if(distance_reading > 4999){
      Serial.print("Bad reading");
      Serial.println(distance_reading);
      distance = String("");
      distance_reading = 0;
    }
    else {
      Serial.print("The distance is: ");
      Serial.println(distance_reading);
      DistArray[number] = distance_reading;
      distance = String("");
      number++;
    }
  }
  }
       
  }

    sort(DistArray,50);

      Serial.print("Sorted Array: ");
  for(int i=0; i<50; i++) {
     Serial.print(DistArray[i]); 
     Serial.println(",");
  }
      median = DistArray[25];
     Serial.print("median =");
     Serial.println(median);

loggingDateTime();
loggingDistance();
//reset all necessary variables
// TESTing sd card
myFile = SD.open("TEST.txt");
  if (myFile) {
    Serial.println("Read:");
    // Reading the whole file
    while (myFile.available()) {
      Serial.write(myFile.read());
   }
    myFile.close();
  }
  else {
    Serial.println("error opening TEST.txt");
  }
  
i = 0;
median = 0;
memset(DistArray, 0, sizeof(50));
number = 0;

int time30sec=4; // time to sleep for 41 sec
int time1min=8; 
int time5min=38;
int time10min=75;
int time15min=113;
Serial.println("Entering sleep");
  delay(100);
  for(int i=0;i<time15min;i++){ //Change this number to change measurement frequency
  LowPower.powerDown(SLEEP_8S,ADC_OFF,BOD_OFF);
}
  Serial.println("Awake...");
  
}

void sort(int a[], int size) {        //JWP: Array Sorting code
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}
