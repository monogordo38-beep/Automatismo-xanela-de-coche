/* Título: Práctica 05 - Automatismo xanela de coche
 Autor: Brais Gude Martínez
 Data: 06/02/2026
 Descrición: Este programa simula o control dunha xanela de coche mediante dous motores CC e dous relés.

 Funcionamento: 
 1. Unha pulsación activa o motor nunha dirección (SUBIR).
 2. A seguinte pulsación activa o motor na dirección contraria (BAIXAR).

 Condicións (AND):
 1. O motor solo funciona durante 7 segundos (interpretando o límite físico da xanela)
 2. Usase un bucle while non bloqueante que permite deter e cambiar a marcha inmediatamente se se preme o botón mentres se move.

 Anotación:
 O arduino interpreta se hai una pulsación moi breve que pare todo. Só se se mantén pulsado por 1 segundo ou mais
 arranca o outro motor. 
 */

#define BOTON 9 
#define SUBIR 4  
#define BAIXAR 5 

// Millis é demasiado grande e desborda un int. Usase unsigned long. 7s funcionamento máximo.
#define TEMPO_MAX 7000

bool tocaSubir = true; // Memoria de estado: controla se a seguinte acción é subir (true) ou baixar (false)

void activarSubida();
void activarBaixada();
void paroTotal();

void setup() {
 
  pinMode(BOTON, INPUT); 
  pinMode(SUBIR, OUTPUT);
  pinMode(BAIXAR, OUTPUT);

  paroTotal(); // Inicia todo parado.
  
  Serial.begin(9600);
  Serial.println("Todo preparado e listo para usarse!!"); //Mensaxiño todo ready.
}

void loop() {
  // Lectura inicial do pulsador
  if (digitalRead(BOTON) == HIGH) {
    
    // Pequeno 'debounce' para evitar lecturas falsas por rebotes mecánicos
    delay(50); 
    
    if (digitalRead(BOTON) == HIGH) {
      
      // Espera ata que se levante o dedo do pulsador
      while(digitalRead(BOTON) == HIGH) {}

      // Inicio da secuencia de movemento con memoria de estado previo. Dependendo do estado anterior sube ou baixa.
      if (tocaSubir) {
        activarSubida();
        tocaSubir = false; 
        Serial.println("Xanela subindo");
      } else {
        activarBaixada();
        tocaSubir = true;
        Serial.println("Xanela baixando");
      }

      unsigned long tempoInicio = millis();
      bool interrupcionUsuario = false;

      // Bucle usando while con dous condicións controladas por unha AND
      // 1. Non pasaran 7 segundos dende o inicio e 2. O usuario NON premera o botón (interrupción).
      
      while ((millis() - tempoInicio < TEMPO_MAX) && !interrupcionUsuario) {
        
        // Dentro do bucle escoitamos CONSTANTEMENTE ao pulsador
        if (digitalRead(BOTON) == HIGH) {
           delay(50); // Debounce rápido
           if (digitalRead(BOTON) == HIGH) {
             // Se se detecta pulsación, activamos a bandeira para romper o bucle 'while'
             interrupcionUsuario = true;
             Serial.println("Parada por usuario detectada!");
           }
        }
      }

      paroTotal(); // Ao sair do bucle para todo.
      
    }
  }
}

// Marchas de baixada/subida (con seguridade) e parada total.

void activarSubida() {
  digitalWrite(BAIXAR, LOW); 
  digitalWrite(SUBIR, HIGH); 
}

void activarBaixada() {
  digitalWrite(SUBIR, LOW);  
  digitalWrite(BAIXAR, HIGH); 
}

void paroTotal() {
  digitalWrite(SUBIR, LOW);
  digitalWrite(BAIXAR, LOW);
}