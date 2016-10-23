/*
 * Autor: Héctor Andrés Porras Loría.
 * 
 * Se leen valores de un registro de corrimiento entrada paralelo salida serie.
 * Los valores corresponden a rangos de temperatura digitalizados por medio de un
 * ADC Flash de 3 bits. La conversión a temperatura se realiza con las ecuaciones de
 * recta del circuito acondicionador de señal (CAS) y el sensor de temperatura TMP36.
 * 
 * Etapa    Ecuación
 * ============================
 * CAS      Vo = 12,5*Vin - 7.5
 * Sensor   Vo = 0.01*T + 0.5
 * 
 * Se obtienen los siguientes rangos.
 * 
 * Salida   Temperatura (°C)
 * =========================
 * 000      > 45
 * 001      40-45
 * 010      35-40
 * 011      30-35
 * 100      25-30
 * 101      20-25
 * 110      15-20
 * 111      < 15
 *  
 */

/* ========== Pin definition ========== */
int CLK_INH = 6;  // Clock inhibit
int CLK = 7;      // Clock
int SH_LD = 8;    // Shift/Load
int DATA = 9;     // Data

/* ========== Variables ========== */
byte value = 0;   // Contiene el valor final 
int tmp = 0;      // Contiene cada bit que se obtiene de manera serial
String msg = "";  // Mensaje a imprimir

void setup() 
{
  Serial.begin(9600); // Inicializar serial

  /* ========== Pin modes ========== */
  pinMode(CLK_INH, OUTPUT);
  pinMode(SH_LD, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, INPUT);
}

void loop() 
{
  /* Ambos pines en HIGH permiten la carga paralela al registro */
  digitalWrite(CLK_INH, HIGH);
  digitalWrite(SH_LD, HIGH);  

  delay(20);

  /* Se cambia a LOW para iniciar la carga al registro */
  digitalWrite(SH_LD, LOW);

  delay(20);

  /*
   * La carga paralela se da en la transición de LOW a HIGH del pin SH_LD
   * Para obtener la salida de manera serial se mantiene SH_LD en HIGH y
   * CLK_INH en LOW.
   */
  digitalWrite(SH_LD, HIGH);
  digitalWrite(CLK_INH, LOW);

  delay(20);

  /* 
   * Ciclo que obtiene los 3 bits y forma el valor final  
   * En la transición de LOW a HIGH del reloj el siguiente valor se 
   * desplaza a la salida.
   */
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(CLK, LOW);
    delay(20);    
    tmp = digitalRead(DATA);
    if (tmp)
    {
      value = value | (1 << (2 - i));
    }
    delay(20);
    digitalWrite(CLK, HIGH); 
    delay(20);
  }

  /* Dependiendo de la salida se forma el mensaje a imprimir */
  switch(value)
  {
    case 0:
    {
      msg = "Temperature over 45 degrees Celsius";
      break;
    }
    case 1:
    {
      msg = "Temperature between 40 and 45 degrees Celsius";
      break;
    }
    case 2:
    {
      msg = "Temperature between 35 and 40 degrees Celsius";
      break;
    }
    case 3:
    {
      msg = "Temperature between 30 and 35 degrees Celsius";
      break;
    }
    case 4:
    {
      msg = "Temperature between 25 and 30 degrees Celsius";
      break;
    }
    case 5:
    {
      msg = "Temperature between 20 and 25 degrees Celsius";
      break;
    }
    case 6:
    {
      msg = "Temperature between 15 and 20 degrees Celsius";
      break;
    }
    case 7:
    {
      msg = "Temperature below 15 degrees Celsius";
      break;
    }
  }

  /* Se imprime el mensaje y se reinicia el valor a 0 */
  Serial.println(msg);
  Serial.println("==============");
  value = 0;
   
  delay(500);
}
 
