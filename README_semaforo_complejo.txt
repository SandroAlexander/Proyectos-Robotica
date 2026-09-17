# Instrucciones de Conexión (TinkerCAD / Físico)
Para manejar todo con un solo Arduino UNO, aprovechamos que los pines analógicos (A0 a A5) también pueden funcionar como pines digitales.

1. Luces (Tiras Neopixel)
Semáforo Vehicular (3 LEDs en serie):

Pin DIN conectado al pin digital 6 del Arduino.

5V al bus de 5V y GND al bus de GND.

Semáforo Peatonal (2 LEDs en serie):

Pin DIN conectado al pin digital 7 del Arduino.

5V al bus de 5V y GND al bus de GND.

2. Pulsador (Control de paso)
Conecta un extremo del pulsador al pin digital 2 del Arduino.

Conecta el otro extremo directamente a GND. (No necesitas resistencia, el código usa INPUT_PULLUP interno).

3. Pantalla LCD 1 (Vehicular)
Esta utiliza los pines digitales tradicionales.

Alimentación: VSS a GND, VDD a 5V.

Retroiluminación: A (Ánodo) a 5V con una resistencia de 220Ω, K (Cátodo) a GND.

Contraste (V0): Al pin central de un potenciómetro (los otros dos a 5V y GND).

Control/Datos:

RS al pin digital 13

RW a GND (¡Muy importante en TinkerCAD!)

E al pin digital 12

D4, D5, D6, D7 a los pines digitales 11, 10, 9 y 8 respectivamente.

4. Pantalla LCD 2 (Peatonal)
Para no quedarnos sin pines, conectamos esta pantalla a los pines analógicos.

Alimentación, Retroiluminación y Contraste: Idéntico a la Pantalla 1.

Control/Datos:

RS al pin analógico A0

RW a GND

E al pin analógico A1

D4, D5, D6, D7 a los pines analógicos A2, A3, A4 y A5 respectivamente.

Esta configuración separa perfectamente la lógica: el vehículo se detiene, pasan 300ms de seguridad absoluta, el peatón cruza, el peatón recibe su aviso parpadeante sin bloquear que se presione el botón (añadiendo extensión de tiempo fluidamente) y los textos se actualizan en las pantallas sin mostrar "basura" en la segunda línea.

