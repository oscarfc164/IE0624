#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define VEHICULAR_ROJO PINB3
#define VEHICULAR_VERDE PINB2
#define PEATONAL_A_ROJO PIND1
#define PEATONAL_A_VERDE PIND0
#define PEATONAL_B_ROJO PIND5
#define PEATONAL_B_VERDE PIND4

#define A 1
#define B 2
#define C 3
#define D 4

int B1_B2;
int state;
int timer_counter;
int ten_sec_c;
int three_sec_c;
int ten_sec;
int three_sec;

void paso_autos() {
    PORTB &= ~(1 << VEHICULAR_ROJO);
    PORTB |= (1 << VEHICULAR_VERDE);
    PORTD &= ~(1 << PEATONAL_A_VERDE) | ~(1 << PEATONAL_B_VERDE);
    PORTD |= (1 << PEATONAL_A_ROJO) | (1 << PEATONAL_B_ROJO);
}

void detener_autos() {
    PORTB |= (1 << VEHICULAR_ROJO);
    PORTB &= ~(1 << VEHICULAR_VERDE);
    PORTD &= ~(1 << PEATONAL_A_ROJO) & ~(1 << PEATONAL_B_ROJO);
    PORTD |= (1 << PEATONAL_A_VERDE) | (1 << PEATONAL_B_VERDE);
}

void parpadeo_vehicular() {
    PORTB ^= (1 << VEHICULAR_VERDE);
}

void parpadeo_peatonal() {
    PORTD ^= (1 << PEATONAL_A_VERDE);
    PORTD ^= (1 << PEATONAL_B_VERDE);
}

void reinicio() {
    PORTB &= ~(1 << VEHICULAR_ROJO) & ~(1 << VEHICULAR_VERDE);
    PORTD &= ~(1 << PEATONAL_A_ROJO) & ~(1 << PEATONAL_A_VERDE) & ~(1 << PEATONAL_B_ROJO) & ~(1 << PEATONAL_B_VERDE);
}

ISR(INT0_vect) // Interrupt service routine
{
    B1_B2 = 1;
}


ISR(TIMER0_OVF_vect) {
    if (timer_counter == 63){
      timer_counter = 0;
      ++three_sec_c;
      ++ten_sec_c;
    } else timer_counter++;

    if (ten_sec_c >= 10){
      ten_sec = 1;
    }

    if (three_sec_c >= 3){
      three_sec = 1;
    }
  }


void external_interrupt() {
    GIMSK |= (1 << INT0); // Habilitar la INT0 (interrupción externa)
    MCUCR |= (1 << ISC01); // Configurar como flanco descendente
}

void timer_config() {
    TCCR0A = 0x00; // Modo normal
    TCCR0B = 0x00;
    TCCR0B |= (1 << CS00) | (1 << CS02); // Prescaler de 1024
    sei();
    TCNT0 = 0;
    TIMSK |= (1 << TOIE0); // Habilitar la interrupción del timer0
}

int main() {
    reinicio();
    external_interrupt();
    timer_config();

    DDRB |= (1 << VEHICULAR_ROJO) | (1 << VEHICULAR_VERDE); // Configurar B2 y B3 como salidas
    DDRD |= (1 << PEATONAL_A_ROJO) | (1 << PEATONAL_A_VERDE) | (1 << PEATONAL_B_ROJO) | (1 << PEATONAL_B_VERDE);

    state = A;
    B1_B2 = 0;

    while (1) {
      switch (state)
      {
      case A:
        paso_autos();
        if(B1_B2 == 1){
          timer_counter = 0;
          three_sec_c = 0;
          three_sec = 0;
          state = B;
        }
        break;

      case B:
        parpadeo_vehicular();
        _delay_ms(500);
        if(three_sec == 1){
          PORTB &= ~(1 << VEHICULAR_VERDE);
          PORTB |= (1 << VEHICULAR_ROJO);
        if(three_sec_c == 4){
          timer_counter = 0;
          ten_sec_c = 0;
          ten_sec = 0;
          state = C;
        }
        }
      break;

      case C:
        reinicio();
        detener_autos();
        if(ten_sec == 1){
          reinicio();
          timer_counter = 0;
          three_sec_c = 0;
          three_sec = 0;
          state = D;
          B1_B2 = 0;
          
        }
      break;

      case D:
        parpadeo_peatonal();
        _delay_ms(500);
        if(three_sec == 1){
          PORTD |= (1 << PEATONAL_A_VERDE);
          PORTD |= (1 << PEATONAL_B_VERDE);
          PORTD &= ~(1 << PEATONAL_A_ROJO);
          PORTD &= ~(1 << PEATONAL_B_ROJO);
        if(three_sec_c == 4){
          reinicio();
          state = A;
        }
        }
      break;

      default:
        state = A;
        break;
      }
    }
}
