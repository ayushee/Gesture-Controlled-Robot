#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
#include <VirtualWire.h>
char *controller;
//ADXL345 adxl = ADXL345(10);           // USE FOR SPI COMMUNICATION, ADXL345(CS_PIN);
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION
 

void setup(){
  
  Serial.begin(9600);                 // Start the serial terminal
   pinMode(13,OUTPUT);
   vw_set_ptt_inverted(true); //
   vw_set_tx_pin(12);
   vw_setup(4000);// speed of data transfer Kbps
  
  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
   
  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
 
  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 
  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
 
  // Setting all interupts to take place on INT1 pin
  //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);
  
//attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);   // Attach Interrupt

}

/****************** MAIN CODE ******************/
/*     Accelerometer Readings and Interrupt    */
void loop(){
  
  // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  // Output Results to Serial
  /* UNCOMMENT TO VIEW X Y Z ACCELEROMETER VALUES */  
  //Serial.print(x);
 // Serial.print(", ");
  //Serial.print(y);
  //Serial.print(", ");
  //Serial.println(z); 

  if(x>=-5 && x<=10)
  {
    if(y>=-35 && y<-12)
     {
        controller="r"  ;
        vw_send((uint8_t *)controller, strlen(controller));
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(13,1);
        Serial.println("r");
        delay(5);
     }
     else if(y>0 && y<=35)
     {
        controller="l"  ;
        vw_send((uint8_t *)controller, strlen(controller));
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(13,1);
        Serial.println("l");
        delay(5);
     }
     else
     {
        controller="s"  ;
        vw_send((uint8_t *)controller, strlen(controller));
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(13,1);
        Serial.println("s");
        delay(5);
        
     }
  }
  else if(y>=-12 && y<=0)
  {
    if(x>=-35 && x<-5)
    {
        controller="b"  ;
        vw_send((uint8_t *)controller, strlen(controller));
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(13,1);
        Serial.println("b");
        delay(5);
    }
    else if(x>10 && x<=35)
    {
        controller="f"  ;
        vw_send((uint8_t *)controller, strlen(controller));
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(13,1);
        Serial.println("f");
        delay(5);
    }    
    else
     {
        controller="s"  ;
        vw_send((uint8_t *)controller, strlen(controller));
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(13,1);
        Serial.println("s");
        delay(5);
     }
    
  }
  ADXL_ISR();

  // You may also choose to avoid using interrupts and simply run the functions within ADXL_ISR(); 
  //  and place it within the loop instead.  
  // This may come in handy when it doesn't matter when the action occurs. 

}

/********************* ISR *********************/
/* Look for Interrupts and Triggered Action    */
void ADXL_ISR() {
  
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource();
  
  // Free Fall Detection
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
    Serial.println("*** FREE FALL ***");
    //add code here to do when free fall is sensed
  } 
  
  // Inactivity
  if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
    Serial.println("*** INACTIVITY ***");
     //add code here to do when inactivity is sensed
  }
  
  // Activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    Serial.println("*** ACTIVITY ***"); 
     //add code here to do when activity is sensed
  }
  
  // Double Tap Detection
  if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
    Serial.println("*** DOUBLE TAP ***");
     //add code here to do when a 2X tap is sensed
  }
  
  // Tap Detection
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
    Serial.println("*** TAP ***");
     //add code here to do when a tap is sensed
  } 
}

