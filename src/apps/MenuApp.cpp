#include "../sdk/UiSDK.h"
#include "../sdk/UiTemplates.h"
#include "../watchy/Watchy.h"

enum class MenuCategory : uint8_t {
  Clock = 0,
  Connectivity,
  Display,
  Tools,
  System,
  Count
};

constexpr uint8_t kTopCount = static_cast<uint8_t>(MenuCategory::Count);

UIMenuItemSpec kTopMenuItems[] = {
    {"Clock"}, {"WiFi"}, {"Display"}, {"Tools & Apps"}, {"System Settings"},
};

UIMenuItemSpec kClockMenu[] = {
    {"Alarm"},        {"Timer"},         {"Chronometer"}, {"Set Time"},
    {"Sun rise/set"}, {"Moon rise/set"}, {"Moon phase"},
    {"Time Format"},  {"Set Timezone"},
};

UIMenuItemSpec kConnectivityMenu[] = {
    {"Set up WiFi"}, {"Sync with NTP"}, {"Ping"},      {"Traceroute"},
    {"DNS Look-up"}, {"Whois"},         {"Port scan"}, {"HTTP client"},
};

UIMenuItemSpec kDisplayMenu[] = {
    {"Watchfaces"},
    {"Invert Colors"},
};

UIMenuItemSpec kToolsMenu[] = {
    {"Web Search"}, {"Text browser"}, {"News"}, {"Radio"}, {"Morse Game"},
};

UIMenuItemSpec kSystemMenu[] = {
    {"Vibrate Motor"}, {"Show Accelerometer"}, {"Update FW"},
    {"About Watchy"},  {"Force Reset"},
};

struct MenuGroup {
  const UIMenuItemSpec *items;
  uint8_t length;
};

const MenuGroup kMenuGroups[] = {
    {kClockMenu,
     static_cast<uint8_t>(sizeof(kClockMenu) / sizeof(kClockMenu[0]))},
    {kConnectivityMenu, static_cast<uint8_t>(sizeof(kConnectivityMenu) /
                                             sizeof(kConnectivityMenu[0]))},
    {kDisplayMenu,
     static_cast<uint8_t>(sizeof(kDisplayMenu) / sizeof(kDisplayMenu[0]))},
    {kToolsMenu,
     static_cast<uint8_t>(sizeof(kToolsMenu) / sizeof(kToolsMenu[0]))},
    {kSystemMenu,
     static_cast<uint8_t>(sizeof(kSystemMenu) / sizeof(kSystemMenu[0]))},
};

const MenuGroup *groupFor(uint8_t category) {
  return (category < kTopCount) ? &kMenuGroups[category] : nullptr;
}

uint8_t lengthFor(uint8_t level, uint8_t category) {
  if (level == 0) {
    return kTopCount;
  }
  const MenuGroup *g = groupFor(category);
  return g ? g->length : 0;
}

const UIMenuItemSpec *itemsFor(uint8_t level, uint8_t category) {
  if (level == 0) {
    return kTopMenuItems;
  }
  const MenuGroup *g = groupFor(category);
  return g ? g->items : nullptr;
}

uint8_t clampIndex(uint8_t idx, uint8_t len) {
  if (len == 0) {
    return 0;
  }
  return (idx >= len) ? static_cast<uint8_t>(len - 1) : idx;
}

void renderMenu(Watchy &watchy, byte menuIndex, uint8_t visibleRows) {
  // Removed waitForAllButtonsReleased() to allow faster scrolling and
  // held-button repeats.
  const uint8_t itemCount = lengthFor(menuLevel, menuCategory);
  const UIMenuItemSpec *items = itemsFor(menuLevel, menuCategory);
  if (items == nullptr || itemCount == 0) {
    return;
  }

  menuIndex = clampIndex(menuIndex, itemCount);

  UiSDK::initScreen(watchy.display);
  UIMenuSpec menuSpec{};
  menuSpec.x = 0;
  menuSpec.y = MENU_HEIGHT;
  menuSpec.itemHeight = MENU_HEIGHT;
  menuSpec.font = &FreeMonoBold9pt7b;
  menuSpec.items = items;
  menuSpec.itemCount = itemCount;
  menuSpec.selectedIndex = menuIndex;
  menuSpec.visibleCount = visibleRows;
  menuSpec.startIndex =
      UiTemplates::calcMenuStartIndex(menuIndex, visibleRows, itemCount);

  UIAppSpec app{};
  app.menus = &menuSpec;
  app.menuCount = 1;

  UiSDK::renderApp(watchy, app);

  guiState = MAIN_MENU_STATE;
  alreadyInMenu = true;
}

uint8_t Watchy::activeMenuLength() const {
  return lengthFor(menuLevel, menuCategory);
}

bool Watchy::isInSubMenu() const { return menuLevel != 0; }

void Watchy::enterSubMenu(uint8_t categoryIndex) {
  if (categoryIndex >= kTopCount) {
    return;
  }
  menuLevel = 1;
  menuCategory = categoryIndex;
  menuIndex = 0;
  alreadyInMenu = false;
}

void Watchy::returnToTopMenu() {
  menuLevel = 0;
  menuCategory = 0;
  menuIndex = clampIndex(menuIndex, kTopCount);
  alreadyInMenu = false;
}

void Watchy::launchMenuAction(uint8_t categoryIndex, uint8_t itemIndex) {
  alreadyInMenu = false;

  switch (static_cast<MenuCategory>(categoryIndex)) {
  case MenuCategory::Clock:
    if (itemIndex == 0) {
      showAlarm();
    } else if (itemIndex == 1) {
      showTimer();
    } else if (itemIndex == 2) {
      showChronometer();
    } else if (itemIndex == 3) {
      showSetTime();
    } else if (itemIndex == 4) {
      showSunRise();
    } else if (itemIndex == 5) {
      showMoonRise();
    } else if (itemIndex == 6) {
      showMoonPhase();
    } else if (itemIndex == 7) {
      showTimeFormatSetup();
    } else if (itemIndex == 8) {
      showTimezoneSetup();
    }
    break;
  case MenuCategory::Connectivity:
    if (itemIndex == 0) {
      setupWifi();
    } else if (itemIndex == 1) {
      showSyncNTP();
    }
    break;
  case MenuCategory::Display:
    if (itemIndex == 0) {
      showWatchfaceSelector();
    } else if (itemIndex == 1) {
      showInvertColors();
    } else if (itemIndex == 2) {
      showInvertColors();
    } // Invert is basically "Theme" toggle in this firmware
    break;
  case MenuCategory::Tools:
    if (itemIndex == 0) {
      showGoogleSearch();
    } else if (itemIndex == 1) {
      showTextBrowserHome();
    } else if (itemIndex == 2) {
      showNewsReader();
    } else if (itemIndex == 3) {
      showRadio();
    } else if (itemIndex == 4) {
      showMorseGame();
    } else if (itemIndex == 5) {
      showPing();
    } else if (itemIndex == 6) {
      showTraceroute();
    } else if (itemIndex == 7) {
      showDig();
    } else if (itemIndex == 8) {
      showWhois();
    } else if (itemIndex == 9) {
      showPortScanner();
    } else if (itemIndex == 10) {
      showPostman();
    }
    break;
  case MenuCategory::System:
    if (itemIndex == 0) {
      showBuzz();
    } else if (itemIndex == 1) {
      showAccelerometer();
    } else if (itemIndex == 2) {
      showUpdateFW();
    } else if (itemIndex == 3) {
      showAbout();
    } else if (itemIndex == 4) {
      showForceReset();
    }
    break;
  default:
    break;
  }
}

void Watchy::showMenu(byte menuIndex) {
  const uint8_t visibleRows = 9; // shared layout
  renderMenu(*this, menuIndex, visibleRows);
}

void Watchy::showFastMenu(byte menuIndex) {
  // Same rendering path keeps things simple; still responds quickly
  const uint8_t visibleRows = 9; // shared layout
  renderMenu(*this, menuIndex, visibleRows);
}
