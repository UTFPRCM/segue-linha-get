/*
 * Robô Seguidor de Linha com Sensor de Cor
 *
 * Segue uma linha preta sobre fundo branco usando dois sensores
 * infravermelhos. Quando os dois sensores enxergam "preto" ao mesmo tempo,
 * o robô está sobre um marcador/cruzamento e o sensor de cor TCS3200 decide
 * a ação:
 *
 *   VERMELHO -> vira para a ESQUERDA
 *   VERDE    -> vira para a DIREITA
 *   AZUL     -> para por alguns segundos e depois segue em frente
 *   nenhuma  -> cruzamento comum, segue em frente
 *
 * Hardware: Arduino Uno, ponte H dupla, 2 motores DC com redução,
 * 2 sensores IR de reflexão e 1 sensor de cor TCS3200.
 *
 * Versão refatorada a partir de src/historico/2018_seguidor3.ino.
 */

// ===================== DEPURAÇÃO =====================
// 1 = imprime as leituras dos sensores no Monitor Serial (9600 baud).
// Use para calibrar LIMIAR_PRETO e os limiares de cor.
#define DEBUG 0

// ===================== PINOS =========================
// Ponte H: cada motor tem um pino PWM para frente e outro para ré
const uint8_t MOTOR_E_FRENTE = 11;
const uint8_t MOTOR_E_TRAS   = 10;
const uint8_t MOTOR_D_FRENTE = 6;
const uint8_t MOTOR_D_TRAS   = 5;

// Sensores IR de linha.
// No seguidor3.ino o sensor em A0 se chamava "esquerdo", mas pela reação dos
// motores ele estava montado do lado DIREITO. Se o seu robô fugir da linha
// em vez de corrigir, troque estes dois pinos.
const uint8_t SENSOR_DIREITO  = A0;
const uint8_t SENSOR_ESQUERDO = A1;

// Sensor de cor TCS3200
const uint8_t COR_S0  = 4;
const uint8_t COR_S1  = 7;
const uint8_t COR_S2  = 8;
const uint8_t COR_S3  = 12;
const uint8_t COR_OUT = 13;

// ===================== AJUSTES =======================
// Leitura analógica acima deste valor = preto
const int LIMIAR_PRETO = 500;

// Velocidades (PWM 0-255)
const int VEL_RETA    = 70;   // os dois motores na reta
const int VEL_CURVA   = 215;  // motor externo durante a correção
const int VEL_MANOBRA = 150;  // giros e travessias nos marcadores
const int VEL_FREIO   = 255;  // ré rápida para frear antes de corrigir

// Tempos (ms)
const unsigned long TEMPO_CURVA          = 40;   // pulso de correção
const unsigned long TEMPO_FREIO          = 25;   // ré para frear
const unsigned long PAUSA_TROCA_LADO     = 700;  // espera ao mudar de lado (evita zigue-zague)
const unsigned long TEMPO_FREIO_MARCADOR = 20;   // ré curta ao chegar no marcador
const unsigned long TEMPO_GIRO           = 200;  // giro no próprio eixo
const unsigned long TEMPO_AVANCO         = 300;  // avanço para sair do marcador
const unsigned long TEMPO_ASSENTAR       = 200;  // parado após a manobra
const unsigned long TEMPO_PARADA_AZUL    = 3000; // parada no marcador azul

// Sensor de cor: o TCS3200 devolve a largura do pulso, ou seja,
// quanto MENOR o valor, MAIS daquela cor. Uma cor só é aceita se for o menor
// dos três canais e estiver abaixo do seu limiar (o preto dá valores altos
// em todos os canais). Só o vermelho veio calibrado do original; ajuste os
// demais com DEBUG = 1 posicionando o sensor sobre cada marcador.
const unsigned long LIMIAR_VERMELHO = 48;
const unsigned long LIMIAR_VERDE    = 60;
const unsigned long LIMIAR_AZUL     = 60;
const uint8_t       AMOSTRAS_COR    = 3;     // leituras por canal (média)
const unsigned long TIMEOUT_COR_US  = 50000; // não trava se o sensor falhar

// =====================================================

enum Cor { NENHUMA, VERMELHO, VERDE, AZUL };
enum Estado { RETA, CURVA_ESQUERDA, CURVA_DIREITA, MARCADOR };

Estado ultimoEstado = RETA;

void setup() {
  pinMode(MOTOR_E_FRENTE, OUTPUT);
  pinMode(MOTOR_E_TRAS, OUTPUT);
  pinMode(MOTOR_D_FRENTE, OUTPUT);
  pinMode(MOTOR_D_TRAS, OUTPUT);

  pinMode(SENSOR_ESQUERDO, INPUT);
  pinMode(SENSOR_DIREITO, INPUT);

  pinMode(COR_S0, OUTPUT);
  pinMode(COR_S1, OUTPUT);
  pinMode(COR_S2, OUTPUT);
  pinMode(COR_S3, OUTPUT);
  pinMode(COR_OUT, INPUT);
  // Escala de frequência de saída do TCS3200 em 20%
  digitalWrite(COR_S0, HIGH);
  digitalWrite(COR_S1, LOW);

#if DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  int leituraE = analogRead(SENSOR_ESQUERDO);
  int leituraD = analogRead(SENSOR_DIREITO);
  bool pretoE = leituraE > LIMIAR_PRETO;
  bool pretoD = leituraD > LIMIAR_PRETO;

#if DEBUG
  imprimirLeituras(leituraE, leituraD);
#endif

  if (pretoE && pretoD) {
    tratarMarcador();
  } else if (pretoD) {
    corrigir(CURVA_DIREITA);
  } else if (pretoE) {
    corrigir(CURVA_ESQUERDA);
  } else {
    motores(VEL_RETA, VEL_RETA);
    ultimoEstado = RETA;
  }
}

// ===================== MOVIMENTO =====================

// Velocidade positiva = frente, negativa = ré, 0 = parado
void motor(uint8_t pinoFrente, uint8_t pinoTras, int vel) {
  analogWrite(pinoFrente, vel > 0 ? vel : 0);
  analogWrite(pinoTras, vel < 0 ? -vel : 0);
}

void motores(int velE, int velD) {
  motor(MOTOR_E_FRENTE, MOTOR_E_TRAS, velE);
  motor(MOTOR_D_FRENTE, MOTOR_D_TRAS, velD);
}

void parar() {
  motores(0, 0);
}

// A linha saiu por um dos lados: gira só o motor oposto para voltar a ela.
// Ao trocar de lado, freia e espera o robô estabilizar antes de corrigir.
void corrigir(Estado lado) {
  if (ultimoEstado != lado) {
    motores(-VEL_FREIO, -VEL_FREIO);
    delay(TEMPO_FREIO);
    parar();
    delay(PAUSA_TROCA_LADO);
  }

  if (lado == CURVA_DIREITA)
    motores(VEL_CURVA, 0);
  else
    motores(0, VEL_CURVA);
  delay(TEMPO_CURVA);

  ultimoEstado = lado;
}

// Avança para sair de cima do marcador e para por um instante
void atravessar() {
  motores(VEL_MANOBRA, VEL_MANOBRA);
  delay(TEMPO_AVANCO);
  parar();
  delay(TEMPO_ASSENTAR);
}

void girar(int velE, int velD) {
  motores(velE, velD);
  delay(TEMPO_GIRO);
  atravessar();
}

void tratarMarcador() {
  motores(-VEL_MANOBRA, -VEL_MANOBRA);
  delay(TEMPO_FREIO_MARCADOR);
  parar();

  switch (lerCor()) {
    case VERMELHO:
      girar(-VEL_MANOBRA, VEL_MANOBRA);  // esquerda
      break;
    case VERDE:
      girar(VEL_MANOBRA, -VEL_MANOBRA);  // direita
      break;
    case AZUL:
      delay(TEMPO_PARADA_AZUL);
      atravessar();
      break;
    default:
      atravessar();  // cruzamento sem cor
      break;
  }

  ultimoEstado = MARCADOR;
}

// ===================== SENSOR DE COR =================

// Média da largura de pulso de um canal do TCS3200 (S2/S3 selecionam o filtro)
unsigned long lerCanal(uint8_t s2, uint8_t s3) {
  digitalWrite(COR_S2, s2);
  digitalWrite(COR_S3, s3);
  unsigned long soma = 0;
  for (uint8_t i = 0; i < AMOSTRAS_COR; i++)
    soma += pulseIn(COR_OUT, LOW, TIMEOUT_COR_US);
  return soma / AMOSTRAS_COR;
}

Cor lerCor() {
  unsigned long r = lerCanal(LOW, LOW);
  unsigned long b = lerCanal(LOW, HIGH);
  unsigned long g = lerCanal(HIGH, HIGH);

  // Timeout (pulseIn devolve 0): sensor desconectado, ignora
  if (r == 0 || g == 0 || b == 0) return NENHUMA;

  if (r < g && r < b && r < LIMIAR_VERMELHO) return VERMELHO;
  if (g < r && g < b && g < LIMIAR_VERDE) return VERDE;
  if (b < r && b < g && b < LIMIAR_AZUL) return AZUL;
  return NENHUMA;
}

#if DEBUG
void imprimirLeituras(int leituraE, int leituraD) {
  static const char *NOMES[] = {"nenhuma", "vermelho", "verde", "azul"};
  Serial.print("IR esq: ");
  Serial.print(leituraE);
  Serial.print("  IR dir: ");
  Serial.print(leituraD);
  Serial.print("  R: ");
  Serial.print(lerCanal(LOW, LOW));
  Serial.print("  G: ");
  Serial.print(lerCanal(HIGH, HIGH));
  Serial.print("  B: ");
  Serial.print(lerCanal(LOW, HIGH));
  Serial.print("  cor: ");
  Serial.println(NOMES[lerCor()]);
}
#endif
