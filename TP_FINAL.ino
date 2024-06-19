#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// defines notas musicales
#define D  585
#define E  670
#define FS 740
#define G  785
// defines notas musicales

// defines pines de teclado
#define COL4 2
#define COL3 3
#define COL2 4
#define COL1 5
#define FILA4 6
#define FILA3 7
#define FILA2 8
#define FILA1 9
#define BUZZER 10
#define SENSOR_IN A0
#define SENSOR_OUT A1
// defines pines de teclado

LiquidCrystal_I2C lcd(0x20,16,2); // Display
//DistanceSensor sensor(SENSOR_OUT, SENSOR_IN); // Sensor

//------Teclado------
const byte FILA = 4;
const byte COL = 4;

char botones[FILA][COL] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinFila[FILA] = {FILA1, FILA2, FILA3, FILA4};
byte pinCol[COL] = {COL1, COL2, COL3, COL4};

Keypad teclado = Keypad(makeKeymap(botones), pinFila, pinCol, FILA, COL);
//------Teclado------


//-----Variables-----
int hora = 0,
minuto = 0,
segundo = 0,
hAlarma = 0,
mAlarma = 0,
sAlarma = 0;

unsigned long millisAhora, millisAlarma,
millisAntes = millis(), millisAntesAlarma = millis();
int estado = 0;
bool setSegundo=false, // flag que se activa al asignar los segundos
alarma=false, // alarma activada/desactivada
alarmaTexto=true;
long distanceInch; //DistanceSensor distancia como una variable LONG
//int distancia = sensor.getCM();

// 0 = Standby, 1 = Asignar hora, 2 = Asignar minuto, 3 = Asignar segundo,
// 4 = Asignar hora alarma, 5 = Asignar minuto alarma, 6 = Asignar segundo alarma,
// 7 = Activar/desactivar alarma (desactivada por defecto) 8 = Alarma sonando
//-----Variables-----

//-----Variables Ecuación-----
int resultadoEsperado = 0;
String respuestaUsuario = "";
int x = 0;
int y = 0;
int z = 0;
//-----Variables Ecuación-----

//-----Sensor-----
bool usuarioPresente = false;
unsigned long tiempoInicial = 0;
//-----Sensor-----

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER, OUTPUT);
  pinMode(SENSOR_OUT, OUTPUT);
  pinMode(SENSOR_IN, INPUT);
}

void loop() {
  char tecla = teclado.getKey();
  millisAlarma = millis();
    
  if( (millisAlarma - millisAntesAlarma >= 2000) ){
    millisAntesAlarma = millisAlarma;
  }
  
  millisAhora = millis();
  
  if( (millisAhora - millisAntes >= 1000) && (!setSegundo) ){
    segundo++;
    millisAntes = millisAhora;
    alarmaTexto = !alarmaTexto;

    if(segundo >= 60){
      segundo = 0;
      minuto++;

      if(minuto >= 60){
        minuto = 0;
        hora++;
        if(hora >= 24) hora = 0;
      }
    }
  }
  
  if(alarma 			 && // si la alarma esta activada,
     !setSegundo		 && // si no se estan ajustando los segundos
    (hora == hAlarma)    && // y la hora es correcta
    (minuto == mAlarma)  &&
    (segundo == sAlarma)) {
    lcd.clear();
    millisAlarma=0;
    estado=8; //suena la alarma
  }
  
  //------Maquina de estados------

  switch (estado) {
    case 0: // Standby (Muestra la hora actual)
      lcd.setCursor(4,0);
      if(hora < 10) lcd.print("0");
      lcd.print(hora);
      lcd.print(":");
      if(minuto < 10) lcd.print("0");
      lcd.print(minuto);
      lcd.print(":");
      if(segundo < 10) lcd.print("0");
      lcd.print(segundo);
      
       if (tecla != NO_KEY) {
         
        switch (tecla) {
          case 'A': // Asignar hhmmss
            lcd.clear();
            estado++;
            break;
          case 'B': // Asignar hhmmss alarma
            lcd.clear();
            estado = 4;
            break;
          case 'C': // Activar/desactivar alarma
            alarma = !alarma;
            lcd.clear();
            estado = 7;
            break;
        }
      }
      break;
      
    case 1: // Asignar hora
      
      lcd.setCursor(0,0);
      lcd.print("Asignar hora:");
      lcd.setCursor(0,1);
      if(hora < 10) lcd.print("0");
      lcd.print(hora);
      lcd.print(":");
      if(minuto < 10) lcd.print("0");
      lcd.print(minuto);
      lcd.print(":");
      if(segundo < 10) lcd.print("0");
      lcd.print(segundo);
      
      
      if (tecla != NO_KEY) {
        switch (tecla) {
          case 'A':
            lcd.clear();
            estado--;
            break;
          case '0'...'9':
            hora = (hora * 10) + (tecla - '0');
            if(hora >= 24) hora = 0;
            break;
          case 'D':
            lcd.clear();
            estado++;
            break;
        }
      }
      break;
      
    case 2: // Asignar minuto
     
      lcd.setCursor(0,0);
      lcd.print("Asignar minuto:");
      lcd.setCursor(0,1);
      if(hora < 10) lcd.print("0");
      lcd.print(hora);
      lcd.print(":");
      if(minuto < 10) lcd.print("0");
      lcd.print(minuto);
      lcd.print(":");
      if(segundo < 10) lcd.print("0");
      lcd.print(segundo);
      
     
      if (tecla != NO_KEY) {
        switch (tecla) {
          case 'A':
            lcd.clear();
            estado--;
            break;
          case '0'...'9':
            minuto = (minuto * 10) + (tecla - '0');
            if(minuto >= 60) minuto = 0;
            break;
          case 'D': 
            setSegundo=true; // hace que no pasen los segundos
            lcd.clear();
            estado++;
            break;
        }
      }
      break;
      
    case 3: // Asignar segundo
      
      lcd.setCursor(0,0);
      lcd.print("Asignar segundo:");
      lcd.setCursor(0,1);
      if(hora < 10) lcd.print("0");
      lcd.print(hora);
      lcd.print(":");
      if(minuto < 10) lcd.print("0");
      lcd.print(minuto);
      lcd.print(":");
      if(segundo < 10) lcd.print("0");
      lcd.print(segundo);
      
     
      if (tecla != NO_KEY) {
        switch (tecla) {
          case 'A':
            lcd.clear();
            setSegundo=false; // vuelven a pasar los segundos
            estado--;
            break;
          case '0'...'9':
            segundo = (segundo * 10) + (tecla - '0');
            if(segundo >= 60) segundo = 0;
            break;
          case 'D':
            lcd.clear();
            setSegundo=false; // idem
            estado = 0;
            break;
        }
      }
      break;
      
    case 4: // Asignar hora alarma
      
      lcd.setCursor(0,0);
      lcd.print("Hora de alarma:");
      lcd.setCursor(0,1);
      if(hAlarma < 10) lcd.print("0");
      lcd.print(hAlarma);
      lcd.print(":");
      if(mAlarma < 10) lcd.print("0");
      lcd.print(mAlarma);
      lcd.print(":");
      if(sAlarma < 10) lcd.print("0");
      lcd.print(sAlarma);
      
      
      if (tecla != NO_KEY) {
        switch (tecla) {
          case 'B':
            lcd.clear();
            estado = 0;
            break;
          case '0'...'9':
            hAlarma = (hAlarma * 10) + (tecla - '0');
            if(hAlarma >= 24) hAlarma = 0;
            break;
          case 'D':
            lcd.clear();
            estado++;
            break;
        }
      }
      break;
      
    case 5: // Asignar minuto alarma
     
      lcd.setCursor(0,0);
      lcd.print("Minuto de alarma:");
      lcd.setCursor(0,1);
      if(hAlarma < 10) lcd.print("0");
      lcd.print(hAlarma);
      lcd.print(":");
      if(mAlarma < 10) lcd.print("0");
      lcd.print(mAlarma);
      lcd.print(":");
      if(sAlarma < 10) lcd.print("0");
      lcd.print(sAlarma);
      
     
      if (tecla != NO_KEY) {
        switch (tecla) {
          case 'B':
            lcd.clear();
            estado--;
            break;
          case '0'...'9':
            mAlarma = (mAlarma * 10) + (tecla - '0');
            if(mAlarma >= 60) mAlarma = 0;
            break;
          case 'D':
            lcd.clear();
            estado++;
            break;
        }
      }
      break;
      
    case 6: // Asignar segundo alarma
      
      lcd.setCursor(0,0);
      lcd.print("Seg. de alarma:");
      lcd.setCursor(0,1);
      if(hAlarma < 10) lcd.print("0");
      lcd.print(hAlarma);
      lcd.print(":");
      if(mAlarma < 10) lcd.print("0");
      lcd.print(mAlarma);
      lcd.print(":");
      if(sAlarma < 10) lcd.print("0");
      lcd.print(sAlarma);
      
      
      if (tecla != NO_KEY) {
        switch (tecla) {
          case 'B':
            lcd.clear();
            estado--;
            break;
          case '0'...'9':
            sAlarma = (sAlarma * 10) + (tecla - '0');
            if(sAlarma >= 60) sAlarma = 0;
            break;
          case 'D':
            lcd.clear();
            alarma=true;
            estado = 7;
            break;
        }
      }
      break;
      
    case 7: // Activar/desactivar alarma
     
      if (alarma) {
        lcd.setCursor(0,0);
        lcd.print("Alarma ON a las:");
        lcd.setCursor(4,1);
        if(hAlarma < 10) lcd.print("0");
        lcd.print(hAlarma);
        lcd.print(":");
        if(mAlarma < 10) lcd.print("0");
        lcd.print(mAlarma);
        lcd.print(":");
        if(sAlarma < 10) lcd.print("0");
        lcd.print(sAlarma);
      } else {
        lcd.setCursor(3,0);      
        lcd.print("Alarma OFF");
      }
      
      
      if (((tecla != NO_KEY) && (tecla == 'C' || tecla == 'D'))) {
        lcd.clear();
        estado = 0;
      }
      break;
      
    case 8: // Alarma sonando
      
      if (alarmaTexto) {
        lcd.setCursor(5,0);
        lcd.print("ALARMA");
      } else {
        lcd.clear();
      }
      
     //tapa tapita tapon, esto seria un switch
    	tone(BUZZER, G, 250);
        if((millisAlarma - millisAntesAlarma >= 250)) tone(BUZZER, D, 100);
    	if((millisAlarma - millisAntesAlarma >= 375)) tone(BUZZER, D, 100);
   		if((millisAlarma - millisAntesAlarma >= 500)) tone(BUZZER, E, 250);
    	if((millisAlarma - millisAntesAlarma >= 750)) tone(BUZZER, D, 125);
    	if((millisAlarma - millisAntesAlarma >= 1250)) tone(BUZZER, FS, 250);
    	if((millisAlarma - millisAntesAlarma >= 1500)) tone(BUZZER, G, 250);
      
      // Esperar que el usuario esté presente frente al sensor por 6 segundos
      if (!usuarioPresente) {
        // Activar sensor y medir distancia
        digitalWrite(SENSOR_OUT, LOW);
        delayMicroseconds(2);
        digitalWrite(SENSOR_OUT, HIGH);
        delayMicroseconds(10);
        digitalWrite(SENSOR_OUT, LOW);
        long duration = pulseIn(SENSOR_IN, HIGH);
        distanceInch = duration * 0.0133 / 2;  // Formula para convertir la duracion del pulso en pulgadas
        
        // Si el usuario está a menos de 40 pulgadas (son 100cm aprox.)
        if (distanceInch <= 40) {
          usuarioPresente = true;
          tiempoInicial = millis();
        }
      }
      
      // Cuando este parado durante 6 seg muestra la ecuacion 
      if (usuarioPresente && (millis() - tiempoInicial >= 6000)) {
        // Generar ecuación aleatoria (x * y) + x
        x = random(1, 101); // x del 1 al 100
        y = random(1, 11);  // y del 1 al 10
        z = random(1,101); // z del 1 al 100
        resultadoEsperado = (x * y) + z;
        
        // Mostrar ecuación en la pantalla LCD
        lcd.clear();
        lcd.print(x);
        lcd.print(" * ");
        lcd.print(y);
        lcd.print(" + ");
        lcd.print(z);
        lcd.setCursor(0, 1);
        lcd.print(" = ?");
        
        // Esperar a que el usuario ingrese una respuesta
        respuestaUsuario = "";
        tecla = NO_KEY;
        while (tecla != '#') {
          tecla = teclado.getKey();
          if (tecla >= '0' && tecla <= '9') {
            respuestaUsuario += tecla;
            lcd.setCursor(3, 1);
            lcd.print(respuestaUsuario);
          }
        }
        
        // Comprobar si la respuesta es correcta
        if (respuestaUsuario.toInt() == resultadoEsperado) {
          alarma = false; // Desactivar alarma
          lcd.clear();
          lcd.print("Correcto!");
          delay(2000); // Mostrar mensaje por 2 segundos
          lcd.clear();
          estado = 0; // Volver al estado de Standby
        } else {
          lcd.clear();
          lcd.print("Incorrecto!");
          delay(2000); // Mostrar mensaje por 2 segundos
          usuarioPresente = false; // Reiniciar la espera del usuario
          estado = 8; // Volver a mostrar la alarma y esperar al usuario
        }
      }
      break;
  }
  
  delay(50); //SAQUENLO (el miercoles)
}
