#include "Arduino.h"
#include "../inc/OpenWeatherApi.h"
#include <AZ3166WiFi.h>
#include <ArduinoJson.h>
#include "http_client.h"

//String apiKey= "31098654d4752362e6b8e05bf9d673fa"; 
//String location= "Cape Town"; 
//int status = WL_IDLE_STATUS; 
//char server[] = "api.openweathermap.org"; 



OpenWeatherApi::OpenWeatherApi()
{
	
}

void OpenWeatherApi::Init(WiFiClient& client)
{
	_client = client;
}

void OpenWeatherApi::testConnection()
{
  oWeatherData weatherData;

  char tmpServer[] = "http://api.openweathermap.org/data/2.5/weather?q=London&appid=31098654d4752362e6b8e05bf9d673fa&units=metric"; 
  HTTPClient *httpClient = new HTTPClient(HTTP_GET, tmpServer);
  const Http_Response *result = httpClient->send();
  if (result != NULL) {
    const char *response = result->body;
     Serial.println(response);

    DynamicJsonDocument doc(1024);
    doc.capacity();
    DeserializationError err = deserializeJson(doc, response); 
    
 
 if (err) {
      // Deserialization failed
     
      Serial.print(F("deserializeJson() failed with code"));
      Serial.println(err.c_str());
    } else {

           Serial.print("Worked");
     uint16_t resultCode = doc["cod"];
      Serial.println(resultCode);

    weatherData.lon = doc["coord"]["lon"];
    weatherData.lat = doc["coord"]["lat"];
    weatherData.temp = doc["main"]["temp"];
    weatherData.tempMin = doc["main"]["temp_min"];
    weatherData.tempMax = doc["main"]["temp_max"];
    weatherData.pressure = doc["main"]["pressure"];
    weatherData.humidity = doc["main"]["humidity"];
    weatherData.windSpeed = doc["wind"]["speed"];
    weatherData.winDeg = doc["wind"]["deg"];
    weatherData.dateTime = doc["main"]["dt"];
    String icon = doc["weather"][0]["icon"];

    doc.clear();
    doc.garbageCollect();

    char iconarr[5];


    icon.toCharArray(weatherData.icon, 5);

       //Serial.println(lon);
       Serial.println(weatherData.icon);
    }

  }
  else
  {
     Serial.print("Error Code: ");
    Serial.println(httpClient->get_error());
  }


  //return weatherData;
}

void OpenWeatherApi::getWeather(String location) { 
 Serial.println("\nStarting connection to server..."); 
 // if you get a connection, report back via serial: 
  char tmpServer[] = "api.openweathermap.org"; 
 String apiKey= "31098654d4752362e6b8e05bf9d673fa"; 

 if (_client.connect(tmpServer, 80)) { 
   Serial.println("connected to server"); 
   // Make a HTTP request: 
   _client.print("GET /data/2.5/forecast?"); 
   _client.print("q="+location); 
   _client.print("&appid="+apiKey); 
   _client.print("&cnt=3"); 
   _client.println("&units=metric"); 
   _client.println("Host: api.openweathermap.org"); 
   _client.println("Connection: close"); 
   _client.println(); 
 } else { 
   Serial.println("unable to connect"); 
 } 
 delay(1000); 
 String line = ""; 

 char buff[2048];
String jsonPayload;
int buffIndex=0;

 while (_client.connected()) { 
   Serial.println("Connected"); 
   //line = _client.readStringUntil('\n'); 
  // Serial.println(line); 

/*  while (true)
    {
        int ret = _client.read(buff, sizeof(buff));
        if (ret <= 0)
        break;
        for(int i = 0; i < ret; i++)
        {
            Serial.print((char)buff[i]);
        }
    }*/

    while (_client.available())
    {
      char c = _client.read();
      buff[buffIndex] = c;
       buffIndex++;
       Serial.println(buffIndex);
    }

   
  
 } 

//DynamicJsonDocument openWeatherDoc(1024);
StaticJsonDocument<1024> openWeatherDoc;
DeserializationError err = deserializeJson(openWeatherDoc, buff);
   //jsonPayload = buff;
//   Serial.println(buff);

if (err.Ok)
  Serial.print("ALL GOOD");
  else if (err.IncompleteInput)
  Serial.print("INCOMPLETE");
  else if (err.InvalidInput)
    Serial.print("INVALID");



}