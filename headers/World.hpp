#pragma once

#include "IsoTileMap.hpp"
#include <Stardust-Celeste.hpp>
#include <vector>
#include "Cursor.hpp"
#include "Utilities/Singleton.hpp"

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Utilities::Input;


class World : public Singleton{

public:

    static World& instance()
    {
        static World INSTANCE;
        return INSTANCE;
    }

    auto getCursor(){
        return cursor; 
    }
    void generateMap(){

        tilemap = create_scopeptr<ISOTileMap>(BASIC_TILE, glm::vec2(1,1) );

        tilemap->generate_tiles();

        tilemap->generate_map();

        cursor = create_refptr<Cursor>(CURSOR,glm::vec2(1,1) ) ;
    }

   
    //son constantes

    void update(double dt); // movemos controles si es necesario
    void draw(){

        cursor->draw();

        if (tilemap.get() != nullptr)
            tilemap->draw();

        auto tiles = tilemap->get_tile_map();
        for( int i = 0; i < tiles.size() ; i++){
            // pls kill me
            auto tile =  tiles.at(i);

            if(tile->entity){
                tile->entity->draw();
            }
       }


    }; // pintamos tiles, cursor y entidades

    auto getEntity(int idx){
        return tilemap.get()->tileMap.at(idx)->entity;
    }

private:
    World(){};
    ScopePtr<ISOTileMap> tilemap;
    RefPtr<Cursor> cursor;


};