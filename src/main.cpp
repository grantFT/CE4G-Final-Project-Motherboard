#include <Arduino.h>
#include <config.hpp>

// contains all statistics (in stats form) from all microcontrollers
std::vector<stats> allData = {}; 
// gets the statistics measured from this mother microcontroller
stats getAll();
// calculates averages as well as minimum/maximum values
void calc();

// idk
void handleMessage(AdafruitIO_Data *data);

void setup() {
  // start the serial connection
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // initialise peripherals
  GPS::initialise();
  screen::initialise();
  BMESetup();

  // connect to io.adafruit.com
  IO::try_connect();

  allData.push_back(getAll());

  IO::weather1->onMessage(handleMessage);
}

void loop() {
  IO::run();
  // delays 15 seconds
  delay(15000);

  // collects data from GPS and BME
  auto my_coords{GPS::query_GPS()};
  BMELoop();

  if (IO::IO_connected) {
    IO::motherfeed->save(1, my_coords.latitude, my_coords.longitude,
                      my_coords.altitude); // UPLOADS TO IO /maybe
                      screen::clear();
                      screen::screen.setTextColor(ST77XX_CYAN);
    screen::screen.print("IO connected");          
  } else {
    screen::clear();
    screen::screen.setTextColor(ST77XX_WHITE);
    screen::screen.print("IO not connected");
  }

  // updates data from this microcontroller
  allData[0]=getAll();
  calc(allData);


  // waits 45 seconds, totaling one minute per loop
  delay(45000);
}

// Takes info from feed
void handleMessage(AdafruitIO_Data *data) {
  String received_string = data->toString();
  Serial.println(received_string);
  allData[1]=(seperate(received_string));
  Serial.println(allData[1].temp);
  Serial.println(allData[1].hum);
  screen::screen.print("Information received");
  Serial.println("handleMessage end");
  delay(5000);
}

stats getAll() {
  return {GPS::GPS.latitude, GPS::GPS.longitude, GPS::GPS.altitude, ((bme.temperature * 1.8)+32), bme.humidity, static_cast<double>(bme.pressure)/100};
}