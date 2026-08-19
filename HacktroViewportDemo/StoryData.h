#pragma once

// Flash-resident landmarks for the five-minute authored demo. An offline
// converter can append larger packed chunks using this same palette-index form.
struct StoryVoxel {
  int8_t x;
  int8_t y;
  int8_t z;
  uint8_t palette;
};

const StoryVoxel STORY_LANDMARKS[] PROGMEM = {
  // Opening signal lattice / corridor pylons.
  {-24,-12,-12,1}, {-24,-12,-4,1}, {-24,-12,4,1}, {-24,-12,12,1},
  {-12,-12,-12,1}, {-12,-12,-4,1}, {-12,-12,4,1}, {-12,-12,12,1},
  {  0,-12,-12,1}, {  0,-12,-4,2}, {  0,-12,4,1}, {  0,-12,12,1},
  { 12,-12,-12,1}, { 12,-12,-4,1}, { 12,-12,4,1}, { 12,-12,12,1},
  { 24,-12,-12,1}, { 24,-12,-4,1}, { 24,-12,4,1}, { 24,-12,12,1},

  // Machine-city bridge and elevated robot lane.
  {-18,0,-8,4}, {-12,0,-8,4}, {-6,0,-8,4}, {0,0,-8,4},
  {6,0,-8,4}, {12,0,-8,4}, {18,0,-8,4},
  {-18,3,-8,2}, {-12,3,-8,2}, {-6,3,-8,2}, {0,3,-8,2},
  {6,3,-8,2}, {12,3,-8,2}, {18,3,-8,2},

  // Ring-world fragments and final beacon.
  {-16,18,-10,7}, {-10,20,-7,7}, {-4,21,-3,7}, {4,21,3,7},
  {10,20,7,7}, {16,18,10,7},
  {0,28,-12,5}, {0,28,-6,5}, {0,28,0,6}, {0,28,6,5}, {0,28,12,5}
};

constexpr uint8_t STORY_LANDMARK_COUNT = sizeof(STORY_LANDMARKS) / sizeof(STORY_LANDMARKS[0]);

// Compact palette-indexed walker silhouette for Act III. Its alternating leg
// and arm positions are derived from the story clock; only readable body
// voxels are stored in flash.
const StoryVoxel STORY_WALKER[] PROGMEM = {
  {-1, 0,  4, 6}, {0, 0, 4, 6}, {1, 0, 4, 6},
  {-1, 0,  2, 2}, {0, 0, 2, 2}, {1, 0, 2, 2},
  {-1, 0,  0, 2}, {0, 0, 0, 3}, {1, 0, 0, 2},
  {-1, 0, -3, 5}, {1, 0, -3, 5},
  {-1, 0, -6, 5}, {1, 0, -6, 5}
};

constexpr uint8_t STORY_WALKER_COUNT = sizeof(STORY_WALKER) / sizeof(STORY_WALKER[0]);
