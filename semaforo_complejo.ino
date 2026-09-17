#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

// ==========================================
// 1. CONSTANTES DE TIEMPO (en milisegundos)
// ==========================================
const unsigned long TIEMPO_AMARILLO    = 200; // Tiempo del auto en amarillo
const unsigned long TIEMPO_SEGURIDAD   = 300;  // Pausa en rojo antes de dar verde al peatón
const unsigned long TIEMPO_PEATON_BASE = 3000; // Tiempo base para que pase el peatón
const unsigned long TIEMPO_EXTENSION   = 2000; // Segundos extra si presionan el botón cruzando
const unsigned long TIEMPO_PARPADEO    = 250;  // Velocidad de parpadeo (peatón)
const int NUM_PARPADEOS = 4;                   // Cuántas veces parpadea el verde peatonal

// ==========================================
// 2. CONFIGURACIÓN DE PINES
// ==========================================
#define PIN_BOTON 2
#define PIN_VEHICULAR 6
#define PIN_PEATONAL 7

// Estados del Semáforo (usamos int para evitar el bug de TinkerCAD)
#define ESTADO_CIRCULA 0
#define ESTADO_FRENANDO 1
#define ESTADO_SEGURIDAD 2
#define ESTADO_CAMINE 3

// Inicialización de Neopixels
// Vehicular: 3 Leds (0:Rojo, 1:Amarillo, 2:Verde)
Adafruit_NeoPixel vehiculos = Adafruit_NeoPixel(3, PIN_VEHICULAR, NEO_GRB + NEO_KHZ800);
// Peatonal: 2 Leds (0:Rojo, 1:Verde)
Adafruit_NeoPixel peatones = Adafruit_NeoPixel(2, PIN_PEATONAL, NEO_GRB + NEO_KHZ800);

// Inicialización de Pantallas LCD
LiquidCrystal lcdVehiculo(13, 12, 11, 10, 9, 8);
LiquidCrystal lcdPeaton(A0, A1, A2, A3, A4, A5);

// ==========================================
// 3. VARIABLES DE CONTROL
// ==========================================
int estadoActual = ESTADO_CIRCULA;
unsigned long tiempoInicioEstado = 0;
unsigned long tiempoPeatonActual = TIEMPO_PEATON_BASE;
bool botonPresionadoPreviamente = false;

// ==========================================
// 4. FUNCIONES PRINCIPALES
// ==========================================

void setup() {
  // Configurar Neopixels
  vehiculos.begin();
  vehiculos.show();
  peatones.begin();
  peatones.show();

  // Configurar Pantallas
  lcdVehiculo.begin(16, 2);
  lcdPeaton.begin(16, 2);

  // Configurar Pulsador
  pinMode(PIN_BOTON, INPUT_PULLUP);

  // Iniciar el sistema
  cambiarEstado(ESTADO_CIRCULA);
}

void loop() {
  bool botonPresionado = (digitalRead(PIN_BOTON) == LOW);
  unsigned long tiempoActual = millis();
  unsigned long tiempoTranscurrido = tiempoActual - tiempoInicioEstado;

  // -- Máquina de Estados --
  
  if (estadoActual == ESTADO_CIRCULA) {
    // Si los autos circulan y presionan el botón, empieza a frenar
    if (botonPresionado && !botonPresionadoPreviamente) {
      cambiarEstado(ESTADO_FRENANDO);
    }
  } 
  else if (estadoActual == ESTADO_FRENANDO) {
    // Pasa de Amarillo a Rojo de seguridad
    if (tiempoTranscurrido >= TIEMPO_AMARILLO) {
      cambiarEstado(ESTADO_SEGURIDAD);
    }
  } 
  else if (estadoActual == ESTADO_SEGURIDAD) {
    // Pausa de 300ms de seguridad exclusiva
    if (tiempoTranscurrido >= TIEMPO_SEGURIDAD) {
      cambiarEstado(ESTADO_CAMINE);
    }
  } 
  else if (estadoActual == ESTADO_CAMINE) {
    // Si presionan el botón mientras cruzan, extendemos el tiempo
    if (botonPresionado && !botonPresionadoPreviamente) {
      tiempoPeatonActual += TIEMPO_EXTENSION;
    }

    // Si se acabó el tiempo del peatón
    if (tiempoTranscurrido >= tiempoPeatonActual) {
      ejecutarParpadeoPeatonal();
      cambiarEstado(ESTADO_CIRCULA);
    }
  }

  // Guardar estado del botón para detectar un solo "clic" y no dejarlo pegado
  botonPresionadoPreviamente = botonPresionado;
  delay(50); // Pequeño debounce para evitar falsos contactos del botón
}

// ==========================================
// 5. FUNCIONES DE APOYO
// ==========================================

// Usamos 'int' en vez de enum para sortear el error de compilación
void cambiarEstado(int nuevoEstado) {
  estadoActual = nuevoEstado;
  tiempoInicioEstado = millis();

  if (nuevoEstado == ESTADO_CAMINE) {
    // Reiniciar el tiempo al valor por defecto cada que empiece a cruzar un peatón
    tiempoPeatonActual = TIEMPO_PEATON_BASE; 
  }

  actualizarLuces();
  actualizarPantallas();
}

void actualizarLuces() {
  vehiculos.clear();
  peatones.clear();

  // Asignamos colores según el estado
  if (estadoActual == ESTADO_CIRCULA) {
    vehiculos.setPixelColor(2, vehiculos.Color(0, 255, 0)); // Vehículos Verde
    peatones.setPixelColor(0, peatones.Color(255, 0, 0));   // Peatones Rojo
  } 
  else if (estadoActual == ESTADO_FRENANDO) {
    vehiculos.setPixelColor(1, vehiculos.Color(255, 255, 0)); // Vehículos Amarillo
    peatones.setPixelColor(0, peatones.Color(255, 0, 0));     // Peatones Rojo
  } 
  else if (estadoActual == ESTADO_SEGURIDAD) {
    vehiculos.setPixelColor(0, vehiculos.Color(255, 0, 0));   // Vehículos Rojo
    peatones.setPixelColor(0, peatones.Color(255, 0, 0));     // Peatones Rojo (300ms)
  } 
  else if (estadoActual == ESTADO_CAMINE) {
    vehiculos.setPixelColor(0, vehiculos.Color(255, 0, 0));   // Vehículos Rojo
    peatones.setPixelColor(1, peatones.Color(0, 255, 0));     // Peatones Verde
  }

  vehiculos.show();
  peatones.show();
}

void actualizarPantallas() {
  lcdVehiculo.clear();
  lcdPeaton.clear();

  if (estadoActual == ESTADO_CIRCULA) {
    lcdVehiculo.print("Circule");
    lcdPeaton.print("Alto");
  } 
  else if (estadoActual == ESTADO_FRENANDO) {
    lcdVehiculo.print("Cambiando");
    lcdPeaton.print("Alto");
  } 
  else if (estadoActual == ESTADO_SEGURIDAD) {
    lcdVehiculo.print("Alto");
    lcdPeaton.print("Alto");
  } 
  else if (estadoActual == ESTADO_CAMINE) {
    lcdVehiculo.print("Alto");
    lcdPeaton.print("Camine");
  }
}

void ejecutarParpadeoPeatonal() {
  for (int i = 0; i < NUM_PARPADEOS; i++) {
    // Apagar verde peatonal
    peatones.setPixelColor(1, peatones.Color(0, 0, 0)); 
    peatones.show();
    delay(TIEMPO_PARPADEO);
    
    // Encender verde peatonal
    peatones.setPixelColor(1, peatones.Color(0, 255, 0)); 
    peatones.show();
    delay(TIEMPO_PARPADEO);
  }
}
