#include "../watchy/Watchy.h"
#include "../sdk/UiSDK.h"
#include "../sdk/UiTemplates.h"

namespace {
struct TimeFormatState {
  Watchy *watchy = nullptr;
  volatile bool exitRequested = false;
  bool dirty = true;
};

static TimeFormatState sTfState;

static void tfBack(Watchy *watchy) {
  UiTemplates::waitForAllButtonsReleased(50, 100);
  sTfState.exitRequested = true;
}

static void tfMenu(Watchy *watchy) {
  UiTemplates::waitForAllButtonsReleased(50, 100);  
  sTfState.exitRequested = true;
}

static void tfToggle(Watchy *watchy) {
  UiTemplates::waitForAllButtonsReleased(50, 100);
  gUse24Hour = !gUse24Hour;
  sTfState.dirty = true;
}
} // namespace

void Watchy::showTimeFormatSetup() {
  guiState = APP_STATE;

  sTfState.watchy = this;
  sTfState.exitRequested = false;
  sTfState.dirty = true;

  // Ensure all buttons are released before starting the app to avoid phantom toggles
  UiTemplates::waitForAllButtonsReleased(50, 400);
  delay(50); // small settlement delay

  setButtonHandlers(tfBack, tfToggle, tfMenu, tfToggle);

  UIControlsRowLayout controls[4] = {
      {"BACK", &Watchy::backPressed},
      {"TOGGLE", &Watchy::upPressed},
      {"SAVE", &Watchy::menuPressed},
      {"TOGGLE", &Watchy::downPressed},
  };

  while (!sTfState.exitRequested) {
    if (!sTfState.dirty) {
      UiSDK::renderControlsRow(*this, controls);
      delay(20);
      continue;
    }
    sTfState.dirty = false;

    UiSDK::initScreen(display);

    UITextSpec text{};
    text.x               = 0;
    text.y               = 60;
    text.w               = 0;
    text.h               = 0;
    text.font            = &FreeMonoBold9pt7b;
    text.fillBackground  = false;
    text.invert          = false;
    text.text            = gUse24Hour
                           ? String("Time Format:\n\n   24 Hour")
                           : String("Time Format:\n\n   12 Hour");

    UIAppSpec app{};
    app.texts        = &text;
    app.textCount    = 1;
    app.images       = nullptr;
    app.imageCount   = 0;
    app.menus        = nullptr;
    app.menuCount    = 0;
    app.buttons      = nullptr;
    app.buttonCount  = 0;
    app.checkboxes   = nullptr;
    app.checkboxCount= 0;
    app.scrollTexts  = nullptr;
    app.scrollTextCount = 0;
    app.callbacks    = nullptr;
    app.callbackCount= 0;

    app.controls[0] = controls[0];
    app.controls[1] = controls[1];
    app.controls[2] = controls[2];
    app.controls[3] = controls[3];

    UiSDK::renderApp(*this, app);
  }

  clearButtonHandlers();

  // Save the new format selection
  saveNVSConfig();
  
  delay(100);
  showMenu(menuIndex);
}
