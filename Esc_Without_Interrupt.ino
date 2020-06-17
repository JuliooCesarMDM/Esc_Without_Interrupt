/*************************************
   AUTOR:JULIO CESAR MATIAS
   DATA:JANEIRO DE 2020
   ARDUINO IDE:VERSÃO 1.8.9
**************************************/

/*
  MUITOS UTILIZAM A BIBLIOTECA SERVO PARA FAZER O CONTROLE DE ESC'S,ISSO ESTÁ ERRADO,A BIBLIOTECA SERVO TEM PROBLEMAS
  A FORMA CORRETA É UTILIZANDO MANIPULAÇÃO DIRETA DOS REGISTRADORES OU UTILIZANDO ESTE ALGORITIMO DESENVOLVIDO POR MIM.
  ESSE SKETCH SERVE PARA CONTROLAR SERVOS TAMBÉM,COM UM DESEMPENHO MELHOR DO A BIBLIOTECA SERVO
  NO MEU GITHUB CONTÉM O SKETCH MULTISERVOS PARA O CONTROLE DE MAIS DE 1 ESC OU SERVO USANDO ESTE MESMO ALGORITIMO.
*/

//#define PulseFrequency 20000 //50Hz
//#define PulseFrequency 10000 //100Hz
#define PulseFrequency 2048  //490Hz

uint8_t  ESCSignalState;
uint8_t  ESCSignalDelay;

uint32_t ESCSignalSetTimer;
uint32_t ESCSignalRunTimer;

void setup() {
  pinMode(3, OUTPUT);
  for (uint8_t Init_Esc = 0; Init_Esc < 30; Init_Esc++) {
    GimbalSignal_pulse(1000);
    delayMicroseconds(1000);
    GimbalSignal_pulse(0);
    delayMicroseconds(3000);
  }
}

void loop() {
  //APLICA 1500 PARA O VALOR DO PULSO,METADE DA VELOCIDADE DO MOTOR (SE O SEU ESC ESTIVER CALIBRADO PARA RECEBER PULSOS ENTRE 1000 A 2000)
  GimbalSignal_pulse(1500);
}

static void GimbalSignal_pulse(uint16_t Pulse) {
  if (!ESCSignalState && !ESCSignalDelay) {
    PORTD |= 1 << 3; //PORTA 3 EM HIGH
    ESCSignalState      = 1;
    ESCSignalSetTimer   = micros();
  }
  if (ESCSignalState && !ESCSignalDelay) {
    ESCSignalRunTimer   = micros();
    if (ESCSignalRunTimer - ESCSignalSetTimer >= Pulse) {
      PORTD &= ~(1 << 3); //PORTA 3 EM LOW
      ESCSignalDelay    = 1;
      ESCSignalSetTimer = micros();
    }
  }
  if (ESCSignalDelay) {
    ESCSignalRunTimer = micros();
    if (ESCSignalRunTimer - ESCSignalSetTimer >= PulseFrequency - Pulse) {
      ESCSignalDelay  = 0;
      ESCSignalState  = 0;
    }
  }
}
