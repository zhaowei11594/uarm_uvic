#include <uArm.h>
#include <math.h>


#define USE_SERIAL_CMD  1

int count = 0;
unsigned long lastTime = millis();
unsigned long tickStartTime = lastTime;
unsigned long startTime = lastTime;

 
void setup() {
  Serial.begin(115200);
  Init(); // Don't remove
  service.setButtonService(false);
  moveTo(100, 150, 170);
  lastTime = millis();
 
}

void loop() {
  bool runFlag = true;
  float freqArray [500];
  int freqCnt = 0;
  while(runFlag)
  {
    count++;
    run();
    moveTo(100, 150, 150);
    moveTo(150,150,200);
    if(count == 100) //Frequency Calculator
    {
      unsigned long currentTime = millis();
      freqArray[freqCnt]= float(count)/(currentTime-lastTime);
      Serial.println(freqArray[freqCnt]);
      lastTime = currentTime;run();
      freqCnt++;
      count = 0;
    } 
    if ((lastTime-startTime) >= 10000)
    {
      runFlag = false;
    } 
  }
// / moveTo(100, 150, 150);
 /* for(int i=0; i<500; i++)
  {
    Serial.println( freqArray[i]);
    delay(100);
  } */
  
}
void run()
{
    #if USE_SERIAL_CMD == 1
    handleSerialCmd();
    #endif
    manage_inactivity();
}

void Init()
{
  uArmInit(); // Don't remove
  service.init();

  #if USE_SERIAL_CMD == 1
  serialCmdInit();
  

  #endif
}

void manage_inactivity(void)
{
#if USE_SERIAL_CMD == 1
  getSerialCmd(); // for serial communication
#endif
  
  service.run();  // for led, button, bt etc.

  // because there is no other hardware timer available in UNO, so use a soft timer
  // it's necessary for button,led, bt
  // so Don't remove it if you need them
  if(millis() - tickStartTime >= TICK_INTERVAL)
  {
    tickStartTime = millis();
    tickTaskRun();
  }   
}

void tickTaskRun()
{
    buttonPlay.tick();
    buttonMenu.tick();
#ifdef MKII
    ledRed.tick();
    service.btDetect();
#endif    
}

int cosX( unsigned long time)
{

  
}


