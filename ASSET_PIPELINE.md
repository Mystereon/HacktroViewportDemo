# Authored Asset Pipeline

> **The ESP32 should render the world, not build it.** Mesh processing happens once on a desktop; the cube receives flash-resident palette-indexed voxels.

## Why this exists

The initial Hacktro proof deliberately used tiny procedural samples. The serious version needs persistent places: huge silhouettes, architecture, faces, landscape fragments, props, and character beats that remain coherent as the 5×5×5 camera window moves through them.

The repository therefore uses `StoryVoxel { int8_t x, y, z, palette }` as its universal authored-data atom. It is easy to inspect, stores only occupied space, and remains compatible with the raster-engine project.

| Stage | Runs on | Output |
|---|---|---|
| Acquire | Desktop | A locally saved permissively licensed OBJ, GLB, PLY, or STL asset plus its licence record |
| Voxelise | Desktop | A temporary dense `trimesh` grid, used only during conversion |
| Compact | Desktop | Sorted occupied coordinates, one palette index each |
| Emit | Desktop | A `PROGMEM` C++ header containing `StoryVoxel` records |
| Render | ESP32-S3 | Only the points encountered by the 125-voxel viewport per frame |

## Converter

Install the maintained [`trimesh`](https://github.com/mikedh/trimesh) library, then run:

```bash
python3 tools/mesh_to_storyvoxels.py asset.glb HacktroViewportDemo/ImportedDancer.h \
  --name DANCER_STAGE --palette 7 --resolution 48 \
  --source "CC0 asset title, creator, source URL"
```

The tool is intentionally conservative. It refuses overly large point clouds by default, centres coordinates into the signed 8-bit world range, and leaves licensing responsibility visible in the generated header. Keep the original source and add a row to `ASSET_MANIFEST.md` before publishing converted data.

## 4 MB no-OTA asset budget

The first target is not to fill flash frivolously; it is to reserve it for authored value.

| Allocation | Target | Role |
|---|---:|---|
| Runtime renderer, FastLED, scheduler | ≤ 700 KB | Stable code layer |
| Camera, palette, title, and transition data | ≤ 64 KB | Five-minute narrative control |
| Packed environment chunks | 1.0–1.8 MB | Architecture, landscapes, stage structure, and distant worlds |
| Characters and silhouette keyframes | 128–384 KB | Dancer, walker, creatures, and visual jokes |
| Imported converted set pieces | 0.5–1.0 MB | Selected permissive mesh-derived landmarks |
| Reserved growth margin | ≥ 250 KB | New acts, performance diagnostics, and creator evolution |

`partitions_4mb_no_ota.csv` gives a factory application almost the full 4 MB device flash. It removes OTA and filesystem partitions, so retain a copy of each release binary before flashing hardware.

## Scene-flow rule

Imported data is not a licence to fill the cube. Large assets must be written as persistent world-space forms. A camera pass should reveal a face, arm, tower, cloudbank, character, or horizon **in pieces** through black/atmosphere, so the viewer reconstructs a world beyond the cube.
