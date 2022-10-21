#include "Cursor.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "World.hpp"
#include "fwd.hpp"
#include <vector>

Cursor::Cursor(u32 idSprite, glm::vec2 coords) : Entity(idSprite, coords) {}

auto Cursor::getNearTiles(RefPtr<Entity> ent) {
  std::vector<RefPtr<ISOTile>> list = {};

  int dist = ent->props.moveDistance;

  auto coords = ent->getCoords();
  // generate only the correct cords
  for (int i = -dist; i <= dist; i++) {
    for (int j = -dist; j <= dist; j++) {

      if (coords.x + i > MAP_SIZE - 1 || coords.y + j > MAP_SIZE - 1) {
        continue;
      }

      if (coords.x + i < 0 || coords.y + j < 0) {
        continue;
      }

      auto tile = World::instance().getTile({coords.x + i, coords.y + j});

      list.push_back(tile);
    }
  }

  return list;
}

void Cursor::attackEntity() {

  if (selectedEntity) {
    auto entityCoords = selectedEntity->getCoords();
    auto cursorCoords = this->coords;

    int distance = 1;

    auto d = glm::abs(entityCoords - cursorCoords);

    bool valid = d.x <= distance && d.y <= distance;
    auto objetive = World::instance().getTile(cursorCoords)->entity;
    if (valid && objetive != nullptr) {
      SC_APP_INFO("Atacamos entidad!!!");

      World::instance().showEnemyActions(selectedEntity, objetive);

      // DESENCADENAR ATAQUE
      selectedEntity->attack(objetive);
    }
  }
}

void Cursor::moveEntity() {

  // is valid movement?
  // ent.coords, ent.distance cursor.coords
  if (selectedEntity && selectedEntity->isMoved == false) {
    selectedEntity->isMoved = true;
    auto entityCoords = selectedEntity->getCoords();
    auto cursorCoords = this->coords;

    int distance = selectedEntity->props.moveDistance;
    auto d = glm::abs(entityCoords - cursorCoords);

    bool valid = d.x <= distance && d.y <= distance;

    // are we in a valid distance and there is not entty in that tile?
    if (valid && World::instance().getTile(cursorCoords)->entity == nullptr) {

      auto tiles = getNearTiles(selectedEntity);

      for (auto tile : tiles) {
        tile->color = {255, 255, 255, 255};
      }

      World::instance().getTile(selectedEntity->getCoords())->entity = nullptr;

      selectedEntity->setCoords(cursorCoords);

      selectedEntity->unSelect();

      // necesary? i need to think
      World::instance().flushTileMap();
    }
  }
}

void Cursor::selectEntity() {

  // If we know the size o the map (8*8) we can transform actual coords to
  // vector index

  int idx = this->coords.y + coords.x * MAP_SIZE;
  auto ent = World::instance().getEntity(idx);

  if (selectedEntity) {
    auto tiles = getNearTiles(selectedEntity);

    for (auto tile : tiles) {
      tile->color = {255, 255, 255, 255};
    }

    selectedEntity->unSelect();
  }

  if (ent) {

    if (ent->props.enumType == EntityEnum::Orc) {
      return;
    }

    SC_APP_INFO("Enidad seleccionada");
    ent->select();
    selectedEntity = ent;

    auto tiles = getNearTiles(selectedEntity);

    for (auto tile : tiles) {
      tile->color = {0, 255, 255, 255};
    }
  }

  World::instance().flushTileMap();
}

void Cursor::move(Direction dir) {

  switch (dir) {
  case Direction::up:
    if (coords.y - 1 >= 0)
      coords = glm::vec2(coords.x, coords.y - 1);
    break;
  case Direction::down:
    if (coords.y + 1 < MAP_SIZE)
      coords = glm::vec2(coords.x, coords.y + 1);
    break;
  case Direction::left:
    if (coords.x - 1 >= 0)
      coords = glm::vec2(coords.x - 1, coords.y);
    break;
  case Direction::right:
    if (coords.x + 1 < MAP_SIZE)
      coords = glm::vec2(coords.x + 1, coords.y);
    break;
  }

  this->applyChange();
}

void Cursor::applyChange() {

  auto pos = calculate_pixels(coords.x, coords.y);

  auto bounds = Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W, TILE_SIZE_H)};
  sprite = create_scopeptr<Graphics::G2D::Sprite>(idSprite, bounds);
  sprite->set_layer(1);
}
