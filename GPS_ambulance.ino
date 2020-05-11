#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "#roy";
const char* password = "Astapor22";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device

float latitude , longitude;




void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{

  ss.begin(9600);
  Serial.println();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}


void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  while (ss.available() > 0) //while data is available
    if (gps.encode(ss.read())) //read gps data
    {
      //if (gps.location.isValid()) //check whether gps location is valid
      {
        latitude = gps.location.lat();
        //lat_str = String(latitude , 6); // latitude location is stored in a string
        longitude = gps.location.lng();
        //lng_str = String(longitude , 6); //longitude location is stored in a string
        //Serial.println(lat_str);
        //Serial.println(lng_str);
        
      }

    }
    char lat_t[7];
    char long_t[7];
    dtostrf(latitude, 7, 2, lat_t);
    dtostrf(longitude, 7, 2, long_t);
    Serial.println(client.publish("lat",lat_t));
    delay(100);
    Serial.println(client.publish("long",long_t));
 
  delay(1000);
}
