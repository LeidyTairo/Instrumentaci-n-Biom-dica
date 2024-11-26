// #include <Arduino.h>
// #include <XSpaceBioV10.h>
// #include <XSControl.h>
// #include <XSpaceIoT.h>
// #include <WiFi.h>
// #include <FirebaseESP32.h>
// #if defined(ESP32)
//   #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
// #endif

// #include <Firebase_ESP_Client.h>
// #include "addons/TokenHelper.h"
// #include "addons/RTDBHelper.h"

// #include <Adafruit_NeoPixel.h>
// #include "DFRobotDFPlayerMini.h"

// //BPM

// const float UpperThreshold = 1.18;
// const float LowerThreshold = 1.14;
// float bpm = 0;
// bool IgnoreReading = false; 
// int pulseCount = 0;
// unsigned long pulseTimes[4] = {0, 0, 0, 0};
// unsigned long PulseInterval = 0;
// unsigned long lastBpmUpdateTime = 0;

// const int bpmBufferSize = 7;
// float bpmBuffer[bpmBufferSize] = {0};
// int bpmBufferIndex = 0;

// // Variables para Firebase
// #define WIFI_SSID "Ryoshin's cel"
// #define WIFI_PASSWORD "soypobre"
// #define API_KEY "AIzaSyDahsleW4BOMdoqPThTkhND2m6gkxKtc40"
// #define DATABASE_URL "https://smartbeat-8548b-default-rtdb.firebaseio.com"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// unsigned long sendDataPrevMillis = 0;
// int count = 0;
// bool signupOK = false;

// // Definición del flag
// bool alertaInfarto = false;

// // Definir los pines de conexión
// #define RX_PIN 16 // Conectado al TX del DFPlayer
// #define TX_PIN 17 // Conectado al RX del DFPlayer
// #define VIBRADOR_PIN 13  // Pin para el vibrador

// bool reproduciendo=0;

// HardwareSerial mySerial(2); // Serial 2 para comunicación con DFPlayer
// DFRobotDFPlayerMini myDFPlayer;

// // Configuración para lectura de Vbat
// #define VBAT_PIN 36          // Pin analógico IO36 para leer Vbat

// // Variables para el estado
// float Vbat = 0.0;            // Voltaje de la batería
// bool enableFlag = 0;         // Quinto flag para habilitar o deshabilitar los LEDs

// // Configuración de los LEDs WS2812B
// #define LED_PIN 21           // Pin conectado a los LEDs
// #define NUM_LEDS 15          // Número total de LEDs en la cadena

// Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// // Global variables to store raw and filtered ECG values
// double raw_ecg = 0;
// double filtered_ecg = 0;
// double raw_ecg2 = 0;
// double filtered_ecg2 = 0;
// double raw_ecg3 = 0;
// double filtered_ecg3 = 0;

// XSpaceBioV10Board Board;
// XSFilter Filter;

// // MODULOS
// void setColor(int red, int green, int blue) {
//   for (int i = 0; i < NUM_LEDS; i++) {
//     leds.setPixelColor(i, leds.Color(red, green, blue));
//   }
//   leds.show();               // Actualizar los LEDs con el nuevo color
// }

// void activarAlarma() {
//   // Activar el vibrador
//   digitalWrite(VIBRADOR_PIN, HIGH);

//   // Reproducir canción #1 si no se está reproduciendo ya
//   if (reproduciendo=0) {
//     myDFPlayer.play(1);
//     Serial.println("Reproduciendo alarma...");
//   }
// }

// void desactivarAlarma() {
//   // Apagar el vibrador
//   digitalWrite(VIBRADOR_PIN, LOW);

//   // Detener la reproducción del DFPlayer
//   if (reproduciendo=1) {
//     myDFPlayer.stop();
//     Serial.println("Alarma desactivada.");
//   }
// }

// void FilterTask(void *pv) {

//   while (1) {
//     // Read raw ECG voltage from AD8232 module
//     raw_ecg = Board.AD8232_GetVoltage(AD8232_XS1);
//     filtered_ecg = Filter.SecondOrderLPF(raw_ecg, 40, 0.001);

//     raw_ecg2 = Board.AD8232_GetVoltage(AD8232_XS2);
//     filtered_ecg2 = Filter.SecondOrderLPF(raw_ecg2, 40, 0.001);

//     raw_ecg3 = raw_ecg2-raw_ecg;
//     filtered_ecg3 = Filter.SecondOrderLPF(raw_ecg3, 40, 0.001);
//     // Delay task execution for 1 millisecond
//     vTaskDelay(1);
//   }
//   // Delete task (never reached)
//   vTaskDelete(NULL);
// }

// void setup() {
//   // Initialize serial communication with the computer at baud rate 115200
//   Serial.begin(115200);

//   // WiFi Connection
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   Serial.print("----Connecting to Wi-Fi----");
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(300);
//   }
//   Serial.println();
//   Serial.print("Connected with IP: ");
//   Serial.println(WiFi.localIP());

//   // Firebase Configuration
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;

//   if (Firebase.signUp(&config, &auth, "", "")) {
//     Serial.println("Firebase Sign Up Successful");
//     signupOK = true;
//   } else {
//     Serial.printf("Firebase Sign Up Failed: %s\n", config.signer.signupError.message.c_str());
//   }

//   config.token_status_callback = tokenStatusCallback;
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);

//    // Estado inicial del vibrador
//   pinMode(VIBRADOR_PIN, OUTPUT);
//   digitalWrite(VIBRADOR_PIN, LOW); // Asegurar que el vibrador esté apagado al inicio
//   mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // Configurar Serial 2

//   leds.begin();              // Inicializar la tira de LEDs
//   leds.show();               // Apagar los LEDs al inicio

//   // Configurar pin VBAT como entrada
//   pinMode(VBAT_PIN, INPUT);

//   // Perform initial setup for the XSpace Bio v1.0 board
//   Board.init();
//   // Activate the AD8232 sensor at slot XS1 to start monitoring
//   Board.AD8232_Wake(AD8232_XS1);
//   Board.AD8232_Wake(AD8232_XS2);

//   // Inicializar DFPlayer
//   if (!myDFPlayer.begin(mySerial)) { // Verificar si se comunica correctamente
//     Serial.println("Error inicializando el DFPlayer.");
//     while (true); // Detener si falla la inicialización
//   }
  
//   Serial.println("DFPlayer Mini inicializado correctamente.");
//   myDFPlayer.volume(20); // Configurar volumen (0 a 30)

//   // Create filtering task with stack size of 3000 bytes
//   xTaskCreate(FilterTask, "FilterTask", 3000, NULL, 1, NULL);
// }

// void loop() {

//   // Verifica si Firebase está listo
//   if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
//     sendDataPrevMillis = millis();

//     // Escribir un número entero en la base de datos
//     if (Firebase.setInt(fbdo, "/test/int", count)) { // Cambiado a Firebase.setInt
//       Serial.println("Firebase Integer Write PASSED");
//       Serial.println("PATH: " + fbdo.dataPath());
//     } else {
//       Serial.println("Firebase Integer Write FAILED");
//       Serial.println("REASON: " + fbdo.errorReason());
//     }
//     count++;

//   // LEDS
//   Vbat = analogRead(VBAT_PIN) * (3.3 / 4095.0) * 2; 
//   // *3.3 para convertir el ADC a voltaje, /4095 para resolución de 12 bits
//   // *2 si usas un divisor resistivo para escalar el voltaje de batería

//   Serial.print("Vbat: ");
//   Serial.println(Vbat);

//   // Verificar si los LEDs deben encenderse
//   if (enableFlag) {
//     if (Vbat > 3.6) {
//       setColor(0, 0, 255);   // Azul (Vbat > 3.6)
//     } else if (Vbat <= 3.6 ) {
//       setColor(255, 165, 0); // Naranja (Vbat < 3.6)
//     } 
//   } else {
//     setColor(0, 0, 0);       // Apagar LEDs si enableFlag es 0
//   }

//   // BPM

//   // Detección de picos en la señal ECG filtrada
//   if (filtered_ecg2 >= UpperThreshold && !IgnoreReading) {
//     for(int i = 0; i<3; i++){
//       pulseTimes[i] = pulseTimes[i + 1];
//     }
//     pulseTimes[3] = millis();
//     pulseCount++;
//     IgnoreReading = true;
//   }

//   if(filtered_ecg2 < LowerThreshold){
//     IgnoreReading = false;
//   }
//   if(pulseCount >= 4 && millis() > 2000){
//     PulseInterval = pulseTimes[3] - pulseTimes[0];
//     float tempBPM = (3.0 / (PulseInterval / 1000.0)) * 60.0;
//     if(tempBPM >= 29 && tempBPM <= 330){
//       bpm = tempBPM;
//       bpmBuffer[bpmBufferIndex] = bpm;
//       bpmBufferIndex = (bpmBufferIndex + 1) % bpmBufferSize;
//     }else{
//       // Ajustar la matriz si el valor de BPM no es correcto
//       for (int i = 0; i < 3; i++){
//         pulseTimes[i] = pulseTimes[i + 1];
//       }
//       pulseCount = 3;
//     }
//   }


// // Enviar la primera derivada (filtered_ecg)
//     if (Firebase.setFloat(fbdo, "/test/filtered_ecg", filtered_ecg)) {
//       Serial.println("Filtered ECG 1 written to Firebase successfully");
//     } else {
//       Serial.println("Failed to write Filtered ECG 1:");
//       Serial.println(fbdo.errorReason());
//     }

//     // Enviar la segunda derivada (filtered_ecg2)
//     if (Firebase.setFloat(fbdo, "/test/filtered_ecg2", filtered_ecg2)) {
//       Serial.println("Filtered ECG 2 written to Firebase successfully");
//     } else {
//       Serial.println("Failed to write Filtered ECG 2:");
//       Serial.println(fbdo.errorReason());
//     }

//     // Enviar la tercera derivada (filtered_ecg3)
//     if (Firebase.setFloat(fbdo, "/test/filtered_ecg3", filtered_ecg3)) {
//       Serial.println("Filtered ECG 3 written to Firebase successfully");
//     } else {
//       Serial.println("Failed to write Filtered ECG 3:");
//       Serial.println(fbdo.errorReason());
//     }
//   }

//     // Calcular el BPM promedio
//   float sum = 0.0;
//   int count = 0;
//   float avgBPM = 0.0;
//   for (int i = 0; i < bpmBufferSize; i++){
//     if(bpmBuffer[i] > 0){
//       sum += bpmBuffer[i];
//       count++;
//     }
//   }
//   avgBPM = (count > 0) ? avgBPM / count : 0.0;

//   // filtered ECG values to Serial Monitor
//   Serial.println((String)filtered_ecg + " " + (String)filtered_ecg2 + " " + (String)filtered_ecg3);

//   Serial.print(" | BPM Actual: ");
//   Serial.print(bpm, 1);
//   // Serial.print(" | -: ");
//   // Serial.println( avgBPM, 1);

//   //  ALARMA
//   if (alertaInfarto) {
//     activarAlarma();
//   } else {
//     desactivarAlarma();
//   }
//   // Pause the loop for 10 milliseconds to control the data rate output
//   delay(5);
// }

//-------------------------------------------------


// -------------------------
// #include <Arduino.h>
// #include <XSpaceBioV10.h>
// #include <XSControl.h>
// #include <XSpaceIoT.h>
// #include <WiFi.h> 
// #include <FirebaseESP32.h>
// #if defined(ESP32)
//   #include <WiFi.h>
// #elif defined(ESP8266)
//   #include <ESP8266WiFi.h>
// #endif
// #include <Firebase_ESP_Client.h>
// #include "addons/TokenHelper.h"
// #include "addons/RTDBHelper.h"

// // Variables para ECG y BPM
// double raw_ecg = 0;
// double filtered_ecg = 0;
// double raw_ecg2 = 0;
// double filtered_ecg2 = 0;
// double raw_ecg3 = 0;
// double filtered_ecg3 = 0;

// const float UpperThreshold = 1.18;
// const float LowerThreshold = 1.14;
// float bpm = 0;
// bool IgnoreReading = false; 
// int pulseCount = 0;
// unsigned long pulseTimes[4] = {0, 0, 0, 0};
// unsigned long PulseInterval = 0;
// unsigned long lastBpmUpdateTime = 0;

// const int bpmBufferSize = 7;
// float bpmBuffer[bpmBufferSize] = {0};
// int bpmBufferIndex = 0;

// XSpaceBioV10Board Board;
// XSFilter Filter;

// // Variables para Firebase
// #define WIFI_SSID "Ryoshin's cel"
// #define WIFI_PASSWORD "soypobre"
// #define API_KEY "AIzaSyDahsleW4BOMdoqPThTkhND2m6gkxKtc40"
// #define DATABASE_URL "https://smartbeat-8548b-default-rtdb.firebaseio.com"

// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// unsigned long sendDataPrevMillis = 0;
// int count = 0;
// bool signupOK = false;

// void FilterTask(void *pv) {
//   while (1) {
//     raw_ecg = Board.AD8232_GetVoltage(AD8232_XS1);
//     filtered_ecg = Filter.SecondOrderLPF(raw_ecg, 40, 0.001);

//     raw_ecg2 = Board.AD8232_GetVoltage(AD8232_XS2);
//     filtered_ecg2 = Filter.SecondOrderLPF(raw_ecg2, 40, 0.001);

//     raw_ecg3 = raw_ecg2 - raw_ecg;
//     filtered_ecg3 = Filter.SecondOrderLPF(raw_ecg3, 40, 0.001);

//     vTaskDelay(1);
//   }
//   vTaskDelete(NULL);
// }

// void setup() {
//   // Serial
//   Serial.begin(115200);

//   // WiFi Connection
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   Serial.print("----Connecting to Wi-Fi----");
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(300);
//   }
//   Serial.println();
//   Serial.print("Connected with IP: ");
//   Serial.println(WiFi.localIP());

//   // Firebase Configuration
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;

//   if (Firebase.signUp(&config, &auth, "", "")) {
//     Serial.println("Firebase Sign Up Successful");
//     signupOK = true;
//   } else {
//     Serial.printf("Firebase Sign Up Failed: %s\n", config.signer.signupError.message.c_str());
//   }

//   config.token_status_callback = tokenStatusCallback;
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);

//   // XSpace Bio Board
//   Board.init();
//   Board.AD8232_Wake(AD8232_XS1);
//   Board.AD8232_Wake(AD8232_XS2);

//   xTaskCreate(FilterTask, "FilterTask", 3000, NULL, 1, NULL);
// }

// void loop() {
//   // Verifica si Firebase está listo
//   if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 100 || sendDataPrevMillis == 0)) {
//     sendDataPrevMillis = millis();

//     // Escribir un número entero en la base de datos
//     if (Firebase.setInt(fbdo, "/test/int", count)) { // Cambiado a Firebase.setInt
//       Serial.println("Firebase Integer Write PASSED");
//       Serial.println("PATH: " + fbdo.dataPath());
//     } else {
//       Serial.println("Firebase Integer Write FAILED");
//       Serial.println("REASON: " + fbdo.errorReason());
//     }
//     count++;

//     // Enviar la primera derivada (filtered_ecg)
//     if (Firebase.setFloat(fbdo, "/test/filtered_ecg", filtered_ecg)) {
//       Serial.println("Filtered ECG 1 written to Firebase successfully");
//     } else {
//       Serial.println("Failed to write Filtered ECG 1:");
//       Serial.println(fbdo.errorReason());
//     }

//     // Enviar la segunda derivada (filtered_ecg2)
//     if (Firebase.setFloat(fbdo, "/test/filtered_ecg2", filtered_ecg2)) {
//       Serial.println("Filtered ECG 2 written to Firebase successfully");
//     } else {
//       Serial.println("Failed to write Filtered ECG 2:");
//       Serial.println(fbdo.errorReason());
//     }

//     // Enviar la tercera derivada (filtered_ecg3)
//     if (Firebase.setFloat(fbdo, "/test/filtered_ecg3", filtered_ecg3)) {
//       Serial.println("Filtered ECG 3 written to Firebase successfully");
//     } else {
//       Serial.println("Failed to write Filtered ECG 3:");
//       Serial.println(fbdo.errorReason());
//     }
//   }

//   // Mostrar valores ECG en el monitor serial
//   Serial.println((String)filtered_ecg + " " + (String)filtered_ecg2 + " " + (String)filtered_ecg3);
//   //delay(5);

// }

// ----------- ECG
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
const double threshold = 1.8;     // Umbral 

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