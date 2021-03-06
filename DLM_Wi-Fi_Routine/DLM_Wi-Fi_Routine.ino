#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

//Hard Coded GUID
const char* GUID = "229371a2-0af0-4514-ade7-ad339f94ced4";

//Hard Coded Topics
char* dlmLockTopic = "dlm/lock/229371a2-0af0-4514-ade7-ad339f94ced4";
char* dlmStatusTopic = "dlm/status";

//Callback to be used by PubSubClient
void callback(char* topic, byte* payload, unsigned int length) {

  //Check for lock/unlock commands
  if (strcmp(topic,dlmLockTopic)==0){

    //Send lock commands to microcontroller and publish status
    if ((char)payload[0] == '1')
    {
      Serial.write("LOCK");
      publishStatus(true);
    }
    if ((char)payload[0] == '0')
    {
      Serial.write("UNLOCK");
      publishStatus(false);
    }
      
    //Delay to allow actuation to progress
    delay(500);
  } 
}

const char* mqtt_server = "livebolt.rats3g.net";

//Establish WiFi Client for MQTT connection
WiFiClientSecure espClient;
PubSubClient client(mqtt_server,8883,callback,espClient);

//Publishes DLM status to DLM status topic
void publishStatus(boolean value)
{
  //Serial.println(dlmStatusTopic);
  if(value)
  {
    client.publish(dlmStatusTopic,"229371a2-0af0-4514-ade7-ad339f94ced4,true");
  }
  else
  {
    client.publish(dlmStatusTopic,"229371a2-0af0-4514-ade7-ad339f94ced4,false");
  }
}

void setup()
{ 
  //Initialize serial connection
  Serial.begin(9600);
  Serial.println();
  
  //Check Memory for WiFi Credentials

  //If no credentials, go to AP mode
    //Collect, test, and store credentials
  //Else
  //Connect to WiFi (hard coded)
  WiFi.begin("Embedded Systems Class", "embedded1234");
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  //Connect to MQTT server
  reconnect();
}

//Connect to MQTT
void reconnect() {
  
  // Loop until reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("ESP8266Client","livebolt","livebolt")) 
    {

      Serial.println("connected");
      
      Serial.println("Subscribing to: ");
      Serial.print(dlmLockTopic);
      client.subscribe(dlmLockTopic);
      
    } else {
      
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      // Wait 5 seconds before retrying
      delay(5000);
      }
   }
 }


void loop() {

  //If MQTT Client disconnected, reconnect
  if (!client.connected()) {
    reconnect();
  }

  //Loop Client for new messages
  client.loop();
  delay(500);
}
