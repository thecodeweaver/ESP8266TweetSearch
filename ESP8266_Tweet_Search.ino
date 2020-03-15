#include <ESP8266WiFi.h>
#include <TwitterWebAPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Config.h"

// Twitter clients
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, TIMEZONE_DIFFERENCE * 3600, 60000);
TwitterClient twitter(timeClient, CONSUMER_KEY, CONSUMER_SECRET, ACCESS_TOKEN, ACCESS_TOKEN_SECRET);

// SSD1306 display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() 
{
    Serial.begin(115200);
    Serial.println();

    // Setup SSD1306 display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("[SSD1306 OLED Display] Allocation failed! Rebooting..."));
        ESP.restart();
    }

    display.clearDisplay();

    // Setup WiFi
    if (!WiFi.isConnected())
    {
        WiFi.disconnect();
        WiFi.setAutoConnect(true);
        WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
    
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            GFX_text_setup(0, 0, WHITE, 2);
            display.println(F("Couldn't connect to WiFi! Rebooting..."));
            delay(3000);
            ESP.restart();
        }
    }

    // Setup NTP
    twitter.startNTP();
}

void loop() 
{
    // Loop over our search terms and display a number of Tweets from each one
    for (int i = 0; i < (sizeof(SEARCH_TERMS) / sizeof(SEARCH_TERMS[0])); i++)
    {
        // Query Twitter API
        String response = twitter.searchTwitter((const char*) SEARCH_TERMS[i].c_str());

        // Grab tweets from the response
        for (int j = 0; j < TWEET_COUNT; j++)
        {
            String tweet = get_tweet(response, j);
            
            if (tweet != "")
            {
                display.clearDisplay();
                GFX_text_setup(0, 0, WHITE, 2);
                display.println(tweet);
                display.display();
                delay(10000);
            }
        }
    }

    // Search for Tweets every 5 minutes
    for (int i = 0; i < 5; i++)
        delay(60000);
}

// Parse the response from the Twitter API
// Grabs a tweet from a specific index within the 'statuses' JSON array
// Inspired by: https://github.com/debsahu/TwitterWebAPI/blob/master/examples/esp8266/TwitterSearch/TwitterSearch.ino
String get_tweet(String response, int index)
{
    DynamicJsonBuffer json_buffer;
    JsonObject& json = json_buffer.parseObject(response.c_str());

    if (!json.success())
    {
        String s = "";
        return s;
    }

    if (json.containsKey("statuses"))
    {
        String user = json["statuses"][index]["user"]["screen_name"];
        String tweet = json["statuses"][index]["text"];

        if (user.length() == 0 || tweet.length() == 0)
        {
            String s = "";
            return s;
        }

        String text = "@" + user + ": " + tweet;

        json_buffer.clear();

        return text;
    }
    else if (json.containsKey("errors"))
    {
        String error = json["errors"][0]["message"];
        
        json_buffer.clear();

        return error;
    }

    String s = "";
    return s;
}

// Make displaying text easier (wrapper for Adafruit GFX functions)
void GFX_text_setup(uint16_t x, uint16_t y, uint16_t color, uint8_t size)
{
    display.setCursor(x, y);
    display.setTextColor(color);
    display.setTextSize(size);
}