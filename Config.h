/*
 * Config file for the ESP8266 Tweet Search
 */ 

#ifndef CONFIG_H
#define CONFIG_H

static const char* WIFI_NETWORK = "";
static const char* WIFI_PASSWORD = "";

// Twitter GET request timeout
#define TWI_TIMEOUT 3000  // in msec

// NTP Server to use to set time
static const char* NTP_SERVER = "time.nist.gov";
// Timezone to use (number of hours behind UTC)
static const int TIMEZONE_DIFFERENCE = -8; // PST time

// Twitter API things
static const char CONSUMER_KEY[] = "";
static const char CONSUMER_SECRET[] = "";
static const char ACCESS_TOKEN[] = "";
static const char ACCESS_TOKEN_SECRET[] = "";

// Twitter search terms
static const std::string SEARCH_TERMS[] =
{
    "#starwars",
    "#cybersecurity",
    "#malware"
};

// Number of tweets to display from each search term
#define TWEET_COUNT 5

// Constants for the SSD1306 OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // OLED reset pin

#endif