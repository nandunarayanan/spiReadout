/*
 * spiSpeed.c:
 *	Code to measure the SPI speed/latency.
 *	Copyright (c) 2014 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://github.com/WiringPi/WiringPi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <linux/spi/spidev.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define	TRUE	(1==1)
#define	FALSE	(!TRUE)

#define	SPI_CHAN		0
#define	NUM_TIMES		   100
#define	MAX_SIZE		      (1024*1024)
#define  READ_PARAM_NOS    6
static int myFd ;

unsigned int microsecond = 1000000;
unsigned int param_nos = READ_PARAM_NOS;

void spiSetup (int speed)
{
  if ((myFd = wiringPiSPISetup (SPI_CHAN, speed)) < 0)
  {
    fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE) ;
  }
}


int main (void)
{
   int speed, times, size ;
   unsigned int idata =0xABCD;
   int spiFail ;
   unsigned char *myData ;
   double timePerTransaction, perfectTimePerTransaction, dataSpeed ;

   if ((myData = (unsigned char*) malloc (MAX_SIZE)) == NULL)
   {
    fprintf (stderr, "Unable to allocate buffer: %s\n", strerror (errno)) ;
    exit (EXIT_FAILURE) ;
   }

   wiringPiSetup () ;

   spiSetup (1000000) ;
  
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;
   printf ("|             iMOTION SPI ReadOut - RPi               |\n") ;
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;
   printf ("|   Iu   |   Iv   | VdcRaw | RAngle | Valpha | Vbeta  |\n") ;
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;   
   size = sizeof(idata);
   //myData = (unsigned char*)&idata;
   
   while(1)
   {
      spiFail = FALSE ;
      param_nos = READ_PARAM_NOS;
      while(param_nos--)
      {
         idata =0xABCD;
         if (wiringPiSPIDataRW (SPI_CHAN, (unsigned char*)&idata, 2) == -1)
         {
            printf ("SPI failure: %s\n", strerror (errno)) ;
            spiFail = TRUE ;
            break ;
         }
         printf ("| 0x%04x ", idata) ;
     
      }//end of sub while
      
      if (spiFail)
      {
         break ;
      }
      printf ("|\n") ;
      sleep(1);//sleeps for 1 second
   } //end of main while
   
   printf("Break with SPI fail!!\n");
   close (myFd) ;

   return 0 ;
}
