#include <VirtualWire.h>
void setup()
{
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_rx_pin(12);
    vw_setup(4000);  // Bits per sec
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    vw_rx_start();       // Start the receiver PLL running
}
    void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      if(buf[0]=='l'){
      Serial.print("l");
   digitalWrite(13,1);
      }  
   if(buf[0]=='r'){
     Serial.print("r");
  digitalWrite(13,0);
    }
     if(buf[0]=='f'){
      Serial.print("f");
   digitalWrite(13,1);
      }  
   if(buf[0]=='b'){
     Serial.print("b");
  digitalWrite(13,0);
   }
   if(buf[0]=='s'){
     Serial.print("s");
  digitalWrite(13,0);
   }
    

}
}
