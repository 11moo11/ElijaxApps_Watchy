#include "WatchfaceRegistry.h"

#include "../sdk/UiSDK.h"
#include "../watchy/Watchy.h"

// ============================================================================
// WATCHFACE REGISTRY LIST
// To remove a watchface, just comment out its line below (add // at the start).
// The compiler will automatically remove the code from flash to save space.
// ============================================================================

// Placed AFTER includes so this overrides any system definition of _
#undef _
#define _(name, func) // Disabled: excluded from build and flash

#define WATCHFACE_LIST(X)                                                      \
  _("7_SEG", draw7Seg)                                                         \
  _("7_SEG_LIGHT", draw7SegLight)                                              \
  _("Analog", drawAnalog)                                                      \
  _("Bad_For_Eye", drawBadForEye)                                              \
  _("Bahn", drawBahn)                                                          \
  _("Basic", drawBasic)                                                        \
  _("BCD", drawBCD)                                                            \
  X("beastie", drawBeastie)                                                    \
  _("Big_Time", drawBigTime)                                                   \
  X("Binary", drawBinary)                                                      \
  _("BinaryBlocks", drawBinaryBlocks)                                          \
  X("BotWatchy", drawBotWatchy)                                                \
  X("Brainwork", drawBrainwork)                                                \
  _("BTTF", drawBTTF)                                                          \
  X("Calculator_Watchy", drawCalculator_Watchy)                                \
  _("Calculateur", drawCalculateur)                                            \
  _("Calendar_WatchFace", drawCalendar_WatchFace)                              \
  _("Captn_Wednesday", drawCaptn_Wednesday)                                    \
  X("Castle_of_Watchy", drawCastleOfWatchy)                                    \
  X("Chaos_-_Lorenz", drawChaosLorenzAttractor)                                \
  _("Chronometer", drawChronometer)                                            \
  _("CityWeather", drawCityWeather)                                            \
  _("Dali", drawDali)                                                          \
  _("dkTime", drawDkTime)                                                      \
  _("Digdug_Watch", drawDigdug)                                                \
  _("DOS", drawDos)                                                            \
  _("erika_Type", drawErika_Type)                                              \
  X("Exactly-Words", drawExactlyWords)                                         \
  _("Hobbit_Time", drawHobbitTime)                                             \
  _("Jarvis", drawJarvis)                                                      \
  X("Kave_Watchy", drawKaveWatchy)                                             \
  _("Keen", drawKeen)                                                          \
  _("Kitty", drawKitty)                                                        \
  _("Last_Laugh", drawLastLaugh)                                               \
  _("LCARS", drawLCARS)                                                        \
  _("Line", drawLine)                                                          \
  X("MacPaint", drawMacPaint)                                                  \
  _("Mario", drawMario)                                                        \
  _("Marquee", drawMarquee)                                                    \
  _("Maze", drawMaze)                                                          \
  X("MetaBall", drawMetaBall)                                                  \
  X("Mickey", drawMickey)                                                      \
  X("Multi_face", drawMulti_face_Watchy)                                       \
  _("Orbital", drawOrbital)                                                    \
  _("Pip-Boy", drawPipBoy)                                                     \
  _("Pokemon", drawPokemon)                                                    \
  _("Poe", drawPoe)                                                            \
  _("pxl999", drawPxl999)                                                      \
  _("QLock", drawQLock)                                                        \
  X("QR_Watchface", drawQrWatchface)                                           \
  _("Re-Dub", drawReDub)                                                       \
  X("Revolution", drawRevolution)                                              \
  _("S2Analog", drawS2Analog)                                                  \
  X("Shadow_Clock", drawShadowClock)                                           \
  _("SW_Watchy", drawSWWatchy)                                                 \
  _("Shijian", drawShijian)                                                    \
  _("Skully", drawSkully)                                                      \
  _("Skykid_Watch", drawSkykid)                                                \
  X("Slacker", drawSlacker)                                                    \
  X("SmartWatchy", drawSmartWatchy)                                            \
  X("Spiral_Watchy", drawSpiralWatchy)                                         \
  _("Squarbital", drawSquarbital)                                              \
  _("Squaro", drawSquaro)                                                      \
  _("Star_Wars", drawStarWarsAurebesh)                                         \
  _("StarryHorizon", drawStarryHorizon)                                        \
  _("Stationary_Text", drawStationaryText)                                     \
  _("Steps", drawSteps)                                                        \
  _("Sundial", drawSundial)                                                    \
  _("Tetris", drawTetris)                                                      \
  _("The_Blob", drawBlob)                                                      \
  _("Triangle", drawTriangle)                                                  \
  _("TypoStyle", drawTypoStyle)                                                \
  _("Watchy_Akira", drawWatchyAkira)                                           \
  _("Watchy_PS", drawPowerShell)                                               \
  _("Watchy7Seg", drawWatchySevenSegment)                                      \
  _("X_Marks_Spot", drawXMarksTheSpot)

// ============================================================================

namespace WatchfaceRegistryDraw {
// Generate forward declarations for all selected watchfaces
#define WF_DECLARE(name, func) void func(Watchy &watchy);
WATCHFACE_LIST(WF_DECLARE)
#undef WF_DECLARE
} // namespace WatchfaceRegistryDraw

namespace WatchfaceRegistry {

// Generate the dispatch table
const Entry kWatchfaces[] = {
#define WF_ENTRY(name, func) {name, &WatchfaceRegistryDraw::func},
    WATCHFACE_LIST(WF_ENTRY)
#undef WF_ENTRY
};

// Generate the menu items
const UIMenuItemSpec kWatchfaceMenuItems[] = {
#define WF_MENU(name, func) {name},
    WATCHFACE_LIST(WF_MENU)
#undef WF_MENU
};

// Automatically calculate the count
const uint8_t kWatchfaceCount = sizeof(kWatchfaces) / sizeof(kWatchfaces[0]);

} // namespace WatchfaceRegistry
