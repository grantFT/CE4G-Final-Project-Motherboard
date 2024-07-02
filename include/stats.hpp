#ifndef STATS_HPP
#define STATS_HPP

struct stats {
    double lat;
    double lon;
    double ele;
    double temp;
    double hum;
    double pres;

    stats() = default;

    stats(double lat, double lon, double ele, double temp, double hum, double pres)
    : lat(lat),
    lon(lon),
    ele(ele),
    temp(temp),
    hum(hum),
    pres(pres) {}
};

#endif