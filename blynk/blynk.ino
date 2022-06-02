//Included libraries
#define BLYNK_PRINT Serial

#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>      // Liquid Crystal I2C library
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "BlynkSimpleEsp32.h"
LiquidCrystal_I2C lcd(0x27, 20, 4); // Display address 0x27, I2C 20 x 4
// Read the data from the sensor
float voltage = 220.0;
float current = 0.7;
float power = 9.0;
float energy = 3.0 ;
float frequency;
float pf = 10.0 ;
int timedelay = 20000L; //set timer to run get data once every minute or 20,000 miliseconds
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 16, 17);
#else
PZEM004Tv30 pzem(Serial2);
#endif

float
costPerHour = 1.5 ,//usd
cost = 0.0;
// You should get Auth Token in the Blynk App.
char auth[] = "JpgjVrRy-WKeHQGyKyfBqobQzpMWcrEa";
// Your WiFi credentials.
char ssid[] = "Wifi_Network_Here";
char pass[] = "Wifi_Password_Here";
BlynkTimer timer;
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void Sensors () //main function to read sensors and push to blynk
{
  Serial.print("Custom Address:");
  Serial.println(pzem.readAddress(), HEX);

  // Read the data from the sensor
  voltage = pzem.voltage();
  current = pzem.current();
  power = pzem.power();
  energy = pzem.energy();
  frequency = pzem.frequency();
  pf = pzem.pf();

  // Check if the data is valid
  if (isnan(voltage)) {
    Serial.println("Error reading voltage");
  } else if (isnan(current)) {
    Serial.println("Error reading current");
  } else if (isnan(power)) {
    Serial.println("Error reading power");
  } else if (isnan(energy)) {
    Serial.println("Error reading energy");
  } else if (isnan(frequency)) {
    Serial.println("Error reading frequency");
  } else if (isnan(pf)) {
    Serial.println("Error reading power factor");
  } else {

    // Print the values to the Serial console
    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy, 3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(pf);


    //This connects vales to virtual pins defined in the widgets in the Blynk app
    Blynk.virtualWrite(V5, voltage );// Send volatge to virtual pin 5
    Blynk.virtualWrite(V6, current); // Send current to virtual pin 6
    Blynk.virtualWrite(V7, power); // Send power to viryual pin 7
    Blynk.virtualWrite(V8, frequency); // Sendfreq to 8
    Blynk.virtualWrite(V9, pf); // Send pf to 9
    Blynk.virtualWrite(V10, cost); // Send cost to 10

  }
}
void setup()
{
  Serial.begin(9600); // Debug console
  Blynk.begin(auth, ssid, pass); // connect to blynk
  timer.setInterval(timedelay, Sensors); // Setup a function to be called every minute or what timedelay is set to
  Serial.begin(115200);
  lcd.init();         // start the lcd library
  lcd.backlight();    // turn on the lcd backlight
  lcd.clear();        // clear the cld screen
  // Uncomment in order to reset the internal energy counter
  // pzem.resetEnergy()
}
//Void loop should only contain blynk.run and timer
void loop() {
  Blynk.run(); // Run blynk
  timer.run(); // Initiates BlynkTimer

  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");      lcd.print(voltage);      lcd.println("V");
  lcd.print("   Current: ");      lcd.print(current);      lcd.println("A");
  lcd.setCursor(0, 1);
  lcd.print("Power: ");        lcd.print(power);        lcd.println("W");
  lcd.print("    Energy: ");       lcd.print(energy, 3);     lcd.println("kWh");
  lcd.setCursor(0, 2);
  lcd.print("Frequency: ");    lcd.print(frequency, 1); lcd.println("Hz");
  lcd.print("  PF: ");           lcd.println(pf);
  // cost of electricity per hour = kwh x time in hours
  //per usint cost = 1 kw
  //electricity consumed
  cost = energy * costPerHour;
  lcd.setCursor(0, 3);
  lcd.print("energy consumed: ");    lcd.print(cost, 1); lcd.println("USD");
}
