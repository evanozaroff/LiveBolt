#include <PubSubClient.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Hard Coded GUID
const char* GUID = "cc46356a-2ea8-4fd9-b4d2-1305c974c788";

//Hard Coded Credentials
const char* ssid = "Embedded Systems Class";
const char* pasword = "embedded1234";
const char* mqtt_server = "livebolt.rats3g.net";

//Hard Coded Topics
char* idmStateTopic = "idm/status";

//Boolean to track open/close state
bool closed = false;

//Establish WiFi Client for MQTT connection
WiFiClientSecure espClient;
PubSubClient client(mqtt_server,8883,espClient);


void connectWiFi()
{
  
  WiFi.begin("Embedded Systems Class", "embedded1234");
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}



void connectMQTT() 
{
  
  // Loop until reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("ESP8266Client","livebolt","livebolt")) 
    {

      Serial.println("connected");
      
      Serial.println("Subscribing to: ");
      Serial.print(idmStateTopic);
      client.subscribe(idmStateTopic);
      
    } else {
      
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      // Wait 5 seconds before retrying
      delay(5000);
      }
   }
 }



void setup() {
  
  //Initialize serial port
  Serial.begin(9600); 

  //Connect to WiFi
  connectWiFi();
}


void loop() {

  //Read analog value for this cycle
  int analogValue = analogRead(A0);

  Serial.println(analogValue);

  //Check for state change and publish
  if(analogValue >= 500 && closed)
  {

    //Publish state change to closed 
    connectMQTT();
    client.publish(idmStateTopic,"cc46356a-2ea8-4fd9-b4d2-1305c974c788,false");
    
    
    //Change state to open
    closed = false;
    Serial.println();
    Serial.println("State has been changed to OPEN");
  }
  else if(analogValue < 500 && !closed)
  {
    
    //Publish state change to open
    connectMQTT();
    client.publish(idmStateTopic,"cc46356a-2ea8-4fd9-b4d2-1305c974c788,true");
    
    //Change state to closed
    closed = true;
    Serial.println();
    Serial.println("State has been changed to CLOSED");
  }

    //Disconnect. We dont need to waste our energy with these connections
    client.disconnect();

    //Delay, we dont need to run this shit that often
    delay(750);
  
}





