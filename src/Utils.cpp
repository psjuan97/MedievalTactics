#include "Utils.hpp"
#include "ID_CONSTANTS.hpp"

glm::vec2 calculate_pixels(int x, int y) {


  int screen_x = (x - y) * TILE_SIZE_W / 2 + OFFSET_X + 2;
  int screen_y = (x + y) * TILE_SIZE_H / 2 + OFFSET_Y;

  return glm::vec2(screen_x, screen_y);
}


Direction getOrientationFromCoords(glm::vec2 ori, glm::vec2 dest) {

  auto calc = ori - dest;

  if (calc.x > 0) {
    return Direction::left;

  } else if (calc.x < 0) {
    return Direction::right;
  }

  if (calc.y > 0) {
    return Direction::up;

  } else {
    return Direction::down;
  }
}
