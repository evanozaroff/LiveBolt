#include <ESP8266WiFi.h>

void setup()
{
   //Serial monitor startup
  Serial.begin(115200);
  Serial.println();
  
//  //WiFi Connection
//
//
//  //Hard-coded access to WiFi network
//  WiFi.begin("Embedded Systems Class", "embedded1234");
//
//  //Wait until connected, then print connection
//  Serial.print("Connecting");
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println();
//
//  Serial.print("Connected, IP address: ");
//  Serial.println(WiFi.localIP());
//
//  //MQTT Connection

//  PubSubClient client;
//  client.setClient();


  //Set up pins
  pinMode(0, OUTPUT);
  pinMode(12, INPUT);
  
}
 
void loop() {

  Serial.println("PinRead: ");
  Serial.print(digitalRead(12));
  
  if(digitalRead(12) == HIGH)
  {
      digitalWrite(0, LOW);
  }
  else
  {
    digitalWrite(0, HIGH);
  }

  delay(2000);

}
