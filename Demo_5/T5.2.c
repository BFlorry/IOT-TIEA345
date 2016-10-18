#define BCM2708_PERI_BASE        0x3f000000 // TST: changed for Pi 2, check version
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
 
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)
 
int  mem_fd;
void *gpio_map;
 
// I/O access
volatile unsigned *gpio;
 
 
// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
 
#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0
 
#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH
 
#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock
 
void setup_io();
 
static int led = 21, btn = 20, pir = 25, pedRed = 22, pedGrn = 27, carRed = 5, carYlw = 13, carGrn = 19;

// http://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c
static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

void printButton(int g)
{
  if (GET_GPIO(g)) // !=0 <-> bit is 1 <- port is HIGH=3.3V
    printf("Button pressed!\n");
  else // port is LOW=0V
    printf("Button released!\n");
}

void clear() {
	GPIO_CLR = 1<<led;
    GPIO_CLR = 1<<pedRed;
    GPIO_CLR = 1<<pedGrn;
    GPIO_CLR = 1<<carRed;
    GPIO_CLR = 1<<carYlw;
    GPIO_CLR = 1<<carGrn;
}


void pedToGrn() {
    GPIO_CLR = 1<<pedRed;
    GPIO_SET = 1<<pedGrn;
    sleep(1);
}

void pedToRed() {
    GPIO_SET = 1<<pedRed;
	GPIO_CLR = 1<<pedGrn;
    sleep(1);
}

void carToRed() {
    GPIO_CLR = 1<<carRed;
    GPIO_SET = 1<<carYlw;
    GPIO_CLR = 1<<carGrn;
    sleep(1);
	GPIO_SET = 1<<carRed;
    GPIO_CLR = 1<<carYlw;
    sleep(1);
	
	pedToGrn();
}

void carToGrn() {
	pedToRed();
	
    GPIO_SET = 1<<carRed;
    GPIO_SET = 1<<carYlw;
	GPIO_CLR = 1<<carGrn;
    sleep(1);
    GPIO_CLR = 1<<carYlw;
    GPIO_SET = 1<<carGrn;
    sleep(1);
	
}

void reserve() {
    // pedestrian light on
    GPIO_SET = 1<<led;
    int time = 0;
	
	// wait for movement for max 6sec
    while(GET_GPIO(pir) && time < 600) {
        usleep(10000); //sleep for 10 milliseconds (0.01sec)
        time++;
    }
	
    carToRed(); //Let pedestrians pass
	
    GPIO_CLR = 1<<led;

    sleep(5); //Sleep for 5 seconds
	
    carToGrn(); //Let cars pass
}
 
int main(int argc, char **argv)
{
  signal(SIGINT, intHandler);

  int g,rep;
 
  // Set up gpi pointer for direct register access
  setup_io();
 
 /************************************************************************\
  * You are about to change the GPIO settings of your computer.          *
  * Mess this up and it will stop working!                               *
  * It might be a good idea to 'sync' before running this program        *
  * so at least you still have your code changes written to the SD-card! *
 \************************************************************************/
 
  INP_GPIO(pedRed);
  INP_GPIO(pedGrn);
  INP_GPIO(carRed);
  INP_GPIO(carYlw);
  INP_GPIO(carGrn);
  INP_GPIO(led);
  INP_GPIO(btn);
  INP_GPIO(pir);

  OUT_GPIO(pedRed);
  OUT_GPIO(pedGrn);
  OUT_GPIO(carRed);
  OUT_GPIO(carYlw);
  OUT_GPIO(carGrn);
  OUT_GPIO(led);

  // set starting leds
  GPIO_SET = 1<<carGrn;
  GPIO_SET = 1<<pedRed;

  while (keepRunning) {
      usleep(10000);
      // wait for button input
      if(GET_GPIO(btn)) {
          reserve();
      }
  }

  clear();
 
  return 0;
 
} // main
 

//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }
 
   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );
 
   close(mem_fd); //No need to keep mem_fd open after mmap
 
   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);//errno also set!
      exit(-1);
   }
 
   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;
 
 
} // setup_io
