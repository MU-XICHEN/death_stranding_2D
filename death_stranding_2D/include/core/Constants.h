#pragma once

// Text
#define ROLE_HP_TEXT_SEP 5

// World
#define MAX_LAYERS 10
#define MAX_ENTITIES 100

// Grid
// Due to GridSystem for now, the grid size must be larger than or equal to the role size
#define GRID_SIZE 128
#define GRID_ROWCNT 10
#define GRID_COLCNT 15

// Window
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// Information Panel
#define INFORMATION_PANEL_HEIGHT 0

// Viewport
#define VIEWPORT_WIDTH WINDOW_WIDTH
#define VIEWPORT_HEIGHT (WINDOW_HEIGHT)

// Map
#define MAP_WIDTH (GRID_SIZE * GRID_COLCNT)
#define MAP_HEIGHT (GRID_SIZE * GRID_ROWCNT)
#define MAX_PATH_LEN 256

// initial States of Role
// Role
/**
 * ❗Note:
 *  ensure that the role size does not exceed twice the grid size
 *  for preventing items from spanning multiple grid cells.
 *  In getCandidateGridsByDisVec2(Grid.h), this is ensured by extending one more grid in all directions.
 */
#define ROLE_SIZE 64
// Hero
#define HERO_HP 100.0f
#define HERO_MANA 100.0f
#define HERO_PIXEL_PER_SEC 200.0f
#define HERO_DANGER_DISTANCE 100.0f
#define HERO_DEFENSE 5.0f
#define HERO_DAMAGE 10.0f

// Monster
#define M_BEAST_HP 100.0f
#define M_BEAST_PIXEL_PER_SEC 130.0f
#define M_BEAST_DANGER_DISTANCE 100.0f
#define M_BEAST_DEFENSE 5.0f
#define M_BEAST_DAMAGE 10.0f
#define M_BEAST_COLLITION_DAMAGE 5.0f

#define M_GHOST_HP 150.0f
#define M_GHOST_PIXEL_PER_SEC 150.0f
#define M_GHOST_DANGER_DISTANCE 50.0f
#define M_GHOST_DEFENSE 8.0f
#define M_GHOST_DAMAGE 20.0f
#define M_GHOST_COLLITION_DAMAGE 10.0f

// Potion
#define POTION_SIZE 30
