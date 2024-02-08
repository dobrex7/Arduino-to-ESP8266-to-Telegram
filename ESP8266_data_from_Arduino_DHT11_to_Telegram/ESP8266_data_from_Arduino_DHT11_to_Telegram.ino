#include <ESP8266WiFi.h> //knjiznica za povezovanje na wifi esp8266
#include <WiFiClientSecure.h> //knjiznica za povezovanje na wifi esp8266
#include <UniversalTelegramBot.h> //knjiznica za telegram bota
#include<SoftwareSerial.h> // knjiznica za serijsko komunikacijo (z arduinom)

const char* ssid = "ssid";// ssid oz. Ime wifija
const char* password = "password"; // geslo od wifija

#define BOTtoken "token" // Bottoken, ki smo ga dobili od botfather-ja
#define CHAT_ID "ID" // chatID, ki smo ga dobili od chatid bota

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
SoftwareSerial NodeMcu_SoftSerial(D1, D2);  //SRX = D1 povezano na 11, D2 povezano na 10 na Arduinu

String dataIn; // niz z imenom dataIn, ki bo vseboval podatke, ki jih dobi od Arduina

void setup()
{
  Serial.begin(115200); // inicializiramo serijsko komunikacijo na vrednosti 115200
  NodeMcu_SoftSerial.begin(9600);

  configTime(0, 0, "pool.ntp.org");      
  client.setTrustAnchors(&cert);
  // Povezava na wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int a = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    a++;
  }
  // Serijski izpisi v programu, ter izpisi za Bota v telegramu.
  Serial.println("");
  Serial.println("WiFi je povezan");
  Serial.print("IP naslov: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Wifi je povezan!", "");
  bot.sendMessage(CHAT_ID, "Program je zagnan!", "");
}

void loop() {
  // Pocistimo niz na zacetku vsakega kroga
  dataIn = "";

  // Preberi znake iz arduina, dokler se ne pojavi newLine 
  while (NodeMcu_SoftSerial.available() > 0) {
    char incomingChar = NodeMcu_SoftSerial.read();
    if (incomingChar == '\n') {
      // Procesiraj podatke, ko je newLine in jih izpisi v serijski zaslon ter poslji na telegram
      Serial.println(dataIn);
      bot.sendMessage(CHAT_ID, dataIn);

      // Pobrisi podatke za naslednji krog/ponovitev 
      dataIn = "";
    } else {
      // zdruzi znake v nizu
      dataIn += incomingChar;
    }
  }
  // Delay za stabilnost
  delay(1000);
}

