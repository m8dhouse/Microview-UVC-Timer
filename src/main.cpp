/*
Microview UVC timer
GitHub: M8dhouse
11/7/2020
*/
#include <MicroView.h>		// include MicroView library

int relayPin = 2;				  // set relayPin as pin 2 of Arduino
int startPinSwitch = 5;   // push button switch 
int coverReedSwitch = 3;  // reed switch in cover box
int buzzerPin = 6;        // buzzer pin
int timeLeftOver = 0;     // timeLeftOver when interrrupted
int timerValue = 15;      // UVC exposure in seconds
                          // UVC exposure in minutes => change line 79 for loop
String versionUVC = "1.0";
MicroViewWidget *widget;

void endUVC()
{
  digitalWrite(relayPin, LOW);      // turn the RELAY off by making the voltage LOW
  uView.clear(PAGE);			          // clear page
  uView.setCursor(0,25);		
	uView.print("Finished   ");
	uView.display();
  digitalWrite(buzzerPin, HIGH);    // turn the buzzer on 
  delay(1000);
  digitalWrite(buzzerPin, LOW);   // turn the buzzer off
  delay(250);
  digitalWrite(buzzerPin, HIGH);    // turn the buzzer on 
  delay(500);
  digitalWrite(buzzerPin, LOW);   // turn the buzzer off  
  delay(250);
  digitalWrite(buzzerPin, HIGH);    // turn the buzzer on 
  delay(250);
  digitalWrite(buzzerPin, LOW);   // turn the buzzer off
  delay(1000);
}


void boxOpened()
{

  while (digitalRead(coverReedSwitch) == HIGH)
  {
    digitalWrite(relayPin, LOW);    // turn the RELAY off
    uView.setCursor(0,0);		
    uView.print("Close lid ");
    uView.display();
  }

  uView.setCursor(0,0);			    
  uView.print("Radiating   ");		
  uView.display();		
  digitalWrite(buzzerPin, HIGH);    // turn the buzzer on 
  delay(250);
  digitalWrite(buzzerPin, LOW);   // turn the buzzer off	
  delay(2000);    				// wait       
  digitalWrite(relayPin, HIGH); // turn the RELAY ON 

}

void startUVC ()
{
  uView.clear(PAGE);			  // clear page
  uView.setCursor(0,0);			
	uView.print("DANGER    ");		
  uView.setCursor(0,10);
  uView.print("UV-C !!   ");
	uView.display();
  delay(2000);    				// wait 
  uView.clear(PAGE);			  // clear page	
  widget= new MicroViewGauge(40,30,0,timerValue);// draw Gauge widget at x=32,y=30,min=0, max=100
  uView.setCursor(0,0);		
	uView.print("Radiating ");			
	uView.display();			
  digitalWrite(buzzerPin, HIGH);    // turn the buzzer on 
  delay(250);
  digitalWrite(buzzerPin, LOW);   // turn the buzzer off	
  delay(2000);    				// wait 

  while (digitalRead(coverReedSwitch) == LOW)  // while cover closed
  {
    digitalWrite(relayPin, HIGH);   // turn the RELAY ON 
    // testing seconds for(int i=timerValue*2; i>0;i--) {

   for(int i=timerValue*120; i>0;i--) {

      if (digitalRead(coverReedSwitch) == HIGH) boxOpened();
      timeLeftOver = i;       // save time left over in var
      if ( (i % 120) == 0) { 
        widget->setValue(i/120);    // give a value to widget
        uView.display();        // display current page buffer 
      }
      
      delay(500);    				  // wait for half a second (hence the 120 -> 2*60 for a minute) 
    }
   break;                     // time done - reed switch still closed but time is done
  };
 
  endUVC();                   // end procedure
}


void setup() {  

	uView.begin();				            // start MicroView

	pinMode(relayPin, OUTPUT);	      // initialize the digital pin as an output.
  pinMode(buzzerPin, OUTPUT);	      // initialize the digital pin as an output.
  digitalWrite(buzzerPin, LOW);    // turn the buzzer off 

	pinMode(startPinSwitch, INPUT_PULLUP);	// initialize the digital pin
	pinMode(coverReedSwitch, INPUT_PULLUP);	// initialize the digital pin 
  uView.clear(PAGE);
}

void loop() {

  while (digitalRead(startPinSwitch) == HIGH)  // waiting for start button - loop
  {
    digitalWrite(relayPin, LOW);              // turn the RELAY off 			
    uView.setCursor(0,0);		
    uView.print("Waiting   ");
    uView.setCursor(40,40);	
    uView.print("v" + versionUVC);
    uView.display();

  }
  
  // button closed so we start
  if (digitalRead(coverReedSwitch) == LOW) // cover closed?
  {
    startUVC();                            // start UVC procedure
  }
  else
  {
    boxOpened();                          // cover open - wait
  }
    
  }
  






