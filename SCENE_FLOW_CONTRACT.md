# Scene-Flow Contract

> **The camera observes a world; it does not merely animate LEDs.**

The CubeFX Clouds pattern is the visual benchmark for the Hacktro rebuild. Its effective quality comes from sampling one continuous three-dimensional field with a moving coordinate offset. At any moment the cube sees a small part of the same larger cloud mass; motion is therefore read as passage through space rather than as a collection of unrelated changes.

## Required behaviour for every Hacktro act

| Principle | Requirement |
|---|---|
| **Spatial continuity** | A voxel’s state must derive from stable world coordinates, packed world data, or a continuous atmospheric field—not a frame-local random decision. |
| **Off-screen implication** | Landmarks, weather, characters, lanes, and colour fields must enter and leave the 5×5×5 physical window consistently. |
| **Parallax** | At least two depths must move differently in an environment act: near particles/architecture, far atmosphere/horizon, or a distant landmark. |
| **Atmosphere** | Background low-frequency fog, cloud, nebula, dust, rain, or star drift should bind solid voxel objects into the same place. |
| **Camera intention** | A path must have a destination, reveal, orbit, descent, climb, or escape. Camera coordinates must not jitter merely to look busy. |
| **Ring continuity** | The rear ring describes the same world state as the matrix: sky, warning, energy, water, deep space, or final afterglow. |

## Technical implementation

The ESP32 evaluates only the 125 physical cube samples per frame. A scene combines no more than three bounded layers:

1. **Far field:** a low-frequency integer noise/gradient field with a slow world-space drift.
2. **Mid field:** packed static world chunks, horizon bands, lanes, or distant structures.
3. **Near field:** animated keyframe characters, sparse particles, or immediate scene landmarks.

The layers are sampled in world coordinates and composited with direct integer brightness rules. No giant RAM framebuffer, per-frame floating-point math, or whole-world draw pass is permitted.

## Five-minute act standard

Each act must remain recognisable if observed midway through it, yet feel as if it came from and is going somewhere. The five-minute story is allowed to become surreal; it is not allowed to become arbitrary.
