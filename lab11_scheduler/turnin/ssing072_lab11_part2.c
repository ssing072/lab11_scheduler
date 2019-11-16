#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "keypad.h"
#include "timer.h"
#include "io.h"




typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a % b;
		if( c == 0 ) { return b; }
		a = b;
		b = c;
	}
	return 0;
}
enum paginate_state{ PAGE };

unsigned char sentence[71] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','C','S','1','2','0','B',' ','i','s',' ','a',' ','L','e','g','e','n','d','.','.','.',' ','w','a','i','t',' ','f','o','r',' ','i','t',' ','D','A','R','Y','!',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned char i;
unsigned char letter = 0;

int paginate(int state){
	switch(state){
		case PAGE:
		state = PAGE;
		break;
	}
	switch(state){
		case PAGE:
		for(i = 1; i <= 16; i++){
			LCD_Cursor(i);
			LCD_WriteData(sentence[letter + i - 1]);
			if(letter + i == 71){
				letter = 1;
			}
		}
		letter++;
		break;
	}
	
	return state;
}

int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; 
	DDRD = 0xFF; PORTD = 0x00;
	
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	
	task1.state = PAGE;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &paginate;
	
	 
	unsigned short i;
	unsigned long GCD = tasks[0]->period;
	for(i=1;i<numTasks; i++){
		GCD = findGCD(GCD, tasks[i]->period);
	}
	
	TimerSet(GCD);
	TimerOn();
	
	LCD_init();
	LCD_ClearScreen();
    while (1) 
    {
		for (i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;	
			}
			tasks[i]->elapsedTime += GCD;//GCD
		}
		while(!TimerFlag){};
		TimerFlag = 0;
    }
}
