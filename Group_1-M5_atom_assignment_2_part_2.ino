 #include "M5Atom.h"
//library for the webserver 
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>


//libary for the temperature and humidity sensor
#include <HDC2080.h>
#define ADDR 0x40
HDC2080 sensor(ADDR);

// Settind credentials for wifi
const char *ssid = "Design";
const char *password = "Designinnovation2021";
bool IMU6886Flag=false;
long var = 0;

WiFiServer server(80);

//initializing variables
float accX_avg = 0, accY_avg = 0, accZ_avg = 0;
int n_average = 15;
bool IMU_ready = false;


int GRB_COLOR_WHITE = 0xffffff;
int GRB_COLOR_BLACK = 0x000000;
int GRB_COLOR_RED = 0x00ff00;
int GRB_COLOR_ORANGE = 0xa5ff00;
int GRB_COLOR_YELLOW = 0xffff00;
int GRB_COLOR_GREEN = 0xff0000;
int GRB_COLOR_BLUE = 0x0000ff;
int GRB_COLOR_PURPLE = 0x008080;



//wifi variables
String temC;
String temF;
String Humidity1;



uint8_t Display_Buffer[2 + 5 * 5 * 3];
int colors[] = {GRB_COLOR_BLACK, GRB_COLOR_RED};
int colors1[] = {GRB_COLOR_BLACK, GRB_COLOR_BLUE};

int FirstDigit = 0;
int FirstDigitRoundedDown = 0;
int SecondDigit = 0;
int ThirdDigit = 0;
int FirstDecimalPlaceHolder = 0;
int FirstDecimal = 0;

float tempF = 0;
float temp = 0;
float temp2=0;

int one[25] =
{
  0, 0, 1, 0, 0,
  0, 1, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 1, 1, 1, 0
};

int two[25] =
{
  0, 1, 1, 1, 0,
  0, 0, 0, 0, 1,
  0, 0, 1, 1, 0,
  0, 1, 0, 0, 0,
  0, 1, 1, 1, 1
};

int three[25] =
{
  0, 1, 1, 1, 0,
  0, 0, 0, 0, 1,
  0, 0, 1, 1, 0,
  0, 0, 0, 0, 1,
  0, 1, 1, 1, 0
};

int four[25] =
{
  0, 1, 0, 1, 0,
  0, 1, 0, 1, 0,
  0, 1, 1, 1, 1,
  0, 0, 0, 1, 0,
  0, 0, 0, 1, 0
};

int five[25] =
{
  0, 1, 1, 1, 1,
  0, 1, 0, 0, 0,
  0, 1, 1, 1, 0,
  0, 0, 0, 0, 1,
  0, 1, 1, 1, 0
};

int six[25] =
{
  0, 0, 1, 1, 0,
  0, 1, 0, 0, 0,
  0, 1, 1, 1, 0,
  0, 1, 0, 0, 1,
  0, 0, 1, 1, 0
};

int seven[25] =
{
  1, 1, 1, 1, 1,
  0, 0, 0, 1, 0,
  0, 0, 1, 0, 0,
  0, 1, 0, 0, 0,
  1, 0, 0, 0, 0
};

int eight[25] =
{
  0, 1, 1, 1, 0,
  0, 1, 0, 1, 0,
  0, 0, 1, 0, 0,
  0, 1, 0, 1, 0,
  0, 1, 1, 1, 0
};

int nine[25] =
{
  0, 1, 1, 1, 0,
  0, 1, 0, 1, 0,
  0, 1, 1, 1, 0,
  0, 0, 0, 1, 0,
  0, 0, 0, 1, 0
};

int decimal[25] =
{
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 1, 0, 0
};

int zero[25] =
{
  0, 1, 1, 1, 0,
  0, 1, 0, 1, 0,
  0, 1, 0, 1, 0,
  0, 1, 0, 1, 0,
  0, 1, 1, 1, 0
};

int C[25] =
{
  1, 0, 1, 1, 1,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 1, 1
};

int F[25] =
{
  1, 0, 1, 1, 1,
  0, 0, 1, 0, 0,
  0, 0, 1, 1, 0,
  0, 0, 1, 0, 0,
  0, 0, 1, 0, 0
};

int percent[25] =
{
  0, 0, 1, 0, 1,
  0, 0, 0, 1, 0,
  0, 0, 1, 0, 0,
  0, 1, 0, 0, 0,
  1, 0, 1, 0, 0
};

int negative[25] =
{
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 1, 1, 1, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0
};


int *digits[15] = {zero, one, two, three, four, five, six, seven, eight, nine, decimal, C, F, percent, negative};
float tempvalues[24]={};  
float tempC = 0;

float temperature2 = 0;
float temperature = 0, humidity = 0;
float temperature3=0;


int i=0;

//function call
void ChangeUnits(); 
void displaying(int arr[], int color[]);
void showhumidity();
void showTempC();
void showTempF();

//setting the setup function 
void setup(){


  while (!Serial);

  // Initialize I2C communication
  sensor.begin();

  // Begin with a device reset
  sensor.reset();

  // Configure Measurements
  sensor.setMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
  sensor.setRate(ONE_HZ);                     // Set measurement frequency to 1 Hz
  sensor.setTempRes(FOURTEEN_BIT);
  sensor.setHumidRes(FOURTEEN_BIT);

  sensor.triggerMeasurement();
  delay(1000);
  sensor.setTempOffsetAdjust(128); // offset –20.62°C
  sensor.setHumidityOffsetAdjust(128); // offset –25%RH
  delay(1000);
  sensor.setTempOffsetAdjust(0); // offset 0°C
  sensor.setHumidityOffsetAdjust(0); // offset 0%RH
  delay(1000);

     M5.begin(true, false, true);
    delay(50);
    Serial.println("M5ATOM Tilt-Game");
    Serial.println("v1.2 | 02.04.2020");
    // Show startup animation
    //display_line();
    Serial.println("");
    Serial.println("INIT IMU");
    // check if IMU is ready
    IMU6886Flag = M5.IMU.Init() == 0;

  if (!IMU6886Flag)
  {
    Serial.println("Error initializing the IMU! :-(");
  }

  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("My IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
    // start the ATOM device with serial and Display
    // begin(SerialEnable, I2CEnable, DisplayEnable)
      
    
 delay(250);
 

}


//main loop function 
void loop(){


  static uint32_t t = 0;
  if (millis() - t > 2000) {
    t = millis();
    
    temperature=sensor.readTemp();
    humidity=sensor.readHumidity();
    
  }

  
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /Home" or "GET /L":


        //Home page
if (currentLine.endsWith("GET /Home")) {
          Serial.println("Home");
          
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<script type=\"text/javascript\">");
          client.println("window.onload = function() {");
          client.println("httpGetAsync(\"/Home\");");
          client.println("function httpGetAsync(theUrl) { ");
          client.println("var xmlHttp = new XMLHttpRequest();");
          client.println("var frequency_val = 1000;");
          client.println("xmlHttp.onreadystatechange = function() { ");
          client.println("if (xmlHttp.readyState == 4 && xmlHttp.status == 200)");
          client.println("document.getElementById(\"Home\").innerHTML = xmlHttp.responseText; }");
          client.println("xmlHttp.open(\"GET\", theUrl, true); ");
          client.println("xmlHttp.send(null)");
          client.println("setTimeout(function(){httpGetAsync(\"/Home\")}, frequency_val); } }");
          client.println("</script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h2 style=\"font-size: 72px; display: inline; color: black\">Weather Station: </h2> ");
          client.println("<h2 id=\"Home\" style=\"font-size: 36px; display: inline; color:maroon\"> </h2> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:gray\">Welcome to the Homepage.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:blue\">Click <a href=\"/TemperatureC\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to see the current temperature in C.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:blue\">Click <a href=\"/Humidity\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to see the relative humidity.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:blue\">Click <a href=\"/TemperatureF\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to see the current temperature in F.</p> <br>");
          client.println("</body>");
          client.println("</html>");
        }



//current temperature in C
           if (currentLine.endsWith("GET /TemperatureC")) {
          //INTERESTING PART 3
          Serial.println("TempC");
                   
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<script type=\"text/javascript\">");
          client.println("window.onload = function() {");
          client.println("httpGetAsync(\"/TemperatureC\");");
          client.println("function httpGetAsync(theUrl) { ");
          client.println("var xmlHttp = new XMLHttpRequest();");
          client.println("var frequency_val = 1000;");
          client.println("xmlHttp.onreadystatechange = function() { ");
          client.println("if (xmlHttp.readyState == 4 && xmlHttp.status == 200)");
          client.println("document.getElementById(\"TemperatureC\").innerHTML = xmlHttp.responseText; }");
          client.println("xmlHttp.open(\"GET\", theUrl, true); ");
          client.println("xmlHttp.send(null)");
          client.println("setTimeout(function(){httpGetAsync(\"/TemperatureC\")}, frequency_val); } }");
          client.println("</script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h2 style=\"font-size: 38px; display: inline; color: black\">Temperature: </h2> ");
          client.println("<h2 id=\"TemperatureC\" style=\"font-size: 36px; display: inline; color:maroon\"> </h2> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:blue\">The current temperature is: .</p> <br>");
          temC.concat(temperature);
          client.print("<p style=\"font-size: 72px; display: inline; color:blue\"> " + temC + " &deg;C.</p><br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:gray\">Click <a href=\"/m5Temperature\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to see the current temperature in &deg;C on your device.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:gray\">Click <a href=\"/Home\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to turn go back to home page.</p> <br>");
          client.println("</body>");
          client.println("</html>");
          temC.clear();
        }


     //Relative Humidity
        if (currentLine.endsWith("GET /Humidity")) {
          //INTERESTING PART 2
          Serial.println("Humidity");
               
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<script type=\"text/javascript\">");
          client.println("window.onload = function() {");
          client.println("httpGetAsync(\"/Humidity\");");
          client.println("function httpGetAsync(theUrl) { ");
          client.println("var xmlHttp = new XMLHttpRequest();");
          client.println("var frequency_val = 1000;");
          client.println("xmlHttp.onreadystatechange = function() { ");
          client.println("if (xmlHttp.readyState == 4 && xmlHttp.status == 200)");
          client.println("document.getElementById(\"Humidity\").innerHTML = xmlHttp.responseText; }");
          client.println("xmlHttp.open(\"GET\", theUrl, true); ");
          client.println("xmlHttp.send(null)");
          client.println("setTimeout(function(){httpGetAsync(\"/Humidity\")}, frequency_val); } }");
          client.println("</script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h2 style=\"font-size: 38px; display: inline; color: black\">Relative Humidity: </h2> ");
          client.println("<h2 id=\"AverageC\" style=\"font-size: 36px; display: inline; color:maroon\"> </h2> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:blue\">The current relative Humidity is: .</p> <br>");
           Humidity1.concat(humidity);   
          client.print("<p style=\"font-size: 72px; display: inline; color:blue\"> " + Humidity1 + " %.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:gray\">Click <a href=\"/m5Humidity\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to see the current humidity on your device.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:gray\">Click <a href=\"/Home\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to turn go back to home page.</p> <br>");
          client.println("</body>");
          client.println("</html>");
          Humidity1.clear();
        }


//current tempreature in F
if (currentLine.endsWith("GET /TemperatureF")) {
          //INTERESTING PART 2
          Serial.println("TempF");
                  
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<script type=\"text/javascript\">");
          client.println("window.onload = function() {");
          client.println("httpGetAsync(\"/TemperatureF\");");
          client.println("function httpGetAsync(theUrl) { ");
          client.println("var xmlHttp = new XMLHttpRequest();");
          client.println("var frequency_val = 1000;");
          client.println("xmlHttp.onreadystatechange = function() { ");
          client.println("if (xmlHttp.readyState == 4 && xmlHttp.status == 200)");
          client.println("document.getElementById(\"TemperatureF\").innerHTML = xmlHttp.responseText; }");
          client.println("xmlHttp.open(\"GET\", theUrl, true); ");
          client.println("xmlHttp.send(null)");
          client.println("setTimeout(function(){httpGetAsync(\"/TemperatureF\")}, frequency_val); } }");
          client.println("</script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h2 style=\"font-size: 38px; display: inline; color: black\">Temperature: </h2> ");
          client.println("<h2 id=\"on\" style=\"font-size: 36px; display: inline; color:maroon\"> </h2> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:blue\">The Current temperature is: .</p> <br>");
          temp2 = temperature * 9 / 5 + 32;
          temF.concat(temp2);  
          client.print("<p style=\"font-size: 72px; display: inline; color:blue\"> " + temF + " &deg;F.</p> <br>");
           client.print("<p style=\"font-size: 36px; display: inline; color:gray\">Click <a href=\"/m5TempF\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to change current temperature from &deg;C to &deg;F on your device.</p> <br>");
          client.print("<p style=\"font-size: 36px; display: inline; color:gray\">Click <a href=\"/Home\"><button style=\"font-size: 36px; display: inline; color:blue\">here</button></a> to turn go back to home page.</p> <br>"); client.println("</body>");
          client.println("</html>");
          temF.clear();
          
          
        }

        if (currentLine.endsWith("GET /m5Temperature")) {
          //INTERESTING PART 3
         
          showTempC();
          M5.dis.clear();
          
       
        }

 if (currentLine.endsWith("GET /m5Humidity")) {
          //INTERESTING PART 3
         
          showhumidity();
          M5.dis.clear();
       
        }  

         if (currentLine.endsWith("GET /m5TempF")) {
          //INTERESTING PART 3
         
          ChangeUnits();
          M5.dis.clear();
       
        }  


 
      }
    }


     // close the connection:
    client.stop();
   Serial.println("Client Disconnected.");
   
  }
}

//function to show humidity value on M5 atom screen 
void showhumidity()
{
      FirstDigit = floor(humidity / 10);
      FirstDigitRoundedDown = FirstDigit * 10;
      SecondDigit = floor(humidity - FirstDigitRoundedDown);
      FirstDecimalPlaceHolder = floor(humidity);
    
      FirstDecimal = 10 * (humidity - FirstDecimalPlaceHolder);
    
     
      displaying(digits[FirstDigit], colors);
      delay(750);
      displaying(digits[SecondDigit], colors);
      delay(750);
      displaying(digits[10], colors);
      delay(750);
      displaying(digits[FirstDecimal], colors);
      delay(750);
      displaying(digits[13], colors);
      delay(750);
}

//function to show temperature value in degrees Celsius on M5 atom screen 
void showTempC()
{
        if (temperature>= 0) {
        FirstDigit = floor(temperature / 10);
        FirstDigitRoundedDown = FirstDigit * 10;
        SecondDigit = floor(temperature - FirstDigitRoundedDown);
        FirstDecimalPlaceHolder = floor(temperature);
      
        FirstDecimal = 10 * (temperature - FirstDecimalPlaceHolder);
      
       
        displaying(digits[FirstDigit], colors);
        delay(750);
        displaying(digits[SecondDigit], colors);
        delay(750);
        displaying(digits[10], colors);
        delay(750);
        displaying(digits[FirstDecimal], colors);
        delay(750);
        displaying(digits[11], colors);
        delay(750);
        }
      
      
      if(temperature<0){
       temperature3=abs(temperature);
         FirstDigit = floor(temperature3 / 10);
        FirstDigitRoundedDown = FirstDigit * 10;
        SecondDigit = floor(temperature3 - FirstDigitRoundedDown);
        FirstDecimalPlaceHolder = floor(temperature3);
      
        FirstDecimal = 10 * (temperature3 - FirstDecimalPlaceHolder);
      
        displaying(digits[14], colors);
        delay(750);
        displaying(digits[FirstDigit], colors);
        delay(750);
        displaying(digits[SecondDigit], colors);
        delay(750);
        displaying(digits[10], colors);
        delay(750);
        displaying(digits[FirstDecimal], colors);
        delay(750);
        displaying(digits[11], colors);
        delay(750);
      }
        
}

//function to show temperature value in degree Fahrenheit on M5 atom screen 
void showTempF()
{
        if(temp<0){
    
      temperature2=abs(temp);
      FirstDigit = floor(temperature2 / 10);
      FirstDigitRoundedDown = FirstDigit * 10;
      SecondDigit = floor(temperature2 - FirstDigitRoundedDown);
      FirstDecimalPlaceHolder = floor(temperature2);
    
      FirstDecimal = 10 * (temperature2 - FirstDecimalPlaceHolder);
    
      displaying(digits[14], colors);
      delay(750);
      displaying(digits[FirstDigit], colors);
      delay(750);
      displaying(digits[SecondDigit], colors);
      delay(750);
      displaying(digits[10], colors);
      delay(750);
      displaying(digits[FirstDecimal], colors);
      delay(750);
      displaying(digits[12], colors);
      delay(750);
    }
    
      if (temp < 100) {
        FirstDigit = floor(temp / 10);
        FirstDigitRoundedDown = FirstDigit * 10;
        SecondDigit = floor(temp - FirstDigitRoundedDown);
        FirstDecimalPlaceHolder = floor(temp);
    
        FirstDecimal = 10 * (temp - FirstDecimalPlaceHolder);
    
        //Serial.println(temp);
        displaying(digits[FirstDigit], colors);
        delay(750);
        displaying(digits[SecondDigit], colors);
        delay(750);
        displaying(digits[10], colors);
        delay(750);
        displaying(digits[FirstDecimal], colors);
        delay(750);
        displaying(digits[12], colors);
        delay(750);
      }

  else {
      FirstDigit = floor(temp / 100);
      FirstDigitRoundedDown = FirstDigit * 100;
      SecondDigit = (floor(temp - FirstDigitRoundedDown)) / 10;
      ThirdDigit = floor((temp - (FirstDigitRoundedDown + SecondDigit * 10)));
  
      FirstDecimalPlaceHolder = floor(temp);
      FirstDecimal = floor(10 * (temp - (FirstDigitRoundedDown + SecondDigit * 10 + ThirdDigit)));
  
  
      //Serial.println(temp);
      displaying(digits[FirstDigit], colors);
      delay(750);
      displaying(digits[SecondDigit], colors);
      delay(750);
      displaying(digits[ThirdDigit], colors);
      delay(750);
      displaying(digits[10], colors);
      delay(750);
      displaying(digits[FirstDecimal], colors);
      delay(750);
      displaying(digits[12], colors);
      delay(750);

  }
}



void displaying(int arr[], int color[])
{
  for (int i = 0; i < 25; i++)
  {
    M5.dis.drawpix(i, color[arr[i]]);
  }
}

//function to change temperature units from degrees Celsius to degrees Fahrenheit
void ChangeUnits() {
  temp = (temperature* 9 / 5) + 32;
  showTempF();

}
     
