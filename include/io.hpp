#ifndef IO_HPP
#define IO_HPP

// Fetch your key from Adafruit
#define IO_USERNAME "name"
#define IO_KEY "key"

/******************************* WIFI **************************************/

#define WIFI_SSID "wifi"
#define WIFI_PASS "pass"

#include <AdafruitIO_WiFi.h>

namespace IO {

bool IO_connected{false};

// IO handler
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Feeds uploaded from this microcontroller (motherfeed)
AdafruitIO_Feed *motherfeed = io.feed("Motherboard coords");
AdafruitIO_Feed *connections = io.feed("Microcontroller connections");
AdafruitIO_Feed *location = io.feed("Average location");
AdafruitIO_Feed *temperature = io.feed("Average temperature");
AdafruitIO_Feed *humidity = io.feed("Average humidity");
AdafruitIO_Feed *pressure = io.feed("Average pressure");
AdafruitIO_Feed *temprange = io.feed("Temperature range");
AdafruitIO_Feed *humrange = io.feed("Humidity range");

// Feeds to be taken from
AdafruitIO_Feed *weather1 = io.feed("weather1");

// attempt connection
void try_connect() {
  Serial.print("Connecting to Adafruit IO");

  io.connect();

  // waits for a connection for 10 seconds
  for (std::size_t i{0}; (i < 120) || (io.status() < AIO_CONNECTED); i++) {
    Serial.print(".");
    delay(500);
  }

  if (io.status() < AIO_CONNECTED) {
    Serial.println("Connection failed");
  } else {
    Serial.println("Connected to Adafruit IO\n");
    Serial.println(io.statusText());
    IO_connected = true;
  }
}

// run if connected
void run() { io.run(); }

}  // namespace IO

#endif  // IO_HPP
