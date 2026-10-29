#pragma once

// World
#define MAX_LAYERS 10
#define MAX_ENTITIES 100

// Grid
// Due to GridSystem for now, the grid size must be larger than or equal to the role size
#define GRID_SIZE 64
#define GRID_ROWCNT 10
#define GRID_COLCNT 15

// Window
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Information Panel
#define INFORMATION_PANEL_HEIGHT 0

// Viewport
#define VIEWPORT_WIDTH WINDOW_WIDTH
#define VIEWPORT_HEIGHT (WINDOW_HEIGHT)

// Map
#define MAP_WIDTH (GRID_SIZE * GRID_COLCNT)
#define MAP_HEIGHT (GRID_SIZE * GRID_ROWCNT)
#define MAX_PATH_LEN 256

// Role
#define PIXEL_PER_SEC 200.0f
/**
 * ❗Note:
 *  ensure that the role size does not exceed twice the grid size
 *  for preventing items from spanning multiple grid cells.
 *  In getCandidateGridsByDisVec2(Grid.h), this is ensured by extending one more grid in all directions.
 */
#define ROLE_SIZE 60

// Monster
#define M_Beast_PIXEL_PER_SEC 150.0f
#define M_Beast_DANGER_DISTANCE 100.0f
#define M_Ghost_PIXEL_PER_SEC 175.0f
#define M_Ghost_DANGER_DISTANCE 50.0f

// Text
#define HeroTextSep 5