#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

namespace screen {

Adafruit_ST7789 screen = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void initalise();
void clear();

/**
 * @brief initialise screen
 */
void initialise() {
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  screen.init(135, 240);
  screen.setRotation(3);
  clear();
  screen.setTextSize(3);
}

/**
 * @brief clear screen
 */
void clear() {
  screen.fillScreen(ST77XX_BLACK);
  screen.setCursor(0, 10);
}

/**
 * @brief display info
 *
 * @param connected
 * @param latitude
 * @param lat
 * @param longitude
 * @param lon
 * @param altitude
 */
void display(bool connected, double latitude, char lat, double longitude,
             char lon, double altitude) {
  clear();
  screen.setTextColor(ST77XX_ORANGE);
  if (connected)
    screen.println("Connected");
  else
    screen.println("Not connected");

  screen.setTextColor(ST77XX_CYAN);
  screen.printf("-------------");

  screen.setTextColor(ST77XX_WHITE);
  screen.printf("Lat: %.2f %c\n", latitude, lat);
  screen.printf("Lon: %.2f %c\n", longitude, lon);
  screen.printf("Ele: %.2f m\n", altitude);
}

}  // namespace screen

#endif  // SCREEN_HPP
