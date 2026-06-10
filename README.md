# AgroSat - Edge Computing & Computer Systems

## Descrição do projeto

O AgroSat é uma estação agrícola embarcada em Arduino. Ela simula, em escala de campo, a lógica de observação da Terra usada por satélites como Sentinel-2 e Landsat.

O circuito coleta temperatura, umidade do ar e luminosidade solar. Depois cruza os dados com thresholds agronômicos e aciona alertas visuais e sonoros.

## Objetivo da solução

- Apoiar pequenos agricultores com dados locais em tempo real.
- Prevenir perdas por seca, geada, calor extremo e umidade crítica.
- Demonstrar edge computing conectado à indústria espacial.
- Reduzir decisões por intuição na irrigação e manejo.

## Componentes utilizados

- Arduino Uno
- Sensor DHT22
- Sensor LDR/fotoresistor
- Display LCD 16x2 I2C
- Buzzer
- LED RGB
- Resistores de 220 ohms
- Jumpers e protoboard

## Funcionamento

1. O DHT22 mede temperatura e umidade do ar.
2. O LDR estima a luminosidade solar em lux.
3. O Arduino calcula um `ndvi_proxy` simplificado usando a luminosidade.
4. O firmware classifica o risco como `NORMAL`, `GEADA`, `CALOR`, `SECA` ou `UMID.ALTA`.
5. O LCD mostra leituras e status.
6. O LED RGB e o buzzer sinalizam alertas.

## Thresholds usados

- Geada: temperatura menor ou igual a 4 °C.
- Calor extremo: temperatura maior ou igual a 35 °C.
- Seca: umidade menor ou igual a 35% e luminosidade forte.
- Umidade crítica: umidade maior ou igual a 85%.

## Instruções de execução

1. Abra o projeto no Wokwi.
2. Importe `sketch.ino`, `diagram.json` e `libraries.txt`.
3. Execute a simulação.
4. Altere temperatura, umidade e lux nos sensores virtuais.
5. Observe LCD, LED RGB, buzzer e Monitor Serial.

## Link Tinkercad analisado

https://www.tinkercad.com/things/kbteE1diXTA-smashing-hillar

## Integrantes

- Pedro Viana - RM 572971
- Lucas Kobayashi - RM 569905
- Jhonathan Dourado - RM 569568
- João Matheus - RM 569850
