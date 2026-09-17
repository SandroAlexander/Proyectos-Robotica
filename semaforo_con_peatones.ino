/*
  Semáforo de autos y peatones sincronizado - Arduino UNO
  ---------------------------------------------------------
  Autos:     Rojo, Amarillo, Verde
  Peatones:  Rojo, Verde (parpadea como aviso antes de cambiar a rojo)

  Conexiones sugeridas (LED -> resistencia de 220-330 ohm -> GND):
  Auto Rojo      -> Pin 2
  Auto Amarillo  -> Pin 3
  Auto Verde     -> Pin 4
  Peatón Rojo    -> Pin 5
  Peatón Verde   -> Pin 6
*/

// Pines de los autos
const int autoRojo = 11;
const int autoAmarillo = 12;
const int autoVerde = 13;

// Pines de los peatones
const int peatonRojo = 10;
const int peatonVerde = 9;

// Tiempos de cada fase (milisegundos) - ajústalos a tu gusto
const unsigned long tVerdeAuto        = 5000; // autos avanzan
const unsigned long tAmarilloAuto     = 2000; // autos se detienen
const unsigned long tVerdePeatonFijo  = 3000; // peatones cruzan (fijo)
const unsigned long tVerdePeatonAviso = 2000; // aviso parpadeando antes de cambiar
const unsigned long tTransicionRoja   = 1000; // ambos en rojo antes de reiniciar
const unsigned long intervaloParpadeo = 250;  // velocidad del parpadeo

void setup() {
  pinMode(autoRojo, OUTPUT);
  pinMode(autoAmarillo, OUTPUT);
  pinMode(autoVerde, OUTPUT);
  pinMode(peatonRojo, OUTPUT);
  pinMode(peatonVerde, OUTPUT);
}

void loop() {
  // --- Fase 1: Autos en VERDE / Peatones en ROJO (fijo) ---
  digitalWrite(autoVerde, HIGH);
  digitalWrite(autoAmarillo, LOW);
  digitalWrite(autoRojo, LOW);
  digitalWrite(peatonRojo, HIGH);
  digitalWrite(peatonVerde, LOW);
  delay(tVerdeAuto);

  // --- Fase 2: Autos en AMARILLO / Peatones siguen en ROJO ---
  digitalWrite(autoVerde, LOW);
  digitalWrite(autoAmarillo, HIGH);
  delay(tAmarilloAuto);

  // --- Fase 3: Autos en ROJO / Peatones en VERDE fijo (pueden cruzar) ---
  digitalWrite(autoAmarillo, LOW);
  digitalWrite(autoRojo, HIGH);
  digitalWrite(peatonRojo, LOW);
  digitalWrite(peatonVerde, HIGH);
  delay(tVerdePeatonFijo);

  // --- Fase 4: Aviso - el verde del peatón PARPADEA antes de cambiar ---
  unsigned long inicio = millis();
  while (millis() - inicio < tVerdePeatonAviso) {
    digitalWrite(peatonVerde, !digitalRead(peatonVerde));
    delay(intervaloParpadeo);
  }
  digitalWrite(peatonVerde, LOW);

  // --- Fase 5: Peatón en ROJO, autos siguen en ROJO (transición de seguridad) ---
  digitalWrite(peatonRojo, HIGH);
  delay(tTransicionRoja);

  // El ciclo se reinicia (vuelve a la Fase 1)
}
