// Demo 
#include "uArm.h"
#include <math.h>

#define USE_SERIAL_CMD	0	// 1: use serial for control	0: just use arduino to control(release ROM and RAM space)

unsigned long tickStartTime = millis(); // get timestamp;
unsigned long startTime, lastTime;
int freqWait = 0;
int count =0;
boolean changeFreq = true;
void setup()
{
	Serial.begin(115200);
	Init(); // Don't remove

	debugPrint("debug start"); // uncomment DEBUG in uArmConfig.h to use debug function
	int frequency = 200;
  freqWait = (1000/frequency);
	// TODO
	service.setButtonService(false);	// disable build in button service

	moveTo(0, 150, 150);
  startTime = millis();
  lastTime = startTime;

}

void loop()
{
	if((millis()-lastTime) >= freqWait)
 {
    lastTime = millis();
  	run(); // Don't remove
    circle(30, 2.0);

 }
}

void circle(float radius, float period)
{
  //amplitude of sin and cos must equal radius (mm)
  //sin/cos take in radians so the period (seconds) is initially 2pi
  //// millis div by (2pi*period)
  count++;
  if (count%10 == 0){
    Serial.println(count*1000.0/(millis()-startTime));
  }
  float originX = 0;
  float originY = 130;
  float newX = radius*sin(2*M_PI*(millis()-startTime)/(period*1000)) + originX;
  float newY = radius*cos(2*M_PI*(millis()-startTime)/(period*1000)) + originY;
  moveTo(newX,newY,120,0);
}

void tickTimeOut()
{
	
}

////////////////////////////////////////////////////////////
// DO NOT EDIT
void Init()
{
	uArmInit();	// Don't remove //calls pinmode on buttons
	service.init();

	#if USE_SERIAL_CMD == 1
	serialCmdInit();
	

	#endif
}

void run()
{
	#if USE_SERIAL_CMD == 1
	handleSerialCmd();
	#endif

	manage_inactivity(); // Don't remove
}

void tickTaskRun()
{
	tickTimeOut();

    buttonPlay.tick();
    buttonMenu.tick();
#ifdef MKII
    ledRed.tick();
    service.btDetect();
#endif    
}

void manage_inactivity(void)
{
#if USE_SERIAL_CMD == 1
	getSerialCmd();	// for serial communication
#endif
	service.run();	// for led, button, bt etc.

	// because there is no other hardware timer available in UNO, so use a soft timer
	// it's necessary for button,led, bt
	// so Don't remove it if you need them
	if(millis() - tickStartTime >= TICK_INTERVAL)
	{
		tickStartTime = millis();
		tickTaskRun();
	}   
}
