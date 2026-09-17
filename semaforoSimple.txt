const int ledRojo = 11;     
const int ledAmarillo = 12;     
const int ledVerde = 13;     

void setup() {
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
}


void loop() {

  // Encender y apagar LED verde
  digitalWrite(ledVerde, HIGH); 
  delay(750);

  digitalWrite(ledVerde, LOW); 
    
  // Encender y apagar LED amarillo
  digitalWrite(ledAmarillo, HIGH); 
  delay(750);

  digitalWrite(ledAmarillo, LOW); 
  
    // Encender y apagar LED rojo
  digitalWrite(ledRojo, HIGH); 
  delay(750);

  digitalWrite(ledRojo, LOW); 
}
