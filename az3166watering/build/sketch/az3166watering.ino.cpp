#include <Arduino.h>
#line 1 "f:\\IoT\\az3166watering\\az3166watering.ino"
#include <ArduinoJson.h>
#include <AZ3166WiFi.h>
#include <OledDisplay.h>
#include "inc/DfRobotMoistureSensor.h"
#include "inc/DfRobotRelay.h"
#include "inc/Az3166PlantOled.h"
#include "inc/Az3166Sensors.h"

#define SERIAL_VERBOSE_LOGGING_ENABLED 1
#include "src/iotc/iotc.h"
#include "src/iotc/common/string_buffer.h"
#include <EEPROMInterface.h>

static IOTContext context = NULL;

DfRobotMoistureSensor moisture(5);
DfRobotRelay relay(19);

Az3166PlantOled plantDisplay;
Az3166Sensors sensors;

WiFiClient wificlient;

//char servertest[] = "www.httpbin.org";

char ssid[] = "DevicesGuest";  // your network SSID (name)
char password[] = "GuLogon0511"; // your network password

IOTConnectType connectType = IOTC_CONNECT_SYMM_KEY;
const char *scopeId = "0ne001AE3E5";
const char *deviceId = "1gwmxf5zf4h";
const char *deviceKey = "BYj5bA5iFOnyBOVzNvBjM1BumvOl2NE84bf0k6u82sQ=";

int keyIndex = 0; // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;

static float humidity;
static float temperature;
static float pressure;
static int pumpState;

static bool isConnected = false;

#line 44 "f:\\IoT\\az3166watering\\az3166watering.ino"
void onEvent(IOTContext ctx, IOTCallbackInfo *callbackInfo);
#line 67 "f:\\IoT\\az3166watering\\az3166watering.ino"
void setup();
#line 124 "f:\\IoT\\az3166watering\\az3166watering.ino"
void loop();
#line 44 "f:\\IoT\\az3166watering\\az3166watering.ino"
void onEvent(IOTContext ctx, IOTCallbackInfo *callbackInfo)
{
  if (strcmp(callbackInfo->eventName, "ConnectionStatus") == 0)
  {
    LOG_VERBOSE("Is connected ? %s (%d)", callbackInfo->statusCode == IOTC_CONNECTION_OK ? "YES" : "NO", callbackInfo->statusCode);
    isConnected = callbackInfo->statusCode == IOTC_CONNECTION_OK;
  }

  AzureIOT::StringBuffer buffer;
  if (callbackInfo->payloadLength > 0)
  {
    buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
  }
  LOG_VERBOSE("- [%s] event was received. Payload => %s", callbackInfo->eventName, buffer.getLength() ? *buffer : "EMPTY");

  if (strcmp(callbackInfo->eventName, "Command") == 0)
  {
    LOG_VERBOSE("- Command name was => %s\r\n", callbackInfo->tag);
  }
}

static unsigned prevMillis = 0, loopId = 0;

void setup()
{
  Serial.begin(115200);

  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true)
      ;
  }

  const char *fv = WiFi.firmwareVersion();
  Serial.printf("Wi-Fi firmware: %s\r\n", fv);

  Screen.init();
  sensors.Init();

  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(1000);
  }

  Serial.print("Connected to Wifi");

  int errorCode = iotc_init_context(&context);
  if (errorCode != 0)
  {
    LOG_ERROR("Error initializing IOTC. Code %d", errorCode);
    return;
  }

  iotc_set_logging(IOTC_LOGGING_API_ONLY);

  // for the simplicity of this sample, used same callback for all the events below
  iotc_on(context, "MessageSent", onEvent, NULL);
  iotc_on(context, "Command", onEvent, NULL);
  iotc_on(context, "ConnectionStatus", onEvent, NULL);
  iotc_on(context, "SettingsUpdated", onEvent, NULL);
  iotc_on(context, "Error", onEvent, NULL);

  errorCode = iotc_connect(context, scopeId, deviceKey, deviceId, connectType);
  if (errorCode != 0)
  {
    LOG_ERROR("Error @ iotc_connect. Code %d", errorCode);
    return;
  }
  LOG_VERBOSE("Done!");

  prevMillis = millis();
}

void loop()
{
  Serial.printf("Temperature: %f\n", temperature);

  humidity = sensors.ReadHumidity();
  pressure = sensors.ReadPressure();
  temperature = sensors.ReadTemperature();

  MoistureDegree moistureDegree;

  int a1 = moisture.GetSensorReading();

  Serial.println(a1);

  int moisturePerc = moisture.GetMoisturePercentage();
  Serial.println(moisturePerc);

  plantDisplay.SetHumidity(humidity);
  plantDisplay.SetTemperature(temperature);
  plantDisplay.SetPressure(pressure);
  plantDisplay.SetMoisturePercentage(moisturePerc);

  if (moisturePerc < 40)
  {
    plantDisplay.SetPlantState(1);
  }
  else
    plantDisplay.SetPlantState(0);

  plantDisplay.InvalidateDisplay();

  if (moisturePerc < 30)
  {
    pumpState = 1;
    relay.setRelay(HIGH);
  }

  if (moisturePerc > 50)
  {
    pumpState = 0;
    relay.setRelay(LOW);
  }

  if (isConnected)
  {
    unsigned long ms = millis();
    if (ms - prevMillis > 15000)
    { // send telemetry every 15 seconds
      char msg[128] = {0};
      int pos = 0, errorCode = 0;

      prevMillis = ms;
      pos = snprintf(msg, sizeof(msg) - 1, "{\"temperature\":%f,\"humidity\":%f,\"pressure\":%f,\"moisture\":%d,\"moistureperc\":%d,\"pumpstate\":%d}", temperature, humidity, pressure, a1, moisturePerc, pumpState);
      errorCode = iotc_send_telemetry(context, msg, pos);

      msg[pos] = 0;

      if (errorCode != 0)
      {
        LOG_ERROR("Sending message has failed with error code %d", errorCode);
      }
    }
  }

  if (context)
    iotc_do_work(context); // do background work for iotc

  delay(1000);
}

