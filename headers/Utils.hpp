#pragma once

#include "Stardust-Celeste.hpp"

enum Direction { up, down, left, right };

glm::vec2 calculate_pixels(int x, int y);

Direction getOrientationFromCoords(glm::vec2 ori, glm::vec2 dest);