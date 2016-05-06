/*
  Sending a tweet via a Twitter account using the Temboo Arduino Yun SDK. I2C!!!
*/

// include Grove LED Bar
#include <Grove_LED_Bar.h>
Grove_LED_Bar bar(9, 8, 0);

// barometric data get
#include "Barometer.h"
#include <Wire.h>
float pressure;
float temperature;
Barometer myBarometer;

#include <Bridge.h>
#include <Temboo.h>
#include "twitterApi.h" // contains twitter account info
#include "TembooAccount.h" // contains Temboo account info

int numRuns = 1; //execution count, so this doesn't run forever
int maxRuns = 1; //max number of times the Twitter Update Choreo should run

String runningstatus = "default";
String tweeting = "default";
String pa = "default";


void setup()
{
  Serial.begin(9600);
  myBarometer.init();
  
  // For debugging
  delay(4000);
  while (!Serial);
  Bridge.begin();

  // led bar begin
  bar.begin();
}

int previousState = 0;

void loop()
{
  temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP()); //Get the barometric pressure

  if (numRuns <= maxRuns)
  {
    int currentState = 0;
    if (pressure > 102000.00)
    {
      currentState = 1;
      tweeting = t1;
      runningstatus = "s1";
    }
    else if (pressure <= 102000.00 && pressure > 101500.00)
    {
      currentState = 2;
      tweeting = t2;
      runningstatus = "s2";
    }
    else if (pressure <= 101500.00 && pressure > 101000.00)
    {
      currentState = 3;
      tweeting = t3;
      runningstatus = "s3";
    }
    else if (pressure <= 101000.00 && pressure > 100500.00)
    {
      currentState = 4;
      tweeting = t4;
      runningstatus = "s4";
    }
    else if (pressure <= 100500.00 && pressure > 100100.00)
    {
      currentState = 5;
      tweeting = t5;
      runningstatus = "s5";
    }
    else
    {
      currentState = 6;
      tweeting = t6;
      runningstatus = "s6";
    }

    String tweetText(tweeting + " (" + String(pressure, 0) + " pa)");
    Serial.println(runningstatus);
    Serial.print(pressure);
    Serial.println(" Pa");



    if (currentState != previousState) {
      TembooChoreo StatusesUpdateChoreo;

      // invoke the Temboo Client
      // *NOTE that the client must be reinvoked, and repopulated with appropriate arguments, each time its run() method is called.
      StatusesUpdateChoreo.begin();

      // Set Temboo account credentials
      StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
      StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
      StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);

      // Identify the Choreo to run (Twitter/Tweets/StatusesUpdate)
      StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");

      // add the Twitter account information
      StatusesUpdateChoreo.addInput("ConsumerKey",TWITTER_API_KEY);
      StatusesUpdateChoreo.addInput("ConsumerSecret",TWITTER_API_SECRET);
      StatusesUpdateChoreo.addInput("AccessToken",TWITTER_ACCESS_TOKEN);
      StatusesUpdateChoreo.addInput("AccessTokenSecret",TWITTER_ACCESS_TOKEN_SECRET);
      StatusesUpdateChoreo.addInput("StatusUpdate",tweetText);

      // tell the Process to run and wait for the result.
      // The return code (returnCode) will tell us whether the Temboo client
      // was able to send our request to Temboo servers
      unsigned int returnCode = StatusesUpdateChoreo.run();

      // a return code of zero (0) means everything worked
      if (returnCode == 0)
      {
        Serial.println("Sent!");
      }
      else
      {
        // a non-zero return code means there was an error
        // read and print the error message
        while (StatusesUpdateChoreo.available())
        {
          char c = StatusesUpdateChoreo.read();
          Serial.print(c);
        }

        StatusesUpdateChoreo.close();
      }
      previousState = currentState;
  }
  else 
  {
    Serial.println("ops, still the same:3");
  }

  // do nothing for the next 10 mins - 600000
  Serial.println("chilling for 10 mins..");

  // led- Walk through the levels
  int i=0;
  bar.setLevel(i);
  delay(60000); 
  Serial.println("10%");
  bar.setLevel(i+1);
  delay(60000);
  Serial.println("20%");
  bar.setLevel(i+2);
  delay(60000);
  Serial.println("30%");
  bar.setLevel(i+3);
  delay(60000);
  Serial.println("40%");
  bar.setLevel(i+4);
  delay(60000);
  Serial.println("50%");
  bar.setLevel(i+5);
  delay(60000);
  Serial.println("60%");
  bar.setLevel(i+6);
  delay(60000);
  Serial.println("70%");
  bar.setLevel(i+7);
  delay(60000);
  Serial.println("80%");
  bar.setLevel(i+8);
  delay(60000);
  Serial.println("90%");
  bar.setLevel(i+9);
  delay(60000);
  Serial.println("100%");

  Serial.println();
  
//  
//  // do nothing for the next 90 seconds
//  Serial.println("chilling for 10 mins..");
//  Serial.println();
//  delay(600000);
}
}

