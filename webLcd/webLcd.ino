
#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>      // Liquid Crystal I2C library
#include <WiFi.h>
#include <Wire.h>

const char* ssid = "PTCL-BB";
const char* password = "5387c614";

WiFiServer server(80);

String header;

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
costPerHour = 150.0 ,//usd per hour
cost = 0.0;
void setup() {
  Serial.begin(115200);
  lcd.init();         // start the lcd library
  lcd.backlight();    // turn on the lcd backlight
  lcd.clear();        // clear the cld screen
  // Uncomment in order to reset the internal energy counter
  // pzem.resetEnergy()
  Serial.begin(9600);

  Serial.print("Connecting to Wifi Network");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi.");
  Serial.println("IP address of ESP32 is : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
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
    lcd.setCursor(0, 1);
    lcd.print("Current: ");      lcd.print(current);      lcd.println("A");
    // lcd.setCursor(0, 1);
    // lcd.print("Power: ");        lcd.print(power);        lcd.println("W");
    // lcd.print("    Energy: ");       lcd.print(energy, 3);     lcd.println("kWh");
    lcd.setCursor(0, 2);
    lcd.print("Frequency: ");    lcd.print(frequency, 1); lcd.println("Hz");
    //lcd.print("  PF: ");           lcd.println(pf);
    // cost of electricity per hour = kwh x time in hours
    //per usint cost = 1 kw
    //electricity consumed
    cost = energy * costPerHour;
    lcd.setCursor(0, 3);
    lcd.print("BILL: ");    lcd.print(cost, 1); lcd.println("USD");

  }
  Serial.println();
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("Web Client connected ");
    String request = client.readStringUntil('\r');
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.println(" < !DOCTYPE html > <html>");
    client.println("<head> < meta name = \"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println(" < / style > < / head > <body><h1>ENERGY METER Web Server < / h1 >");
    client.println("<h2>pzem voltage/current sensor</h2>");
    client.println(" <table><tr><th>MEASUREMENT < / th > <th> < / th > <th>  VALUE < / th > < / tr >");
    client.println("<tr><td>curent (A)</td><td></td><td><span class=\"sensor\">");
    client.println(current);
    client.println(" Amps</span></td></tr>");
    client.println(" <tr><td>voltage < / td > <td> < / td > <td><span class = \"sensor\">");
    client.println(voltage);
    client.println("volts</span></td></tr>");
    client.println("<tr><td>power < / td > <td> < / td > <td><span class = \"sensor\">");
    client.println(power);
    client.println("watts</span></td></tr>");
    client.println(" <tr><td>energy cost used  < / td > <td> < / td > <td><span class = \"sensor\">");
    client.println(cost);
    client.println(" joules/h</span></td></tr>");
    client.println("< / body> < / html >");
    client.println();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  delay(500);
}
