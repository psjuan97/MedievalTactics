#pragma once

#include "Cursor.hpp"
#include "EntityManager.hpp"
#include "Utilities/Types.hpp"
#include "Utils.hpp"

#include "IsoTileMap.hpp"
#include "Utilities/Singleton.hpp"
#include "common.hpp"
#include "fwd.hpp"
#include <Stardust-Celeste.hpp>
#include <vector>

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Utilities::Input;

class World : public Singleton {

public:
  static World &instance() {
    static World INSTANCE;
    return INSTANCE;
  }

  auto getCursor() { return cursor; }
  void generateMap() {

    tilemap = create_scopeptr<ISOTileMap>(BASIC_TILE, glm::vec2(1, 1));

    tilemap->generate_tiles();

    tilemap->generate_map();

    cursor = create_refptr<Cursor>(CURSOR, glm::vec2(1, 1));
  }

  // son constantes

  void update(double dt); // movemos controles si es necesario
  void draw() {

    cursor->draw();

    if (tilemap.get() != nullptr)
      tilemap->draw();

    auto tiles = tilemap->get_tile_map();
    for (int i = 0; i < tiles.size(); i++) {
      auto tile = tiles.at(i);

      if (tile->entity) {
        tile->entity->draw();

        if (tile->entity->getArrow()) {
          tile->entity->getArrow()->draw();
        }
      }
    }

  }; // pintamos tiles, cursor y entidades

  auto getEntity(int idx) { return tilemap.get()->tileMap.at(idx)->entity; }

  auto getTile(glm::vec2 cord) { return tilemap->getTile(cord); }

  auto flushTileMap() { tilemap->generate_map(); }

  // lot of complecity, better have a list of user entity
  std::vector<RefPtr<Entity>> getHeroEnt() {

    std::vector<RefPtr<Entity>> list = {};
    auto tiles = tilemap->get_tile_map();
    for (int i = 0; i < tiles.size(); i++) {
      auto tile = tiles.at(i);

      if (tile->entity && tile->entity->props.role == RoleEntity::Hero) {
        list.push_back(tile->entity);
      }
    }

    return list;
  }

  void showEnemyActions(RefPtr<Entity> ent, RefPtr<Entity> hero) {

    // Selecionar objetivo -> hero o house en el mapa

    auto orientation =
        getOrientationFromCoords(ent->getCoords(), hero->getCoords());

    u32 sprite = 0;

    switch (orientation) {
    case Direction::down:
      sprite = EntityManager::instance().arrow_down;
      break;

    case Direction::up:
      sprite = EntityManager::instance().arrow_up;
      break;

    case Direction::left:
      sprite = EntityManager::instance().arrow_left;
      break;

    case Direction::right:
      sprite = EntityManager::instance().arrow_right;
      break;
    }
    ent->setArrow(sprite);

    // movernos para acercarnos al objetivo

    // indicar que vamos a atacar al objetivo
  }

  void enemyTurn() {
    auto l = getHeroEnt();
    auto tiles = tilemap->get_tile_map();

    for (int i = 0; i < tiles.size(); i++) {
      auto tile = tiles.at(i);
      auto enemy = tile->entity;
      if (enemy && enemy->props.role == RoleEntity::Enemy) {
        enemy->isMoved = false;
      }
    }

    for (int i = 0; i < tiles.size(); i++) {
      auto tile = tiles.at(i);
      auto enemy = tile->entity;
      if (enemy && enemy->props.role == RoleEntity::Enemy &&
          enemy->isMoved == false) {
        enemy->isMoved = true;
        SC_APP_INFO("Enemigo en tile {}", i);
        auto hero = l[i % l.size()];
        auto e_c = enemy->getCoords();
        auto h_c = hero->getCoords();

        auto path = tilemap->getPath(e_c, h_c);

        int pathSize = path.size() - 1 <= enemy->props.moveDistance
                           ? path.size() - 1
                           : enemy->props.moveDistance;

        SC_APP_INFO("User very far, chop path to {}", pathSize);

        // vaciamos la tile origen
        World::instance().getTile(enemy->getCoords())->entity = nullptr;

        // movemos la entidad al destino
        enemy->setCoords(path.at(pathSize));

        // pintamos las tiles que hemos recorrido para mejor feedback con el
        // usuario
        for (int w = 0; w < pathSize; w++) {
          World::instance().getTile(path.at(w))->color = {125, 125, 125, 255};
          World::instance().flushTileMap();
        }

        // if distance between two is less than one tile, then no attack
        if (pathSize == 1 ||pathSize == 0 ) { // is this correct?
          showEnemyActions(enemy, hero);
          enemy->pedingAction = 1;
        }
      }
    }
    // iterate over all the enemies
    // Move the enemies to the hero and house

    // tell the world their intention
  }

  auto getEnemies() {
    std::vector<RefPtr<Entity>> l = {};
    auto tiles = tilemap->get_tile_map();

    for (int i = 0; i < tiles.size(); i++) {
      auto tile = tiles.at(i);
      auto enemy = tile->entity;
      if (enemy && enemy->props.role == RoleEntity::Enemy) {
        l.push_back(enemy);
      }
    }

    return l;
  }

private:
  World(){};
  ScopePtr<ISOTileMap> tilemap;
  RefPtr<Cursor> cursor;
};