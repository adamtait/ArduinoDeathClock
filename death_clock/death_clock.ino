#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <math.h>


// THESE TWO CONSTANTS NEED TO BE SET
#define CURRENT_EPOCH_TIME_SECONDS      1510359495    // IMPORTANT: update this number to the current epoch time, each time the board is restarted.
#define FINAL_EPOCH_TIME_SECONDS        2895868800    // this is the expected epoch time of your life expectancy








#define eeprom_addr 0
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);




void setup() 
{
  lcd.begin(16, 2);
  
  // SET TIME
  long stored_epoch_time = 0;
  EEPROM.get( eeprom_addr, stored_epoch_time );

  long t = CURRENT_EPOCH_TIME_SECONDS;
  if ( stored_epoch_time > t )  { t = stored_epoch_time; }
  setTime( t );   // used by now()
}


void loop() 
{

  long current_epoch_time = now();
  long diff = FINAL_EPOCH_TIME_SECONDS - current_epoch_time;

  printToLcd(0, formatLong(years(diff), 5) + " years");
  printToLcd(1, printFormat(days(diff), hours(diff), minutes(diff), seconds(diff)));

  // store current time in EEPROM
  if ( current_epoch_time % (60 * 10) == 0 )  {   // only store time every 10 minutes. EEPROM has a limit to lifetime writes. What does 10 minutes matter in the whole scheme of your life, anyways?
    EEPROM.put( eeprom_addr, current_epoch_time ); 
  }       
  
  delay(1000);
}



// Time Helpers

long seconds (long epoch_seconds)
{
  return epoch_seconds % 60;
}

long minutes (long epoch_seconds)
{
  return (epoch_seconds / 60) % 60;
}

long hours (long epoch_seconds)
{
  return (epoch_seconds / 60 / 60) % 24;
}

long days (long epoch_seconds)
{
  return (epoch_seconds/ 60 / 60 / 24) % 365;
}

long years (long epoch_seconds)
{
  return epoch_seconds/ 60 / 60 / 24 / 365;
}



// Print Format Helpers

String printFormat(long days, long hours, long minutes, long seconds)
{
  return formatLong(days, 3) + "d "
          + formatLong(hours, 2) + ":"
          + formatLong(minutes, 2) + ":"
          + formatLong(seconds, 2);
}

String formatLong(long n, int digitLen)     { return reduceFormatLong("", n, digitLen); }

String reduceFormatLong(String acc, long n, int digitLen)
{
  if (digitLen > 1)
  {
    long upperBound = pow(10, (digitLen - 1));
    if (n < upperBound) {
      return reduceFormatLong("0" + acc, n, (digitLen - 1));
    }
  }
  return acc + String(n);
}



// LCD helpers


void printToLcd(int row, String s)
{
  lcd.setCursor(0, row);
  lcd.print( s + "  " );
}

