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
float tempC;
float tempF;
int yDist;
float[] temp_history = new float[100];

void setup()
{
  /* Set the size of the window */
  size(360, 320);
  /* Init serial communication port */
  comm_port = new Serial(this, "/dev/ttyACM0", 9600);
  /* Fill tempHistory with default values */
  for (int index = 0; index < 100; index++)
  {
    temp_history[index] = 0;
  }
}

void draw()
{
  /* Get the temperature from the serial port */
  while (comm_port.available() > 0) 
  {
    tempC = comm_port.read();
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
    stroke(0);
    fill(255, 255, 255);
    rect(90, 80, 100, 100);
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
      point(90 + index, 180 - temp_history[index]); 
    }
    
    /* Write reference values */
    fill(0, 0, 0);
    textAlign(RIGHT);
    text("50 °C", 45, 25); 
    text("0 °C", 45, 187);
    /* Draw triangle pointer */
    yDist = int(160 - (160 * (tempC * 0.01)));
    stroke(0);
    triangle(75, yDist + 20, 85, yDist + 15, 85, yDist + 25);
    
    /* Write the temperature in Celsius and Farenheit */
    fill(0, 0, 0);
    textAlign(LEFT);
    text(str(int(tempC)) + " °C", 115, 37);
    tempF = ((tempC * 9) / 5) + 32;
    text(str(int(tempF)) + " °F", 115, 65);
  }
}