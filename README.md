# HacktroViewportDemo

> **FEED ME , I'M POOR AND I MADE THIS FOR FREE - <https://paypal.me/Mystereon>**

**HacktroViewportDemo** is a standalone ESP32-S3 sketch for a 5×5×5 WS2812B cube followed by a 12-pixel rear acrylic ring. It borrows the pacing and colour energy of classic home-computer demos, but treats the LED cube as a **moving physical viewport** into a much larger implicit three-dimensional world.

The project is credited to **Dad (MysterEon) & Manus** and is intentionally separate from the main CubeFX controller. It is an uploadable technical demo and a compact reference for performance-first volumetric rendering.

## Performance contract

The demo schedules one complete 137-pixel LED output update every **41 ms**, giving a consistent **24 fps** presentation rate. The target is smooth full-cube refresh, not an inflated benchmark number.

| Rule | Implementation |
|---|---|
| No blocking scene timing | `millis()` scheduler; there is no `delay()` call. |
| Fixed work per frame | Exactly **125** virtual-world samples, one for each physical matrix voxel. |
| No virtual framebuffer | World detail is calculated only at the current viewport coordinates. |
| No expensive per-frame trigonometry | FastLED `sin8()` lookup-table calls and integer/fixed-point coordinates only. |
| No heavy camera transform | Camera positions use Q8 fixed-point values and simple integer motion. |
| Visible performance telemetry | Serial output reports the worst CPU-side render time once per second. |

The WS2812B chain itself consumes roughly 4 ms to transmit 137 pixels, so the 24 fps scheduler leaves practical time for computation while preserving a smooth visual cadence.

## Hardware

| Connection | Meaning |
|---|---|
| ESP32-S3-Zero **GPIO6** | Matrix LED 0 DIN, through the usual resistor / level shifting arrangement. |
| Matrix LED 124 DOUT | Rear-ring DIN. |
| Matrix LEDs | 125: 5×5×5, bottom-rear-left origin, non-serpentine. |
| Rear ring | 12 additional pixels, output indices 125–136. |
| Firmware profile | ESP32S3 Dev Module, 4 MB flash, **Huge APP / no OTA**, QSPI PSRAM enabled. |

Power the LEDs from a suitable external 5 V supply and share its ground with the S3-Zero.

## Scene sequence

The simple short loop has been replaced by an authored five-minute story. It has nine unique acts, a fixed 300-second timeline, flash-resident landmark data, title signals, evolving palette language, and a final ascent. The rear ring is a deliberately separate border / overscan effect around the acrylic enclosure.

| Scene | Virtual world | Rear-ring role |
|---|---|---|
| Cyan tunnel | Moving diamond tunnel and spokes | Fast cyan scan border |
| Vector grid | Infinite gridded space | Blue-violet wireframe edge |
| Plasma volume | Palette-cycling implicit colour field | Fast chroma sweep |
| Vector city | Procedural illuminated block skyline | Amber/blue city lights |
| Starfield | Deterministic deep-space particles | Soft blue CRT-style border |
| Negative-space dancer | A large sparse two-pose figure scanned in world space | Alternating violet/gold stage-light border |

Every act follows the [`SCENE_FLOW_CONTRACT.md`](SCENE_FLOW_CONTRACT.md): the cube is a small moving window through one coherent off-screen place, rather than a container for unrelated LED effects. The dancer act is deliberately black except for the persistent sparse figure; negative space is part of its composition.

The full non-repeating story structure and its exact timing are in [`FIVE_MINUTE_ACT_SHEET.md`](FIVE_MINUTE_ACT_SHEET.md). Authored landmarks and the sparse dancer are isolated in [`HacktroViewportDemo/StoryData.h`](HacktroViewportDemo/StoryData.h).

## Real converted asset: Quaternius robot

The Walker act now scans through a **real CC0 Quaternius LowPoly Robot** supplied by the project owner. Its OBJ mesh is surface-voxelised offline at 36 cells across its longest axis, producing [`GeneratedQuaterniusRobot.h`](HacktroViewportDemo/GeneratedQuaterniusRobot.h): **3,270** palette-indexed flash voxels. At boot, those points become a 25×21×37 single-bit occupancy cache, so each of the 125 per-frame viewport samples remains one bounded bit lookup rather than a mesh or point-cloud search.

Source, licence, parameters, generated-header path, and act usage are recorded in [`ASSET_MANIFEST.md`](ASSET_MANIFEST.md). The converter itself is [`tools/mesh_to_storyvoxels.py`](tools/mesh_to_storyvoxels.py); its asset workflow is documented in [`ASSET_PIPELINE.md`](ASSET_PIPELINE.md).

With the imported robot included, the verified ESP32-S3 Huge APP/no-OTA build uses **443,287 bytes (14%)** of its 3 MB application partition and **30,180 bytes (9%)** of static RAM.

## Editing it

The five functions named `sampleTunnel`, `sampleGrid`, `samplePlasma`, `sampleVectorCity`, and `sampleStarfield` define the virtual worlds. Each receives one integer virtual coordinate and returns one `CRGB` result. Add new scenes by adding a sampler, extending `DemoScene`, and providing a camera path in `cameraForScene`.

> Keep samples cheap. Do not allocate a giant world array, call `sqrt()`, call floating-point trig, or attempt to render locations outside the 125 voxels currently visible on the physical cube.

## Upload

Open `HacktroViewportDemo/HacktroViewportDemo.ino` in Arduino IDE, install **FastLED**, select **ESP32S3 Dev Module**, choose the Huge APP/no-OTA partition profile, then upload. The serial monitor at 115200 baud prints once-per-second renderer timing information.

## License

MIT. See [`LICENSE`](LICENSE).
