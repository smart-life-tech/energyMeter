
#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>      // Liquid Crystal I2C library
LiquidCrystal_I2C lcd(0x27, 20, 4); // Display address 0x27, I2C 20 x 4
/* Hardware Serial2 is only available on certain boards.
  https://github.com/mandulaj/PZEM-004T-v30/archive/refs/heads/master.zip
*/
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 16, 17);
#else
PZEM004Tv30 pzem(Serial2);
#endif
float
costPerHour = 1.5 ,//usd
cost = 0.0;
void setup() {
  Serial.begin(115200);
  lcd.init();         // start the lcd library
  lcd.backlight();    // turn on the lcd backlight
  lcd.clear();        // clear the cld screen
  // Uncomment in order to reset the internal energy counter
  // pzem.resetEnergy()
}

void loop() {

  Serial.print("Custom Address:");
  Serial.println(pzem.readAddress(), HEX);

  // Read the data from the sensor
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

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

  Serial.println();
  delay(2000);
}
