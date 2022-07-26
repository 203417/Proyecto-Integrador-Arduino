int humF;
#define sensorPin A0
void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);
}

void loop() {
  humF = analogRead(sensorPin);
  Serial.println(humF);
  delay(1000);
   if (humF >= 1000){
    Serial.println(">> EL SENSOR ESTA DECONECTADO O FUERA DEL SUELO <<");
   }
   else if (humF <1000 && humF >= 600){
    Serial.println(">> EL SUELO ESTA SECO <<");
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
    digitalWrite(LED_BUILTIN, HIGH);
   }
   else if (humF < 600 && humF >= 370){
    Serial.println(">> EL SUELO ESTA HUMEDO <<");
   }
   else if (humF < 370){
    Serial.println(">> EL SENSOR ESTA PRACTICAMENTE EN AGUA <<");
   }   
   delay(15000);// esperamos un segundo
}
