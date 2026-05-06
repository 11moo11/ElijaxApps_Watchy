#include "./../watchy/Watchy.h"
#include "./../sdk/UiSDK.h"
#include "./../sdk/UiTemplates.h"

void renderStatus(Watchy &watchy, const char *l1, const char *l2 = nullptr,
                  bool /*partial*/ = false) {
  UiSDK::initScreen(watchy.display);
  watchy.display.display(true); // ensure any previous content is fully rendered before drawing status
  const char *lines[2] = {l1, l2};
  UiTemplates::renderStatusLines(
    watchy,
    lines,
    2,
    /*x=*/10,
    /*y=*/40,
    /*lineSpacing=*/26,
    UiSDK::defaultFont(),
    "BACK"
  );
  watchy.display.display(true);
}

void Watchy::setupWifi() {
  display.epd2.setBusyCallback(0); // temporarily disable lightsleep on busy

  renderStatus(*this, "WiFi Setup", "Starting AP...");
  WiFiManager wifiManager;
  wifiManager.setAPCallback(_configModeCallback);
  wifiManager.setTimeout(WIFI_AP_TIMEOUT);

  // Force the config portal since the user explicitly requested WiFi Setup
  if (!wifiManager.startConfigPortal(WIFI_AP_SSID)) {
    renderStatus(*this, "WiFi Setup Failed", "Returning to menu..."); 
    delay(2000); 
    showMenu(menuIndex); 
    return;
  } else {
    char ipBuf[32];
    snprintf(ipBuf, sizeof(ipBuf), "%s", WiFi.localIP().toString().c_str());
    UiSDK::initScreen(display); 
    display.display(true);
    
    weatherIntervalCounter = -1; // force weather refresh
    lastIPAddress = WiFi.localIP();
    wifiManager.getWiFiSSID().toCharArray(lastSSID, 30);
    wifiManager.getWiFiPass().toCharArray(lastPassword, 64);
    
    renderStatus(*this, "WiFi Connected!", ipBuf); 
    display.display(true); 
  }

  // enable lightsleep on busy
  //display.epd2.setBusyCallback(WatchyDisplay::busyCallback);
  guiState = APP_STATE;
}
