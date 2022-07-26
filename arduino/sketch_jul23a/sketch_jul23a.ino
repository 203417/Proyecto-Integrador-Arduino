
#include <HTTPClient.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoHttpClient.h>
#include <LiquidCrystal.h> 
#include <WiFi.h>
#define COLS 16 
#define ROWS 2
#define VELOCIDAD 150
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
#define TRIG_PIN 5 
#define ECHO_PIN 22 
#define Bomba 2
#define SensorPin 26
float humFN;
int humF;
String total_hum;
String nivel,humsuelo;
float duration_us, distance_cm;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

const char* ssid     = "EspWifi";
const char* password = "12345678";

void setup() {
  delay(10);
  Serial.begin(9600);
// Configuración
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(35, INPUT);
  pinMode(25, OUTPUT);
  //pinMode(26, INPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println(F("DHTxx test!"));
  lcd.begin(COLS, ROWS);
  dht.begin();
//wifi
  WiFi.begin(ssid, password);

  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }

  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  float dht111= dht1();
  float dht112= dht2();
  float hum_sue = hum_suelo();
  String niv = ultra();
  String texto_fila = ("Temperatura:"+String(dht112)+"C Humedad:"+String(dht111)+"% la humedad es de:"+String(hum_sue)+"% ");
  //String hum_su=hum_suelo();
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

    HTTPClient http;
    String datos_a_enviar = "temp=" + String(dht112) + "&hum=" + String(dht111) + "&niv="+ String(niv)+ "&hum_su="+String(hum_sue);
    
    http.begin("http://192.168.43.43/esp-post.php");        //Indicamos el destino
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Preparamos el header text/plain si solo vamos a enviar texto plano sin un paradigma llave:valor.

    int codigo_respuesta = http.POST(datos_a_enviar);   //Enviamos el post pasándole, los datos que queremos enviar. (esta función nos devuelve un código que guardamos en un int)

    if(codigo_respuesta>0){
      Serial.println("Código HTTP ► " + String(codigo_respuesta));   //Print return code

      if(codigo_respuesta == 200){
        String cuerpo_respuesta = http.getString();
        Serial.println("El servidor respondió ▼ ");
        Serial.println(cuerpo_respuesta);

      }

    }else{

     Serial.print("Error enviando POST, código: ");
     Serial.println(codigo_respuesta);

    }

    http.end();  //libero recursos

  }else{

     Serial.println("Error en la conexión WIFI");

  }
  pantalla(texto_fila);
}

String ultra(){
    //ultrasonico
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  int n=distance_cm;
    if (n >= 10){
    Serial.println("nivel de agua bajo");
    nivel = "nivel de agua bajo";
    }
    else if (n <9 && n >=5){
    Serial.println("nivel de agua medio");
    nivel = "nivel de agua medio";
    }
    else if (n < 4 && n >= 1){
    Serial.println("nivel de agua alto");
    nivel = "nivel de agua alto";
    }
  delay(500);
  //ultrasonico
  return nivel;
}

float dht2(){
   //DHT112
  delay(2000); //Es un sensor lento, por lo que hay que darle tiempo.
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
  Serial.println(F("Failed to read from DHT sensor!"));
  }
  return t;
  //DHT112
}

float dht1(){
   //DHT11
  delay(2000); //Es un sensor lento, por lo que hay que darle tiempo.
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
  Serial.println(F("Failed to read from DHT sensor!"));
  }

  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.print(F("% Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C"));
  return h;
  //DHT11
}

float hum_suelo(){
   //humedad de suelo
   humF = analogRead(35);
   Serial.println(humF);

      humFN = map(humF, 4095, 0, 0, 100);
      if (humFN > 100) {
          humFN = 100;
        }
      Serial.println(humFN);
  //humedad de suelo
  return humFN;
}

String pantalla(String texto_fila){
  //LCD
  // Obtenemos el tamaño del texto
  int tam_texto=texto_fila.length();

  // Mostramos entrada texto por la izquierda
  for(int i=tam_texto; i>0 ; i--)
  {
    String texto = texto_fila.substring(i-1);

    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(0, 0);

    // Escribimos el texto
    lcd.print(texto);

    // Esperamos
    delay(VELOCIDAD);
  }

  // Desplazamos el texto hacia la derecha
  for(int i=1; i<=16;i++)
  {
    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(i, 0);

    // Escribimos el texto
    lcd.print(texto_fila);

    // Esperamos
    delay(VELOCIDAD);
  }

  // Desplazamos el texto hacia la izquierda en la segunda fila
  for(int i=16;i>=1;i--)
  {
    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(i, 1);

    // Escribimos el texto
    lcd.print(texto_fila);

    // Esperamos
    delay(VELOCIDAD);
  }

  // Mostramos salida del texto por la izquierda
  for(int i=1; i<=tam_texto ; i++)
  {
    String texto = texto_fila.substring(i-1);

    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(0, 1);

    // Escribimos el texto
    lcd.print(texto);

    // Esperamos
    delay(VELOCIDAD);
  }
  //LCD
}
