/*
  AgroSat - Estacao de Monitoramento Agricola
  Global Solution FIAP 2026 - Edge Computing & Computer Systems

  Sensores:
  - DHT22: temperatura e umidade do ar
  - LDR: luminosidade solar
  Atuadores:
  - LCD 16x2 I2C: status em campo
  - LED RGB: alerta visual
  - Buzzer: alerta sonoro
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22

const int LDR_PIN = A0;
const int BUZZER_PIN = 8;
const int LED_R = 9;
const int LED_G = 10;
const int LED_B = 11;

const float TEMP_GEADA = 4.0;
const float TEMP_CALOR = 35.0;
const float UMIDADE_SECA = 35.0;
const float UMIDADE_ALTA = 85.0;
const long LUX_FORTE = 80000;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setLed(int red, int green, int blue) {
  analogWrite(LED_R, red);
  analogWrite(LED_G, green);
  analogWrite(LED_B, blue);
}

long estimarLux(int leituraAnalogica) {
  return map(leituraAnalogica, 0, 1023, 0, 100000);
}

float estimarNdviProxy(long lux) {
  float ndvi = lux / 100000.0;
  return constrain(ndvi, 0.0, 1.0);
}

String classificarRisco(float temperatura, float umidade, long lux) {
  if (temperatura <= TEMP_GEADA) return "GEADA";
  if (temperatura >= TEMP_CALOR) return "CALOR";
  if (umidade <= UMIDADE_SECA && lux >= LUX_FORTE) return "SECA";
  if (umidade >= UMIDADE_ALTA) return "UMID.ALTA";
  return "NORMAL";
}

void aplicarAlerta(String risco) {
  if (risco == "NORMAL") {
    setLed(0, 180, 30);
    noTone(BUZZER_PIN);
    return;
  }

  if (risco == "GEADA") setLed(0, 80, 255);
  if (risco == "CALOR") setLed(255, 40, 0);
  if (risco == "SECA") setLed(255, 170, 0);
  if (risco == "UMID.ALTA") setLed(80, 160, 255);

  tone(BUZZER_PIN, 1200, 180);
}

void mostrarNoLcd(float temperatura, float umidade, long lux, String risco) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatura, 1);
  lcd.print(" U:");
  lcd.print(umidade, 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print(risco);
  lcd.print(" L:");
  lcd.print(lux / 1000);
  lcd.print("k");
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("AgroSat online");
  lcd.setCursor(0, 1);
  lcd.print("Edge + satelite");
  delay(2000);
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int leituraLdr = analogRead(LDR_PIN);
  long lux = estimarLux(leituraLdr);
  float ndviProxy = estimarNdviProxy(lux);

  if (isnan(temperatura) || isnan(umidade)) {
    lcd.clear();
    lcd.print("Erro no DHT22");
    setLed(255, 0, 0);
    tone(BUZZER_PIN, 600, 250);
    delay(2000);
    return;
  }

  String risco = classificarRisco(temperatura, umidade, lux);
  aplicarAlerta(risco);
  mostrarNoLcd(temperatura, umidade, lux, risco);

  Serial.print("temp=");
  Serial.print(temperatura);
  Serial.print("C, umidade=");
  Serial.print(umidade);
  Serial.print("%, lux=");
  Serial.print(lux);
  Serial.print(", ndvi_proxy=");
  Serial.print(ndviProxy, 2);
  Serial.print(", risco=");
  Serial.println(risco);

  delay(5000);
}
