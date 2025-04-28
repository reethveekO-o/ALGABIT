#include <Wire.h> 

// Define variables to store simulated latitude and longitude
float latitude = 0.0;
float longitude = 0.0;

// Define the analog pins connected to the sensors
const int PH_SENSOR_PIN = A0; // pH sensor analog pin
const int LOCATION_POT_PIN = A1; // Potentiometer analog pin for location
const int BOD_POT_PIN = A2; // Potentiometer analog pin for BOD sensor

// Define ultrasonic sensor pins
const int trigPin = 2; // Trigger pin of ultrasonic sensor
const int echoPin = 3; // Echo pin of ultrasonic sensor

// Define the photodiode pins and thresholds
const int sensorPin1 = A5;    // Photodiode 1 connected to analog pin A5
const int sensorPin2 = A3;    // Photodiode 2 connected to analog pin A3
const int threshold = 80;   // Adjust threshold value as needed
bool obstacleDetected1 = false;
bool obstacleDetected2 = false;

// Define vibration motor pins
const int vibrationMotorPin1 = 4; // Vibration motor 1 connected to digital pin 4
const int vibrationMotorPin2 = 5; // Vibration motor 2 connected to digital pin 5

// Flag to control the loop execution
bool continueLoop = true;

void setup() {
  pinMode(trigPin, OUTPUT); // Set trigger pin as output
  pinMode(echoPin, INPUT); // Set echo pin as input
  pinMode(vibrationMotorPin1, OUTPUT); // Set vibration motor pin 1 as output
  pinMode(vibrationMotorPin2, OUTPUT); // Set vibration motor pin 2 as output
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  if (!continueLoop) {
    return; // Exit the loop immediately if continueLoop is false
  }
  
  // Read the value of the potentiometer connected to the pH sensor
  int phSensorValue = analogRead(PH_SENSOR_PIN);
  
  // Convert analog value to pH value (simplified, not accurate)
  float pH = map(phSensorValue, 0, 1023, 0, 14); // Assumes pH range from 0 to 14
  
  // Read the value of the potentiometer connected to the BOD sensor
  int bodSensorValue = analogRead(BOD_POT_PIN);
  
  // Map BOD sensor value to a range (if necessary)
  // Adjust this mapping according to the actual range of the BOD sensor
  int mappedBODValue = map(bodSensorValue, 0, 1023, 1, 20); // Adjusted to match the BOD sensor's range

  // Print time of recording and pH value to Serial Monitor
  Serial.print("Time: ");
  Serial.print(millis() / 1000); // Convert milliseconds to seconds
  Serial.print("s - pH Value: ");
  Serial.println(pH);

  // Print BOD sensor value to Serial Monitor
  Serial.print("BOD Level: ");
  Serial.print(mappedBODValue, 1); // Display BOD level with one decimal point
  Serial.println(" mg/L");

  // Check if pH value is outside the desired range
  if (pH > 9 || pH < 5 || mappedBODValue > 10) {
    // Send message if pH is too basic, too acidic, or BOD level is too high
    if (pH > 9) {
      Serial.println("The water is too basic.");
    } else if (pH < 5) {
      Serial.println("The water is too acidic.");
    }
    if (mappedBODValue > 10) {
      Serial.println("Warning! BOD levels have reached unsafe levels! Warning!");
    }
    // Call function to send GPS location and measure distance
    sendLocationAndMeasureDistance();
    continueLoop = false; // Update flag to stop the loop
  }

  delay(5000); // Delay for stability
  
  // Obstacle detection for photodiode 1
  if (!obstacleDetected1) {
    int sensorValue1 = analogRead(sensorPin1);
    if (sensorValue1 > threshold) {
      Serial.println("Obstacle detected by Photodiode 1"); // Print message to serial monitor
      delay(5000); // Wait for 5 seconds for stabilization
      obstacleDetected1 = true; // Set obstacle detected flag for photodiode 1
      activateVibrationMotor(1); // Activate the vibration motor for photodiode 1
    }
  }
  
  // Obstacle detection for photodiode 2
  if (!obstacleDetected2) {
    int sensorValue2 = analogRead(sensorPin2);
    
    if (sensorValue2 > threshold) {
      Serial.println("Obstacle detected by Photodiode 2"); // Print message to serial monitor
      delay(5000); // Wait for 5 seconds for stabilization
      obstacleDetected2 = true; // Set obstacle detected flag for photodiode 2
      activateVibrationMotor(2); // Activate the vibration motor for photodiode 2
    }
  }
}

// Function to simulate sending GPS location and measure distance
void sendLocationAndMeasureDistance() {
  // Read the value of the potentiometer connected to the location sensor
  int locationValue = analogRead(LOCATION_POT_PIN);
  
  // Map the potentiometer value (0-1023) to latitude (-90 to 90) and longitude (-180 to 180)
  latitude = map(locationValue, 0, 1023, -90, 90);
  longitude = map(locationValue, 0, 1023, -180, 180);
  
  // Print the simulated coordinates to the serial monitor
  Serial.print("Latitude: ");
  Serial.print(latitude, 6); // Print latitude with 6 decimal places
  Serial.print(", Longitude: ");
  Serial.println(longitude, 6); // Print longitude with 6 decimal places

  // Trigger the ultrasonic sensor to measure distance
  long duration, distance;
  
  // Clear trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a pulse to trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration of the pulse on echo pin
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance based on the speed of sound
  distance = duration * (34300.0 / 1000000.0) / 2;
; // Divide by 2 for one-way travel time
  
  // Print distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// Function to activate the vibration motor for a specific duration
void activateVibrationMotor(int photodiode) {
  if (photodiode == 1) {
    digitalWrite(vibrationMotorPin1, HIGH); // Activate the vibration motor for photodiode 1
    delay(3000); // Vibrate for 3 seconds
    digitalWrite(vibrationMotorPin1, LOW); // Turn off the vibration motor for photodiode 1
  } else if (photodiode == 2) {
    digitalWrite(vibrationMotorPin2, HIGH); // Activate the vibration motor for photodiode 2
    delay(3000); // Vibrate for 3 seconds
    digitalWrite(vibrationMotorPin2, LOW); // Turn off the vibration motor for photodiode 2
  }
}
