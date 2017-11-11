#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <math.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// IMPORTANT: update this number to the current epoch time, each time the board is restarted.
#define CURRENT_EPOCH_TIME_SECONDS  1510358835
#define FINAL_EPOCH_TIME_SECONDS    2895868800


void setup() 
{
  setTime(CURRENT_EPOCH_TIME_SECONDS);
  lcd.begin(16, 2);

  lcd.noDisplay();
  lcd.display();

//  Serial.begin(9600);   // for debugging
}


void loop() 
{

  long current_epoch_time = now();
  long diff = FINAL_EPOCH_TIME_SECONDS - current_epoch_time;

  printToLcd(0, formatLong(years(diff), 5) + " years");
  printToLcd(1, printFormat(days(diff), hours(diff), minutes(diff), seconds(diff)));
  
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

