#include "../watchy/Watchy.h"
#include "../sdk/UiSDK.h"
#include "../sdk/UiTemplates.h"

namespace {
struct TzEntry {
    const char* label;
    int16_t offsetMin;
};

const TzEntry kTzList[] = {
    {"UTC -12:00", -720},
    {"SST (UTC -11)", -660},
    {"HST (UTC -10)", -600},
    {"AKST (UTC -9)", -540},
    {"PST (UTC -8)", -480},
    {"MST (UTC -7)", -420},
    {"CST (UTC -6)", -360},
    {"EST (UTC -5)", -300},
    {"AST (UTC -4)", -240},
    {"NST (UTC -3:30)", -210},
    {"BRT (UTC -3)", -180},
    {"FNT (UTC -2)", -120},
    {"CVT (UTC -1)", -60},
    {"GMT (UTC +0)", 0},
    {"CET (UTC +1)", 60},
    {"EET (UTC +2)", 120},
    {"MSK (UTC +3)", 180},
    {"IRST (UTC +3:30)", 210},
    {"GST (UTC +4)", 240},
    {"AFT (UTC +4:30)", 270},
    {"PKT (UTC +5)", 300},
    {"IST (UTC +5:30)", 330},
    {"NPT (UTC +5:45)", 345},
    {"BST (UTC +6)", 360},
    {"MMT (UTC +6:30)", 390},
    {"ICT (UTC +7)", 420},
    {"AWST (UTC +8)", 480},
    {"ACWST (UTC +8:45)", 525},
    {"JST (UTC +9)", 540},
    {"ACST (UTC +9:30)", 570},
    {"AEST (UTC +10)", 600},
    {"ACDT (UTC +10:30)", 630},
    {"AEDT (UTC +11)", 660},
    {"NZST (UTC +12)", 720},
    {"CHAST (UTC +12:45)", 765},
    {"NZDT (UTC +13)", 780},
    {"LINT (UTC +14)", 840},
};
const uint8_t kTzCount = sizeof(kTzList) / sizeof(kTzList[0]);

struct TimezoneState {
  Watchy *watchy = nullptr;
  volatile bool exitRequested = false;
  bool dirty = true;
  uint8_t selectedIndex = 13;
};

static TimezoneState sTzState;

static void tzBack(Watchy *watchy) {
  UiTemplates::waitForAllButtonsReleased(50, 100);
  sTzState.exitRequested = true;
}

static void tzMenu(Watchy *watchy) {
  UiTemplates::waitForAllButtonsReleased(50, 100);  
  sTzState.exitRequested = true;
}

static void tzDown(Watchy *watchy) {
  UiTemplates::waitForAllButtonsReleased(50, 100);
  if (sTzState.selectedIndex < kTzCount - 1) {
    sTzState.selectedIndex++;
  } else {
    sTzState.selectedIndex = 0; // Wrap around
  }
  sTzState.dirty = true;
}

static void tzUp(Watchy *watchy) {
  UiTemplates::waitForAllButtonsReleased(50, 100);
  if (sTzState.selectedIndex > 0) {
    sTzState.selectedIndex--;
  } else {
    sTzState.selectedIndex = kTzCount - 1; // Wrap around
  }
  sTzState.dirty = true;
}
} // namespace

void Watchy::showTimezoneSetup() {
  guiState = APP_STATE;

  sTzState.watchy = this;
  sTzState.exitRequested = false;
  sTzState.dirty = true;
  
  int16_t currentOffset = static_cast<int16_t>(gmtOffset / 60);
  sTzState.selectedIndex = 13; // Default to GMT
  for (int i = 0; i < kTzCount; ++i) {
      if (kTzList[i].offsetMin == currentOffset) {
          sTzState.selectedIndex = i;
          break;
      }
  }

  UiTemplates::waitForAllButtonsReleased();

  setButtonHandlers(tzBack, tzUp, tzMenu, tzDown);

  UIControlsRowLayout controls[4] = {
      {"BACK", &Watchy::backPressed},
      {"UP", &Watchy::upPressed},
      {"SAVE", &Watchy::menuPressed},
      {"DOWN", &Watchy::downPressed},
  };

  UIMenuItemSpec menuItems[kTzCount];
  for(int i = 0; i < kTzCount; ++i) {
      menuItems[i].label = kTzList[i].label;
  }

  while (!sTzState.exitRequested) {
    UiSDK::renderControlsRow(*this, controls);

    if (!sTzState.dirty) {
      delay(10);
      continue;
    }
    sTzState.dirty = false;

    UiSDK::initScreen(display);

    UITextSpec headerText;
    headerText.x               = 4;
    headerText.y               = 24;
    headerText.w               = 0;
    headerText.h               = 0;
    headerText.font            = &FreeMonoBold9pt7b;
    headerText.fillBackground  = false;
    headerText.text            = "Set Timezone:";
    headerText.invert          = false;

    UIMenuSpec menu{};
    menu.x = 0;
    menu.y = 30;
    menu.w = 200;
    menu.h = 160;
    menu.itemHeight = 32;
    menu.font = &FreeMonoBold9pt7b;
    menu.items = menuItems;
    menu.itemCount = kTzCount;
    menu.selectedIndex = sTzState.selectedIndex;
    menu.startIndex = UiTemplates::calcMenuStartIndex(sTzState.selectedIndex, 4, kTzCount);
    menu.visibleCount = 4;

    UIAppSpec app{};
    app.texts        = &headerText;
    app.textCount    = 1;
    app.images       = nullptr;
    app.imageCount   = 0;
    app.menus        = &menu;
    app.menuCount    = 1;
    app.buttons      = nullptr;
    app.buttonCount  = 0;
    app.scrollTexts  = nullptr;
    app.scrollTextCount = 0;
    app.callbacks    = nullptr;
    app.callbackCount = 0;

    app.controls[0] = controls[0];
    app.controls[1] = controls[1];
    app.controls[2] = controls[2];
    app.controls[3] = controls[3];

    UiSDK::renderApp(*this, app);
  }

  clearButtonHandlers();

  // Save the new timezone
  gmtOffset = static_cast<long>(kTzList[sTzState.selectedIndex].offsetMin) * 60L;
  settings.gmtOffset = static_cast<int>(gmtOffset);
  saveNVSConfig();
  
  delay(100);
  showMenu(menuIndex);
}
