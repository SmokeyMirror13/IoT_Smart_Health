#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "#roy";
const char* password = "Astapor22";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int st = 0;
int counter=0;

#define MUX_A D4
#define MUX_B D3
#define MUX_C D2

#define ANALOG_INPUT A0

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

void setup() {
 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT); 
  pinMode(D1, INPUT);
  pinMode(D0, INPUT);  
  //client.setCallback(callback);
}
void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}
void resp()
{
  changeMux(LOW, HIGH, HIGH);
  int val = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 3 pin of Mux
  
  if(millis()-st==60000)
{

  st=millis();
  counter=0;
  Serial.println(counter);

}
//Serial.println(val);
if(val<415)
{
  
  counter++;
  delay(1000);
}

  char r_r1[7];
  dtostrf(counter, 7, 2, r_r1);
  Serial.println(client.publish("rramb",r_r1));
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float value;
  
  changeMux(LOW, LOW, LOW);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 0 pin of Mux
  float t1=(value/1023) *3300;  
  float   t2=(t1/10);
  float  t3=((t2 * 9)/5 + 32);
  Serial.println(t3); 
  char t_t1[7];
  dtostrf(t3, 7, 2, t_t1);
  Serial.println(client.publish("tempamb",t_t1));
  
  changeMux(LOW, LOW, HIGH);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 1 pin of Mux
  int h1=value/8.6;
  char h_h1[7];
  dtostrf(h1, 7, 2, h_h1);
  Serial.println(client.publish("heartamb",h_h1));

  resp();
  
  

  if(digitalRead(D1)==1||digitalRead(D0)==1)
 {
  Serial.println("!");
 }
 else
 {
  changeMux(LOW, HIGH, LOW);
  value = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 2 pin of Mux
  int e1=value;
  char e_e1[7];
  dtostrf(e1, 7, 2, e_e1);
  Serial.println(client.publish("ecg",e_e1));
  
 }
 
    delay(1000);
  }
