#include "Entity.hpp"
#include "Graphics/2D/Tilemap.hpp"
#include <Utilities/Types.hpp>

using namespace Stardust_Celeste;

struct ISOTile : public Graphics::G2D::Tile {
  RefPtr<Entity> entity;
};

class ISOTileMap : public Graphics::G2D::Tilemap {
public:
  ISOTileMap(u32 texture, glm::vec2 atlasSize)
      : Graphics::G2D::Tilemap(texture, atlasSize) {

    randomMap();
  };

  void randomMap();

  void generate_tiles();
  auto generate_map() -> void;
  auto getTile(glm::vec2 cord) {
    return tileMap.at(cord.x * MAP_SIZE + cord.y);
  }
  std::vector<RefPtr<ISOTile>> get_tile_map() { return tileMap; }
  auto add_tile(ISOTile tile) -> void;

  std::vector<glm::vec2> getPath(glm::vec2 from, glm::vec2 to);

public:
  std::vector<RefPtr<ISOTile>> tileMap;
};