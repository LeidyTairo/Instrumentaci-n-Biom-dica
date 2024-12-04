# Instrumentación-Biomédica
Proyecto de detección de Infarto Agudo al miocardio

Codigo 3 Derivadas-BPM:

´´´ python 
#include <Arduino.h>
#include <XSpaceBioV10.h>
#include <XSControl.h>
#include <XSpaceIoT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

double raw_ecg = 0;
double filtered_ecg = 0;
double raw_ecg2 = 0;
double filtered_ecg2 = 0;
double raw_ecg3 = 0;
double filtered_ecg3 = 0;

unsigned long lastPeakTime = 0;    // Tiempo del último pico detectado
int bpm = 0;                      // Latidos
bool peakDetected = false;        // Bandera 
const double threshold = 1.8;     // Umbral para COMTEC - 2.8 para humano aprox (verificar despues de pruebas) 

XSpaceBioV10Board Board;
XSFilter Filter;

void FilterTask(void *pv) {


  while (1) {
    // Read raw ECG voltage from AD8232 module
    raw_ecg = Board.AD8232_GetVoltage(AD8232_XS1);
    filtered_ecg = Filter.SecondOrderLPF(raw_ecg, 40, 0.001);

    raw_ecg2 = Board.AD8232_GetVoltage(AD8232_XS2);
    filtered_ecg2 = Filter.SecondOrderLPF(raw_ecg2, 40, 0.001);

    raw_ecg3 = raw_ecg2-raw_ecg;
    filtered_ecg3 = Filter.SecondOrderLPF(raw_ecg3, 40, 0.001);
    // Delay task execution for 1 millisecond to acumulate preovious data 
    vTaskDelay(1);
  }
  // Delete task (never reached)
  vTaskDelete(NULL);
}

void setup() {
  // Initialize serial communication with the computer at baud rate 115200
  Serial.begin(115200);
  // Perform initial setup for the XSpace Bio v1.0 board
  Board.init();
  // Activate the AD8232 sensor at slot XS1-2 to start monitoring
  Board.AD8232_Wake(AD8232_XS1);
  Board.AD8232_Wake(AD8232_XS2);
  // Create filtering task with stack size of 3000 bytes
  xTaskCreate(FilterTask, "FilterTask", 3000, NULL, 1, NULL);
}

void loop() {
    // Detección de picos en la señal ECG filtrada
  // Detección de picos R y cálculo de BPM
  static double previousValue = 0; // Almacena el valor anterior para comparación
  unsigned long currentTime = millis();

  if (filtered_ecg2 > threshold && filtered_ecg2 > previousValue && !peakDetected) {
    peakDetected = true; // Detectar el pico R
    if (lastPeakTime > 0) {
      unsigned long interval = currentTime - lastPeakTime; // Intervalo entre picos en ms
      bpm = 60000 / interval; // Calcular BPM
    }
    lastPeakTime = currentTime; // Actualizar el tiempo del último pico
  }

  // Restablecer bandera de detección si la señal cae por debajo del umbral
  if (filtered_ecg2 < threshold ) {
    peakDetected = false;
  }

  // Almacenar el valor actual como el valor anterior para la siguiente iteración
  previousValue = filtered_ecg2;

    // Imprimir resultados filtrados y BPM en el monitor serie
  Serial.println((String)filtered_ecg + " " + (String)filtered_ecg2 + " " + (String)filtered_ecg3 + " BPM: "+ (String)bpm);

  delay(10);
}
´´´
