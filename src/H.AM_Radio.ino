/*
 * libraries used:
 *
 * https://github.com/etherkit/Si5351Arduino
 * https://github.com/dxinteractive/ResponsiveAnalogRead
 *
 * Built using the Pi Pico arduino environment by Earle F. Philhower, III
 *
 * https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
 *
 */

//#define DEBUG

#define XTAL_FREQ     25000000ul
#define IF_FREQUENCY  11060000ul // crystal filter centre frequency
#define SI5351_CAL    0l // not really needed!
#define PIN_SDA       4u
#define PIN_SCL       5u
#define TUNE_PIN      28u
#define PIN_REG       23u // Pico regulator
#define UNUSED_PIN_00 0u
#define UNUSED_PIN_01 1u
#define UNUSED_PIN_02 2u
#define UNUSED_PIN_03 3u
#define UNUSED_PIN_06 6u
#define UNUSED_PIN_07 7u
#define UNUSED_PIN_08 8u
#define UNUSED_PIN_09 9u
#define UNUSED_PIN_10 10u
#define UNUSED_PIN_11 11u
#define UNUSED_PIN_12 12u
#define UNUSED_PIN_13 13u
#define UNUSED_PIN_14 14u
#define UNUSED_PIN_15 15u
#define UNUSED_PIN_16 16u
#define UNUSED_PIN_17 17u
#define UNUSED_PIN_18 18u
#define UNUSED_PIN_19 19u
#define UNUSED_PIN_20 20u
#define UNUSED_PIN_21 21u
#define UNUSED_PIN_22 22u
#define UNUSED_PIN_24 24u
#define UNUSED_PIN_25 25u
#define UNUSED_PIN_26 26u
#define UNUSED_PIN_27 27u

#include <ResponsiveAnalogRead.h>
#include <si5351.h>
#include <Wire.h>

ResponsiveAnalogRead tune(TUNE_PIN,true);
Si5351 si5351;

void setup()
{
  pinMode(PIN_REG,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(UNUSED_PIN_00,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_01,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_02,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_03,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_06,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_07,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_08,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_09,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_10,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_11,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_12,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_13,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_14,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_15,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_16,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_17,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_18,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_19,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_20,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_21,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_22,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_24,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_25,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_26,INPUT_PULLDOWN);
  pinMode(UNUSED_PIN_27,INPUT_PULLDOWN);
  digitalWrite(PIN_REG,HIGH);
  digitalWrite(LED_BUILTIN,LOW);
  analogReadResolution(12);
  tune.setAnalogResolution(4096);
  Wire.setSDA(PIN_SDA);
  Wire.setSCL(PIN_SCL);
  Wire.setClock(400000ul);
  const bool si5351_found = si5351.init(SI5351_CRYSTAL_LOAD_10PF, XTAL_FREQ, SI5351_CAL);
  if (!si5351_found)
  {
    for (;;)
    {
      digitalWrite(LED_BUILTIN,HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN,LOW);
      delay(500);
    }
  }
#ifdef DEBUG
  Serial.begin(57600);
  for (int i=0;i<5;i++)
  {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(25);
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
  }
#endif
}

void loop() 
{
  volatile static uint32_t old_frequency = 0u;
  tune.update(analogRead(TUNE_PIN));
  // 20 - 4046 (506,000Hz - 1,713,800Hz)
  const uint32_t frequency = IF_FREQUENCY + 500000ul + 300ul * tune.getValue();
  if (frequency != old_frequency)
  {
    old_frequency = frequency;
    si5351.set_freq(frequency*SI5351_FREQ_MULT, SI5351_CLK1);
#ifdef DEBUG
    Serial.print(tune.getValue());
    Serial.print(":  ");
    Serial.println(frequency);
#endif
  }
}
