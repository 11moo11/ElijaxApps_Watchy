#include "WatchfaceRegistry.h"

#include "../sdk/UiSDK.h"
#include "../watchy/Watchy.h"

// ============================================================================
// WATCHFACE REGISTRY LIST
// ============================================================================
// HOW TO ENABLE/DISABLE a watchface:
//   X("Name", drawFunc)  = ENABLED  (compiled + shown in menu)
//   _("Name", drawFunc)  = DISABLED (excluded from build entirely)
//
// Only edit this list. platformio.ini and the build script update automatically.
// ============================================================================
#define _(name, func) // Disabled — excluded from build

#define WATCHFACE_LIST(X) \
  _("7_SEG",             draw7Seg) \
  _("7_SEG_LIGHT",       draw7SegLight) \
  _("Analog",            drawAnalog) \
  _("Bad_For_Eye",       drawBadForEye) \
  _("Bahn",              drawBahn) \
  _("Basic",             drawBasic) \
  _("BCD",               drawBCD) \
  _("beastie",           drawBeastie) \
  _("Big_Time",          drawBigTime) \
  _("Binary",            drawBinary) \
  _("BinaryBlocks",      drawBinaryBlocks) \
  _("BotWatchy",         drawBotWatchy) \
  _("Brainwork",         drawBrainwork) \
  _("BTTF",              drawBTTF) \
  _("Calculator_Watchy", drawCalculator_Watchy) \
  _("Calculateur",       drawCalculateur) \
  _("Calendar_WatchFace",drawCalendar_WatchFace) \
  _("Captn_Wednesday",   drawCaptn_Wednesday) \
  _("Castle_of_Watchy",  drawCastleOfWatchy) \
  _("Chaos_Lorenz",      drawChaosLorenzAttractor) \
  _("Chronometer",       drawChronometer) \
  _("CityWeather",       drawCityWeather) \
  _("Dali",              drawDali) \
  _("dkTime",            drawDkTime) \
  _("Digdug_Watch",      drawDigdug) \
  _("DOS",               drawDos) \
  _("erika_Type",        drawErika_Type) \
  _("Exactly-Words",     drawExactlyWords) \
  _("Hobbit_Time",       drawHobbitTime) \
  _("Jarvis",            drawJarvis) \
  _("Kave_Watchy",       drawKaveWatchy) \
  _("Keen",              drawKeen) \
  _("Kitty",             drawKitty) \
  _("Last_Laugh",        drawLastLaugh) \
  _("LCARS",             drawLCARS) \
  _("Line",              drawLine) \
  _("MacPaint",          drawMacPaint) \
  _("Mario",             drawMario) \
  _("Marquee",           drawMarquee) \
  _("Maze",              drawMaze) \
  _("MetaBall",          drawMetaBall) \
  _("Mickey",            drawMickey) \
  _("Multi_face_Watchy", drawMulti_face_Watchy) \
  _("Orbital",           drawOrbital) \
  _("Pip-Boy",           drawPipBoy) \
  _("Pokemon",           drawPokemon) \
  _("Poe",               drawPoe) \
  _("pxl999",            drawPxl999) \
  _("QLock",             drawQLock) \
  _("QR_Watchface",      drawQrWatchface) \
  _("Re-Dub",            drawReDub) \
  _("Revolution",        drawRevolution) \
  _("S2Analog",          drawS2Analog) \
  _("Shadow_Clock",      drawShadowClock) \
  _("SW_Watchy",         drawSWWatchy) \
  _("Shijian",           drawShijian) \
  _("Skully",            drawSkully) \
  _("Skykid_Watch",      drawSkykid) \
  _("Slacker",           drawSlacker) \
  _("SmartWatchy",       drawSmartWatchy) \
  _("Spiral_Watchy",     drawSpiralWatchy) \
  _("Squarbital",        drawSquarbital) \
  _("Squaro",            drawSquaro) \
  _("Star_Wars_Aurebesh",drawStarWarsAurebesh) \
  _("StarryHorizon",     drawStarryHorizon) \
  _("Stationary_Text",   drawStationaryText) \
  _("Steps",             drawSteps) \
  X("Sundial",           drawSundial) \
  _("Tetris",            drawTetris) \
  X("The_Blob",          drawBlob) \
  X("Triangle",          drawTriangle) \
  X("TypoStyle",         drawTypoStyle) \
  X("Watchy_Akira",      drawWatchyAkira) \
  X("Watchy_PowerShell", drawPowerShell) \
  X("WatchySevenSegment",drawWatchySevenSegment) \
  X("X_marks_the_spot",  drawXMarksTheSpot)

// ============================================================================

namespace WatchfaceRegistryDraw {
#define WF_DECLARE(name, func) void func(Watchy &watchy);
WATCHFACE_LIST(WF_DECLARE)
#undef WF_DECLARE
} // namespace WatchfaceRegistryDraw

namespace WatchfaceRegistry {

const Entry kWatchfaces[] = {
#define WF_ENTRY(name, func) {name, &WatchfaceRegistryDraw::func},
WATCHFACE_LIST(WF_ENTRY)
#undef WF_ENTRY
};

const UIMenuItemSpec kWatchfaceMenuItems[] = {
#define WF_MENU(name, func) {name},
WATCHFACE_LIST(WF_MENU)
#undef WF_MENU
};

const uint8_t kWatchfaceCount = sizeof(kWatchfaces) / sizeof(kWatchfaces[0]);

} // namespace WatchfaceRegistry
