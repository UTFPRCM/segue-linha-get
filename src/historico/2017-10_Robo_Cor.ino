//Programa: Robo_Segue_Linha
 
//Pinos de conexao do modulo color

#define s0 4
#define s1 7
#define s2 8
#define s3 10
#define out 11

//Variaveis cores
int red = 0;
int green = 0;
int blue = 0;

int vermelho = 0;
int verde = 0; 
int azul = 0;
int preto = 0;
int branco = 0;

int i = 0;

int dir = 0;
int esq = 0;

// Motor 1 (Esquerda)
#define m1A_pin 3
#define m1B_pin 5

// Motor 2 (Direita)
#define m2A_pin 9
#define m2B_pin 6

// Potencias do motor:
// Potencia do motor principal quando anda para os lado
// Este parametro define quao rapido o robo realiza as curvas
#define potL 250

// Potencia do motor secundario quando anda para os lados
// Este parametro define quao abrupto a robo realiza as curvas
#define potS 200

// Potencia quando o robo anda para frente
#define potF 90
// Valor de referencia da LINHA a ser seguida (adquirido pelos sensores)
#define linha 500



void setup()
{
  //pinos sensor color
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
  //pinos motor
  pinMode(m1A_pin, OUTPUT);
  pinMode(m1B_pin, OUTPUT);
  pinMode(m2A_pin, OUTPUT);
  pinMode(m2B_pin, OUTPUT);

  Serial.begin(9600);
}
 
void loop()
{
  //Detecta a cor
  color();
  
  int se = analogRead(A0);
  int sd = analogRead(A1);

  if(se > 450)
  {
    esq = 1;
  } else 
  {
    esq = 0;
  }

  if(sd > 555)
  {
    dir = 1;
  } else 
  {
    dir = 0;
  }

  //Mostra valores no serial monitor
 /* Serial.print("Vermelho :");
  Serial.print(red, DEC);
  Serial.print(" Verde : ");
  Serial.print(green, DEC);
  Serial.print(" Azul : ");
  Serial.print(blue, DEC);
  Serial.println();*/
 
/* Serial.print(" Esquerda ");
 Serial.print(esq,DEC);
 Serial.print(' ');
 Serial.print(se,DEC);
 Serial.print(" Direita ");
 Serial.print(dir,DEC);
 Serial.print(' ');
 Serial.println(sd,DEC);*/


 //Verifica se a cor é branco
  if (red < 110 && blue < 110 && green < 110)
  {
   // Serial.println("Branco");
    vermelho = 0;
    verde = 0; 
    azul = 0;
    preto = 0;
    branco = 1;    
  }

  //Verifica se a cor é preto
  else if (red > 150 && blue > 150 && green > 150)
  {
   // Serial.println("Preto");
    vermelho = 0;
    verde = 0; 
    azul = 0;
    preto = 1;
    branco = 0;    
  }
 
 //Verifica se a cor vermelha foi detectada
  else if (red < blue && red < green && red < 150)
  {
   // Serial.println("Vermelho");
    vermelho = 1;
    verde = 0; 
    azul = 0;
    preto = 0;
    branco = 0;
  }
 
  //Verifica se a cor azul foi detectada
  else if (blue < red && blue < green && blue < 150)
  {
   // Serial.println("Azul");
    vermelho = 0;
    verde = 1; 
    azul = 0;
    preto = 0;
    branco = 0;
  }
 
  //Verifica se a cor verde foi detectada
  else if (green < red && green < blue && green < 150)
  {
   // Serial.println("Verde");
    vermelho = 0;
    verde = 1; 
    azul = 0;
    preto = 0;
    branco = 0;    
  }
 



  if (preto == 1 or branco == 1)
 // if (1) 
  {
    //segue reto
    if(esq == dir)
    {
      control('w');
    }
    //vira direta
    else if(esq == 1 && dir == 0)
    {
      control('d');
    }
    //vira esquerda
    else if(esq == 0 && dir ==1 )
    {
      control('a');
    }
  } 
  else if(vermelho == 1)
  {
    control('a');
    delay(350);
  }
  else if(verde == 1)
  {
    control('d');
    delay(350);
  }
 /*else if(azul == 1)
  {
    control('d');
    delay(350);
    control('w');
    delay(1000);
    control('a');
    delay(700);
    control('w');
    delay(100);
    control('a');
    delay(1000);
    control('w');
    delay(1000);
    control('d');
    delay(1000);
  }*/

}
 
void color()
{
  //Rotina que le o valor das cores
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}

//Funcao que controla os motores
void control(char dir){
    switch(dir){
       //Anda para frente   
       case 'w':
           analogWrite(m1A_pin, 0);
           analogWrite(m1B_pin, potF);
           analogWrite(m2A_pin, 0);
           analogWrite(m2B_pin, potF);
           break;
              
       //Anda para tras   
       case 'z':
           analogWrite(m1A_pin, potF);
           analogWrite(m1B_pin, 0);
           analogWrite(m2A_pin, potF);
           analogWrite(m2B_pin, 0);
           break;
              
       //Anda para direita   
       case 'd':
           analogWrite(m1A_pin, potS);
           analogWrite(m1B_pin, 0);
           analogWrite(m2A_pin, 0);
           analogWrite(m2B_pin, potL);
           break;
              
       //Anda para esquerda
       case 'a':
           analogWrite(m1A_pin, 0);
           analogWrite(m1B_pin, potL);
           analogWrite(m2A_pin, potS);
           analogWrite(m2B_pin, 0);
           break;        
      }       
}

