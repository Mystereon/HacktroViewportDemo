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

// The dancer is deliberately an outline-like negative-space figure, not a
// filled voxel statue. Two broad, readable poses alternate on the stage while
// the small physical cube scans across her much larger world-space form.
const StoryVoxel STORY_DANCER_POSE_A[] PROGMEM = {
  {-1,0,18,7}, {0,0,19,7}, {1,0,18,7},
  {0,0,14,6}, {0,0,10,6}, {0,0,6,6}, {0,0,2,5},
  {-4,0,11,7}, {-7,0,14,7}, {-9,0,17,7},
  {4,0,11,7}, {7,0,8,7}, {9,0,5,7},
  {-3,0,-3,5}, {-5,0,-9,5},
  {3,0,-3,5}, {5,0,-9,5}
};

const StoryVoxel STORY_DANCER_POSE_B[] PROGMEM = {
  {-1,0,18,7}, {0,0,20,7}, {1,0,18,7},
  {0,0,14,6}, {0,0,10,6}, {0,0,6,6}, {0,0,2,5},
  {-4,0,11,7}, {-7,0,8,7}, {-10,0,6,7},
  {4,0,11,7}, {7,0,15,7}, {9,0,19,7},
  {-3,0,-3,5}, {-7,0,-8,5},
  {3,0,-3,5}, {7,0,-7,5}
};

constexpr uint8_t STORY_DANCER_POINT_COUNT = sizeof(STORY_DANCER_POSE_A) / sizeof(STORY_DANCER_POSE_A[0]);
