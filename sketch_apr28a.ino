#include <DHT.h>

#define DHTPIN 2        // Pin to which DHT22 sensor is connected
#define DHTTYPE DHT22   // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

const int greenLED = 3;  // Pin for humidity  Levels
const int blueLED = 4;  // Pin for humidity LED
const int redLED = 5;  // Pin for temperature LED
const int irSensor1Pin = 6; // Pin for the first IR sensor
const int irSensor2Pin = 7; // Pin for the second IR sensor
const int buzzerPin = 8;    // Pin for the buzzer

int count = 1;  // Initialize the count of people

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(irSensor1Pin, INPUT);
  pinMode(irSensor2Pin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  // Read the state of the IR sensors
  int sensor1State = digitalRead(irSensor1Pin);
  int sensor2State = digitalRead(irSensor2Pin);

  // Update count based on sensor states
  if (sensor1State == HIGH && sensor2State == LOW) {
    count++; // Someone entered
  } else if (sensor1State == LOW && sensor2State == HIGH) {
    count--; // Someone left
  }

  // Ensure count is not negative
  count = max(0, count);

  // Print the current count
  Serial.print("No of people in the Room: ");
  Serial.println(count);

  // Check if count is zero, and activate the buzzer
  if (count == 0) {
    digitalWrite(buzzerPin, LOW); // Turn on buzzer
  } else {
    digitalWrite(buzzerPin, HIGH); // Turn off buzzer
  }

  float temperature = dht.readTemperature();  // Reading temperature in Celsius
  float humidity = dht.readHumidity();        // Reading humidity

  // Check if any reading failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C\t Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (temperature < 27.9) {
    analogWrite(redLED, 0);
    Serial.println("Alert: Turn off appliances to conserve energy!");
  } else {
    analogWrite(redLED, 255);
  }

  if (humidity > 95) {
    analogWrite(blueLED, 0);
    Serial.println("Alert: Turn off appliances to conserve energy!");
  } else {
    analogWrite(blueLED, 255);
  }

  // Set humidity LED brightness based on humidity level
  if (humidity > 94) {
    analogWrite(greenLED, 0); // Fully bright (255) for low humidity
  } else
   if (humidity >= 92 && humidity < 94) {
    analogWrite(greenLED, 128); // Half brightness (128) for moderate humidity
  } else {
    analogWrite(greenLED, 255); // Off (0) for high humidity
  }

  Serial.println("\n");
  
  delay(2000); // Delay between readings
}
