# Epic Technical-Demo Brief

> The present sketch proves that a 5×5×5 physical window can traverse an implicit world. The serious revision must prove that the ESP32-S3 can carry an **authored world** with enough persistent structure to make the window feel like a view into somewhere else.

## What changes

| Proof-of-architecture behaviour | Required technical-demo behaviour |
|---|---|
| Five simple implicit samplers | Multiple persistent packed world chunks plus atmospheric layers |
| Dozens of hand-authored points | Tens of thousands of compact, palette-indexed imported or authored voxel records stored in flash |
| A short story with scene labels | A five-minute route with geography, landmarks, returns, discoveries, character beats, and a real finale |
| A small walker cue | Sparse animated silhouettes that persist in world space while the viewport crosses them |
| Palette movement alone | Camera intention, depth, horizon, parallax, environment, and rear-ring state all describe the same place |

## Non-negotiable targets

| Metric | First substantial target |
|---|---:|
| Physical output | 125 matrix voxels plus 12 rear-ring pixels at **24 fps** |
| CPU world samples | Remain bounded at **125** visible output samples per frame |
| Static authored world data | At least **50,000 occupied palette-indexed records** or an equivalently dense compressed chunk set |
| Imported / converted assets | At least **three** individually licensed, manifest-recorded source assets or original authored equivalents |
| Character data | At least **three** readable animation poses per featured silhouette |
| Atmospheric scene layers | At least far/mid/near depth layers in environment acts |
| Flash usage | Aim for **1.5–2.5 MB** of the available no-OTA factory application area, reserving release and creator headroom |

## Four-megabyte discipline

The point is not to pad flash. World data must earn its space through readable geography, visual continuity, reusable landmarks, or animation. Palette-indexed points, chunk tables, coordinate deltas, run lengths, and repeated-instance transforms are preferred over raw full-colour frames.

## Validation ladder

The first serious milestone is an imported/converted world chunk that can be crossed at 24 fps without a render-time spike. The next is a full environment act with far atmosphere, a near silhouette, and a ring state that agrees with the scene. Only after those prove stable should the full five-minute route become larger.
