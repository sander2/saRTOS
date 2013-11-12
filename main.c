#define F_CPU 16000000ull

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>

int main(void);
void NumPrimes();
void ToggleLed();

struct  __attribute__ ((__packed__)) context
{
    uint8_t registers[32];
    uint8_t sreg;
    void *stack_pointer;
    uint16_t stack_space;
    void (*Start)(void);
};

struct context SavedContext[2] =
{
    {
    	{0,},	// cant do named initialization when using c++ compiler...
    	0,
    	NULL,
    	256,
        NumPrimes,
    },
    {
    	{0,},
    	0,
    	NULL,
    	256,
        ToggleLed,
    },
};

uint8_t currentThreadID = 0;
struct context *context;

ISR(TIMER2_OVF_vect, ISR_NAKED)
{

    asm("push R31");    // store R31 on stack
    asm("IN R31,0x3F"); // save SREG in R31

    asm("push r26");
    asm("push r27");
    asm("push R28");
    asm("push R29");
    asm("ldi r28, lo8(context)"); // load address of the context pointer into Y
    asm("ldi r29, hi8(context)");
    asm("ld r26, Y+"); // load the value of the context pointer into X
    asm("ld r27, Y+");
    asm("pop r29");
    asm("pop r28");

    asm("ST X+, R0");
    asm("ST X+, R1");
    asm("ST X+, R2");
    asm("ST X+, R3");
    asm("ST X+, R4");
    asm("ST X+, R5");
    asm("ST X+, R6");
    asm("ST X+, R7");
    asm("ST X+, R8");
    asm("ST X+, R9");
    asm("ST X+, R10");
    asm("ST X+, R11");
    asm("ST X+, R12");
    asm("ST X+, R13");
    asm("ST X+, R14");
    asm("ST X+, R15");
    asm("ST X+, R16");
    asm("ST X+, R17");
    asm("ST X+, R18");
    asm("ST X+, R19");
    asm("ST X+, R20");
    asm("ST X+, R21");
    asm("ST X+, R22");
    asm("ST X+, R23");
    asm("ST X+, R24");
    asm("ST X+, R25");
    asm("POP R25");     // R26-27 = X
    asm("POP R24");     // R26-27 = X
    asm("ST X+, R24");  // R26-27 = X
    asm("ST X+, R25");  // R26-27 = X
    asm("ST X+, R28");
    asm("ST X+, R29");
    asm("ST X+, R30");
    asm("POP R0");      // get original R31 from stack
    asm("ST X+, R0");   // original R31
    asm("ST X+, R31");  // SREG
    asm("IN R0,0x3D");  // SP; all pushes are popped here; SP is same as at begin of ISR
    asm("IN R1,0x3E");  // SP
    asm("ST X+, R0");   // SP   
    asm("ST X+, R1");   // SP


    // clear R1; avr-gcc assumes R1 to be zero when executing C code
    asm("CLR R1");
    // clear overflow flag
    TIFR2 = 0;
    // do the actual context switch
    if (++currentThreadID >= sizeof(SavedContext) / sizeof(SavedContext[0]))
        currentThreadID = 0;
    context = &SavedContext[currentThreadID];


    asm("ldi r26, lo8(context)");   // load address of the context pointer into X
    asm("ldi r27, hi8(context)");  
    asm("ld r28, X+");              // load the value of the context pointer into Y
    asm("ld r29, X+");
    asm("LDD R16, Y+32");   // SREG
    asm("OUT 0x3F, R16");   // SREG
    asm("LDD R16, Y+33");   // SP
    asm("LDD R17, Y+34");   // SP
    asm("OUT 0x3D, R16");   // SP
    asm("OUT 0x3E, R17");   // SP   
    asm("LD R0,  Y+");
    asm("LD R1,  Y+");
    asm("LD R2,  Y+");
    asm("LD R3,  Y+");
    asm("LD R4,  Y+");
    asm("LD R5,  Y+");
    asm("LD R6,  Y+");
    asm("LD R7,  Y+");
    asm("LD R8,  Y+");
    asm("LD R9,  Y+");
    asm("LD R10, Y+");
    asm("LD R11, Y+");
    asm("LD R12, Y+");
    asm("LD R13, Y+");
    asm("LD R14, Y+");
    asm("LD R15, Y+");
    asm("LD R16, Y+");
    asm("LD R17, Y+");
    asm("LD R18, Y+");
    asm("LD R19, Y+");
    asm("LD R20, Y+");
    asm("LD R21, Y+");
    asm("LD R22, Y+");
    asm("LD R23, Y+");
    asm("LD R24, Y+");
    asm("LD R25, Y+");
    asm("LD R26, Y+");
    asm("LD R27, Y+");
    asm("LD R30, Y+");  // can't store Y (R28-29 yet, so store on stack)
    asm("LD R31, Y+");  // can't store Y (R28-29 yet, so store on stack)
    asm("PUSH R30");    // can't store Y (R28-29 yet, so store on stack)
    asm("PUSH R31");    // can't store Y (R28-29 yet, so store on stack)
    asm("LD R30, Y+");  
    asm("LD R31, Y+");  
    asm("POP R29");     // finally restore Y (R28-29)
    asm("POP R28");     // finally restore Y (R28-29)

    asm("reti");
}



void NumPrimes()
{

    while(1)
    {
        int i = 0;
        volatile int numPrimes = 0;
        for (i = 3; i < 100; i++)
        {
            int isPrime = 1;
            int j;
            for (j = 2;j < i; j++)
            {
                if (i % j == 0)
                {
                    isPrime = 0;
                    break;
                }
            }
            if (isPrime)
                numPrimes++;
        }
    }
}

void ToggleLed()
{
    while(1)
    {
        PORTB ^= 0x80;
        _delay_ms(500);
    }
}

void InitRTOS()
{
    int i;
    uint16_t *ptr;
    uint16_t sp = SPH<<8 | SPL; // TODO: init
    for (i = 0; i < sizeof(SavedContext)/sizeof(SavedContext[0]); i++)
    {
        SavedContext[i].stack_pointer = (void*)sp;
        ptr = (uint16_t*)(((uint16_t)SavedContext[i].stack_pointer) + 1);
        uint16_t val = (uint16_t)SavedContext[i].Start;
        val = val << 8 | ((val >> 8) & 0xFF);
        memcpy(ptr, &val, sizeof(val));
        sp -= SavedContext[i].stack_space;
    }

    context = &SavedContext[0];

    TCCR2A = 0;
    TCCR2B = (1 << CS22);
    TIMSK2 = 1;
    sei();

    SavedContext[0].Start();
    
    while(1);
    
}
int main(void)
{
    DDRB = 0x80;
    InitRTOS();
    while(1);
    return 0;
}