#include <Arduino.h>

#include <stats.hpp>
#include <vector>
#include <limits>

size_t commas;
String fLat;
String fLon;
String fEle;
String fTemp;
String fHum;
String fPres;
double minTem;
double maxTem;
double minHum;
double maxHum;
double tempRange;
double humRange;
size_t failCount;

stats av{};
stats seperate(String receivedString);

// seperates string from Adafruit IO into each variable
stats seperate(String receivedString) {
    commas = 0;
    fLat = "";
    fLon = "";
    fEle = "";
    fTemp = "";
    fHum = "";
    fPres = "";
    
for(char x : receivedString) {
    if (x==',') {
      commas++;
    } else {
    if(commas>5) {
        break;
    } else if(commas>4) {
        fPres+=x;
    } else if(commas>3) {
      fHum+=x;
    } else if(commas>2) {
      fTemp+=x;
    } else if(commas>1) {
      fEle+=x;
    } else if(commas>0) {
      fLon+=x;
    } else if(commas==0) {
      fLat+=x;
    } 
    }
  }
  return stats(fLat.toDouble(), fLon.toDouble(), fEle.toDouble(), fTemp.toDouble(), fHum.toDouble(), fPres.toDouble());
}

/*
* @brief calculates averages and uploads them.
*/
void calc(std::vector<stats> allData) {
  av.lat=0;
  av.lon=0;
  av.ele=0;
  av.temp=0;
  av.hum=0;
  av.pres=0;
  failCount=0;
  minTem = std::numeric_limits<double>::max();
  maxTem = std::numeric_limits<double>::min();
  minHum = std::numeric_limits<double>::max();
  maxHum = std::numeric_limits<double>::min();
  for(stats v : allData) {
    Serial.println("v temperature:");
    Serial.println(v.temp);
      // calc average
      if(v.lat) {av.lat+=v.lat;} else {failCount++;} // if GPS is unsuccessful, removes data points from average
      if(v.lon) {av.lon+=v.lon;}
      if(v.ele) {av.lon+=v.lon;}
      av.temp+=v.temp/allData.size();
      Serial.println("av.temp =");
      Serial.println(av.temp);
      av.hum+=v.hum/allData.size();
      av.pres+=v.pres/allData.size();
      // min/max/range
      if(av.temp<minTem) {
        minTem=v.temp;
      }
      if(av.temp>maxTem) {
        maxTem=v.temp;
      }
      if(av.hum<minHum) {
        minHum=v.hum;
      }
      if(av.hum>maxHum) {
        maxHum=v.hum;
      }
  }
  av.lat=(av.lat/(allData.size()-failCount));
  av.lon=(av.lon/(allData.size()-failCount));
  av.ele=(av.ele/(allData.size()-failCount));
  Serial.println("average:");
  Serial.println(av.temp);
  tempRange=(maxTem-minTem);
  humRange=(maxHum-minHum);
  IO::location->save(1, av.lat, av.lon, av.ele);
  IO::temperature->save(av.temp);
  IO::humidity->save(av.hum);
  IO::pressure->save(av.pres);
  IO::temprange->save(tempRange);
  IO::humrange->save(humRange);
}