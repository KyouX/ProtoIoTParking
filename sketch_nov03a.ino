#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const int trigPin = 5; //output
const int echoPin = 7; //input
int state, prevState;

//WiFi
const char* ssid = "ulwifi_SAP";
const char* password = "ulw1f1S4P$";

//SAP HCP

const int httpsPort = 443;
const char* host = "iotmmsp1942975305trial.hanatrial.ondemand.com";
String message_id = "85dde182247a31fa3680";
String device_id = "4252e448-914e-46c8-a886-cdd3dee9d257"; 
String oauth_token = "51352b98eb590bd72c87b99e015ca16";

String uri = "/com.sap.iotservices.mms/v1/api/http/data/" + device_id;
String post_payload = "{\"mode\":\"async\", \"messageType\":\"" + message_id + "\", \"messages\":[{\"timestamp\":1413191650, \"slot\":\"20\", \"estado\":\"1\"}]}";


WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  //pinModes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.print("connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("requesting URL: ");
  Serial.println(uri);
  
}

void loop() {
  long duration, cm;

  digitalWrite(trigPin, LOW); //Ensuring a clean HIGH pulse
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5); //Triggering the sensor with a HIGH 2 or more microseconds length pulse
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); //Length of the pulse in microseconds
  cm = microsecondsToCentimeters(duration);

  delay(100);
      
  if (cm <= 20) {
    state = 1; //ocupado
  } else {
    state = 0; //libre
  }
  
  if (state != prevState){
    //enviar data
    Serial.println(post_payload);
    prevState=state;
  }
    
}

long microsecondsToCentimeters(long microseconds) {
  // Speed of sound -> 340 m/s -> 0.034029 centimeter per microsecond
  // 1/0.034029 = 29.3867
  return microseconds / 29 / 2;
}

