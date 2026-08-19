# Asset Manifest

> Do not add converted asset data to this repository until its source, licence, creator, conversion parameters, and intended story use are recorded here.

| Asset ID | Source and creator | Licence | Intended use | Conversion state |
|---|---|---|---|---|
| `QUATERNiUS_LOW_POLY_ROBOT` | [LowPoly Robot by Quaternius](https://quaternius.itch.io/lowpoly-robot); [public FBX/GLTF mirror on Poly Pizza](https://poly.pizza/m/QCm7qe9uNJ) | [CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/) | Large persistent robot scanned through the **Walker** act of the Hacktro authored world | **Converted.** User-provided `AnimatedRobotbyQuaternius.zip` → `OBJ/Robot.obj` → `HacktroViewportDemo/GeneratedQuaterniusRobot.h`. Surface voxelisation at 36 cells across the longest axis, palette index 4 (orange/gold), **3,270** flash-resident points. OBJ coordinates are remapped to Hacktro x/depth/height before a 25×21×37, 2,429-byte runtime occupancy cache is built once at setup. |

The source page explicitly lists 14 animations, including idle, jump, dance, punch, run, death, wave, yes, and no. When an asset is selected, record the local source filename, acquired date, source URL, output voxel resolution, palette mapping, generated header path, source commit, and which five-minute act uses it.

The CC0 status applies to this named asset as shown by the publisher. Each future asset must be checked independently; a platform catalogue or creator profile is not a substitute for the selected asset’s own licence record.
