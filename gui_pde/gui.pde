/* 
  Autor: sowmith mandadi.
  Modificado por: Héctor Andrés Porras Loría.

  Interfaz gráfica que lee valores de temperatura enviados por un Arduino UNO.
  Despliega los valores para que puedan ser visualizados de un modo user-friendly
  así como también muestra un historial de los últimos 100 valores medidos.
  
  Disponible en: https://www.hackster.io/sowmith/visualising-sensor-data-using-arduino-and-processing-e707f0  
*/

/* Se importa la biblioteca de comunicación serial */
import processing.serial.*;

/* Se declaran/inicializan las variables */
Serial comm_port;
float tempC = 0;
String msg1, msg2;
int yDist;
float[] temp_history = new float[100];

void setup()
{
  /* Set the size of the window */
  size(360, 320);
  /* Init serial communication port */
  comm_port = new Serial(this, "/dev/ttyACM0", 9600);
  comm_port.bufferUntil('\n');
  /* Fill tempHistory with default values */
  for (int index = 0; index < 100; index++)
  {
    temp_history[index] = 0;
  }
  
  surface.setTitle("Sensor de temperatura TMP36");
}

void draw()
{
  /* Get the temperature from the serial port */
  while (comm_port.available() > 0) 
  {
    /* Refresh the background to clear old data */
    background(123);
    
    /* Draw the temperature rectangle */
    colorMode(RGB, 160);  // Use color mode sized for fading
    stroke(0);
    rect(49, 19, 22, 162);
    
    /* Fade red and blue within the rectangle */
    for (int colorIndex = 0; colorIndex <= 160; colorIndex++) 
    {
      stroke(160 - colorIndex, 0, colorIndex);
      line(50, colorIndex + 20, 70, colorIndex + 20);
    }
    
    /* Draw graph */
    textAlign(CENTER);
    text("Temperature history", 215, 140);
    
    stroke(0);
    fill(255, 255, 255);
    rect(120, 150, 200, 150);
    for (int index = 0; index < 100; index++)
    {
      if (index == 99)
      {
        temp_history[index] = tempC;
      }
      else
      {
        temp_history[index] = temp_history[index + 1];
      }
      float x, y;
      x = 120 + index;
      y = 300 - 2 * (temp_history[index]);
      point(x, y);
      point(x + 1, y);
      point(x, y - 1);
      point(x + 1, y - 1);
    }
    
    /* Write reference values */
    fill(0, 0, 0);
    textAlign(RIGHT);
    text("50 °C", 45, 25); 
    text("0 °C", 45, 187);
    
    /* Draw triangle pointer */
    yDist = int(160 - (160 * (tempC * 0.02)));
    stroke(0);
    triangle(75, yDist + 20, 85, yDist + 15, 85, yDist + 25);
    
    /* Write the temperature in Celsius and Farenheit */
    fill(0, 0, 0);
    textAlign(CENTER);
    
    switch((int) tempC)
    {
      case 7:
      {
        msg1 = "Temperature below 15 °C";
        msg2 = "Temperature below 59 °F";
        break;
      }
      case 17:
      {
        msg1 = "Temperature between 15 °C and 20 °C";
        msg2 = "Temperature between 59 °F and 68 °F";
        break;
      }
      case 22:
      {
        msg1 = "Temperature between 20 °C and 25 °C";
        msg2 = "Temperature between 69 °F and 77 °F";
        break;
      }
      case 27:
      {
        msg1 = "Temperature between 25 °C and 30 °C";
        msg2 = "Temperature between 77 °F and 86 °F";
        break;
      }
      case 32:
      {
        msg1 = "Temperature between 30 °C and 35 °C";
        msg2 = "Temperature between 86 °F and 95 °F";
        break;
      }
      case 37:
      {
        msg1 = "Temperature between 35 °C and 40 °C";
        msg2 = "Temperature between 95 °F and 104 °F";
        break;
      }
      case 42:
      {
        msg1 = "Temperature between 40 °C and 45 °C";
        msg2 = "Temperature between 104 °F and 113 °F";
        break;
      }
      case 47:
      {
        msg1 = "Temperature over 45 °C";
        msg2 = "Temperature over 113 °F";
        break;
      }
    }
    
    text(msg1, 220, 65);
    text(msg2, 220, 85);
  }
}

/* Processing calls this method whenever there is data on the serial port */
void serialEvent(Serial comm_port)
{
  /* Read data from the serial port */
  String inString = comm_port.readStringUntil('\n');
  if (inString != null)
  {
    inString = trim(inString);
    tempC = float(inString);
  }
}