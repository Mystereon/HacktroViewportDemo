/*
  FEED ME , I'M POOR AND I MADE THIS FOR FREE - https://paypal.me/Mystereon

  HacktroViewportDemo.ino
  A 24 fps Amiga-hacktro-style procedural volumetric demo for a 5x5x5
  WS2812B cube plus a 12-pixel rear acrylic mood ring.

  Created by Dad (MysterEon) & Manus, 2026.

  Hardware
  --------
  Waveshare ESP32-S3-Zero
  GPIO6 -> cube LED 0 DIN -> 125 matrix LEDs -> 12 rear ring LEDs
  Cube origin: bottom-rear-left. x = left-to-right, y = rear-to-front,
  z = bottom-to-top. Matrix is non-serpentine: z*25 + y*5 + x.

  Performance contract
  --------------------
  This sketch schedules a complete 137-pixel output frame every 41 ms
  (24 fps). It never uses delay(), never retains a virtual 3-D framebuffer,
  and never calls sin(), cos(), or sqrt() inside the render loop. The cube
  samples only the 125 voxels visible through its moving virtual camera.
*/

#include <FastLED.h>
#include "StoryData.h"

// Arduino inserts generated function prototypes before user declarations. A
// fixed-underlying-type forward declaration keeps those prototypes valid until
// the full scene enum appears below.
enum DemoScene : uint8_t;
enum StoryAct : uint8_t;

// -----------------------------------------------------------------------------
// Hardware and 24 fps presentation contract
// -----------------------------------------------------------------------------

#define DATA_PIN 6
#define CHIPSET WS2812B
#define COLOR_ORDER GRB

constexpr uint8_t CUBE_X = 5;
constexpr uint8_t CUBE_Y = 5;
constexpr uint8_t CUBE_Z = 5;
constexpr uint16_t MATRIX_LEDS = CUBE_X * CUBE_Y * CUBE_Z;
constexpr uint8_t RING_LEDS = 12;
constexpr uint16_t TOTAL_LEDS = MATRIX_LEDS + RING_LEDS;

constexpr uint8_t GLOBAL_BRIGHTNESS = 100;
constexpr uint8_t RING_BRIGHTNESS = 190;
constexpr uint8_t TARGET_FPS = 24;
constexpr uint16_t FRAME_INTERVAL_MS = 1000U / TARGET_FPS; // 41 ms.
constexpr uint32_t STORY_DURATION_MS = 300000UL;

// Q8 coordinates permit a virtual world much larger than the physical cube
// without allocating a virtual voxel framebuffer.
constexpr int16_t FP_ONE = 256;

CRGB leds[TOTAL_LEDS];
uint32_t nextFrameAt = 0;
uint32_t maxRenderMicros = 0;
uint32_t telemetryAt = 0;

// -----------------------------------------------------------------------------
// Physical mapper and integer-only virtual-world helpers
// -----------------------------------------------------------------------------

uint16_t ledIndex(uint8_t x, uint8_t y, uint8_t z) {
  return uint16_t(z) * 25U + uint16_t(y) * 5U + x;
}

uint8_t hashWorld(int16_t x, int16_t y, int16_t z, uint8_t salt) {
  uint32_t n = uint16_t(x) * 0x45d9f3bUL;
  n ^= uint16_t(y) * 0x119de1f3UL;
  n ^= uint16_t(z) * 0x3449f9UL;
  n ^= uint32_t(salt) * 0x9e3779b9UL;
  n ^= n >> 16;
  n *= 0x45d9f3bUL;
  n ^= n >> 16;
  return uint8_t(n >> 24);
}

inline uint8_t virtualPhase(int16_t value, uint8_t multiplier, uint8_t time) {
  return uint8_t(uint16_t(value) * multiplier + time);
}

enum DemoScene : uint8_t {
  SCENE_TUNNEL,
  SCENE_GRID,
  SCENE_PLASMA,
  SCENE_VECTOR_CITY,
  SCENE_STARFIELD,
  SCENE_COUNT
};

enum StoryAct : uint8_t {
  ACT_BOOT_SIGNAL,
  ACT_CORRIDOR,
  ACT_WALKER,
  ACT_MACHINE_CITY,
  ACT_FRACTURE,
  ACT_DEEP_SPACE,
  ACT_RECONSTRUCTION,
  ACT_FINAL_ASCENT,
  ACT_COUNT
};

struct ActDefinition {
  uint16_t durationMs;
  DemoScene scene;
  uint8_t ringHue;
  uint8_t titleHue;
};

// 28 + 37 + 37 + 38 + 37 + 38 + 47 + 38 seconds = exactly five minutes.
const ActDefinition ACTS[ACT_COUNT] = {
  {28000, SCENE_GRID,        150, 150},
  {37000, SCENE_TUNNEL,      180, 190},
  {37000, SCENE_VECTOR_CITY,  82,  82},
  {38000, SCENE_VECTOR_CITY, 205,  28},
  {37000, SCENE_GRID,          8,   8},
  {38000, SCENE_STARFIELD,   155, 155},
  {47000, SCENE_PLASMA,      224, 224},
  {38000, SCENE_TUNNEL,      150, 150}
};

// -----------------------------------------------------------------------------
// Virtual camera: one physical cube becomes a moving 5x5x5 viewport through a
// larger 96x64x48 implicit world. All transforms are integer/fixed-point.
// -----------------------------------------------------------------------------

void cameraForScene(DemoScene scene, uint16_t age, int32_t &camX, int32_t &camY, int32_t &camZ) {
  const uint16_t beat = age >> 4;
  switch (scene) {
    case SCENE_TUNNEL:
      camX = 48 * FP_ONE + (int16_t(sin8(beat * 3)) - 128) * 10;
      camY = 32 * FP_ONE + (int16_t(sin8(beat * 2 + 71)) - 128) * 8;
      camZ = int32_t(age) * 16;
      break;
    case SCENE_GRID:
      camX = int32_t(age) * 12;
      camY = 18 * FP_ONE + (int16_t(sin8(beat * 2)) - 128) * 7;
      camZ = 14 * FP_ONE + (int16_t(sin8(beat * 4 + 53)) - 128) * 5;
      break;
    case SCENE_PLASMA:
      camX = 32 * FP_ONE + (int16_t(sin8(beat * 2 + 31)) - 128) * 12;
      camY = 28 * FP_ONE + (int16_t(sin8(beat * 3 + 137)) - 128) * 10;
      camZ = 20 * FP_ONE + (int16_t(sin8(beat * 5 + 219)) - 128) * 7;
      break;
    case SCENE_VECTOR_CITY:
      camX = int32_t(age) * 9;
      camY = 8 * FP_ONE;
      camZ = 6 * FP_ONE + (int16_t(sin8(beat * 3)) - 128) * 4;
      break;
    default: // SCENE_STARFIELD
      camX = 40 * FP_ONE + (int16_t(sin8(beat * 5)) - 128) * 8;
      camY = 24 * FP_ONE + (int16_t(sin8(beat * 2 + 93)) - 128) * 8;
      camZ = int32_t(age) * 22;
      break;
  }
}

StoryAct storyActFor(uint32_t elapsed, uint16_t &age, uint8_t &progress) {
  const uint32_t storyTime = elapsed % STORY_DURATION_MS;
  uint32_t cursor = 0;
  for (uint8_t i = 0; i < ACT_COUNT; ++i) {
    const uint32_t end = cursor + ACTS[i].durationMs;
    if (storyTime < end) {
      age = uint16_t(storyTime - cursor);
      progress = uint8_t((uint32_t(age) * 255U) / ACTS[i].durationMs);
      return StoryAct(i);
    }
    cursor = end;
  }
  age = 0;
  progress = 0;
  return ACT_BOOT_SIGNAL;
}

void cameraForAct(StoryAct act, uint16_t age, int32_t &camX, int32_t &camY, int32_t &camZ) {
  cameraForScene(ACTS[act].scene, age, camX, camY, camZ);
  const uint16_t beat = age >> 4;
  switch (act) {
    case ACT_BOOT_SIGNAL:
      camZ = 6 * FP_ONE + int32_t(age) * 7;
      break;
    case ACT_WALKER:
      camX = -18 * FP_ONE + int32_t(age) * 10;
      camY = 3 * FP_ONE;
      camZ = 2 * FP_ONE;
      break;
    case ACT_MACHINE_CITY:
      camX = int32_t(age) * 8;
      camY = 10 * FP_ONE;
      camZ = 8 * FP_ONE + (int16_t(sin8(beat * 2)) - 128) * 5;
      break;
    case ACT_FRACTURE:
      camX = (int16_t(sin8(beat * 5)) - 128) * 16;
      camY = (int16_t(sin8(beat * 3 + 51)) - 128) * 13;
      camZ = int32_t(age) * 18;
      break;
    case ACT_DEEP_SPACE:
      camX = 2 * FP_ONE + (int16_t(sin8(beat)) - 128) * 4;
      camY = 24 * FP_ONE;
      camZ = 2 * FP_ONE + (int16_t(sin8(beat * 2)) - 128) * 3;
      break;
    case ACT_RECONSTRUCTION:
      camX = (int16_t(sin8(beat * 3)) - 128) * 9;
      camY = 14 * FP_ONE + (int16_t(sin8(beat * 2 + 80)) - 128) * 7;
      camZ = (int16_t(sin8(beat * 4)) - 128) * 7;
      break;
    case ACT_FINAL_ASCENT:
      camX = 48 * FP_ONE + (int16_t(sin8(beat * 4)) - 128) * 11;
      camY = 32 * FP_ONE;
      camZ = int32_t(age) * 26;
      break;
    default:
      break;
  }
}

// -----------------------------------------------------------------------------
// Five low-cost implicit scenes. Each call returns one visible voxel only.
// -----------------------------------------------------------------------------

CRGB sampleTunnel(int16_t x, int16_t y, int16_t z, uint8_t time) {
  const int16_t dx = x - 48;
  const int16_t dy = y - 32;
  const uint16_t taxiRadius = abs(dx) + abs(dy);
  const uint8_t tunnelBand = uint8_t((taxiRadius + z + time) & 15);
  const uint8_t spoke = uint8_t((abs(dx - dy) + z * 2 + time) & 31);
  if (tunnelBand < 2 || spoke == 0) {
    const uint8_t value = tunnelBand == 0 ? 255 : 125;
    return CHSV(145 + (z >> 2), 240, value);
  }
  return CRGB::Black;
}

CRGB sampleGrid(int16_t x, int16_t y, int16_t z, uint8_t time) {
  const bool xLine = ((x + time) & 7) == 0;
  const bool yLine = ((y + time / 2) & 7) == 0;
  const bool zLine = ((z + time / 3) & 7) == 0;
  const uint8_t lines = xLine + yLine + zLine;
  if (!lines) return CRGB::Black;
  return CHSV(170 + (time >> 3), lines >= 2 ? 90 : 210, lines >= 2 ? 255 : 115);
}

CRGB samplePlasma(int16_t x, int16_t y, int16_t z, uint8_t time) {
  // FastLED's sin8 is a lookup table. No floating-point trigonometry appears
  // in this per-voxel path.
  const uint8_t wave = sin8(virtualPhase(x, 9, time))
                     + sin8(virtualPhase(y, 13, time >> 1))
                     + sin8(virtualPhase(z, 21, time >> 2));
  const uint8_t hue = wave + (time >> 2);
  const uint8_t value = qadd8(38, scale8(sin8(wave + time), 217));
  return CHSV(hue, 245, value);
}

CRGB sampleVectorCity(int16_t x, int16_t y, int16_t z, uint8_t time) {
  const uint8_t building = hashWorld(x >> 2, y >> 2, 0, 19) & 15;
  const uint8_t height = 4 + building;
  const int16_t localZ = z & 31;
  if (localZ > height) return CRGB::Black;
  const bool edge = ((x & 3) == 0) || ((y & 3) == 0);
  const bool windows = ((x + y + z + time / 2) & 7) == 0;
  if (edge || windows) return CHSV(31 + building * 3, 210, windows ? 255 : 100);
  return CRGB::Black;
}

CRGB sampleStarfield(int16_t x, int16_t y, int16_t z, uint8_t time) {
  const uint8_t h = hashWorld(x + (time >> 2), y, z, 71);
  if (h < 13) return CHSV(150 + (h << 3), 80, 255);
  if (h < 23) return CHSV(205, 180, 110);
  return CRGB::Black;
}

CRGB storyPalette(uint8_t index, uint8_t value) {
  const uint8_t hues[] = {0, 150, 190, 224, 28, 90, 0, 170};
  const uint8_t sats[] = {0, 235, 220, 200, 235, 240, 0, 160};
  return CHSV(hues[index & 7], sats[index & 7], value);
}

CRGB sampleStoryLandmarks(StoryAct act, uint16_t age, int16_t x, int16_t y, int16_t z, uint8_t time, CRGB underlay) {
  for (uint8_t i = 0; i < STORY_LANDMARK_COUNT; ++i) {
    StoryVoxel voxel;
    memcpy_P(&voxel, &STORY_LANDMARKS[i], sizeof(voxel));
    if (voxel.x == x && voxel.y == y && voxel.z == z) {
      const uint8_t pulse = qadd8(90, scale8(sin8(time * 3 + i * 11), 165));
      return storyPalette(voxel.palette, pulse);
    }
  }

  if (act == ACT_WALKER) {
    const int8_t walkerX = int8_t((age / 650U) % 24U) - 12;
    const bool leftStep = ((age / 220U) & 1) == 0;
    for (uint8_t i = 0; i < STORY_WALKER_COUNT; ++i) {
      StoryVoxel voxel;
      memcpy_P(&voxel, &STORY_WALKER[i], sizeof(voxel));
      int8_t wx = voxel.x + walkerX;
      int8_t wz = voxel.z;
      if (i >= 9) wx += (leftStep == (i & 1)) ? -1 : 1;
      if (wx == x && 3 == y && wz == z) return storyPalette(voxel.palette, 255);
    }
  }
  return underlay;
}

CRGB sampleVirtualWorld(DemoScene scene, int16_t x, int16_t y, int16_t z, uint8_t time) {
  switch (scene) {
    case SCENE_TUNNEL:      return sampleTunnel(x, y, z, time);
    case SCENE_GRID:        return sampleGrid(x, y, z, time);
    case SCENE_PLASMA:      return samplePlasma(x, y, z, time);
    case SCENE_VECTOR_CITY: return sampleVectorCity(x, y, z, time);
    default:                return sampleStarfield(x, y, z, time);
  }
}

// -----------------------------------------------------------------------------
// Rear ring: acts like the old CRT overscan/border around the cube viewport.
// -----------------------------------------------------------------------------

void renderRing(StoryAct act, uint8_t time, uint8_t progress) {
  const uint8_t hue = ACTS[act].ringHue;
  for (uint8_t i = 0; i < RING_LEDS; ++i) {
    const uint8_t travel = sin8(time * 3 + uint8_t(i * 255U / RING_LEDS));
    uint8_t localHue = hue + i * 3;
    uint8_t value = scale8(RING_BRIGHTNESS, qadd8(40, scale8(travel, 215)));
    if (act == ACT_MACHINE_CITY && ((i + time / 16) % 3 == 0)) {
      localHue = 200;
      value = RING_BRIGHTNESS;
    }
    if (act == ACT_FRACTURE && ((i + progress / 16) & 1)) {
      localHue = 8;
      value = qadd8(value, 45);
    }
    if (act == ACT_DEEP_SPACE) value = scale8(value, 90);
    if (act == ACT_FINAL_ASCENT && progress > 220) value = 255;
    leds[MATRIX_LEDS + i] = CHSV(localHue, act == ACT_DEEP_SPACE ? 110 : 230, value);
  }
}

void renderActTitleSignal(StoryAct act, uint16_t age) {
  // A small sync/title glyph marks each act without pretending a 5×5×5 cube
  // can carry readable prose at normal viewing distance.
  if (age > 2500) return;
  const uint8_t phase = (age / 250) % 5;
  for (uint8_t x = 0; x < 5; ++x) {
    leds[ledIndex(x, 2, phase)] = CHSV(ACTS[act].titleHue, 220, 255);
  }
}

// -----------------------------------------------------------------------------
// Frame renderer and non-blocking 24 fps scheduler
// -----------------------------------------------------------------------------

void renderFrame(uint32_t now) {
  const uint32_t renderStarted = micros();
  uint16_t sceneAge;
  uint8_t actProgress;
  const StoryAct act = storyActFor(now, sceneAge, actProgress);
  const DemoScene scene = ACTS[act].scene;
  const uint8_t time = uint8_t(now >> 3);

  int32_t cameraX, cameraY, cameraZ;
  cameraForAct(act, sceneAge, cameraX, cameraY, cameraZ);

  // This is the key hacktro viewport rule: exactly 125 world samples. The
  // virtual world can be enormous because only the physical view is rendered.
  for (uint8_t z = 0; z < CUBE_Z; ++z) {
    for (uint8_t y = 0; y < CUBE_Y; ++y) {
      for (uint8_t x = 0; x < CUBE_X; ++x) {
        const int16_t worldX = int16_t((cameraX + int32_t(x) * FP_ONE) >> 8);
        const int16_t worldY = int16_t((cameraY + int32_t(y) * FP_ONE) >> 8);
        const int16_t worldZ = int16_t((cameraZ + int32_t(z) * FP_ONE) >> 8);
        const CRGB procedural = sampleVirtualWorld(scene, worldX, worldY, worldZ, time);
        leds[ledIndex(x, y, z)] = sampleStoryLandmarks(act, sceneAge, worldX, worldY, worldZ, time, procedural);
      }
    }
  }

  renderActTitleSignal(act, sceneAge);
  renderRing(act, time, actProgress);
  const uint32_t renderMicros = micros() - renderStarted;
  if (renderMicros > maxRenderMicros) maxRenderMicros = renderMicros;

  // Optional Serial telemetry is throttled to once each second. It measures
  // CPU-side rendering only; FastLED.show() remains outside this measurement.
  if (now - telemetryAt >= 1000) {
    telemetryAt = now;
    Serial.printf("HACKTRO act=%u scene=%u max renderer=%lu us, contract=%u fps\n",
                  uint8_t(act), uint8_t(scene), (unsigned long)maxRenderMicros, TARGET_FPS);
    maxRenderMicros = 0;
  }
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, TOTAL_LEDS);
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  FastLED.clear(true);
  nextFrameAt = millis();
}

void loop() {
  const uint32_t now = millis();
  if (int32_t(now - nextFrameAt) < 0) return;

  // Resynchronise after a rare long pause rather than attempting a burst of
  // stale frames. This keeps animation time moving forward and refresh smooth.
  if (now - nextFrameAt > FRAME_INTERVAL_MS * 2U) nextFrameAt = now;
  nextFrameAt += FRAME_INTERVAL_MS;

  renderFrame(now);
  FastLED.show();
}
