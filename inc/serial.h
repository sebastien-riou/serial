#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE serial_t;
static int serial_open(serial_t *ctx, const char*port){
    char buf[1024]={0};
    strcpy(buf,"\\\\.\\");
    strncat(buf,port,sizeof(buf)-5);
    *ctx = CreateFile(buf,            //port name 
        GENERIC_READ | GENERIC_WRITE, //Read/Write   				 
        0,            // No Sharing                               
        NULL,         // No Security                              
        OPEN_EXISTING,// Open existing port only                     
        0,            // Non Overlapped I/O                           
        NULL);        // Null for Comm Devices
                         
    if (*ctx == INVALID_HANDLE_VALUE) return 1;
    return 0;
}
static void serial_close(serial_t*ctx){
    CloseHandle(*ctx);//Closing the Serial Port
}

static int serial_dtr(serial_t*ctx,int level){
    if(0==EscapeCommFunction(*ctx, level ? SETDTR : CLRDTR)) return 1;
    return 0;
}

static int serial_rts(serial_t*ctx,int level){
    if(0==EscapeCommFunction(*ctx, level ? SETRTS : CLRRTS)) return 1;
    return 0;
}

static void serial_delay_ms(unsigned int ms){
  Sleep(ms);
}
#else

//POSIX
#include <unistd.h>
#include <fcntl.h> 
#include <termios.h>
typedef serial_t int;

static void serial_delay_ms(unsigned int ms){
    usleep(ms*1000);
}
#endif

static int serial_rts_pulse_ms(serial_t*ctx,unsigned int ms){
    int out = serial_rts(ctx,1);
    serial_delay_ms(ms);
    out|=serial_rts(ctx,0);
    return out;
}

#endif

