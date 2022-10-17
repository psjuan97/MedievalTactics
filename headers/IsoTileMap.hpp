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
      : Graphics::G2D::Tilemap(texture, atlasSize){};

  void generate_tiles(); 

  std::vector<RefPtr<ISOTile>> get_tile_map() { return tileMap; }

  auto add_tile(ISOTile tile) -> void;
  auto generate_map() -> void;

public:
  std::vector<RefPtr<ISOTile>> tileMap;
};