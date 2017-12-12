#include <SoftwareSerial.h>
#include <Stepper.h>

//Create Software Serial Connection with WiFi chip
SoftwareSerial esp8266(2, 3);

//Initialize Stepper Motor
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {

  Serial.println("Setup");

  //Initialize Software Serial Connection
  esp8266.begin(9600);

  //Initialize Serial Connection
  Serial.begin(9600);

  //Initialize Switch Pin. Set to low
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  //Initialize Stepper speed
  myStepper.setSpeed(15);
}

void loop() {

  Serial.println("Running");

  //Read and execute serial command
  if (esp8266.available())
  {
    Serial.println("Receiving serial command: ");

    //Assemble Serial Command
    String serialCommand = "";
    while (esp8266.available())
    {
      serialCommand += (char)esp8266.read();
    }
    Serial.print(serialCommand);

    //Execute lock/unlock
    if (serialCommand == "LOCK")
    {
      Serial.println("Locking deadbolt");
      turnDeadbolt(1);
    }
    else if (serialCommand == "UNLOCK")
    {
      Serial.println("Unlocking deadbolt");
      turnDeadbolt(-1);
    }
  }
}


//Lock is 1, unlock is -1
void turnDeadbolt(int turnDirection)
{
  //Close switch
  digitalWrite(12, HIGH);
  delay(10);

  //Step motor
  myStepper.step(65 * turnDirection);

  //Open switch
  digitalWrite(12, LOW);
  delay(10);
}

//void lock()
//{
//  //Close switch
//  digitalWrite(12, HIGH);
//  delay(10);
//
//  myStepper.setSpeed(15);
//  myStepper.step(65);
//
//  //Open switch
//  digitalWrite(12, LOW);
//  delay(10);
//}
//
//void unlock()
//{
//  //Close switch
//  digitalWrite(12, HIGH);
//  delay(10);
//
//  myStepper.setSpeed(20);
//  myStepper.step(-70);
//
//  //Open switch
//  digitalWrite(12, LOW);
//  delay(10);
//}


