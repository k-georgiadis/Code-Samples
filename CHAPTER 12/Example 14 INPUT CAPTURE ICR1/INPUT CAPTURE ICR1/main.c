
/*
 * ATmega8 Input Capture Mode.c
 * Use With Timer/Counter1
 * Created: 5/27/2020 6:03:43 PM
 * Author: ΓΕΩΡΓΙΑΔΗΣ ΓΕΩΡΓΙΟΣ
 */

#include "avr/io.h"
int main ()
{
    unsigned int t;
    DDRC = 0xFF;	/*ΘύραC ως έξοδος*/
    DDRB = 0xFF;	/*ΘύραΒ ως έξοδος*/
    TCCR1A = 0;
    TIFR = (1<<ICF1); /* clear input capture flag */
    TCCR1B = 0x41; /* σύλληψη στήν άνοδο capture on rising edge */ 

    /* παρακολουθούμε για λήψη συμβάντος - monitor for capture */ 
    while ((TIFR&(1<<ICF1)) == 0); 

    t = ICR1;
    TIFR = (1<<ICF1); /*καθαρισμός τής σημαίας clear capture flag */

    /* monitor for next rising edge capture */
    while ((TIFR&(1<<ICF1)) == 0); 

    t = ICR1 - t; 	/* period = recent capture - previous capture */
    PORTC = t; 	/* put period count on PORTC & PORTB */	
    PORTB = t>>8;	/* διάβασε τα υπόλοιπα 8 ψηφιά, το υψηλό Byte*/ 
 			/* του χρονικού διαστήματος.*/ 
    while (1);    
    return 0;
}
