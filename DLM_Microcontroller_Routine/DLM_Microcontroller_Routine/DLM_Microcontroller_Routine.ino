#include <SoftwareSerial.h>
#include <Stepper.h>

//Create Software Serial Connection with WiFi chip
SoftwareSerial esp8266(2,3);

//Initialize Stepper Motor
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

//Initialize isLocked variable
bool isLocked = false;

void setup() {

  Serial.println("Setup");
  //Initialize Software Serial Connection
  esp8266.begin(9600);

  //Initialize Serial Connection
  Serial.begin(9600);

  //Initialize Switch Pin. Set to high
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  myStepper.setSpeed(15);
}

void loop() {

  //Check Lock State
  Serial.println("Running");

  //Read and execute serial command
  if(esp8266.available())
  {
    Serial.println("Recieving serial command: ");
    
    //Assemble Serial Command
    String serialCommand = "";
    
    while(esp8266.available())
    {
      serialCommand += (char)esp8266.read();
    }

    Serial.print(serialCommand);
    
    //Execute lock/unlock if lock is in proper state
    //if(serialCommand == "LOCK" && !isLocked)
    if(serialCommand == "LOCK")
    {

      Serial.println("Locking deadbolt");
      turnDeadbolt(1);
      
      //REMOVE
      isLocked = true; 
    }
    else if (serialCommand == "UNLOCK")
    {

      Serial.println("Unlocking deadbolt");
      turnDeadbolt(-1);

      //REMOVE
      isLocked = false; 
    }
   }
   //Send lock state to Wi-Fi Chip
}


//Lock is 1, unlock is -1
void turnDeadbolt(int turnDirection)
{
  //Close switch
  digitalWrite(12, HIGH);
  delay(10);

  /*
  //Actuate motor
  for(int i =0; i < 50; i++)
  {
    myStepper.step(turnDirection);
    delay(1);
  }
  */
  myStepper.step(65*turnDirection);

  //Open switch
  digitalWrite(12, LOW);
  delay(10);
}

void lock()
{
  //Close switch
  digitalWrite(12, HIGH);
  delay(10);

  myStepper.setSpeed(15); 
  myStepper.step(65);

  //Open switch
  digitalWrite(12, LOW);
  delay(10);
}

void unlock()
{
  //Close switch
  digitalWrite(12, HIGH);
  delay(10);

  myStepper.setSpeed(20);
  myStepper.step(-70);

  //Open switch
  digitalWrite(12, LOW);
  delay(10);
}


