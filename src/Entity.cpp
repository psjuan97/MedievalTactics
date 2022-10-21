#pragma once
#include "EntityManager.hpp"
#include "World.hpp"
#include <Entity.hpp>
#include <cstddef>
#include <unordered_map>

std::unordered_map<int, u32> Entity::sprite_map = {};

Entity::Entity(u32 idSprite, glm::vec2 coords, bool isAnimated) {

  auto pos = calculate_pixels(coords.x, coords.y);

  auto bounds =
      Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

  if (isAnimated) {
    sprite = create_scopeptr<AnimatedSprite>(idSprite, bounds, glm::vec2{2, 1});
  } else {
    sprite = create_scopeptr<Graphics::G2D::Sprite>(idSprite, bounds);
  }

  sprite->set_layer(1);
  this->idSprite = idSprite;
  this->coords = coords;
};

void Entity::setLife(int l) { life = l; }

void Entity::update(double dt) {

  if (pedingAction == 2) {
    // we need to count and reset pedingAction to 0

    if (actionSprite) {
      actionSprite->update(dt);

      if (actionSprite->animEnd == true) {
        actionSprite->animEnd = false;
        pedingAction = 0;
        removeArrow();
      }
    }

  } else {
    if (sprite)
      sprite->update(dt);
  }
}

void Entity::draw() {
  if (pedingAction == 2) {
    if (actionSprite)
      actionSprite->draw();
    else
      SC_APP_INFO("INVISIBLE ATTACK!!!!!!!");
  } else {

    if (sprite)
      sprite->draw();
    else
      SC_APP_INFO("INVISIBLE ENTITY");
  }
};

glm::vec2 Entity::getCoords() { return coords; }
#include <memory>

// POR CADA SEtCOORDS HAY QUE MODIDICA EL TILEMAP
void Entity::setCoords(glm::vec2 c) {
  this->coords = c;

  if (World::instance().getTile(coords)->entity != nullptr) {
    SC_APP_INFO("ENTIDAD VA A DESAPARECER!!");
    SC_APP_INFO("vec2 {} {}", coords.x, coords.y);
  }
  World::instance().getTile(coords)->entity = shared_from_this();
  auto pos = calculate_pixels(coords.x, coords.y);

  auto bounds =
      Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

  sprite->set_rect(bounds);
}

RefPtr<Graphics::G2D::Sprite> Entity::getArrow() { return arrow; }

void Entity::setArrow(u32 idSprite) {
  auto pos = calculate_pixels(coords.x, coords.y);
  auto bounds =
      Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

  arrow = create_scopeptr<Sprite>(idSprite, bounds);
  arrow->set_layer(1);
}

void Entity::attack(RefPtr<Entity> objetive) {

  SC_APP_INFO("ENTIDAD EN {}-{} ATACANDO!!", coords.x, coords.y);

  // check coord objetive, get atity and apply damage.
  // set to play animation

  // mebye here is not a good ide
  objetive->receiveDamage(this->props.attack);
  
  this->objetive = nullptr;
  this->refreshAttackSprite();
  this->pedingAction = 2;
}

void Entity::receiveDamage(int pto) {
  life = life - pto;
  if (life <= 0) {

    // entidad muerta, nos DESTRUIMOS Y PEDIMOS AL ENTITY MANAGER QUE CREE UNA
    // TUMBA EN NUESTRA POSICION.
    auto grav =
        EntityManager::instance().createEntity(EntityEnum::Graveyard, coords);
    grav->setCoords(coords);
  }
}