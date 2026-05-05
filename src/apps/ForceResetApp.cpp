#include "../watchy/Watchy.h"
#include "../sdk/UiSDK.h"
#include "../sdk/UiTemplates.h"

// Force Reset App
// Shows a confirmation screen: MENU = confirm reset, BACK = cancel.
// On confirm: calls esp_restart() which clears RTC_DATA_ATTR memory and
// re-runs the cold-boot path (RTC.config, _bmaConfig, step counter reset,
// full display refresh).

static volatile bool sResetConfirmed = false;
static volatile bool sResetCancelled = false;

static void onConfirm(Watchy *) { sResetConfirmed = true; }
static void onCancel(Watchy *)  { sResetCancelled = true; }

void Watchy::showForceReset() {
  guiState = APP_STATE;
  UiTemplates::waitForAllButtonsReleased();

  sResetConfirmed = false;
  sResetCancelled = false;

  // Confirmation screen layout
  UIControlsRowLayout controls[4] = {
    {"CANCEL", &Watchy::backPressed},   // BACK  = cancel
    {nullptr,  nullptr},
    {"RESET",  &Watchy::menuPressed},   // MENU  = confirm
    {nullptr,  nullptr},
  };

  setButtonHandlers(onCancel, nullptr, onConfirm, nullptr);

  // Draw the warning screen
  UiSDK::initScreen(display);
  Palette palette = {
    BASE_POLARITY == WatchfacePolarity::WhiteOnBlack ? GxEPD_BLACK : GxEPD_WHITE,
    BASE_POLARITY == WatchfacePolarity::WhiteOnBlack ? GxEPD_WHITE : GxEPD_BLACK,
  };

  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(palette.fg);

  // Title bar
  display.fillRect(0, 0, WatchyDisplay::WIDTH, 20, palette.fg);
  display.setTextColor(palette.bg);
  display.setCursor(4, 14);
  display.print("FORCE RESET");
  display.setTextColor(palette.fg);

  // Body text
  display.setCursor(4, 42);
  display.println("This will:");
  display.setCursor(4, 62);
  display.println("- Restart device");
  display.setCursor(4, 82);
  display.println("- Clear RAM state");
  display.setCursor(4, 102);
  display.println("- Reset step count");
  display.setCursor(4, 122);
  display.println("- Full display");
  display.setCursor(4, 142);
  display.println("  refresh");

  display.setCursor(4, 168);
  display.println("MENU=confirm");
  display.setCursor(4, 186);
  display.println("BACK=cancel");

  display.display(false); // full refresh for the warning screen

  // Wait for button press
  while (!sResetConfirmed && !sResetCancelled) {
    UiSDK::renderControlsRow(*this, controls);
    delay(20);
  }

  clearButtonHandlers();

  if (sResetConfirmed) {
    // Brief "Resetting..." screen before restart
    UiSDK::initScreen(display);
    display.fillRect(0, 0, WatchyDisplay::WIDTH, 20, palette.fg);
    display.setTextColor(palette.bg);
    display.setCursor(4, 14);
    display.print("FORCE RESET");
    display.setTextColor(palette.fg);
    display.setCursor(4, 60);
    display.println("Restarting...");
    display.display(false);
    delay(1000);
    esp_restart(); // soft reset — re-runs cold-boot init() default: branch
  }

  // Cancelled — return to menu
  delay(100);
  showMenu(menuIndex);
}
