# 1 "f:\\IoT\\az3166watering\\az3166watering.ino"
# 2 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 3 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 4 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 5 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 6 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 7 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 8 "f:\\IoT\\az3166watering\\az3166watering.ino" 2


# 11 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 12 "f:\\IoT\\az3166watering\\az3166watering.ino" 2
# 13 "f:\\IoT\\az3166watering\\az3166watering.ino" 2

static IOTContext context = 
# 14 "f:\\IoT\\az3166watering\\az3166watering.ino" 3 4
                           __null
# 14 "f:\\IoT\\az3166watering\\az3166watering.ino"
                               ;

DfRobotMoistureSensor moisture(5);
DfRobotRelay relay(19);

Az3166PlantOled plantDisplay;
Az3166Sensors sensors;

WiFiClient wificlient;

//char servertest[] = "www.httpbin.org";

char ssid[] = "DevicesGuest"; // your network SSID (name)
char password[] = "GuLogon0511"; // your network password

IOTConnectType connectType = 0x01;
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

void onEvent(IOTContext ctx, IOTCallbackInfo *callbackInfo)
{
  if (strcmp(callbackInfo->eventName, "ConnectionStatus") == 0)
  {
    do { Serial.printf("  - "); Serial.printf("Is connected ? %s (%d)", callbackInfo->statusCode == 0x40 ? "YES" : "NO", callbackInfo->statusCode); Serial.printf("\r\n"); } while(0);
    isConnected = callbackInfo->statusCode == 0x40;
  }

  AzureIOT::StringBuffer buffer;
  if (callbackInfo->payloadLength > 0)
  {
    buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
  }
  do { Serial.printf("  - "); Serial.printf("- [%s] event was received. Payload => %s", callbackInfo->eventName, buffer.getLength() ? *buffer : "EMPTY"); Serial.printf("\r\n"); } while(0);

  if (strcmp(callbackInfo->eventName, "Command") == 0)
  {
    do { Serial.printf("  - "); Serial.printf("- Command name was => %s\r\n", callbackInfo->tag); Serial.printf("\r\n"); } while(0);
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
    do { Serial.printf("X - Error at %s:%d\r\n\t", "f:\\IoT\\az3166watering\\az3166watering.ino", 100); Serial.printf("Error initializing IOTC. Code %d", errorCode); Serial.printf("\r\n"); } while(0);
    return;
  }

  iotc_set_logging(0x02);

  // for the simplicity of this sample, used same callback for all the events below
  iotc_on(context, "MessageSent", onEvent, 
# 107 "f:\\IoT\\az3166watering\\az3166watering.ino" 3 4
                                          __null
# 107 "f:\\IoT\\az3166watering\\az3166watering.ino"
                                              );
  iotc_on(context, "Command", onEvent, 
# 108 "f:\\IoT\\az3166watering\\az3166watering.ino" 3 4
                                      __null
# 108 "f:\\IoT\\az3166watering\\az3166watering.ino"
                                          );
  iotc_on(context, "ConnectionStatus", onEvent, 
# 109 "f:\\IoT\\az3166watering\\az3166watering.ino" 3 4
                                               __null
# 109 "f:\\IoT\\az3166watering\\az3166watering.ino"
                                                   );
  iotc_on(context, "SettingsUpdated", onEvent, 
# 110 "f:\\IoT\\az3166watering\\az3166watering.ino" 3 4
                                              __null
# 110 "f:\\IoT\\az3166watering\\az3166watering.ino"
                                                  );
  iotc_on(context, "Error", onEvent, 
# 111 "f:\\IoT\\az3166watering\\az3166watering.ino" 3 4
                                    __null
# 111 "f:\\IoT\\az3166watering\\az3166watering.ino"
                                        );

  errorCode = iotc_connect(context, scopeId, deviceKey, deviceId, connectType);
  if (errorCode != 0)
  {
    do { Serial.printf("X - Error at %s:%d\r\n\t", "f:\\IoT\\az3166watering\\az3166watering.ino", 116); Serial.printf("Error @ iotc_connect. Code %d", errorCode); Serial.printf("\r\n"); } while(0);
    return;
  }
  do { Serial.printf("  - "); Serial.printf("Done!"); Serial.printf("\r\n"); } while(0);

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
    relay.setRelay(0x1);
  }

  if (moisturePerc > 50)
  {
    pumpState = 0;
    relay.setRelay(0x0);
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
        do { Serial.printf("X - Error at %s:%d\r\n\t", "f:\\IoT\\az3166watering\\az3166watering.ino", 183); Serial.printf("Sending message has failed with error code %d", errorCode); Serial.printf("\r\n"); } while(0);
      }
    }
  }

  if (context)
    iotc_do_work(context); // do background work for iotc

  delay(1000);
}
