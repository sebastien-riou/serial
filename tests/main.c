#include <stdio.h>
#include "serial.h"

int main(int argc, char*argv[]){
    if(argc!=2){
        printf("ERROR: Need exactly one argument: the COM port name\n");
        return -1;
    }
    printf("openning port %s\n",argv[1]);
    serial_t ctx_storage;serial_t*ctx=&ctx_storage;
    if (serial_open(ctx,argv[1])){
        printf("Error in opening serial port %s\n",argv[1]);
        //return -2; //skipping return allows testing on linux even when the device access is denied!
    } else {
        printf("opening serial port successful\n");
    }
    printf("Press the following keys:\n");
    printf("1: Set DTR\n");
    printf("2: Clear DTR\n");
    printf("3: Set RTS\n");
    printf("4: Clear RTS\n");
    printf("5: 200ms Pulse on RTS\n");
    printf("q: End Program\n");

    uint8_t ch;
    do
    {
        fflush(stdout);
        ch = getchar();
        switch (ch)
        {
        case '1':   if (serial_dtr(ctx,1))
                        printf ("Error Setting DTR\n");
                    break;
        case '2':   if (serial_dtr(ctx,0))
                        printf ("Error Clearing DTR\n");
                    break;
        case '3':   if (serial_rts(ctx,1))
                        printf ("Error Setting RTS\n");
                    break;
        case '4':   if (serial_rts(ctx,0))
                        printf ("Error Clearing RTS\n");
                    break;
        case '5':   if(serial_rts_pulse_ms(ctx,200))
                        printf("Error toggling RTS");
                    break;
        }
    }   while (ch != 'q');

    serial_close(ctx);
    return 0;
}
