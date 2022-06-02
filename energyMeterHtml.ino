#include <WiFi.h>
#include <Wire.h>

const char* ssid = "PTCL-BB";
const char* password = "5387c614";

WiFiServer server(80);

String header;

void setup()
{
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


  Serial.println();
  delay(500);
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("Web Client connected ");
    String request = client.readStringUntil('\r');
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    /*
      client.println("<!DOCTYPE html><html>");
      client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
      client.println("<link rel=\"icon\" href=\"data:,\">");
      client.println("</style></head><body><h1>ESP32 Web Server BMP180</h1>");
      client.println("<h2>BMP180 Barometic sensor</h2>");
      client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
      client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
      client.println(bmp.readTemperature());
      client.println(" *C</span></td></tr>");
      client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
      client.println(bmp.readPressure());
      client.println(" pa</span></td></tr>");
      client.println("<tr><td>Altitude</td><td><span class=\"sensor\">");
      client.println(bmp.readAltitude());
      client.println(" meters</span></td></tr>");
      client.println("</body></html>");
    */
    client.println(" < !DOCTYPE html > <html>");
    client.println("<head> < meta name = \"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println(" < / style > < / head > <body><h1>ENERGY METER Web Server < / h1 >");
    client.println("<h2>pzem voltage/current sensor</h2>");
    client.println(" <table><tr><th>MEASUREMENT < / th > <th> < / th > <th>  VALUE < / th > < / tr >");
    client.println("<tr><td>curent (A)</td><td></td><td><span class=\"sensor\">");
    client.println(" 44");
    client.println(" Amps</span></td></tr>");
    client.println(" <tr><td>voltage < / td > <td> < / td > <td><span class = \"sensor\">");
    client.println("245");
    client.println("volts</span></td></tr>");
    client.println("<tr><td>power < / td > <td> < / td > <td><span class = \"sensor\">");
    client.println(" 567");
    client.println("watts</span></td></tr>");
    client.println(" <tr><td>energy < / td > <td> < / td > <td><span class = \"sensor\">");
    client.println("55");
    client.println(" joules</span></td></tr>");
    client.println("< / body> < / html >");
    client.println();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
