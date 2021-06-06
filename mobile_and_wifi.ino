/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>



#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "Group1";
const char *password = "Lukeloluoga1420";

WiFiServer server(80);



// Define own colors
// NOTE: The brightness of RGB LED is limited to 20 (global brightness scaling).
// DO NOT set it higher to avoid damaging the LED and acrylic screen.


#include "M5Atom.h"

float accX_avg = 0, accY_avg = 0, accZ_avg = 0;
int n_average = 15;
bool IMU_ready = false;


#define cBLACK 0
#define cRED 1
#define cGREEN 2
#define cBLUE 3
#define cWHITE 4
#define cCYAN 5
#define cYELLOW 6
#define cMAGENTA 7
const uint8_t RGB_colors[8][3] = {{0x00, 0x00, 0x00},
                                  {0xF0, 0x00, 0x00},
                                  {0x00, 0xF0, 0x00},
                                  {0x00, 0x00, 0xF0},
                                  {0x70, 0x70, 0x70},
                                  {0x00, 0x70, 0x70},
                                  {0x70, 0x70, 0x00},
                                  {0x70, 0x00, 0x70}};

int GRB_COLOR_WHITE = 0xffffff;
int GRB_COLOR_BLACK = 0x000000;
int GRB_COLOR_RED = 0x00ff00;
int GRB_COLOR_ORANGE = 0xa5ff00;
int GRB_COLOR_YELLOW = 0xffff00;
int GRB_COLOR_GREEN = 0xff0000;
int GRB_COLOR_BLUE = 0x0000ff;
int GRB_COLOR_PURPLE = 0x008080;



bool IMU6886Flag = false;

//#define CENTER 0
//#define LEFT 1
//#define RIGHT 2
//// if the device was tilted to the left or to the right
//int tilt_move = CENTER;
//// score reached within each level
//int score = 0;
//// game difficulty level
//int game_level = 1;


int fsm=0;
float time1=0;
float time2=1000;
int x=1;

// the difficulty of each level is coded in an array:
// 1 = correct color
// 2 = false color
// 3 = black pixel 
// 4 = white pixel
const int board_levels[13][25] = {{1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 2,2,2,2,2, 2,2,2,2,2}, // 0 (L1a)
                                  {2,2,2,2,2, 2,2,2,2,2, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1}, // 1 (L1b)
                                  {1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,2,2,2, 2,2,2,2,2}, // L2
                                  {1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 2,2,2,2,2, 2,2,2,2,2}, // L3
                                  {1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,2, 2,2,2,2,2, 2,2,2,2,2}, // L4
                                  {1,1,1,1,1, 1,1,1,1,1, 1,1,1,2,2, 2,2,2,2,2, 2,2,2,2,2}, // L5
                                  {3,3,3,3,3, 1,1,1,1,1, 1,1,1,1,1, 2,2,2,2,2, 3,3,3,3,3}, // L6
                                  {3,3,3,3,3, 3,1,1,1,1, 1,1,1,2,2, 2,2,2,2,3, 3,3,3,3,3}, // L7
                                  {3,3,3,3,3, 3,3,3,1,1, 1,1,1,2,2, 2,2,3,3,3, 3,3,3,3,3}, // L8
                                  {3,3,3,3,3, 4,4,4,1,1, 1,1,1,2,2, 2,2,4,4,4, 3,3,3,3,3}, // L9
                                  {3,3,3,3,3, 4,4,4,4,1, 1,1,1,2,2, 2,4,4,4,4, 3,3,3,3,3}, // L10
                                  {4,4,4,4,4, 3,3,3,1,1, 1,1,1,2,2, 2,2,3,3,3, 4,4,4,4,4}, // L11
                                  {4,4,4,4,4, 4,4,4,4,1, 1,1,1,2,2, 2,4,4,4,4, 4,4,4,4,4}, // L12
                                 };
// the board for the actual move is filed with the data corresponding to the level
int board[25];
int board_index;
// For each level, the time, how long the board is visible (in ms)
//int level_display_time[13] = {-1, 350, 300, 250, 200, 200, 150, 100, 50, 40, 30, 20, 10};
//// For each level there is a separate reaction time requirement (in ms)
//int level_reaction_time[13] = {-1, 1500, 1250, 1000, 900, 800, 700, 600, 500, 400, 350, 350, 350};

// A buffer to hold the result of each game
// withing one level (each level contains 25 turns)
uint8_t Result_Buffer[25];
int move_count = 0;

uint8_t Display_Buffer[2 + 5 * 5 * 3];
//mode 1 variables
int colors[] = {GRB_COLOR_BLACK, GRB_COLOR_RED};
int colors1[] = {GRB_COLOR_BLACK, GRB_COLOR_GREEN};

int FirstDigit = 0;
int FirstDigitRoundedDown = 0;
int SecondDigit = 0;
int ThirdDigit = 0;
int FirstDecimalPlaceHolder = 0;
int FirstDecimal = 0;

String temC;
String temF;
String average1;




float tempF = 0;
float temp = 0;

float sum=0;
float average=0;
int count=24;

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

int *digits[13] = {zero, one, two, three, four, five, six, seven, eight, nine, decimal, C, F };
float tempvalues[24]={};  
float tempC = 0;
int i=0;

//function call
void showTemp();
void ChangeUnits(); 
void displaying(int arr[], int color[]);
void mode1();
void mode2();
void mode3();
void mode4();
void mode5();
void showTempA();
void showTempC();
void showTempF();
void findaverage();
void fillDisplay(int fillColor);

//mode 3 variables

float Temp1 = 25;
float Temp2 = 30;
float Temp3 = 35;
float Temp4 = 40;

float accX = 0, accY = 0, accZ = 0;

// fill the display buffer with a solid color and diplay the buffer
// "color" is the index to the RGB_colors array
void fillScreen(uint8_t color){
    Display_Buffer[0] = 0x05;
    Display_Buffer[1] = 0x05;
    for (int i = 0; i < 25; i++){
        Display_Buffer[2 + i * 3 + 0] = RGB_colors[color][0];
        Display_Buffer[2 + i * 3 + 1] = RGB_colors[color][1];
        Display_Buffer[2 + i * 3 + 2] = RGB_colors[color][2];
    }
    M5.dis.displaybuff(Display_Buffer);
}


void setup(){


   
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
    // start the ATOM device with serial and Display
    // begin(SerialEnable, I2CEnable, DisplayEnable)
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
    
 delay(250);

}



/*
void loop(){
  
findaverage(); 
 
M5.dis.clear();


    if (IMU6886Flag){
    while(1)
    {
   M5.IMU.getAccelData(&accX, &accY, &accZ);
  // Average the acceleration data
  // simple "running average" method without storng the data in an array
  accX_avg = ((accX_avg * (n_average-1))+accX)/n_average;
  accY_avg = ((accY_avg * (n_average-1))+accY)/n_average;
  accZ_avg = ((accZ_avg * (n_average-1))+accZ)/n_average;
  
//Serial.print("x");
//Serial.println(accX_avg);
//Serial.print("y");
//Serial.println(accY_avg);
//Serial.print("z");
//Serial.println(accZ_avg);



    
  // otherwise check if the device is tilted to left or right
    if(accX_avg> 0.5)
         { // 0.5g = 45deg tilt angle
          //Serial.print("left");
         fsm++;
         break;
         }

        if(accX_avg< -0.5)
         { // 0.5g = 45deg tilt angle
          //Serial.print("Right");
         fsm--;
         break;
         }  
 

    }

         

                          
                if (fsm == 1) 
              {
              displaying(digits[1], colors1);
              delay(1000);  
                mode1();
                delay(100);
               }
          
              else if (fsm == 2) 
              {
               displaying(digits[2], colors1);
              delay(1000); 
                showTempA();
                
              }
              else if (fsm == 3)
              {
                displaying(digits[3], colors1);
                 delay(1000); 
                 mode3();
                delay(1000);
                 
              }
              else if (fsm == 4) 
               {
              displaying(digits[4], colors1);
              delay(1000); 
               mode4();
               delay(1000);
              }
               else if (fsm == 5) 
              {

               displaying(digits[5], colors1);
              delay(1000); 
               mode5();
               delay(100);
             
              }

              else 
              {
                fsm=0;
                 fillScreen(cBLACK);
              }
   
       
        }
 
  
  M5.update();

}
*/


void fillDisplay(int fillColor)
{
    for(int i = 0; i < 25; i++)
    {
        M5.dis.drawpix(i, fillColor);
    }
}

//mode1 function
void mode1()
{
 

    M5.IMU.getTempData(&tempC);
    M5.dis.clear();

    showTempC();

    

   //delay(300);
  M5.update();
}


//mode 2 function 

void mode2(){
findaverage();
showTempA();

  
}
//mode2
void findaverage(){
time1=millis();
if(time1>(time2*x) && (x<26)){
  time1=millis();
  Serial.println(time1);
  M5.IMU.getTempData(&tempC);
  tempvalues[i]=tempC;
  sum =sum+ tempvalues[i];
   average = sum / x;
   
  
   
//Serial.println("temp: ");
//Serial.println(tempvalues[i]);
//
//
//Serial.println("average: ");
//Serial.println(average);
//
//   Serial.println("x");
//     Serial.println(x);
//
//      Serial.println("i");
//     Serial.println(i);
   x++;
   i++;  
 
}

}

//mode 3
void mode3() 
{
       M5.IMU.getTempData(&tempC);
       
      
    
        if(tempC < Temp1)
        {
  
        M5.dis.drawpix(0, GRB_COLOR_PURPLE); // turn on color
        M5.dis.drawpix(1, GRB_COLOR_BLUE);
        M5.dis.drawpix(2, GRB_COLOR_YELLOW);
        M5.dis.drawpix(3, GRB_COLOR_ORANGE);
        M5.dis.drawpix(4, GRB_COLOR_RED);

         for (int i = 5; i < 25; i++)
      {
        M5.dis.drawpix(i, GRB_COLOR_PURPLE);
      }
        }
        
        else if(tempC < Temp2 && tempC>Temp1)
        {
 
       M5.dis.drawpix(0, GRB_COLOR_PURPLE); // turn on color
        M5.dis.drawpix(1, GRB_COLOR_BLUE);
        M5.dis.drawpix(2, GRB_COLOR_YELLOW);
        M5.dis.drawpix(3, GRB_COLOR_ORANGE);
        M5.dis.drawpix(4, GRB_COLOR_RED);

           for (int i = 5; i < 25; i++)
      {
        M5.dis.drawpix(i, GRB_COLOR_BLUE);
      }
        }
        else if(tempC < Temp3 && tempC>Temp2)
        {
     
      M5.dis.drawpix(0, GRB_COLOR_PURPLE); // turn on color
        M5.dis.drawpix(1, GRB_COLOR_BLUE);
        M5.dis.drawpix(2, GRB_COLOR_YELLOW);
        M5.dis.drawpix(3, GRB_COLOR_ORANGE);
        M5.dis.drawpix(4, GRB_COLOR_RED);

             for (int i = 5; i < 25; i++)
      {
        M5.dis.drawpix(i, GRB_COLOR_YELLOW);
      }
        }
        else if(tempC < Temp4 && tempC>Temp3)
        {
        M5.dis.drawpix(0, GRB_COLOR_PURPLE); // turn on color
        M5.dis.drawpix(1, GRB_COLOR_BLUE);
        M5.dis.drawpix(2, GRB_COLOR_YELLOW);
        M5.dis.drawpix(3, GRB_COLOR_ORANGE);
        M5.dis.drawpix(4, GRB_COLOR_RED);

                for (int i = 5; i < 25; i++)
      {
        M5.dis.drawpix(i, GRB_COLOR_ORANGE);
      }
        }
        else
        {
        M5.dis.drawpix(0, GRB_COLOR_PURPLE); // turn on color
        M5.dis.drawpix(1, GRB_COLOR_BLUE);
        M5.dis.drawpix(2, GRB_COLOR_YELLOW);
        M5.dis.drawpix(3, GRB_COLOR_ORANGE);
        M5.dis.drawpix(4, GRB_COLOR_RED);
        
        for (int i = 5; i < 25; i++)
         {
        M5.dis.drawpix(i, GRB_COLOR_RED);
        }
        }
        
        delay(500);
        M5.update();
    }

void mode4() 
{
         //for loop, 1 to 24
          //establish categories for a specific element of the array
          //drawpix(i,color)
    for(int k=0;k<24;k++){
      
      //Serial.println("temp");
     //Serial.println(tempvalues[k]);
     if(tempvalues[k] < Temp1)
        {
        M5.dis.drawpix(k+1, GRB_COLOR_PURPLE);
      
        }
        
        else if(tempvalues[k]  < Temp2 && tempvalues[k] >Temp1)
        {
 
     
        M5.dis.drawpix(k+1, GRB_COLOR_BLUE);
        }
        
        else if(tempvalues[k]  < Temp3 && tempvalues[k] >Temp2)
        {
    
        M5.dis.drawpix(k+1, GRB_COLOR_YELLOW);
         }

      
        
        else if(tempvalues[k]  < Temp4 && tempvalues[k] >Temp3)
        {
       
        M5.dis.drawpix(k+1, GRB_COLOR_ORANGE);
        }
        
        else
        {
        M5.dis.drawpix(k+1, GRB_COLOR_RED);
        }
        

    }
        
        delay(500);
        M5.update();
    }

    

void mode5()
{
    M5.IMU.getTempData(&tempC);
    M5.dis.clear();

   
    //findaverage();
    //showTempC();

    temp = tempC;
    ChangeUnits();
    showTempF();

  delay(300);
  M5.update();
}

void showTempA()
{
  FirstDigit = floor(average / 10);
  FirstDigitRoundedDown = FirstDigit * 10;
  SecondDigit = floor(average - FirstDigitRoundedDown);
  FirstDecimalPlaceHolder = floor(average);

  FirstDecimal = 10 * (average - FirstDecimalPlaceHolder);

  //Serial.println(average);


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

void showTempC()
{
  FirstDigit = floor(tempC / 10);
  FirstDigitRoundedDown = FirstDigit * 10;
  SecondDigit = floor(tempC - FirstDigitRoundedDown);
  FirstDecimalPlaceHolder = floor(tempC);

  FirstDecimal = 10 * (tempC - FirstDecimalPlaceHolder);

  //Serial.println(tempC);


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

void showTempF()
{

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

void ChangeUnits() {

  tempF = tempC * 9 / 5 + 32;
  temp = tempF;
 

}




void loop() {
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
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<h1>Welcome to the Weather Station</h1>"); //INTERESTING PART 1
            client.print("Click <a href=\"/H\">here</a> to show the current temperature in Degrees centrigrade.<br>");
            client.print("Click <a href=\"/L\">here</a> to show the average temperature in Degrees centrigrade.<br>");
            client.print("Click <a href=\"/T\">here</a> to show the current temperature in Degrees Fahrenheit.<br>");
           //style
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
           
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          //INTERESTING PART 2
          Serial.println("I AM HIGH");
       
             mode1();
           temC.concat(tempC);
          client.print("<p>The current temperature is: " + temC + "  degrees C.</p>");
           temC.clear();
          
          }
        if (currentLine.endsWith("GET /L")) {
          //INTERESTING PART 3
          Serial.println("I AM LOW");
         
            mode2();
           average1.concat(average);
          client.print("<p>This average temperature is : " + average1 + " degrees C.</p>");
         average1.clear();
          

        }

         if (currentLine.endsWith("GET /T")) {
          //INTERESTING PART 2
          Serial.println("I AM HIGH");
             mode5();
           temF.concat(temp);
          client.print("<p>This current temperature is : " + temF + " degrees F.</p>");
          temF.clear();
          }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
