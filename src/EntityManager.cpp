#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Graphics/2D/FontRenderer.hpp"

using namespace Stardust_Celeste;

EntityManager::EntityManager() {

  entityBlueprints = {};
  // Generar todas las entidades

  u32 tex_floor_id = Rendering::TextureManager::get().load_texture(
      "./assets/suelo.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true,
      false, true);

  u32 text_tree_id = Rendering::TextureManager::get().load_texture(
      "./assets/arbol1.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true,
      false, true);

  u32 text_tree2_id = Rendering::TextureManager::get().load_texture(
      "./assets/arbol2.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true,
      false, true);

  u32 text_cursor_id = Rendering::TextureManager::get().load_texture(
      "./assets/cursor.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  u32 text_sword1_id = Rendering::TextureManager::get().load_texture(
      "./assets/swordidledown.png", SC_TEX_FILTER_NEAREST,
      SC_TEX_FILTER_NEAREST, true);

  u32 text_sword2_id = Rendering::TextureManager::get().load_texture(
      "./assets/swordidleup.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  u32 text_orc_id = Rendering::TextureManager::get().load_texture(
      "./assets/orc_iddle_down.png", SC_TEX_FILTER_NEAREST,
      SC_TEX_FILTER_NEAREST, true);

  u32 text_cursor_offset_id = Rendering::TextureManager::get().load_texture(
      "./assets/movement_offset.png", SC_TEX_FILTER_NEAREST,
      SC_TEX_FILTER_NEAREST, true);

  u32 text_house_id = Rendering::TextureManager::get().load_texture(
      "./assets/house.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true);

  arrow_up = Rendering::TextureManager::get().load_texture(
      "./assets/arrow_up.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  arrow_down = Rendering::TextureManager::get().load_texture(
      "./assets/arrow_down.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  arrow_left = Rendering::TextureManager::get().load_texture(
      "./assets/arrow_left.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  arrow_right = Rendering::TextureManager::get().load_texture(
      "./assets/arrow_right.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  auto orc_attack = Rendering::TextureManager::get().load_texture(
      "./assets/orc_attack_down.png", SC_TEX_FILTER_NEAREST,
      SC_TEX_FILTER_NEAREST, true);

  auto hero_attack = Rendering::TextureManager::get().load_texture(
      "./assets/swordup.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  auto graveyard = Rendering::TextureManager::get().load_texture(
      "./assets/graveyard.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  life = Rendering::TextureManager::get().load_texture(
      "./assets/vidas.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);


  hud_cursor = Rendering::TextureManager::get().load_texture(
      "./assets/hud_cursor.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  hud_cross = Rendering::TextureManager::get().load_texture(
      "./assets/hud_select.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  hud_square = Rendering::TextureManager::get().load_texture(
      "./assets/hud_square.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  hud_triangle = Rendering::TextureManager::get().load_texture(
      "./assets/hud_triangle.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  hud_circle = Rendering::TextureManager::get().load_texture(
      "./assets/hud_circle.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  fontID = Rendering::TextureManager::get().load_texture(
      "./assets/default.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true, false, true);

  cursorId = text_cursor_id;
  cursorOffsetId = text_cursor_offset_id;

  entityBlueprints[EntityEnum::Graveyard] = EntityProperties{
      EntityEnum::Graveyard, RoleEntity::Misc, false, 1, 0, 0, graveyard, 0};

  entityBlueprints[EntityEnum::LightTree] =
      EntityProperties{EntityEnum::LightTree,
                       RoleEntity::Decor,
                       false,
                       1,
                       0,
                       1,
                       text_tree_id,
                       0};
  entityBlueprints[EntityEnum::DarkTree] =
      EntityProperties{EntityEnum::DarkTree,
                       RoleEntity::Decor,
                       false,
                       1,
                       0,
                       1,
                       text_tree2_id,
                       0};
  entityBlueprints[EntityEnum::Orc] =
      EntityProperties{EntityEnum::Orc, RoleEntity::Enemy, true, 3, 1, 2,
                       text_orc_id,     orc_attack};
  entityBlueprints[EntityEnum::Soldier] =
      EntityProperties{EntityEnum::Soldier, RoleEntity::Hero, true, 5, 1, 3,
                       text_sword1_id,      hero_attack};
  entityBlueprints[EntityEnum::House] = EntityProperties{EntityEnum::House,
                                                         RoleEntity::Building,
                                                         false,
                                                         1,
                                                         0,
                                                         0,
                                                         text_house_id,
                                                         0};
}

// i dont know if is necessary that a entity have the coords
RefPtr<Entity> EntityManager::createEntity(EntityEnum type, glm::vec2 vec) {
  if (type == EntityEnum::None)
    return nullptr;
  // maybe better idea to have a contructor to pass onlye de type, so the entity
  // have to acces to the blue prints
  EntityProperties props = entityBlueprints[type];
  auto entity = create_refptr<Entity>(props.textureId, vec, props.isAnimated);
  entity->props = props;

  if (props.texactionId != 0) {

    entity->setActionSprite(props.texactionId);
  }

  entity->setLife(props.life);

  return entity;
}