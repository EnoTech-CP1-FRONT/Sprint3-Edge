#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247

// Definições para o display LCD 16x2 I2C
// Endereço I2C do display (geralmente 0x27 ou 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Pinos do Sensor Ultrassônico do Time A
#define TRIG_PIN_A 2
#define ECHO_PIN_A 3

// Pinos do Sensor Ultrassônico do Time B
#define TRIG_PIN_B 5
#define ECHO_PIN_B 6

// Pino do Buzzer
#define BUZZER_PIN 4

// Distância máxima para acionar (em cm)
#define DISTANCIA_LIMITE 20

// Variáveis do placar
int placarTimeA = 0;
int placarTimeB = 0;

// Variáveis para debounce/intervalo dos sensores
long ultimoDisparoTimeA = 0;
long ultimoDisparoTimeB = 0;
long intervaloDisparo = 10000; // 10 segundos

// Frequências das notas do tema da Liga dos Campeões
int melodiaTimeA[] = {587, 523, 659, 587, 523, 587, 659, 587, 523, 440};
// Durações das notas
int duracoesTimeA[] = {150, 150, 150, 150, 150, 150, 300, 150, 150, 300};
int tamanhoTimeA = sizeof(melodiaTimeA) / sizeof(melodiaTimeA[0]);

// Frequências das notas do tema "We Are the Champions"
int melodiaTimeB[] = {440, 523, 587, 523, 523, 587, 523, 440, 440, 392, 440};
// Durações das notas
int duracoesTimeB[] = {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 400};
int tamanhoTimeB = sizeof(melodiaTimeB) / sizeof(melodiaTimeB[0]);

void setup() {
  Serial.begin(9600);

  // Configura os pinos dos sensores ultrassônicos
  pinMode(TRIG_PIN_A, OUTPUT);
  pinMode(ECHO_PIN_A, INPUT);
  pinMode(TRIG_PIN_B, OUTPUT);
  pinMode(ECHO_PIN_B, INPUT);

  // Inicializa o buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Inicializa o display LCD
  lcd.init();
  lcd.backlight();
  
  // Mensagem inicial no display
  lcd.setCursor(0, 0);
  lcd.print("Placar:");
  atualizarDisplay();
}

void loop() {
  long agora = millis();
  
  // Leitura do sensor do Time A
  long distanciaA = medirDistancia(TRIG_PIN_A, ECHO_PIN_A);
  if (distanciaA < DISTANCIA_LIMITE && agora - ultimoDisparoTimeA > intervaloDisparo) {
    placarTimeA++;
    ultimoDisparoTimeA = agora;
    atualizarDisplay();
    emitirSomTimeA();
  }

  // Leitura do sensor do Time B
  long distanciaB = medirDistancia(TRIG_PIN_B, ECHO_PIN_B);
  if (distanciaB < DISTANCIA_LIMITE && agora - ultimoDisparoTimeB > intervaloDisparo) {
    placarTimeB++;
    ultimoDisparoTimeB = agora;
    atualizarDisplay();
    emitirSomTimeB();
  }
}

// Função para medir a distância com o sensor ultrassônico
long medirDistancia(int trigPin, int echoPin) {
  // Limpa o pino Trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Seta o pino Trig para HIGH por 10 microsegundos para enviar o pulso
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lê o pino Echo e retorna a duração do pulso
  long duracao = pulseIn(echoPin, HIGH);

  // Calcula a distância em cm
  long distancia = duracao * 0.034 / 2;
  return distancia;
}

// Função para atualizar o placar no display LCD
void atualizarDisplay() {
  lcd.clear(); // Limpa o display para evitar números sobrepostos
  lcd.setCursor(0, 0);
  lcd.print("Time A: ");
  lcd.print(placarTimeA);

  lcd.setCursor(0, 1);
  lcd.print("Time B: ");
  lcd.print(placarTimeB);
}

void tocarMelodia(int pinoBuzzer, int *melodia, int *duracoes, int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    int frequencia = melodia[i];
    int duracao = duracoes[i];
    if (frequencia > 0) {
      tone(pinoBuzzer, frequencia, duracao);
    }
    delay(duracao);
  }
}

void emitirSomTimeA() {
  tocarMelodia(BUZZER_PIN, melodiaTimeA, duracoesTimeA, tamanhoTimeA);
}

void emitirSomTimeB() {
  tocarMelodia(BUZZER_PIN, melodiaTimeB, duracoesTimeB, tamanhoTimeB);
}