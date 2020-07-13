#include <SPI.h>
#include <WiFi.h>

int sensor = A0; 

const char ssid[] = "mySSID";
const char pass[] = "myPassword";
int keyIndex = 0; 
int status = WL_IDLE_STATUS;
WiFiServer server(80);
String readString;

void setup() {
  pinMode(2,OUTPUT);     
//  pinMode(analogInput, INPUT);
  Serial.begin(9600);  
   while (!Serial) {  
    ; 
  }
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
  
    String fv = WiFi.firmwareVersion();
    if (fv != "1.1.0") {
      Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  Serial.println("Connected to wifi");
  printWifiStatus();
}


void loop() {

  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client");
    bool currentLineIsBlank = true;
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
         }
         if (c == '\n') {          
           Serial.println(readString); //Imprime en el monitor serial
     
           client.println("HTTP/1.1 200 OK");           //envia una nueva pagina en codigo HTML
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<TITLE>Ethernet Arduino</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<hr />");
           client.println("<H1>Arduino WiFi Shield</H1>");
           client.println("<br />");  
           client.println("<H2>Control on/off</H2>");
           client.println("<br />");
           
           client.println("<br />");
           float voltage = (float)25*analogRead(A0)/1023;
           client.print("Current battery voltage: ");
           client.print(voltage);
           client.println("<br />");
           
           client.println("<br />");  
           client.println("<a href=\"/?button2on\"\"> Current flow to Battery </a> ");           // construye en la pagina cada uno de los botones
           client.println(" | | | ");
           client.println("<a href=\"/?button2off\"\"> Current flow to Panel </a><br /> ");   
          
           
           client.println("<hr />");
           client.println("<p>...</p>");  
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           client.stop();
           
           //control del arduino si un boton es presionado
                   
           if (readString.indexOf("?button2on") >0){
               digitalWrite(2, HIGH);
           }
           if (readString.indexOf("?button2off") >0){
               digitalWrite(2, LOW);
           }
                   
           }
            readString="";  
           
         }
       }
    }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
