#include "ESP32-RGB64x32MatrixPanel-I2S-DMA-1.1.0.h"
#include "Fonts/FreeSans9pt7b.h"


/*
 * Play around with panel output settings on the fly.
 */
RGB64x32MatrixPanel_I2S_DMA displayTest;

void draw_stuff();

void setup() {

  
  Serial.begin(115200, SERIAL_8N1); // The default is 8 data bits, no parity, one stop bit. 
  delay(100);
  
    // OK, now we can create our matrix object
  displayTest.begin();
      

  Serial.println(".");
  delay(1000);
   Serial.println(".");
  delay(1000);
  Serial.println(".");
  delay(1000);

    draw_stuff(); // start 

}

void draw_stuff()
{
  displayTest.fillScreen(0); // Flush the screen again

 // draw an 'X' in red
  displayTest.drawLine(0, 0, displayTest.width()-1, displayTest.height()-1, displayTest.color444(15, 0, 0));
  displayTest.drawLine(displayTest.width()-1, 0, 0, displayTest.height()-1, displayTest.color444(15, 0, 0));
  delay(100);

  // draw a blue circle
  displayTest.drawCircle(10, 10, 10, displayTest.color444(0, 0, 15));
  delay(100);

  // fill a violet circle
  displayTest.fillCircle(40, 21, 10, displayTest.color444(15, 0, 15));
  delay(100);



  // put your setup code here, to run once:
  displayTest.setFont(&FreeSans9pt7b);
  // displayTest.setTextSize(2);     // size 2 == 16 pixels high
  displayTest.setTextColor(displayTest.color565(255,255,255));
  
  int16_t xOne, yOne, xPosition;
  uint16_t w, h;
  
  displayTest.getTextBounds("TEST", 0, 0, &xOne, &yOne, &w, &h);
  xPosition = displayTest.width()/2 - w/2 + 1;
  
  displayTest.setCursor(xPosition, 16);
  displayTest.print("TEST"); 

}



const int states = 6;
const int default_hub75_value[states] = {displayTest.oe1_start_x_pos, displayTest.oe1_end_x_pos, displayTest.oe2_start_x_pos, displayTest.oe2_end_x_pos, displayTest.lat_start_x_pos, displayTest.lat_end_x_pos};

int current_state = 0;
int incomingByte  = 0;
void loop() 
{ 
  //for (int i = 0; i < states; i++) { Serial.printf() }
  //Serial.println(displayTest.lat_end_x_pos);

  if (current_state > states-1) current_state = 0;

  switch (current_state)
  {
     case 0:
        Serial.println("***********");
        Serial.printf("OE 1 (Output Enable) Start x Position (default: %d):\n", default_hub75_value[0]);
        break;
     case 1:
        Serial.printf("OE 1 (Output Enable) End  x Position (default: %d):\n", default_hub75_value[1]);
        break;
     case 2:
        Serial.printf("OE 2 (Output Enable) Start x Position (default: %d):\n", default_hub75_value[2]);
        break;
     case 3:
        Serial.printf("OE 2 (Output Enable) End x Position (default: %d):\n", default_hub75_value[3]);
        break;
     case 4:
        Serial.printf("LAT (Latch Start x Position (default: %d):\n", default_hub75_value[4]);
        break;
     case 5:
        Serial.printf("LAT (Latch End x Position (default: %d):\n", default_hub75_value[5]);
        break;
  }

  while (Serial.available() == 0) {} // waiting for input

  int input_serial_count  = 0;
  int new_value           = 0;

  char input_serial_chars[8] = {0};

  // https://forum.arduino.cc/index.php?topic=396450.0
  while (true) 
  {
       incomingByte = Serial.read(); // read the incoming byte:

       //Serial.print("got: ");
       //Serial.println(incomingByte-0, DEC);
                
       if (incomingByte == '\n' && input_serial_count == 0 )
       {
         new_value = default_hub75_value[current_state];
         Serial.print("... setting to default value of "); Serial.print(new_value, DEC); Serial.println(".");
         break;
       }
       else if (incomingByte == '\n' || input_serial_count >= 3) // no need for big numbers
       {
          // Convert if we got a number
         new_value = atoi(input_serial_chars);
         Serial.print("... setting to value of "); Serial.print(new_value, DEC); Serial.println(".");      
         break;    

       } else  {
        //  Serial.print("adding to char buffer at"); Serial.println(input_serial_count, DEC);
          input_serial_chars[input_serial_count++] = incomingByte;

       }

  } // which

  // Clear buffer
  while (Serial.available() > 0) {
      Serial.read();
  }

  // Set value
  switch (current_state)
  {
    case 0:
      displayTest.oe1_start_x_pos = new_value;
      break;
    case 1:
    displayTest.oe1_end_x_pos = new_value;
      break;           
    case 2:
    displayTest.oe2_start_x_pos= new_value;
      break;           
    case 3:
    displayTest.oe2_end_x_pos= new_value;
      break;           
    case 4:
    displayTest.lat_start_x_pos= new_value;
      break;           
    case 5:
    displayTest.lat_end_x_pos  = new_value;       
      break;
}  

  Serial.println("       <Repainting>        ");

/*
  if (Serial.available() > 0) {

      incomingByte = Serial.read(); // read the incoming byte:
      Serial.print(" I received:");
      Serial.println(incomingByte);

  }
*/

      draw_stuff(); // Draw with no parameters within i2c class.

      current_state++;




}
