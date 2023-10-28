/*
 * Led Wave.c
 *
 * Created: 5/11/2015 7:20:44 μμ
 * Author : Κοσμάς Γεωργιάδης
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD = 0xFF; //η  Θύρα D ως έξοδος
    PORTD = 0x00;
    
    int8_t i = 0; //Current LED index.
    uint8_t c = 0; //Flag that determines if the wave is going forward or backwards.
                   // Σημαία που καθορίζει εάν το κύμα κινείται προς τα εμπρός ή προς τα πίσω.η  Θύρα D ως έξοδος
    while (1) 
    {
		//If end is reached, raise flag and correct index.
		// Εάν φτάσει στο τέλος, αυξήστε τη σημαία και διορθώστε το ευρετήριο.
        if(i >= 8)
        {
            c = 1;
            i = 7;
        }
		else if(i <= 0) //Else if start is reached, clear flag and correct index.
        {               // Διαφορετικά, εάν επιτευχθεί η έναρξη, καθαρίστε τη σημαία και το σωστό ευρετήριο
            c = 0;
            i = 1;
        }
		
		//If flag is raised, go backwards (Αν σηκωθεί η σημαία, πηγαίνετε προς τα πίσω). 
        if(c)
			PORTD = (1 << --i);
        else //Else go forward (Αλλιώς προχωρήστε).
			PORTD = (1 << i++);
        
        _delay_ms(100);//Καθυστέρηση. 
    }
}

