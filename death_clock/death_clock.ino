#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <math.h>


// THESE TWO CONSTANTS NEED TO BE SET
#define CURRENT_EPOCH_TIME_SECONDS      1510359495    // IMPORTANT: update this number to the current epoch time, each time the board is restarted.
#define FINAL_EPOCH_TIME_SECONDS        2895868800    // this is the expected epoch time of your life expectancy





// Hardware Consts
#define eeprom_addr 0
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);




// Arduino Required Fns

void setup() 
{
  lcd.begin(16, 2);
  setTimeLibNow();
}

void loop()
{
  long current_epoch_time = now();
  long diff = FINAL_EPOCH_TIME_SECONDS - current_epoch_time;

  printToLcd(0, formatLong(years(diff), 5) + " years");
  printToLcd(1, printFormat(days(diff), hours(diff), minutes(diff), seconds(diff)));

  // Twice a day, update EEPROM value & clear LCD
  long timeSincelastPut = current_epoch_time - EepromGet();
  long secondsInTwelveHours = 43200L;
  if ( timeSincelastPut >= secondsInTwelveHours )
  { // EEPROM has limited lifetime write/erase cycles. At rate of write/12hr, it should last ~137 years.
    EepromPut( current_epoch_time );
    clearLcd();
  }
  
  delay(1000);
}




// LCD Helpers

void printToLcd(int row, String s)
{
  lcd.setCursor(0, row);
  lcd.print( s + "  " );
}

void clearLcd()
{   // turn it off & on again
    lcd.noDisplay();
    lcd.display();
}



// EEPROM Helpers

long eepromValueCache = 0;    // EEPROM reads can be slow, so use a local memory cache.

long EepromGet()
{
  if ( eepromValueCache > 0 )   { return eepromValueCache; }
  
  long t;
  EEPROM.get( eeprom_addr, t );
  eepromValueCache = t;
  return t;
}

void EepromPut(long t)
{ // ATmega's EEPROM has a lifetime of 100k write/erase cycles. Try to use this sparingly.
  EEPROM.put( eeprom_addr, t );
  eepromValueCache = t;
}




// Time Helpers

void setTimeLibNow()
{ // TimeLib.setTime(long) required by TimeLib.now()
  long stored_epoch_time = EepromGet();
  long t = CURRENT_EPOCH_TIME_SECONDS;
  if ( stored_epoch_time > t )  { t = stored_epoch_time; }
  setTime( t );
}

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
  return (epoch_seconds / 60 / 60 / 24) % 365;
}

long years (long epoch_seconds)
{
  return epoch_seconds / 60 / 60 / 24 / 365;
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




