

// EXP 2: LED and Relay Blinking

int ledPin = 12;      // LED connected to pin 12
int relayPin = 11;    // Relay connected to pin 11

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  digitalWrite(relayPin, HIGH);
  delay(1000);

  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, LOW);
  delay(1000);
}

Experiment 3: DHT11 Temperature & Humidity Sensor
Alternative 1 (Using DHT library)

  #include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}

Alternative 2 (Using DHT11 Library) 

#include <DHT11.h>
DHT11 dht11(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  if (result == 0) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }

  delay(2000);
}

Experiment 4: Stepper Motor Control

#include <Stepper.h>

const int stepsPerRevolution = 2048;  // For 28BYJ-48 motor
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(10); // RPM
  Serial.begin(9600);
}

void loop() {
  Serial.println("Clockwise rotation");
  myStepper.step(stepsPerRevolution);
  delay(1000);

  Serial.println("Counter-Clockwise rotation");
  myStepper.step(-stepsPerRevolution);
  delay(1000);
}


Experiment 5: IoT Data Upload to ThingSpeak

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

DHT dht(D5, DHT11);
WiFiClient client;

long myChannelNumber = 3093693;
const char myWriteAPIKey[] = "6VW1X1YCH4NMBT9O";

void setup() {
  Serial.begin(9600);
  WiFi.begin("Shravani", "Shravani@156");

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("NodeMCU Connected!");
  Serial.println(WiFi.localIP());

  dht.begin();
  ThingSpeak.begin(client);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.println("Temperature: " + (String)t);
  Serial.println("Humidity: " + (String)h);

  ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);

  delay(2000);
}

Experiment 6: Web Hosting (LED Control via Browser)

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Caffeine";       // Change this
const char* password = "12345678";   // Change this

ESP8266WebServer server(80);
const int ledPin = 2; // D4 (Built-in LED on NodeMCU)

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleLedOn);
  server.on("/off", handleLedOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Arial;text-align:center;}button{padding:15px 25px;margin:10px;font-size:18px;}</style></head>";
  html += "<body><h2>ESP8266 LED Control</h2>";
  html += "<button onclick=\"location.href='/on'\" style='background-color:green;color:white;'>LED ON</button>";
  html += "<button onclick=\"location.href='/off'\" style='background-color:red;color:white;'>LED OFF</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ON");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  Serial.println("LED OFF");
  server.sendHeader("Location", "/");
  server.send(303);
}


🔹 Experiment 7: Bluetooth Controlled LED

char Incoming_value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    Incoming_value = Serial.read();
    Serial.print(Incoming_value);
    Serial.print("\n");

    if (Incoming_value == '1')
      digitalWrite(13, HIGH);
    else if (Incoming_value == '0')
      digitalWrite(13, LOW);
  }
}


Experiment 9: Mini Project – Smart Plant Vent Controller

#include <DHT.h>
#include <Stepper.h>

#define DHTPIN 7
#define DHTTYPE DHT11
#define STEPS_PER_REV 4096

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

const float HUMIDITY_THRESHOLD = 35.0;

DHT dht(DHTPIN, DHTTYPE);
Stepper stepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(9600);
  dht.begin();
  stepper.setSpeed(10); // RPM
  Serial.println("Smart Plant Vent Controller Started");
}

void loop() {
  float humidity = dht.readHumidity();

  if (isnan(humidity)) {
    Serial.println("Failed to read humidity!");
    delay(2000);
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (humidity > HUMIDITY_THRESHOLD) {
    Serial.println("Humidity above threshold - Opening vent...");
    stepper.step(100);
  } else {
    Serial.println("Humidity below threshold - Closing vent...");
    stepper.step(-100);
  }

  delay(500);
}

