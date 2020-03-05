// IMPORT ALL NEEDED LIBRARIES  

// include priximity sensor library
#include <NewPing.h>

// include clock libraries
#include <Wire.h>
#include <ds3231.h>
struct ts t;

// import OLED libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//import SD module libraries
#include <SPI.h>
#include <SD.h>


// DEFINE PINS

// define pins for the 5 switch buttons
#define switch_1 31
#define switch_2 30
#define switch_3 29
#define switch_4 28
#define switch_5 27

// define pins for the 8 rotatory encoder
#define R1_CLK 47
#define R1_DT 46

#define R2_CLK 45
#define R2_DT 44

#define R3_CLK 43
#define R3_DT 42

#define R4_CLK 41
#define R4_DT 40

#define R5_CLK 39
#define R5_DT 38

#define R6_CLK 37
#define R6_DT 36

#define R7_CLK 35
#define R7_DT 34

#define R8_CLK 33
#define R8_DT 32
