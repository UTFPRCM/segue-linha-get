//controle do motor
const int motorEfwd = 11;
const int motorErev = 10;
const int motorDfwd = 6;
const int motorDrev = 5;
//sensores infra
const int sensorE = A0;
const int sensorD = A1;
bool estE, estD;//estado true=branco, false=preto
int valSensorE = 0, valSensorD = 0;
//sensor de cores
const int s0 = 4;
const int s1 = 7;
const int s2 = 8;
const int s3 = 12;
const int out = 13;
int red = 0;
int green = 0;
int blue = 0;
int cor;
//velocidades
int velD = 0, velE = 0, t1, t2 = 0, voltasD, voltasE;
int velAtualD, velAtualE,vel;
int vmax = 255, vmin = 150;
int valAnt = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(motorEfwd, OUTPUT);
  pinMode(motorDfwd, OUTPUT);
  pinMode(motorErev, OUTPUT);
  pinMode(motorDrev, OUTPUT);
  pinMode(sensorE, INPUT);
  pinMode(sensorD, INPUT);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
  attachInterrupt(digitalPinToInterrupt(2), encoderE, FALLING) ;
  attachInterrupt(digitalPinToInterrupt(3), encoderD, FALLING) ;
  Serial.begin(9600);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  /*t1=millis();
    if((t1-t2)>1000){
    velD=voltasD*60*1000/(t1-t2);
    t2=t1;
    voltasD=0;
    Serial.println(voltasD);

    if (voltasD <= 20) {
    analogWrite(motorEfwd, vmin);
    analogWrite(motorErev, 0);
    } else {
    analogWrite(motorEfwd, 0);

    }
    }
  */
  valSensorE = analogRead(sensorE);
  valSensorD = analogRead(sensorD);

  //ler se está branco ou preto no sensor esquerdo
  if (valSensorE > 500)
    estE = false; //estado true=branco, false=preto
  else
    estE = true;

  //ler se está branco ou preto no sensor direito
  //Serial.println(valSensorD);
  if (valSensorD > 500)
    estD = false; //estado true=branco, false=preto
  else
    estD = true;


  if (estE == false && estD == false) {//preto preto
    analogWrite(motorEfwd, 0);
    analogWrite(motorDfwd, 0);
    analogWrite(motorErev, 150);
    analogWrite(motorDrev, 150);
    delay(20);
    analogWrite(motorErev, 0);
    analogWrite(motorDrev, 0);
    cor = verifica_cor();
    if (cor == 1) { //vermelho->direita
      analogWrite(motorDrev, 0);
      analogWrite(motorEfwd, 0);
      analogWrite(motorDfwd, 150);
      analogWrite(motorErev, 150);
      delay(200);
      analogWrite(motorDrev, 0);
      analogWrite(motorEfwd, 150);
      analogWrite(motorDfwd, 150);
      delay(300);
      analogWrite(motorEfwd, 0);
      analogWrite(motorDfwd, 0);
      delay(200);
    } else if (cor == 2) { //verde->esquerda
      analogWrite(motorDfwd, 0);
      analogWrite(motorErev, 0);
      analogWrite(motorDrev, 150);
      analogWrite(motorEfwd, 150);
      delay(200);
      analogWrite(motorDrev, 0);
      analogWrite(motorEfwd, 150);
      analogWrite(motorDfwd, 150);
      delay(300);
      analogWrite(motorEfwd, 0);
      analogWrite(motorDfwd, 0);
      delay(200);
    } else if (cor == 3) { //azul->obstaculo

    } else {

    }
    valAnt = 0;


  } else if (estE == false && estD == true) {//preto branco
    if (valAnt != 1) {
      pausa();
      vel = 45;
    }
    analogWrite(motorDfwd, 0);
    analogWrite(motorDrev, 0);
    analogWrite(motorEfwd, 215);//215
    analogWrite(motorErev, 0);
    delay(40);
    analogWrite(motorErev, 0);
    //verifica_cor();
    valAnt = 1;
    if (vel < 255)
      vel = vel + 20;


  } else if (estE == true && estD == false) {//branco preto
    if (valAnt != 2) {
      pausa();
      vel = 45;
    }
    analogWrite(motorErev, 0);
    analogWrite(motorEfwd, 0);
    analogWrite(motorDfwd, 215);
    analogWrite(motorDrev, 0);
    delay(40);
    analogWrite(motorDrev, 0);
    //verifica_cor();
    valAnt = 2;
    if (vel < 255)
      vel = vel + 20;


  } else if (estE == true && estD == true) {//branco branco
    analogWrite(motorErev, 0);
    analogWrite(motorDrev, 0);
    analogWrite(motorEfwd, 70);
    analogWrite(motorDfwd, 70);
    //verifica_cor();
    valAnt = 3;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void encoderD() {
  voltasD = voltasD + 1;
}

void encoderE() {
  voltasE = voltasE + 1;
}

void pausa() {
  analogWrite(motorDfwd, 0);
  analogWrite(motorDrev, 255); //255
  analogWrite(motorEfwd, 0);
  analogWrite(motorErev, 255);
  delay(25);
  analogWrite(motorErev, 0);
  analogWrite(motorDrev, 0);
  delay(700);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int verifica_cor() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  red = pulseIn(out, digitalRead(out) == HIGH);
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  blue = pulseIn(out, digitalRead(out) == HIGH);
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  green = pulseIn(out, digitalRead(out) == HIGH);

  if (red < blue && red < green && red < 48) { //identifica a cor vermelha
    /*Serial.println("Vermelho");
      //pausa();
      analogWrite(motorErev, 0);
      analogWrite(motorEfwd, 0);
      analogWrite(motorDfwd, 215);
      analogWrite(motorDrev, 0);
      delay(40);
      analogWrite(motorDrev, 0);*/
    return (1);

  }

  else if (blue < red && blue < green) { //identifica a cor azul
    //Serial.println("Azul");
    return (3);
  }

  else if (green < red && green < blue) { //identifica a cor verde
    /*Serial.println("Verde");
      //pausa();
      analogWrite(motorDfwd, 0);
      analogWrite(motorDrev, 0);
      analogWrite(motorEfwd, 215);
      analogWrite(motorErev, 0);
      delay(40);
      analogWrite(motorErev, 0);*/
    return (2);
  } else
    return (0);
}
