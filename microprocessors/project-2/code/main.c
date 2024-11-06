#include <pic18fregs.h>

// Precisa para SDCC
#pragma config XINST = OFF

// Frequencia do Clock usada
#define CLOCK_FREQ 8 // MHz

// Buffer e rotina para imprimir no
// display de 7 segmentos os numeros de 0-9
// Usando um buffer como lookup table
// do binario para cada numero
#define SEVSEG_DISPLAY LATD
#define SEVSEG_DISPLAY_TRIS TRISD
int numeros_para_7seg[] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
    0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111,
};
void config_7seg(void) {
  SEVSEG_DISPLAY_TRIS = 0x00; // Todos os pinos do display sao de saida
  SEVSEG_DISPLAY = 0x00;      // Todos os pinos do display comecam desligados
}
void printa_numero(int numero) {
  if (numero > 10)
    return;
  SEVSEG_DISPLAY = numeros_para_7seg[numero];
}
// --------

// Rotina de interrupcoes
// TIMER0 - Clock de 0.025 -- O motivo desse clock é para fazer debouncer dos
// botoes INT0 e INT1
// INT0 - Botao velocidade lenta (1s)
// INT1 - Botao velocidade rapida (0.25s)

int ligado = 0;       // 0 - desligado, 1 ligado
int modo = 0;         // 0 - devagar, 1 rapido
int numero_atual = 0; // Numero atual da contagem
// Usando 0.025s de delay no timer0
//  (25 ms * 8000 ciclos / ms / 4 instr/ciclo) / 256 prescaler = C3h
//  -> FFh - C3h =  3Ch
#define TIMER0_LOAD 0x3C

void config_timer_e_botoes(void) {

  // Config do TIMER0 ----
  T0CON = 0b01000111;
  // Timer 0
  // Desativado - 0
  // 8 bits     - 1
  // Intern Clk - 0
  // Subida     - 0
  // PSA Habili - 0
  // 1:256      - 1
  //              1
  //              1
  INTCONbits.TMR0IF = 0;  // Zerar a flag caso ela nao
                          // esteja ja zerada
  INTCON2bits.TMR0IP = 0; // Habilitar interrupt de prioridade
  INTCONbits.TMR0IE = 1;  // Habilita Timer0 gerar
                          // interrupts
  TMR0 = TIMER0_LOAD;     // Carregar com o
  //------
  INTCON2bits.RBPU = 0; // Pullup nos botoes em B
  // Config de INT0
  // Porta B liga em 0, resistores de pullup habilitados
  INTCONbits.INT0IF = 0;   // Zerar a flag caso ela nao esteja ja zerada
  INTCONbits.INT0IE = 1;   // Habilita a interrupcao 2
  INTCON2bits.INTEDG0 = 0; // Habilita ao presionar a tecla

  IPR1bits.TMR1IP = 1; // Habilitar prioridade
  TRISBbits.RB0 = 1;   // Pino B0 como entrada do Interrupt

  //------
  // Config de INT1
  // Porta B liga em 0, resistores de pullup habilitados
  INTCON3bits.INT1IF = 0;  // Zerar a flag caso ela nao esteja ja zerada
  INTCON3bits.INT1IE = 1;  // Habilita a interrupcao 2
  INTCON2bits.INTEDG1 = 0; // Habilita ao presionar a tecla
  TRISBbits.RB1 = 1;       // Pino B0 como entrada do Interrupt

  //------
  RCONbits.IPEN = 0;   // Desabilita prioridades nos interrupts
  INTCONbits.PEIE = 1; // Habilita interrupts de perifericos
  INTCONbits.GIE = 1;  // Habilita que todos os interrupts ocorram
  // Ativacao do TIMER0
  T0CONbits.TMR0ON = 1;
}

// Nosso 'postscaler' para definir que sao
// 0.25s ou 1s
int timer0_postscaler = 0;
void on_timer0_interupt(void) {
  if (ligado == 0) // se nao ligado, nem rodar
    return;

  timer0_postscaler++;
  if ((modo == 1 && timer0_postscaler % 10 == 0 /*25ms*/) ||
      (modo == 0 && timer0_postscaler == 40 /*1s*/)) {
    numero_atual = (numero_atual + 1) % 10;
    printa_numero(numero_atual);
  }

  // Para limitar nosso prescaler entre 1
  // - 40 (comeca em 1 e termina em 40
  // pois a soma ocorre no inicio, e o %
  // no fim)
  timer0_postscaler %= 40;
}
void on_int0_interupt(void) {
  // ligar e colocar modo lento
  if (ligado == 0) {
    printa_numero(numero_atual); // ligar o 7seg
    ligado = 1;
  }
  modo = 0;
}
void on_int1_interupt(void) {
  // ligar e colocar modo rapido
  if (ligado == 0) {
    printa_numero(numero_atual); // ligar o 7seg
    ligado = 1;
  }
  modo = 1;
}

//---------------

void timer_isr(void) __interrupt(1) {
  if (INTCONbits.TMR0IF == 1) { // Tratando TIMER0
    on_timer0_interupt();
    TMR0 = TIMER0_LOAD;
    INTCONbits.TMR0IF = 0;
  }
  if (INTCONbits.INT0IF == 1) { // Tratando INT0
    on_int0_interupt();
    INTCONbits.INT0IF = 0;
  }
  if (INTCON3bits.INT1F == 1) { // Tratando INT1
    on_int1_interupt();
    INTCON3bits.INT1F = 0;
  }
}
// ---------

void main(void) {
  ADCON1 |= 0XF; // Pinos digitais

  config_7seg();           // configurar 7seg
  config_timer_e_botoes(); // configurar interrupts

  while (1) {
  }
}
