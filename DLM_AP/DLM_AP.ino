#include <PubSubClient.h>

#include <ArduinoJson.h>

#include <ConfigManager.h>

const char* GUID = "ab55555a-2ea8-4fd9-b4d2-1305c974c788";

//Hard Coded Credentials
const char* ssid = "Embedded Systems Class";
const char* pasword = "embedded1234";
const char* mqtt_server = "livebolt.rats3g.net";

//Hard Coded Topics
char* idmSetupTopic = "idm/register";
char* idmStateTopic = "idm/status";
char* idmRemoveTopic = "idm/remove/ab55555a-2ea8-4fd9-b4d2-1305c974c788";
char* idmRemoveConfirm = "idm/removeConfirm";

ConfigManager configManager;


void callback(char* topic, byte* payload, unsigned int length) {
  //Clear EEPROM
  Serial.println("Request to remove");
  //sendConfirm();
  configManager.clearEEPROM();
}


//Boolean to track open/close state
bool closed = false;

//Establish WiFi Client for MQTT connection
WiFiClientSecure espClient;
PubSubClient client(mqtt_server,8883,callback,espClient);

void sendConfirm() {
  client.publish(idmRemoveConfirm,"ab55555a-2ea8-4fd9-b4d2-1305c974c788,TestHome");
  delay(1000);
}

struct Config {
    char name[20];
    bool enabled;
    int8 hour;
    char password[20];
    char homeName[100];
    char homePassword[100];

} config;

struct Metadata {
    int8 version;
} meta;

void createCustomRoute(ESP8266WebServer *server) {
    server->on("/custom", HTTPMethod::HTTP_GET, [server](){
        server->send(200, "text/plain", "Hello, World!");
    });
}

boolean justTurnedOn = true;

void setup() {
    Serial.begin(9600);
    
    meta.version = 3;

    // Setup config manager
    
    configManager.setAPName("LiveBolt IDM");
    configManager.setAPFilename("/index.html");
    configManager.addParameter("name", config.name, 20);
    configManager.addParameter("enabled", &config.enabled);
    configManager.addParameter("hour", &config.hour);
    configManager.addParameter("password", config.password, 20, set);
    configManager.addParameter("homeName", config.homeName, 100);
    configManager.addParameter("homePassword", config.homePassword, 100);
    configManager.addParameter("version", &meta.version, get);

    configManager.begin(config);
   
   
    configManager.setAPICallback(createCustomRoute);

    //
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
      Serial.print(idmRemoveTopic);
      client.subscribe(idmRemoveTopic);
      
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
    
    if(configManager.EEPROMisEmpty() && justTurnedOn)
    {
      Serial.println(configManager.EEPROMisEmpty());
      configManager.loop();
      Serial.println("Connected");
      delay(400);
    }
    else
    {
      if(!configManager.EEPROMisEmpty())
      {
        configManager.loop();
        connectMQTT();
        client.publish(idmSetupTopic, "ab55555a-2ea8-4fd9-b4d2-1305c974c788,TestHome,Testing123!,New IDM");
        configManager.clearEEPROM();
      }
      configManager.clearEEPROM();
      Serial.println("looping blank");
      delay(400);
    }
    justTurnedOn = false;    

}

