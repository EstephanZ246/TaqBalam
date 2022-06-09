
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


#define NUM_SPI_DATA    1  // N�mero de palabras que se env�an cada vez
#define VALOR_MAX    4095  // Valor m�ximo del contador
#define SPI_FREC  4000000  // Frecuencia para el reloj del SPI
#define SPI_ANCHO      16  // N�mero de bits que se env�an cada vez, entre 4 y 16

float v0, v1;   // para ilustrar el uso de variables tipo float


char recibido[20];
float coeficientes[20];//Guardamos coeficientes a, denominador (con y(n))
//float coeficientesb[6];//Guardamos coeficientes b, numerador (con x(n))

float x_n, x_n1, x_n2, x_n3, x_n4, x_n5; // Guardar muestras actuales y pasadas
float y_n, y_n1, y_n2, y_n3, y_n4, y_n5;
float ganancia;

// contadores útiles para guardar coeficientes y datos recibidos
int contadornum = 0;
int contadorindex = 0;
char comenzar = 0;
char temporal;
//char prueba[5] = {'1','2','.','3','4'};

long freq_muestreo = 1000;    // En Hz
uint16_t freq_timer = 1000;    // En Hz




//
//*****************************************************************************
//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void UARTIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrrupt status.
    //
    //ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    ui32Status = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    //ROM_UARTIntClear(UART0_BASE, ui32Status);
    UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        //UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
//  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
// GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        int i;
        temporal = UARTCharGetNonBlocking(UART0_BASE);


        if(temporal == 'A'){ // Obtener coeficientes de matlab, interpretar y guardar
            contadorindex = 0;
            contadornum = 0;
            comenzar = 1;
            temporal = UARTCharGetNonBlocking(UART0_BASE);

        }else if (temporal == ','){
            coeficientes[contadorindex] = atof(recibido);

            contadorindex++;
            contadornum = 0;
            temporal= UARTCharGetNonBlocking(UART0_BASE);

            for (i = 0; i < 11 ;i++){
                recibido[i] = 0;
            }

        }else if (temporal == 'B'){

            coeficientes[contadorindex] = atof(recibido);
            contadorindex = 0;
            comenzar = 0;
            contadornum = 0;

            for (i = 0; i < 11 ;i++){
                recibido[i] = 0;
            }
        }

        if (comenzar == 1){
            recibido[contadornum] = temporal;
            contadornum++;
        }

        if (temporal == 'F'){// obtener frec de muestreo y reconfigurar
            contadornum = 0;
            comenzar = 1;
            for (i = 0; i < 11 ;i++){
                recibido[i] = 0;
            }


        }else if (temporal == 'H'){
            contadornum = 0;
            comenzar = 0;

            freq_muestreo = atoi(recibido);
            freq_timer = atoi(recibido);
            TimerLoadSet(TIMER0_BASE, TIMER_A, (uint32_t)(SysCtlClockGet()/freq_muestreo));

            for (i = 0; i < 11 ;i++){
                recibido[i] = 0;
            }

        }







    }
}

//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void
Timer0IntHandler(void)
{
    // Notar que ahora necesitamos dos espacios para las conversiones.
    uint32_t pui32ADC0Value[2];

    // Clear the timer interrupt. Necesario para lanzar la pr�xima interrupci�n.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Trigger the ADC conversion.
    ADCProcessorTrigger(ADC0_BASE, 2);  // Notar el cambio de "secuencia" (2 en lugar de 3).

    // Wait for conversion to be completed.
    while(!ADCIntStatus(ADC0_BASE, 2, false))  // Notar el cambio de "secuencia".
    {
    }

    // Clear the ADC interrupt flag.
    ADCIntClear(ADC0_BASE, 2);  // Notar el cambio de "secuencia".

    // Ahora se leen dos valores, no s�lo uno, seg�n la configuraci�n.
    // En pui32ADC0Value[0] se tendr� el valor del puerto AIN0 y
    // en pui32ADC0Value[1] se tendr� el valor del puerto AIN1, porque as� se
    // configur� en el main.
    ADCSequenceDataGet(ADC0_BASE, 2, pui32ADC0Value);  // Notar el cambio de "secuencia".

   // v0 = pui32ADC0Value[0]*3.3/4095.0;  // Convertir a voltios
   // v1 = pui32ADC0Value[1]*3.3/4095.0;  // Convertir a voltios

    // Se muestra el valor convertido de AIN0 (PE3) y AIN1 (PE2), que son enteros
    // entre 0 y 4095, y los valores en voltios, entre 0.00 y 3.30.
    // Nota: UARTprintf no soporta floats (%f). Ac� se hace un truco para mostrar
    // la parte entera y dos posiciones decimales, como enteros.
    // Notar que el "casting" trunca, no redondea.
    // Si no es indispensable desplegar floats, mejor evitar hacerlo.

    UARTprintf(" %d\n", pui32ADC0Value[0]);

    x_n = 1.0*pui32ADC0Value[0];

   /* float x_n, x_n1, x_n2, x_n3, x_n4, x_n5;
    float y_n, y_n1, y_n2, y_n3, y_n4, y_n5;*/

    // Se aplica ecuacion de diferencias y se guardan valores de muestras

   // y_n = coeficientes[6]*x_n+coeficientes[7]*x_n1+coeficientes[8]*x_n2+coeficientes[9]*x_n3+coeficientes[10]*x_n4+coeficientes[11]*x_n5+((-1)*(coeficientes[1]*y_n1+coeficientes[2]*y_n2+coeficientes[3]*y_n3+coeficientes[4]*y_n4+coeficientes[5]*y_n5));

    y_n = coeficientes[6]*x_n+coeficientes[7]*x_n1+coeficientes[8]*x_n2+coeficientes[9]*x_n3+coeficientes[10]*x_n4+coeficientes[11]*x_n5+((-coeficientes[1]*y_n1-coeficientes[2]*y_n2-coeficientes[3]*y_n3-coeficientes[4]*y_n4-coeficientes[5]*y_n5));

   // y_n = 0.9*x_n-0.9*x_n1+coeficientes[8]*x_n2+coeficientes[9]*x_n3+coeficientes[10]*x_n4+coeficientes[11]*x_n5+((-0.6*y_n1-coeficientes[2]*y_n2-coeficientes[3]*y_n3-coeficientes[4]*y_n4-coeficientes[5]*y_n5));


    y_n5 = y_n4;
    y_n4 = y_n3;
    y_n3 = y_n2;
    y_n2 = y_n1;
    y_n1 = y_n;

    x_n5 = x_n4;
    x_n4 = x_n3;
    x_n3 = x_n2;
    x_n2 = x_n1;
    x_n1 = x_n;

    if (y_n < 0){ // Se trunca la señal para no tener problemas con el módulo DAC
        y_n = 0;
    }
    if (y_n > 4095){
        y_n = 4095;
    }


  //  UARTprint('\n');


    uint32_t pui32DataTx[NUM_SPI_DATA]; // la funci�n put pide tipo uint32_t
    uint8_t ui32Index;

     // El DAC espera 16 bits: 4 de configuraciones y 12 del dato
    pui32DataTx[0] = (uint32_t)((0b0111 << 12) | (0x0FFF & (int)y_n));

    //pui32DataTx[0] = (uint32_t)((0b0111 << 12) | (0x0FFF & pui32ADC0Value[0]));


    //UARTprintf(" %d\n", pui32DataTx[0]);

     // Hacemos la operacion de envio en SPI
    for(ui32Index = 0; ui32Index < NUM_SPI_DATA; ui32Index++)
    {
         // Enviamos los datos utilizando la funci�n put de "bloqueo". Esta funci�n
         // esperar� hasta que haya espacio en el FIFO enviado antes de regresar.
         // Esto asegura que todos los datos que envia se conviertan en el env�o FIFO.
        SSIDataPut(SSI0_BASE, pui32DataTx[ui32Index]);
    }

     // Esperamos hasta que SSI0 termine de transferir todos los datos en el FIFO de transmisi�n.
    while(SSIBusy(SSI0_BASE))
    {
    }


}


void
SPIConfig(void)
{

    uint32_t pui32residual[NUM_SPI_DATA];
    //uint16_t freq_timer = 1000;    // En Hz

    // ------ Configuraci�n del reloj ---------------------------------------------
        // Ajustamos el reloj para 80 MHz (200 MHz / 2.5) usando el PLL.
        SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ); // 80 MHz.
    // ----------------------------------------------------------------------------

        // Se habilita el perif�rico SSI0 para su uso.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

        // Hay que habilitar el puerto donde Usaremos los pines de comunicaci�n SPI.
        // Por seguridad igualmente no va a cambiar nada las configuraci�nes que hicimos
        // en el portA para UART, debido a que no usaremos los mismos pines.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        // Configuramos cada pin con sus respectivas configuraciones de uso
        GPIOPinConfigure(GPIO_PA2_SSI0CLK);     // PA2 - SSI0CLK - Clock
        GPIOPinConfigure(GPIO_PA3_SSI0FSS);     // PA3 - SSI0Fss - Frecuencia
        GPIOPinConfigure(GPIO_PA4_SSI0RX);      // PA4 - SSI0Rx  - Lectura (Unque no lo usaremos para leer el UART)
        GPIOPinConfigure(GPIO_PA5_SSI0TX);      // PA5 - SSI0Tx  - Comunicaci�n dirigida al MCP4921

        // Configuramos los ajustes GPIO para los pines SSI. Esta funci�n tambi�n da
        // control de estos pines al hardware SSI.
        GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                       GPIO_PIN_2);

        // Configuramos el SSI(SPI) para el modo maestro que habilita operaciones
        // de Reloj, frecuencia , comunicaci�n, etc.
        SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_MASTER, SPI_FREC, SPI_ANCHO);

        // Terminamos de habilitar el modulo SSI0
        SSIEnable(SSI0_BASE);

        // Lea cualquier dato residual del puerto SSI. Esto asegura que  Los FIFO recibidos
        // est�n vac�os, por lo que no leemos basura no deseada. esto se hace aqui porque el
        // modo SPI SSI es full-duplex, lo que le permite enviar y recibir al mismo tiempo.
        while(SSIDataGetNonBlocking(SSI0_BASE, &pui32residual[0]))
        {
        }

        // Habilitamos el Peripheral del Timer
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

        // Habilitamos las Interrupciones de Timer
        IntMasterEnable();

        // Configure los dos temporizadores peri�dicos de 32 bits.
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

        // El tercer argumento determina la frecuencia. El reloj se puede obtener
        // con SysCtlClockGet().
        // La frecuencia est� dada por SysCtlClockGet()/(valor del 3er argumento).
        TimerLoadSet(TIMER0_BASE, TIMER_A, (uint32_t)(SysCtlClockGet()/freq_timer));

        // Configure la interrupci�n para el tiempo de espera del temporizador.
        IntEnable(INT_TIMER0A);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

        // Por ultimo habilitamos el Timer para la cumunicaci�n y operaciones en nuestro Puerto A
        TimerEnable(TIMER0_BASE, TIMER_A);

}


//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void
InitConsole(void)
{
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    // Enable UART0 so that we can configure the clock.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);

    IntEnable(INT_UART0);   // INT_UART0 se vuelve INT_UART0_TM4C123
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

//*****************************************************************************
//
// Configure ADC0 for a single-ended input and a single sample.  Once the
// sample is ready, an interrupt flag will be set.  Using a polling method,
// the data will be read then displayed on the console via UART0.
//
//*****************************************************************************
int
main(void)
{


    // Set the clocking to run at 80 MHz (200 MHz / 2.5) using the PLL.  When
    // using the ADC, you must either use the PLL or supply a 16 MHz clock source.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
    //                 SYSCTL_XTAL_16MHZ); // 20 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ); // 80 MHz

    // Set up the serial console to use for displaying messages.  This is
    // just for this example program and is not needed for ADC operation.
    InitConsole();

    // The ADC0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // For this example ADC0 is used with AIN0 and AIN1.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.  GPIO port E needs to be enabled
    // so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

    SPIConfig();

    // Select the analog ADC function for these pins.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);  // Configura el pin PE3
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);  // Configura el pin PE2

    // Se configura la secuencia 2, que permitir�a hasta cuatro muestras (aunque
    // se usar�n dos).
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);

    // Step 0 en la secuencia 2: Canal 0 (ADC_CTL_CH0) en modo single-ended (por defecto).
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0);

    // Step 1 en la secuencia 2: Canal 1 (ADC_CTL_CH1) en modo single-ended (por defecto),
    // y configura la bandera de interrupci�n (ADC_CTL_IE) para "setearse"
    // cuando se tenga esta muestra. Tambi�n se indica que esta es la �ltima
    // conversi�n en la secuencia 2 (ADC_CTL_END).
    // Para m�s detalles del m�dulo ADC, consultar el datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);




    // Since sample sequence 2 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 2);  // Notar el cambio de "secuencia".

    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    ADCIntClear(ADC0_BASE, 2);  // Notar el cambio de "secuencia".

    // Enable the peripherals used by this example.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Enable processor interrupts.
    IntMasterEnable();

    // Configure the two 32-bit periodic timers.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // El tercer argumento determina la frecuencia. El reloj se puede obtener
    // con SysCtlClockGet().
    // La frecuencia est� dada por SysCtlClockGet()/(valor del 3er argumento).
    // Ejemplos: Si se pone SysCtlClockGet(), la frecuencia ser� de 1 Hz.
    //           Si se pone SysCtlClockGet()/1000, la frecuencia ser� de 1 kHz
    //TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
    TimerLoadSet(TIMER0_BASE, TIMER_A, (uint32_t)(SysCtlClockGet()/freq_muestreo));

    // Setup the interrupt for the timer timeout.
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable the timers.
    TimerEnable(TIMER0_BASE, TIMER_A);

    // Las conversiones se hacen al darse la interrupci�n del timer, para que
    // el muestreo sea preciso. Luego de las configuraciones, el programa se
    // queda en un ciclo infinito haciendo nada.
    while(1)
    {


    }
}
