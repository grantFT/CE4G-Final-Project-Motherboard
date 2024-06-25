#include <Arduino.h>

#include <coord.hpp>
#include <gps.hpp>
#include <io.hpp>
#include <screen.hpp>

void handleMessage(AdafruitIO_Data *data);

void setup() {
  // start the serial connection
  Serial.begin(115200);
  while (!Serial);

  // initialise peripherals
  GPS::initialise();
  screen::initialise();

  // connect to io.adafruit.com
  IO::try_connect();

  IO::mustard->onMessage(handleMessage);
}

void loop() {
  IO::run();
  delay(5000);

  auto my_coords{GPS::query_GPS()};

  if (IO::IO_connected) {
    IO::ketchup->save(1, my_coords.latitude, my_coords.longitude,
                      my_coords.altitude);
  } else {
    screen::display(IO::IO_connected, my_coords.latitude, my_coords.lat,
                    my_coords.longitude, my_coords.lon, my_coords.altitude);
  }
}

void handleMessage(AdafruitIO_Data *data) {
  int received_value = data->toInt();

  // Fetch info from IO
  double received_lat = data->lat();
  double received_lon = data->lon();
  double received_ele = data->ele();

  screen::display(IO::IO_connected, received_lat, received_lat < 0 ? 'S' : 'N',
                  received_lon, received_lon < 0 ? 'E' : 'W', received_ele);
}
