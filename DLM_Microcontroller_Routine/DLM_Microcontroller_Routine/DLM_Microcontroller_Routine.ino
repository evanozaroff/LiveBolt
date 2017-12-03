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

  //Initialize Software Serial Connection
  esp8266.begin(9600);

  //Initialize Serial Connection
  Serial.begin(9600);

  //Initialize Switch Pin. Set to high
  pinMode(24, OUTPUT);
  digitalWrite(24, HIGH);
}

void loop() {

  //Check Lock State

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
//      turnDeadbolt(1);

        //Close switch
        digitalWrite(24, LOW);
          

        //Actuate motor
        for(int i =0; i < 50; i++)
        {
          myStepper.step(1);
          delay(10);
        }

      //Open switch
      digitalWrite(24, HIGH);
      
      //REMOVE
      isLocked = true; 
    }
    //else if (serialCommand == "UNLOCK" && isLocked)
//    else if (serialCommand == "UNLOCK")
//    {
//    
//      turnDeadbolt(-1);
//
//      //REMOVE
//      isLocked = false; 
//    }
   }

   //Send lock state to Wi-Fi Chip
}


//Lock is 1, unlock is -1
void turnDeadbolt(int turnDirection)
{
  //Close switch
  
  //Actuate motor
  for(int i =0; i < 50; i++)
  {
    myStepper.step(turnDirection);
    delay(10);
  }

  //Open switch
}


