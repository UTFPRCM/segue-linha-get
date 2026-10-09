# Robô Segue Linha com Sensor de Cor

Robô seguidor de linha (preto e branco) com Arduino, com um extra: um **sensor de cor** lê marcadores coloridos na pista e o robô muda de comportamento conforme a cor encontrada.

| Cor do marcador | Ação do robô |
| :-: | :-- |
| 🟥 **Vermelho** | vira para a **direita** |
| 🟩 **Verde** | vira para a **esquerda** |
| 🟦 **Azul** | **para** por alguns segundos e depois segue |

<p align="center">
  <img src="docs/gifs/2018-10-17-vermelho-verde-azul.gif" width="600" alt="Robô percorrendo a pista com marcadores vermelho, verde e azul">
</p>

Projeto feito durante a faculdade. Este repositório reúne o código final organizado, as fotos e os vídeos da evolução do robô, para servir de registro e de ponto de partida para quem quiser montar o seu.

---

## Sumário

- [O que é um robô segue linha?](#o-que-é-um-robô-segue-linha)
- [O que este projeto faz](#o-que-este-projeto-faz)
- [Veja funcionando](#veja-funcionando)
- [Primeira montagem](#primeira-montagem)
- [Hardware](#hardware)
- [Como o código funciona](#como-o-código-funciona)
- [Como usar](#como-usar)
- [Estrutura do repositório](#estrutura-do-repositório)
- [Ideias para evoluir](#ideias-para-evoluir)

---

## O que é um robô segue linha?

É um robô móvel autônomo que percorre um caminho desenhado no chão, normalmente uma **linha preta sobre fundo branco**. É um dos projetos mais clássicos para quem está começando em robótica e um tipo comum de competição (como a OBR, a Olimpíada Brasileira de Robótica).

O princípio é simples:

1. **Sensores infravermelhos (IR)** ficam na frente do robô, apontados para o chão. Cada um tem um emissor (LED IR) e um receptor (fototransistor).
2. A superfície **branca reflete** bastante luz IR e a **preta absorve**. Assim, a leitura analógica de cada sensor diz se ele está em cima da linha ou não.
3. Com **dois sensores**, um de cada lado da linha, o robô sabe para onde corrigir:

| Sensor esquerdo | Sensor direito | Situação | O que fazer |
| :-: | :-: | :-- | :-- |
| branco | branco | linha está no meio | seguir reto |
| **preto** | branco | linha escapando pela esquerda | virar para a esquerda |
| branco | **preto** | linha escapando pela direita | virar para a direita |
| **preto** | **preto** | cruzamento ou marcador | decidir (aqui entra o sensor de cor) |

Para virar, basta desacelerar ou parar uma das rodas (tração diferencial): o robô gira para o lado da roda mais lenta.

## O que este projeto faz

Além de seguir a linha, o robô tem um **sensor de cor TCS3200** montado entre os sensores IR. Quando **os dois sensores IR enxergam "preto" ao mesmo tempo**, o robô está sobre um marcador ou cruzamento. Ele então:

1. dá uma ré curta para frear e parar exatamente em cima do marcador;
2. lê a cor com o TCS3200;
3. executa a ação:
   - **vermelho**: gira no próprio eixo para a **esquerda** e avança para sair do marcador;
   - **verde**: gira para a **direita** e avança;
   - **azul**: **para por 3 segundos** e depois segue em frente;
   - **nenhuma cor** (cruzamento preto comum): segue em frente.

## Veja funcionando

<table>
  <tr>
    <td align="center"><img src="docs/gifs/2018-10-17-vermelho-verde-azul.gif" width="400"><br><sub>Out/2018: pista com marcadores vermelho, verde e azul</sub></td>
    <td align="center"><img src="docs/gifs/2018-10-17-seguindo-curvas.gif" width="400"><br><sub>Out/2018: seguindo uma pista com curvas</sub></td>
  </tr>
  <tr>
    <td align="center"><img src="docs/gifs/2018-10-11-pista-com-cores.gif" width="400"><br><sub>Out/2018: atalho com marcadores verde e vermelho</sub></td>
    <td align="center"><img src="docs/gifs/2018-10-12-pista-com-cores-2.gif" width="400"><br><sub>Out/2018: mais testes na mesma pista (3x mais rápido)</sub></td>
  </tr>
</table>

## Primeira montagem

Primeira versão em um chassi de acrílico 2WD com motores amarelos de redução, Arduino, ponte H e dois sensores IR na frente. Uma roda boba (caster) atrás dá o terceiro ponto de apoio, e a chave liga/desliga fica embaixo do chassi. A primeira pista era uma linha de fita isolante sobre papel.

<table>
  <tr>
    <td><img src="docs/fotos/IMG-20170805-WA0020.jpg" width="260"></td>
    <td><img src="docs/fotos/IMG-20170810-WA0015.jpg" width="260"></td>
    <td><img src="docs/fotos/IMG-20170810-WA0013.jpg" width="260"></td>
  </tr>
  <tr>
    <td><img src="docs/fotos/IMG-20170812-WA0005.jpg" width="260"></td>
    <td colspan="2" align="center"><img src="docs/gifs/2017-08-05-primeiros-testes.gif" width="400"><br><sub>Primeiros testes seguindo a linha</sub></td>
  </tr>
</table>

### A carroceria

O robô ganhou uma "carroceria" de papel: primeiro um desenho à mão, depois um molde impresso e montado em papel branco. Em seguida veio a versão personalizada, inspirada no **Relâmpago McQueen** (aqui, "Relâmpago Marquinhos", nº 12). Os moldes para imprimir estão em [docs/carroceria/](docs/carroceria/) (laterais + tira de 14 × 41,45 cm com frente, capô, janela, teto e traseira).

<table>
  <tr>
    <td><img src="docs/fotos/IMG-20170811-WA0004.jpg" width="260"><br><sub>Desenho à mão</sub></td>
    <td><img src="docs/fotos/IMG-20170811-WA0006.jpg" width="260"><br><sub>Molde impresso</sub></td>
    <td><img src="docs/fotos/IMG-20170811-WA0011.jpg" width="260"><br><sub>Carroceria branca montada</sub></td>
  </tr>
  <tr>
    <td><img src="docs/fotos/IMG-20170814-WA0007.jpg" width="260"><br><sub>Versão McQueen</sub></td>
    <td><img src="docs/fotos/IMG-20170814-WA0009.jpg" width="260"></td>
    <td><img src="docs/fotos/IMG-20170811-WA0009.jpg" width="260"></td>
  </tr>
  <tr>
    <td colspan="3">
      <img src="docs/carroceria/mcqueen-body-side-segue-linha.png" width="190">
      <img src="docs/carroceria/mcqueen-body-top-segue-linha.png" width="190">
      <img src="docs/carroceria/white-label-body-side-segue-linha.png" width="190">
      <img src="docs/carroceria/white-label-body-top-segue-linha.png" width="190">
      <br><sub>Moldes da carroceria: McQueen e versão em branco para personalizar</sub>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center"><img src="docs/gifs/2017-08-12-carroceria-branca.gif" width="400"><br><sub>Com a carroceria branca</sub></td>
    <td align="center"><img src="docs/gifs/2017-08-14-carroceria-mcqueen.gif" width="400"><br><sub>Com a carroceria McQueen</sub></td>
  </tr>
</table>

### Pista com marcadores e sensor de cor

Entrou o sensor de cor TCS3200 e a pista ficou mais difícil: cruzamentos, marcadores verdes e vermelhos, "oito", zigue-zague e trechos tracejados, no estilo das pistas de competição. Desta fase são os códigos [2017-10_Robo.ino](src/historico/2017-10_Robo.ino), só com o seguidor de linha (a lógica de cores ainda estava comentada), e [2017-10_Robo_Cor.ino](src/historico/2017-10_Robo_Cor.ino), que já virava ao ver vermelho ou verde.

<table>
  <tr>
    <td align="center"><img src="docs/gifs/2017-09-18-pista-com-marcadores.gif" width="220"><br><sub>Set/2017: marcadores nos cruzamentos</sub></td>
    <td align="center"><img src="docs/gifs/2017-09-29-pista-completa-noite.gif" width="220"><br><sub>Set/2017: pista completa (3x)</sub></td>
    <td align="center"><img src="docs/gifs/2017-10-02-pista-completa.gif" width="320"><br><sub>Out/2017: pista completa vista de cima</sub></td>
  </tr>
</table>

### Agosto e setembro de 2018: reconstrução

O robô foi reconstruído: placa própria para os sensores IR (LED emissor azul + receptor preto), placa de ligação com conectores identificados, ponte H com dissipador, sensor de cor TCS3200 numa posição nova e encoders nas rodas (ligados aos pinos 2 e 3, mas sem uso na lógica final). Com a pinagem nova surgiu o código [2018_seguidor3.ino](src/historico/2018_seguidor3.ino), a última versão, que deu origem ao [código atual](src/segue_linha/segue_linha.ino).

<table>
  <tr>
    <td><img src="docs/fotos/IMG-20180811_181037.jpg" width="200"><br><sub>Sensores IR</sub></td>
    <td><img src="docs/fotos/IMG-20180811_182101.jpg" width="200"><br><sub>Sensor de cor TCS3200</sub></td>
    <td><img src="docs/fotos/IMG-20180811_170656.jpg" width="200"><br><sub>Fiação e ponte H</sub></td>
    <td><img src="docs/fotos/IMG-20180811_170139.jpg" width="200"><br><sub>Placa de conexões</sub></td>
  </tr>
  <tr>
    <td><img src="docs/fotos/IMG-20180923-WA0001.jpg" width="200"></td>
    <td><img src="docs/fotos/IMG-20180923-WA0002.jpg" width="200"></td>
    <td><img src="docs/fotos/IMG-20180923-WA0003.jpg" width="200"></td>
    <td><img src="docs/fotos/IMG-20180923-WA0004.jpg" width="200"></td>
  </tr>
</table>

### Outubro de 2018: cores funcionando e apresentação

Testes finais na pista com marcadores **vermelho, verde e azul** (GIFs em [Veja funcionando](#veja-funcionando)) e apresentação do robô em um evento.

<p align="center">
  <img src="docs/fotos/IMG-20181017-WA0017.jpg" width="500" alt="Robô em apresentação na UTFPR">
</p>

### Organização do repositório

O `seguidor3.ino` é a versão mais recente (pinagem do robô de 2018 e lógica de marcadores mais completa) e foi refatorado em [src/segue_linha/segue_linha.ino](src/segue_linha/segue_linha.ino). As versões antigas ficaram em [src/historico/](src/historico/) como registro.

## Hardware

- Arduino Uno (ou compatível)
- Chassi 2WD (acrílico) com 2 motores DC com redução, rodas e roda boba
- Ponte H dupla (ex.: L298N), controlada por 4 pinos PWM
- 2 sensores IR de reflexão (LED IR + fototransistor), saída analógica
- Sensor de cor TCS3200 (GY-31)
- Bateria e chave liga/desliga

### Pinagem (versão final)

| Componente | Sinal | Pino Arduino |
| :-- | :-- | :-: |
| Motor esquerdo | frente / ré | 11 / 10 |
| Motor direito | frente / ré | 6 / 5 |
| Sensor IR direito | analógico | A0 |
| Sensor IR esquerdo | analógico | A1 |
| TCS3200 | S0 / S1 | 4 / 7 |
| TCS3200 | S2 / S3 | 8 / 12 |
| TCS3200 | OUT | 13 |
| Encoders (opcional, não usados) | esquerdo / direito | 2 / 3 |

> **Atenção:** no `seguidor3.ino` original, o sensor em A0 se chamava "esquerdo", mas pela reação dos motores ele estava montado do lado direito do robô. O código novo usa o nome correto. Se o seu robô **fugir** da linha em vez de corrigir, troque `SENSOR_DIREITO` e `SENSOR_ESQUERDO` no topo do sketch.

## Como o código funciona

```mermaid
flowchart TD
    A[Lê os 2 sensores IR] --> B{Esquerdo e direito<br>veem preto?}
    B -- os dois --> M[Ré curta e para]
    M --> C{Lê a cor}
    C -- vermelho --> L[Gira para a esquerda<br>e avança]
    C -- verde --> R[Gira para a direita<br>e avança]
    C -- azul --> P[Para 3 s e avança]
    C -- nenhuma --> F[Avança, cruzamento comum]
    B -- só o direito --> D[Corrige para a direita]
    B -- só o esquerdo --> E[Corrige para a esquerda]
    B -- nenhum --> S[Segue reto]
```

Pontos principais do [segue_linha.ino](src/segue_linha/segue_linha.ino):

- **Todos os ajustes ficam no topo do arquivo:** pinos, velocidades, tempos e limiares. Não precisa mexer na lógica para calibrar.
- **`motores(velE, velD)`** controla as duas rodas com um único comando: valor positivo é frente, negativo é ré, zero é parado.
- **Correção em pulsos:** quando a linha escapa para um lado, só a roda do lado oposto gira por 40 ms. Ao **trocar de lado**, o robô freia e espera um pouco antes de corrigir. Isso evita o zigue-zague com motores rápidos e sensores simples.
- **Sensor de cor:** o TCS3200 gera uma onda quadrada, e quanto **menor** a largura do pulso, mais daquela cor. Cada canal (R, G, B) é lido 3 vezes e é feita a média. Uma cor só vale se for o menor dos três canais **e** estiver abaixo do seu limiar, porque o preto dá valores altos em todos. As leituras têm timeout, para o robô não travar se o sensor desconectar.
- **Modo DEBUG:** com `#define DEBUG 1`, as leituras dos sensores IR, os valores R/G/B e a cor reconhecida aparecem no Monitor Serial.

### O que mudou em relação ao `seguidor3.ino`

- Pinos, velocidades e tempos viraram constantes com nome, em um só lugar.
- O código repetido de `analogWrite` virou as funções `motores()`, `corrigir()`, `girar()` e `atravessar()`.
- **Marcador azul** implementado (antes o bloco estava vazio): para 3 s e segue.
- **Cruzamento sem cor**: o robô agora avança. Antes ficava dando ré curta em loop.
- **Correção de bug:** na manobra do vermelho, o pino de ré do motor esquerdo continuava ligado durante o avanço, então o robô fazia uma curva em vez de seguir reto. Com `motores()` isso não acontece mais.
- Limiar também para verde e azul, para que o preto de um cruzamento não seja lido como azul.
- Removidos código morto e variáveis sem uso (encoders, `vel`, trechos comentados).

## Como usar

1. Instale a [Arduino IDE](https://www.arduino.cc/en/software) (ou o `arduino-cli`).
2. Abra `src/segue_linha/segue_linha.ino`, selecione a placa **Arduino Uno** e a porta, e faça o upload.

   Pelo terminal:
   ```bash
   arduino-cli core install arduino:avr
   arduino-cli compile -b arduino:avr:uno src/segue_linha
   arduino-cli upload  -b arduino:avr:uno -p /dev/ttyUSB0 src/segue_linha
   ```
3. **Calibre** (cada robô, sensor e iluminação é diferente):
   - Mude para `#define DEBUG 1`, faça o upload e abra o Monitor Serial em 9600 baud.
   - Coloque cada sensor IR sobre o branco e sobre o preto e escolha um `LIMIAR_PRETO` entre os dois valores.
   - Coloque o sensor de cor sobre cada marcador (vermelho, verde, azul) e sobre o preto, e ajuste `LIMIAR_VERMELHO`, `LIMIAR_VERDE` e `LIMIAR_AZUL`. **Só o vermelho veio calibrado do código original.**
   - Volte para `#define DEBUG 0`, porque a leitura de cor a cada ciclo deixa o robô lento.
4. Ajuste `VEL_*` e `TEMPO_*` até o robô fazer as curvas sem sair da linha.

### Montando a pista

- Fundo branco fosco (papel, lona ou MDF branco) e fita isolante preta de 18–20 mm.
- Marcadores: quadrados de papel colorido (vermelho, verde, azul) um pouco mais largos que a linha, para que **os dois** sensores IR fiquem sobre ele ao mesmo tempo.
- Evite luz forte direta: ela atrapalha tanto os sensores IR quanto o de cor.

## Estrutura do repositório

```
.
├── src/
│   ├── segue_linha/
│   │   └── segue_linha.ino      # código final (use este)
│   └── historico/               # versões originais, só como registro
│       ├── 2017-10_Robo.ino
│       ├── 2017-10_Robo_Cor.ino
│       └── 2018_seguidor3.ino
└── docs/
    ├── fotos/                   # fotos da montagem (2017–2018)
    ├── gifs/                    # vídeos convertidos para o README
    ├── videos/                  # vídeos originais (.mp4)
    └── carroceria/              # moldes da carroceria para imprimir
```

## Ideias para evoluir

- **Controle PID** com 3 a 5 sensores (ou uma barra QTR-8) para curvas suaves e mais velocidade, sem as pausas atuais.
- Usar os **encoders** que já estavam no robô para fazer giros com ângulo exato em vez de giros por tempo.
- Trocar os `delay()` por uma máquina de estados com `millis()`, para que o robô continue lendo os sensores durante as manobras.
- Desvio de obstáculo com um sensor ultrassônico (HC-SR04).

## Licença

[MIT](LICENSE)
