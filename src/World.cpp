#include "World.hpp"

void World::update(double dt) {

  // General
  music->update();

  auto tiles = tilemap->get_tile_map();
  for (int i = 0; i < tiles.size(); i++) {
    // pls kill me
    auto tile = tiles.at(i);

    if (tile->entity) {
      tile->entity->update(dt);
    }
  }
}