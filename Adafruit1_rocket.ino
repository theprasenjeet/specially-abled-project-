#include <ESP8266WiFi.h>
#include <PubSubClient.h>
char *ssid = "surya";
char *pass = "warmachine076";
char *broker = "io.adafruit.com";
char *username = "warmachine7";
int l1 = 16, l2 = 5, l3 = 4;
int r1 = 0, r2 = 2, r3 = 14;
char *keypass  = "";
char msg[10];
char offlaunch[] = {"OFF"};
char rocketoff[] = {"OFF3"};

String send_payload = "", s1 = "ON", s2 = "ON2", s3 = "ON3", receivedMessage = "";
WiFiClient esp;
PubSubClient client(esp);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("connecting--->  ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.print("connected ");
  client.setServer(broker, 1883);
  client.setCallback(callback);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  digitalWrite(l3, LOW);
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  digitalWrite(r3, LOW);

}

void loop() {
  if (!client.connected())
  {
    if (client.connect("mqtt", username, keypass))
    {
      Serial.println("Connected to Server");
      client.subscribe("warmachine7/feeds/launch");
      client.subscribe("warmachine7/feeds/switch2");
      client.subscribe("warmachine7/feeds/switch3");
    }
  }
  if (Serial.available() > 1)
  {
    send_payload = String(Serial.parseInt());


    send_payload.toCharArray(msg, 5);
    if (send_payload != "0")
    {



      Serial.println(send_payload);

      client.publish("warmachine7/feeds/test-data", msg);
    }
  }
  client.loop();
  //  delay(3000);
}
void callback(char* topic, byte* payload,  int lengths)//byte me char ki ascii value integer me aaygi to unko Type Cast kena padega to print in char Fomate
{
  //  Serial.print("Message Arrived from topic -> [ ");
  //  Serial.println(topic);
  for (int i = 0; i < lengths; i++)
  {
    //    Serial.print(char(payload[i]));
    receivedMessage = receivedMessage + char(payload[i]);
  }
  //    Serial.println("hello "+receivedMessage);
  if (receivedMessage == s1)
  {
    Serial.println("Launchpad Active");
    digitalWrite(l1, HIGH);
    delay(500);
    digitalWrite(l2, HIGH);
    delay(500);
    digitalWrite(l3, HIGH);
    delay(500);
  }
  else if (receivedMessage == s2)
  {
    Serial.println("ROcket 2 lonched");
  }
  else if (receivedMessage == s3)
  {
    Serial.println("ROcket 3 lonched");
    digitalWrite(r1, HIGH);
    delay(2000);
    digitalWrite(r1, LOW);
    delay(2000);
    digitalWrite(r2, HIGH);
    delay(2000);
    digitalWrite(r2, LOW);
    delay(2000);
    digitalWrite(r3, HIGH);
    delay(2000);
    digitalWrite(r3, LOW);
    delay(5000);
    digitalWrite(l1, LOW);
    delay(500);
    digitalWrite(l2, LOW);
    delay(500);
    digitalWrite(l3, LOW);
    delay(500);
    client.publish("warmachine7/feeds/launch", offlaunch);
    client.publish("warmachine7/feeds/switch3", rocketoff);
    Serial.println("terminated");
  }
  receivedMessage = "";
  Serial.println("");
}
