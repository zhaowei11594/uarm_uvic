// Demo 
#include "uArm.h"
#include <math.h>

#define USE_SERIAL_CMD	0	// 1: use serial for control	0: just use arduino to control(release ROM and RAM space)

unsigned long tickStartTime = millis(); // get timestamp;
unsigned long startTime, lastTime, controlLoopTime;
int freqWait = 0;
float curX, curY, curZ, deltaX, deltaY, deltaZ;
boolean newPosFlag = true; //used to initialize a postion delta
boolean switchFlag = true; //used to switch between goal positions
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
  curX = 0.0;
  curY = 150.0;
  curZ = 150.0;
  
  controlLoopTime = startTime;

}

void loop()
{
	if((millis()-controlLoopTime) >= freqWait)
 {
    controlLoopTime = millis();
  	run(); // Don't remove
    if (switchFlag)
    {
      min_jerk(70.0, 200.0, 110.0, 1.0);
    }
    else 
    {
      min_jerk(-80.0, 100.0, 160.0, 3.0);
    }

 }
}

void min_jerk(float destX,float destY, float destZ, float duration)
{
  if(newPosFlag){
    startTime = millis();
    lastTime = startTime;
    deltaX = destX - curX;
    deltaY = destY - curY;
    deltaZ = destZ - curZ;
    newPosFlag = false;
  }
  if((millis() - startTime)<= (duration*1000)) //increment through the min jerk trajec
  {
    unsigned long nowTime = millis();
    float deltaPercent = (-0.5*cos(M_PI*(nowTime-startTime)/(duration*1000))+1) - (-0.5*cos(M_PI*(lastTime-startTime)/(duration*1000))+1); //percentage of the overall distance being completed in this iteration
    float newX = deltaX*deltaPercent + curX;
    float newY = deltaY*deltaPercent + curY;
    float newZ = deltaZ*deltaPercent + curZ;
    moveTo(newX,newY,newZ,0);
    lastTime = nowTime;
    curX = newX; //updating the positions
    curY = newY;
    curZ = newZ;
  }
  else //once position is complete it will print the final position
  {
    Serial.print("pos x: ");
    Serial.print(curX);
    Serial.print(" pos y: ");
    Serial.print(curY);
    Serial.print(" pos z: ");
    Serial.println(curZ);
    switchFlag = !switchFlag;
    newPosFlag = true;
  }
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
