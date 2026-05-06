#include "../watchy/Watchy.h"
#include "../sdk/UiSDK.h"

bool Watchy::connectWiFi()
{
  WiFi.mode(WIFI_STA);
  
  if (strlen(lastSSID) > 0) {
    WiFi.begin(lastSSID, lastPassword);
  } else {
    WiFi.begin(); // Fallback to NVS
  }

  if (WL_CONNECTED == WiFi.waitForConnectResult()) { // attempt to connect for 10s
    lastIPAddress = WiFi.localIP();
    WIFI_CONFIGURED = true;
  } else { // connection failed, time out
    WIFI_CONFIGURED = false;
    // turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
  }
  return WIFI_CONFIGURED;
}