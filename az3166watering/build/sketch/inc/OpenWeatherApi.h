#ifndef OpenWeatherApi_h
#define OpenWeatherApi_h
#include <AZ3166WiFi.h>
#include "Arduino.h"

struct oWeatherData {
  double lon;
  double lat;
  double temp;
  double tempMin;
  double tempMax;
  double pressure;
  double humidity;
  double windSpeed;
  double winDeg;
  u_int16_t dateTime; 
  char icon[5];
};

class OpenWeatherApi
{
  public:
    OpenWeatherApi();
    void getWeather(String location);
    void Init(WiFiClient& client);
    void testConnection();
    
    private:
       WiFiClient _client;
};

#endif
