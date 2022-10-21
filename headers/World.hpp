#pragma once
#include <Audio/Clip.hpp>

#include "Cursor.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Utilities/Types.hpp"
#include "Utils.hpp"

#include "IsoTileMap.hpp"
#include "Utilities/Singleton.hpp"
#include "common.hpp"
#include "fwd.hpp"
#include <Stardust-Celeste.hpp>
#include <cstddef>
#include <vector>

#ifdef PSP
const std::string PLATFORM_PREFIX = ".bgm";
#else
const std::string PLATFORM_PREFIX = ".ogg";
#endif

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Utilities::Input;

class World : public Singleton {

public:
  static World &instance() {
    static World INSTANCE;
    return INSTANCE;
  }

  auto playSound() -> void { music->play(); }

  void drawHud() {
    hud_cursor->draw();
    hud_cross->draw();
    hud_square->draw();
    hud_triangle->draw();
    hud_circle->draw();
  }

  void newLevel() { tilemap->randomMap(); };
  auto getCursor() { return cursor; }
  void generateMap() {

    tilemap = create_scopeptr<ISOTileMap>(BASIC_TILE, glm::vec2(1, 1));

    tilemap->generate_tiles();

    tilemap->generate_map();

    cursor =
        create_refptr<Cursor>(CURSOR, glm::vec2(MAP_SIZE / 2, MAP_SIZE / 2));

    hud_cursor = create_scopeptr<Sprite>(
        EntityManager::instance().hud_cursor,
        Rendering::Rectangle{glm::vec2{25, 180}, glm::vec2{100, 20}});
    hud_cross = create_scopeptr<Sprite>(
        EntityManager::instance().hud_cross,
        Rendering::Rectangle{glm::vec2{25, 220}, glm::vec2{100, 20}});

    hud_square = create_scopeptr<Sprite>(
        EntityManager::instance().hud_square,
        Rendering::Rectangle{glm::vec2{360, 200}, glm::vec2{100, 20}});

    hud_triangle = create_scopeptr<Sprite>(
        EntityManager::instance().hud_triangle,
        Rendering::Rectangle{glm::vec2{360, 220}, glm::vec2{100, 20}});

    hud_circle = create_scopeptr<Sprite>(
        EntityManager::instance().hud_circle,
        Rendering::Rectangle{glm::vec2{360, 240}, glm::vec2{100, 20}});

    if (!music) {
      music =
          create_scopeptr<Audio::Clip>("assets/music" + PLATFORM_PREFIX, true);
    }
  }

  // son constantes

  void update(double dt); // movemos controles si es necesario
  void draw() {
    drawHud();
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

      if (tile->entity && (tile->entity->props.role == RoleEntity::Hero ||
                           tile->entity->props.role == RoleEntity::Building)

      ) {
        list.push_back(tile->entity);
      }
    }

    return list;
  }

  std::vector<RefPtr<Entity>> getHouses() {

    std::vector<RefPtr<Entity>> list = {};
    auto tiles = tilemap->get_tile_map();
    for (int i = 0; i < tiles.size(); i++) {
      auto tile = tiles.at(i);

      if (tile->entity && (tile->entity->props.role == RoleEntity::Building)

      ) {
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
    auto objetives = getHeroEnt();
    auto tiles = tilemap->get_tile_map();

    if (objetives.size() == 0) {
      return;
    }

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
        RefPtr<Entity> hero;

        if (enemy->getObjetive()) {
          if (!enemy->getObjetive()->isLife()) {
            enemy->setObjetive(objetives[i % objetives.size()]);
          }

        } else {

          // if no objetive, enemi won, skip all this code;

          enemy->setObjetive(objetives[i % objetives.size()]);
        }

        hero = enemy->getObjetive();

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
        // -1 -1
        // hotifx no path
        if (path.size() == 0) {
          continue;
        }
        enemy->setCoords(path.at(pathSize));

        // pintamos las tiles que hemos recorrido para mejor feedback con el
        // usuario
        for (int w = 0; w < pathSize; w++) {
          World::instance().getTile(path.at(w))->color = {125, 125, 125, 255};
          World::instance().flushTileMap();
        }

        // if distance between two is less than one tile, then no attack
        if (pathSize == 1 || pathSize == 0) { // is this correct?
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

  ScopePtr<Sprite> hud_cursor;
  ScopePtr<Sprite> hud_cross;

  ScopePtr<Sprite> hud_square;
  ScopePtr<Sprite> hud_triangle;
  ScopePtr<Sprite> hud_circle;

  ScopePtr<Audio::Clip> music = nullptr;
};