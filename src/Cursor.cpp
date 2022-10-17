#include "Cursor.hpp"
#include "World.hpp"

Cursor::Cursor(u32 idSprite, glm::vec2 coords) : Entity(idSprite, coords) {

  this->coord = coords;
}

void Cursor::selectEntity() {

  // If we know the size o the map (8*8) we can transform actual coords to
  // vector index

  int idx = this->coord.y + coord.x * MAP_SIZE;
  auto ent = World::instance().getEntity(idx);

  if (selectedEntity) {
    selectedEntity->unSelect();
  }

  if (ent) {
    SC_APP_INFO("Enidad seleccionada");
    ent->select();
    selectedEntity = ent;
  }
}

void Cursor::move(Direction dir) {

  switch (dir) {
  case Direction::up:
    if (coord.y - 1 >= 0)
      coord = glm::vec2(coord.x, coord.y - 1);
    break;
  case Direction::down:
    if (coord.y + 1 < MAP_SIZE)
      coord = glm::vec2(coord.x, coord.y + 1);
    break;
  case Direction::left:
    if (coord.x - 1 >= 0)
      coord = glm::vec2(coord.x - 1, coord.y);
    break;
  case Direction::right:
    if (coord.x + 1 < MAP_SIZE)
      coord = glm::vec2(coord.x + 1, coord.y);
    break;
  }

  this->applyChange();
}

void Cursor::applyChange() {

  auto pos = calculate_pixels(coord.x, coord.y);

  auto bounds = Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W, TILE_SIZE_H)};
  sprite = create_scopeptr<Graphics::G2D::Sprite>(idSprite, bounds);
  sprite->set_layer(1);
}