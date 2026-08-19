# HacktroViewportDemo: Five-Minute Act Sheet

> **FEED ME , I'M POOR AND I MADE THIS FOR FREE - <https://paypal.me/Mystereon>**

This is a single five-minute volumetric story, not a playlist and not a loop. The physical 5×5×5 matrix is a moving window through an authored world, while the 12-pixel rear ring works as an evolving CRT border, warning system, sky, and final afterglow.

The storyboard assumes a fixed 24 fps schedule: **7,200 complete physical-cube frames** across exactly 300 seconds.

| Act | Time | World beat | Matrix language | Rear-ring language |
|---|---:|---|---|---|
| **I — Boot Signal** | 0:00–0:28 | The viewport wakes inside a dark signal lattice; a single beacon becomes a tunnel. | Sparse cyan lines, short white sync flashes, expanding geometry. | Blue scan line becomes a full cyan overscan border. |
| **II — The Corridor** | 0:28–1:02 | Camera accelerates through a vast archway corridor with recurring authored pylons. | Fast perspective passes, alternating cool grid and hot marker lights. | Travelling magenta/cyan edge chase. |
| **III — The Walker** | 1:02–1:38 | A tiny animated robot appears on an elevated lane, walks beneath the camera, then waves as the route climbs away. | Character silhouettes, acid-green lane lights, amber beacons. | Lime heartbeat punctuated by a gold response. |
| **IV — The Dancer** | 1:38–2:12 | The camera glides across a large abstract dancer whose two bold poses change on the dark stage. The viewport catches head, arms, torso, and legs as parts of one persistent body. | **Sparse violet/gold figure voxels only; true black negative space; no cube-filling background.** | Alternating violet and gold stage-light sweep. |
| **V — Machine City** | 2:12–2:46 | The world opens into a luminous abstract city; building faces, light bridges, and moving data lanes fill the view. | Dense amber/cyan geometry and scrolling-window patterns. | Violet city glow with blue aerial sweeps. |
| **VI — The Fracture** | 2:46–3:18 | The city destabilises: planes separate, colour channels shear, and the camera falls through broken architecture. | Red/magenta fragments, intermittent void, intersecting planes. | Fast amber warning then white dropout. |
| **VII — Deep Space** | 3:18–3:52 | Silence after the fall. A wide starfield, a distant ring-world, and slow large-form movement reset the eye. | Minimal white/blue stars and rare giant violet landmarks. | Very dim blue breathing halo. |
| **VIII — Reconstruction** | 3:52–4:28 | The camera finds fragments reassembling into a rainbow plasma entity around the original beacon. | Controlled rainbow volume, green energy tendrils, tightening orbit. | Rainbow perimeter sweep with bright centreless pulse. |
| **IX — Final Ascent** | 4:28–5:00 | The reconstructed world becomes a final high-speed tunnel, then collapses into the `MYSTEREON` signal and a calm blue afterglow. | All prior palettes resolve into cyan/gold/white; final single beacon. | Full bright edge chase, gold flash, long cyan decay. |

## Non-repetition rule

Every act uses a distinct camera curve, scene sampler, palette family, landmark arrangement, and ring state. No act is restarted within the five-minute cycle. After the final afterglow, the demo returns to the beginning only when the full five minutes have elapsed.

## Shared-data allocation

| Asset family | Flash budget target | Purpose |
|---|---:|---|
| Scene code, FastLED, scheduler | ≤ 700 KB | Fixed runtime layer. |
| Palette tables and camera keyframes | ≤ 32 KB | Eight visual languages plus transitions. |
| Packed static landmarks | 128–512 KB | Arches, pylons, city lanes, ring-world, fragments, beacons. |
| Palette-indexed character frames | ≤ 32 KB | Robot and future silhouettes. |
| Future imported / converted data | Up to 1.5 MB | Pre-voxelised CC0/permissively licensed source assets with manifest entries. |
| Safety margin | ≥ 500 KB | Tooling evolution and future scenes. |

## Authoring rules

The final world remains authored offline and stored in flash. The ESP32 only samples the points that affect the 125 physical voxels on a given frame. Character animation is a small sequence of palette-indexed keyframes with an explicit act cue; it is not skeletal animation or runtime mesh processing.
