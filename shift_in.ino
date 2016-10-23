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
byte value = 0;         // Contiene el valor final 
int tmp = 0;            // Contiene cada bit que se obtiene de manera serial
float temperature = 0;  // Temperatura que será leída y desplegada por la interfaz en Processing

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
  /*
   * CL_INH en HIGH permiten la carga paralela al registro.
   * SH_LD inicia en HIGH para luego iniciar la carga paralela.
   * La carga paralela se hace en la transición de LOW a HIGH.
   */
  digitalWrite(CLK_INH, HIGH);
  digitalWrite(SH_LD, HIGH);  

  delay(20);

  /* Se cambia a LOW para iniciar la carga al registro */
  digitalWrite(SH_LD, LOW);

  delay(20);

  /*
   * La carga paralela se da en la transición de LOW a HIGH del pin SH_LD.
   * Para obtener la salida de manera serial se mantiene SH_LD en HIGH y
   * CLK_INH en LOW.
   */
  digitalWrite(SH_LD, HIGH);
  digitalWrite(CLK_INH, LOW);

  delay(20);

  /* 
   * Ciclo que obtiene los 3 bits y forma el valor final.
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
      temperature = 47.5;
      break;
    }
    case 1:
    {
      temperature = 42.5;
      break;
    }
    case 2:
    {
      temperature = 37.5;
      break;
    }
    case 3:
    {
      temperature = 32.5;
      break;
    }
    case 4:
    {
      temperature = 27.5;
      break;
    }
    case 5:
    {
      temperature = 22.5;
      break;
    }
    case 6:
    {
      temperature = 17.5;
      break;
    }
    case 7:
    {
      temperature = 7.5;
      break;
    }
  }

  /* Se imprime el mensaje y se reinicia el valor a 0 */
  Serial.println(temperature);
  value = 0;
   
  delay(100);
}
 
