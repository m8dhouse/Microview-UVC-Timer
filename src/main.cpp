#include <MicroView.h>		// include MicroView library

int relayPin = 2;				  // set relayPin as pin 2 of Arduino
int startPinSwitch = 5;   //push button switch 
int coverReedSwitch = 3;  // reed switch in cover box
int buzzerPin = 6;        // buzzer pin
int timeLeftOver = 0;     // timeLeftOver when interrrupted
int timerstart = 15;      // UVC exposure in seconds
                          // UVC exposure in minutes => change line 68 for loop
MicroViewWidget *widget;

void endUVC()
{
  digitalWrite(relayPin, LOW);      // turn the RELAY off by making the voltage LOW
  uView.clear(PAGE);			          // clear page
  uView.setCursor(0,25);		
	uView.print("Finished   ");
	uView.display();
  digitalWrite(buzzerPin, LOW);    // turn the buzzer on 
  delay(1000);
  digitalWrite(buzzerPin, HIGH);   // turn the buzzer off
  delay(250);
  digitalWrite(buzzerPin, LOW);    // turn the buzzer on 
  delay(500);
  digitalWrite(buzzerPin, HIGH);   // turn the buzzer off  
  delay(250);
  digitalWrite(buzzerPin, LOW);    // turn the buzzer on 
  delay(250);
  digitalWrite(buzzerPin, HIGH);   // turn the buzzer off
  delay(1000);
}


int boxOpened()
{

  while (digitalRead(coverReedSwitch) == HIGH)
  {
    digitalWrite(relayPin, LOW);    // turn the RELAY off
    uView.setCursor(0,0);		
    uView.print("Close     ");
    uView.setCursor(0,10);		
    uView.print("cover     ");
    uView.display();
  }

  uView.setCursor(0,0);			    
  uView.print("Radiating   ");			
  uView.setCursor(0,10);
  uView.print("Don't open   ");
  uView.display();				      
  digitalWrite(relayPin, HIGH); // turn the RELAY ON 
  return timeLeftOver;
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
  widget= new MicroViewGauge(40,30,0,timerstart*2);// draw Gauge widget at x=32,y=30,min=0, max=100
  uView.setCursor(0,0);		
	uView.print("Radiating ");			
  uView.setCursor(0,10);
  uView.print("Don't open");
	uView.display();				
  delay(2000);    				// wait 

  while (digitalRead(coverReedSwitch) == LOW)
  {
    digitalWrite(relayPin, HIGH);   // turn the RELAY ON 
    for(int i=timerstart*2; i>0;i--) {

    // minutes for(int i=timerstart*120; i>0;i--) {

      if (digitalRead(coverReedSwitch) == HIGH) boxOpened();
      timeLeftOver = i;       // save time left over in var
      widget->setValue(i);    // give a value to widget
      uView.display();        // display current page buffer
      delay(500);    				  // wait for half a second (hence the 120 -> 2*60 for a minute) 
    }
   break;                     // detected reed switch opened
  };
 
  endUVC();                   // time done and reed switch closed
}


void setup() {  

	uView.begin();				            // start MicroView

	pinMode(relayPin, OUTPUT);	      // initialize the digital pin as an output.
  pinMode(buzzerPin, OUTPUT);	      // initialize the digital pin as an output.
  digitalWrite(buzzerPin, HIGH);    // turn the buzzer off 

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
  






