/*
	fpe:       main.c
	Date:      04.04.2017
	Author:    Maximus Byamukama <maximus.byamukama@cedat.mak.ac.ug>
	Details:   sd card driver for AVR-RSS2 mote.
*/
//#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "integer.h"
#include "uart.h"

FATFS FatFs;	// FatFs work area 
FIL *fp;		// file object 


/*---------------------------------------------------------*/
/* User Provided RTC Function called by FatFs module       */
/* Used to provide a Timestamp for SDCard files and folders. Leave untouched if no RTC is present*/
DWORD get_fattime (void)
{
	return 1491306915; 
}



int main(void)
{
	int iterate = 412, j=0,lines=0;
	char buf[100],text[20];
	// reboot delay - probably unnecessary
	_delay_ms(100);
	uart_init ();
	
	// init sdcard
	UINT bw; //function result holder
	f_mount(0, &FatFs);		// Give a work area to the FatFs module 

	fp = (FIL *)malloc(sizeof (FIL));
	if (f_open(fp, "data.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)  	// open file for writing. if not present, create file
	{	// Create a file 
		for(j=0; j<iterate; j++)
		{
			sprintf(text, "This is line %d\n\r",j);
			f_write(fp, text, strlen(text), &bw);	// Write data to the file 
			if (bw == strlen(text)) { //we wrote the entire string
				//say something
			}
		}
	}
	f_close(fp);// Close the file	
	
	// test append
	if (f_open(fp, "data.txt", FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {	// Open existing or create new file
		if (f_lseek(fp, f_size(fp)) == FR_OK) 
		{
			char * text2= "appending new information\n";
			f_write(fp, text2, strlen(text2), &bw);	// Write data to the file
			
			for(j=0; j<iterate; j++)
			{
				sprintf(text, "This is line %d\n",j);
				f_write(fp, text, strlen(text), &bw);	// Write data to the file
				if (bw == strlen(text)) { //we wrote the entire string
					//say something
				}
			}
			
		}		
	}
	f_close(fp);// Close the file*/
	
	// test read
	if (f_open(fp, "data.txt", FA_READ) == FR_OK) 
	{	/* Read all lines and display it */
     for (lines = 0; (f_eof(fp) == 0); lines++)
		{
		   f_gets((char*)buf, sizeof(buf), fp);
		uart_puts(buf);
	    uart_puts("\n\r");
		}		
	}
	sprintf(text,"There are %d lines in the file", lines);
    uart_puts(text);
	f_close(fp);// Close the file	
}