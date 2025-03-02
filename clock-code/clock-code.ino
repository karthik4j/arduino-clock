/*
Credits for libraries used: 
  DS1307RTC.h by Michael Margolis,Paul Stoffregen
  LiquidCrystal_I2C.h by Frank de Brabander, Marco Schwartz
  Wire.h by Arduino 
  TimeLib.h by Paul Stoffregen


ARDUINO CLOCK PROJECT DEVELOPED BY KARTHIK J
GitHub: https://github.com/karthik4j/

Wiring for external USB programmer: yellow -reset grey & white TX and RX black  GND
*/
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
\
//variables to store button state
bool btn_1 =0; 
bool btn_2 =0;
bool btn_3 =0;  
//hourly reminder feature to add;

int menu = 0;                   //a variable to choose the current screen
int alarm_hour = 6;             //a variable to store the hour to ring the alarm
int alarm_minute = 25;           //a variable to store the minute to ring the alarm

bool alarm_trigger = false;  //a variable to constantly ring the alarm and make sure it does not turn off till the user dissmes the alarm
int alarm_now = false;       //a variable to enable or disable the alarm

int brightness = 255;        //a variable to set brigtness(default is full (255))
int nightmode = 0;           //a variable to turn on or off night mode
int reminder = 1;   
       
void setup()
{
  Serial.begin(9600);
  lcd.init();                      
  lcd.backlight(); 
  lcd.setCursor(0,0);
  lcd.print("Alarm Clock");
  delay(500);
  lcd.clear();
  pinMode(3,INPUT);
  pinMode(13,OUTPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(9,OUTPUT);

  if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
    } else {
      Serial.println("DS1307 read error! Check SDA,SCL wiring");
      Serial.println();
    }
}

void chirp() 
//chirp is the sound produced when any button is pressed. 
//Nightmode is a flag that checks if the user want the clock to operate without disturbing anyone sleeping next to them 
//also disables hourly reminders (will be covered later in the code)
{
  if(nightmode != 1)
  digitalWrite(13,HIGH);
  delay(50);
  digitalWrite(13,LOW);
}
void turn_on_alarm()
//Menu to set or reset the alarm. 'On' means the alarm will set for the allocated time.
//'off' means alarm won't go off even if time has been set by user.


//code to set or reset the alarm.
{
   if (alarm_now == 1)
 {
  lcd.setCursor(4,0) ;
  lcd.print("ALARM ON  ");
    

 }
 
 else
 {
  lcd.setCursor(4,0) ;
  lcd.print("ALARM: OFF");
 }
//get user input and toggle between on and off
 if(btn_2 == LOW)
 {
   delay(150);
         chirp();

   alarm_now++;
}
  if(alarm_now >=2){alarm_now =0;}
}
void set_alarm()
//Menu to set alarm_time and alarm_hour.
//Alarm time is set to current time so it's easier to adjust the alarm time.
//This helps us to reduce the number of button presses to set the time.
//Gets the time to set alarm from user using buttons on the clock

{
  
while( btn_1!= LOW)  { 
  //need to include this as the button read value is not updated when stuck in a whie loop as the data is read from the main loop so we have to read it again
  btn_1 = digitalRead(5);
  btn_2 = digitalRead(4);
  btn_3 = digitalRead(3);

  lcd.setCursor(4,0);
  lcd.print("SET ALARM: ");

if(alarm_hour < 10)
{
  lcd.setCursor(4,1);
  lcd.print("H:0"+ String(alarm_hour));
}
else
{
  lcd.setCursor(4,1);
  lcd.print("H:"+ String(alarm_hour));  
}
  if(alarm_minute < 10)
  {
  lcd.setCursor(10,1);
  lcd.print("M:0"+String(alarm_minute)); 
  }
  else
  {
  lcd.setCursor(10,1);
  lcd.print("M:"+String(alarm_minute));
  }

  //get user input  and set hour and minute accordingly 
  if(btn_2 == LOW)
  {
    alarm_hour++;
    delay(100);
    chirp();
   if(alarm_hour > 23){alarm_hour = 0;}
  }
  if(btn_3 == LOW)
  {  delay(100);
      chirp();
    alarm_minute++;
    if(alarm_minute > 59){alarm_minute = 00;}
  } }

} 

void set_brightness()
//Menu used to change/set the  brightness of the LCD screen.
//Gets the value from user using one of the buttons.
//User cannot change the brightness if the clock is in nightmode, i.e. DND Mode
//Default brightness in nightmode is 20 (not percent)
//Highest is 255.
//User can set brightness 0 in day-mode (when nightmode is off)
//This is not a bug, it is a feature.
{
  lcd.setCursor(0,0);
  lcd.print("Brightness:  "+String(brightness)+"   ");

  if(nightmode ==  1)
  {
    lcd.setCursor(0,1);
    lcd.print("Night mode: ON ");
    brightness=20;
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("Night mode: OFF");
  }

  if(btn_2 == LOW)
  {
    brightness = brightness+10;
    delay(100);
    chirp();
    if(brightness >= 255)
    {
      brightness = 0;
    }
  }

  if(btn_3 == LOW)
  {
    nightmode++;
    delay(100);
        chirp();
    if(nightmode >= 2){nightmode = 0;}
  }

}

void alarm_ring(){
    alarm_trigger = true;
    lcd.clear();

    lcd.setCursor(4,0);
    lcd.print("TIME'S UP");
\    

  while (alarm_trigger == true)
  {
    
\
    analogWrite(9,255);

    digitalWrite(13,1);
    delay(50);
    digitalWrite(13,0);
    delay(50);
    
    digitalWrite(13,1);
    delay(50);
    digitalWrite(13,0);
    delay(50);
 
    
    digitalWrite(13,1);
    delay(50);
    digitalWrite(13,0);
    delay(50);

    digitalWrite(13,1);
    delay(50);
    digitalWrite(13,0);
    delay(50);
 
    analogWrite(9,0);
    delay(600);



  if( digitalRead(4)==LOW)
   {
    digitalWrite(13,LOW);
    brightness = 255;
    alarm_trigger = false;
    alarm_now = false;
     lcd.clear();
   }
  } 
}

void hourly_reminder()
{
// hourly reminder is a feature that alerts the user about the passage of time.
// it beeps every 30 minutes 
// it does not beep if "nightmode" is turned on
// it only beeps from 7 am to 11 pm 

  if(btn_2 == 0)
  {
    reminder = reminder+1;
    delay(150);
        chirp();
    if (reminder > 2){reminder = 0;}
  }
  if (reminder == 1)
  {
    lcd.setCursor(3,0);
    lcd.print("Bi-Hourly");
    lcd.setCursor(0,1);
    lcd.print("Reminder is ON ");

  }
  else
  {
    lcd.setCursor(3,0);
    lcd.print("Bi-Hourly");
    lcd.setCursor(0,1);
    lcd.print("Reminder is OFF");
  }
  }

 
 void loop()
{

 //RTC module time structure
 tmElements_t tm;
 analogWrite(9,brightness);



  btn_1 = digitalRead(5);
  btn_2 = digitalRead(4);
  btn_3 = digitalRead(3);

 // to toggle between the menus 
 if(btn_1 == 0)
 {
  menu = menu+1;
  delay(150);
      chirp();
  if(menu >4)
  {menu = 0;}
  lcd.clear();
 }
//menu to set alarm time. Calls the function set_alarm() to do this.
//also sets the alarm_hour and alarm_minute to current hour and minute so it's easier to set the time while setting the alarm.
if(menu == 1) 
  {
    if(alarm_now == 0){alarm_hour=tm.Hour;alarm_minute=tm.Minute;}
    set_alarm();
  }
if(menu == 2){turn_on_alarm();}  //after setting the alarm time, user can decide whether or not to turn on or off the alarm. 
if(menu == 3){set_brightness();} //set brigtness and decide if you want to turn on "nightmode"
if(menu == 4){hourly_reminder();} //turn on or off 'bi-hourly reminder according to the user's choice.


 if(RTC.read(tm)) //checks if it can read the time. If the RTC module looses power at any point or if the on-board battery is depleted it will set the time to 255:255:255 to let the board know that time needs to be set again.
 {

  //if nightmode is on then the LCD backlight is turned off so that it does not disturb the user's sleep.
  //This only works if both nightmode is on and if it is night time (i.e. 22 hrs + or 10PM)
  if(tm.Hour > 22 and nightmode == 1) 
  {
    brightness = 0;
  }
  
//checks if the board was able to establish an connection with the RTC module and to check if the time is set properly.
// If the battery is dead or if there are any issues with the wiring this will be false and will alert the user to set the program using the SetTime program.
  if (RTC.chipPresent()==false) 
  {
        lcd.setCursor(6,1);
        lcd.print('3');
        Serial.print("ERROR");

  }
  if(alarm_now  == 1) //alarm_now is a flag used to decide whether or not to ring the alarm for a set alarm time. 
  {
  
    if(tm.Hour == alarm_hour and tm.Minute == alarm_minute and tm.Second == 0 ) //checks if it's time to ring
   {alarm_ring();} //rings the alarm once this is executed. It will not stop to ring unless the user wakes up or if the button is pressed. Can run indefinitely.
  }
  
  if(reminder == true && nightmode != 1)
   // Checks if the user has enabled bi-hourly reminders and if nightmode is off. If both conditions are satisfied then the clock reminds the user every half an hour till 22:00:00 or 10PM
   // Automatically starts alerting user once its morning again. 
  {
    if(tm.Hour<=22 and tm.Hour >7 )
    {
    if(tm.Minute == 30 and tm.Second == 0 or tm.Minute == 00 and tm.Second == 0)
    {
      digitalWrite(13,1);
      delay(100);
      digitalWrite(13,0);
      
    }
    
  }
 }
 }
// MENU = 0 means to display just the time. No functions are called in this part. Only the background condtions are checked such as hourly reminder, alarm and nightmode conditons.
if(menu ==0 )
{

      lcd.setCursor(0,0);
      lcd.print("Time:");
      lcd.setCursor(0,1);
      lcd.print("Date:");
    
    if(tm.Second%2 == 0)
    {
      lcd.setCursor(8,1);
      lcd.print(":");
    
      lcd.setCursor(11,1);
      lcd.print(":");
    
      lcd.setCursor(8,0);
      lcd.print(":");
    
      lcd.setCursor(13,0);
      lcd.print(":");

     
    }
    else
    {
      
      lcd.setCursor(8,1);
      lcd.print(" ");
    
      lcd.setCursor(11,1);
      lcd.print(" ");
    
      lcd.setCursor(8,0);
      lcd.print(" ");
    
      lcd.setCursor(13,0);
      lcd.print(" "); 
    }

     //----------------------------------------------------------------------------------- 
      if(tm.Second < 10)
      {
        lcd.setCursor(14,0);
        lcd.print("0"+String(tm.Second));
      }
      else
      {
       lcd.setCursor(14,0);
       lcd.print(tm.Second); 
      }
      if(tm.Minute < 10)
      {
       lcd.setCursor(10,0);
       lcd.print("0"+String(tm.Minute));
    
        
      }
      else
      {
        lcd.setCursor(10,0);
        lcd.print(tm.Minute);
      }
      
      if(tm.Hour < 10)
      {
        lcd.setCursor(6,0);
        lcd.print("0"+String(tm.Hour));
      }
      else
      {
        lcd.setCursor(6,0);
        lcd.print(tm.Hour);
      }
    //------------------------------------------------------------------
    if(tm.Day < 10)
      {
        lcd.setCursor(6,1);
        lcd.print("0"+String(tm.Day));
      }
      else
      {
       lcd.setCursor(6,1);
       lcd.print(tm.Day); 
      }
      if(tm.Month < 10)
      {
       lcd.setCursor(9,1);
       lcd.print("0"+String(tm.Month));
        
      }
      else
      {
        lcd.setCursor(9,1);
        lcd.print(tm.Month);
      }
      
    
      lcd.setCursor(12,1);
      lcd.print(tmYearToCalendar(tm.Year));
}
}
