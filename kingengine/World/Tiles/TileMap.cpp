//
//  TileMap.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-14.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "TileMap.hpp"
#include "filereadstream.h"
#include "istreamwrapper.h"

bool TileMap::load( sf::Vector2u tileSize)
{
   
    
    unsigned int width = 32;
    unsigned int height = 32;
    
    

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // populate the vertex array, with one quad per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tmi->tiles[i + j * width];
            
            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = (const sf::Texture*)asset->get();

    // draw the vertex array
    target.draw(m_vertices, states);
}

void TileMap::draw(sf::RenderTarget &target) const{
    sf::RenderStates states;
    draw(target, states);
}

void TileMap::save(sf::RenderTarget &target) const{
    
}

void TileMap::update(){
    
}


bool TileMapInformation::loadJSON(){
    using namespace rapidjson;
   
    std::ifstream read(path.c_str());
    IStreamWrapper isw(read);
    Document document;
    document.ParseStream(isw);
    read.close();
    
    assert(document.IsObject());
    auto getStringMember = [&](const char* name, const Document& doc) -> std::string {
        if(doc.HasMember(name))
            if(doc[name].IsString())
                return doc[name].GetString();
    };
    auto getIntMember = [&](const char* name, const Document& doc) -> uint32_t {
        if(doc.HasMember(name))
            if(doc[name].IsUint())
                return doc[name].GetUint();
    };
    tilemap_name = getStringMember("name", document);
    map_size_x = getIntMember("map_size_x", document);
    map_size_y = getIntMember("map_size_y", document);
    tile_size_x = getIntMember("tile_size_x", document);
    tile_size_y = getIntMember("tile_size_y", document);
    if(document.HasMember("mapping")){
        if(document["mapping"].IsObject()){
           for(auto& map : document["mapping"].GetObject()){
               name_to_position.insert(std::pair<std::string, int>(map.name.GetString(), map.value.GetInt() ) );
           }
        }
    }
    if(document.HasMember("data")){
        if(document["data"].IsArray()){
           for(auto& tile_data : document["data"].GetArray()){
               tiles.push_back(tile_data.GetInt());
           }
        }
    }
    
    return true;
}

bool TileMapInformation::createJSON(){
    using namespace rapidjson;
    Document doc;
    doc.SetObject();
    doc.AddMember("name","tilemap.png",doc.GetAllocator());
    doc.AddMember("map_size_x",256,doc.GetAllocator());
    doc.AddMember("map_size_y",32,doc.GetAllocator());
    doc.AddMember("tile_size_x",32,doc.GetAllocator());
    doc.AddMember("tile_size_y",32,doc.GetAllocator());
    doc.AddMember("max_tile_index",10,doc.GetAllocator());
    Value a(kArrayType);
                    //x*y
    for(int i = 0; i < 64*64; i++){
        a.PushBack(0, doc.GetAllocator());
       
    }
    doc.AddMember("data", a, doc.GetAllocator());
    Document map;
    map.SetObject();
    map.AddMember("grass",0,map.GetAllocator());
    map.AddMember("water",1,map.GetAllocator());
    map.AddMember("treegrass",2,map.GetAllocator());
    map.AddMember("cobble",3,map.GetAllocator());
    map.AddMember("tandungeon_floor",4,map.GetAllocator());
    map.AddMember("tandungeon_brick",5,map.GetAllocator());
    map.AddMember("tandungeon_color",6,map.GetAllocator());
    map.AddMember("tandungeon_leftcorner",7,map.GetAllocator());
    doc.AddMember("mapping", map, doc.GetAllocator());
    
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::ofstream file(path.c_str());
    file << buffer.GetString();
    file.close();
    
    std::ifstream read(path.c_str());
    IStreamWrapper isw(read);
    Document document;
    document.ParseStream(isw);
    read.close();
    
    assert(document.IsObject());
    auto getStringMember = [&](const char* name, const Document& doc) -> std::string {
        if(doc.HasMember(name))
            if(doc[name].IsString())
                return doc[name].GetString();
    };
    auto getIntMember = [&](const char* name, const Document& doc) -> uint32_t {
        if(doc.HasMember(name))
            if(doc[name].IsUint())
                return doc[name].GetUint();
    };
    tilemap_name = getStringMember("name", document);
    map_size_x = getIntMember("map_size_x", document);
    map_size_y = getIntMember("map_size_y", document);
    tile_size_x = getIntMember("tile_size_x", document);
    tile_size_y = getIntMember("tile_size_y", document);
    if(document.HasMember("mapping")){
        if(document["mapping"].IsObject()){
           for(auto& map : document["mapping"].GetObject()){
               name_to_position.insert(std::pair<std::string, int>(map.name.GetString(), map.value.GetInt() ) );
           }
        }
    }
    if(document.HasMember("data")){
        if(document["data"].IsArray()){
           for(auto& tile_data : document["data"].GetArray()){
               tiles.push_back(tile_data.GetInt());
           }
        }
    }
    
 
    return true;
}
bool TileMapInformation::saveJSON(){
    using namespace rapidjson;
    Document doc;
    doc.SetObject();
    doc.AddMember("name","tilemap.png",doc.GetAllocator());
    doc.AddMember("map_size_x",map_size_x,doc.GetAllocator());
    doc.AddMember("map_size_y",map_size_y,doc.GetAllocator());
    doc.AddMember("tile_size_x",tile_size_x,doc.GetAllocator());
    doc.AddMember("tile_size_y",tile_size_y,doc.GetAllocator());
    doc.AddMember("max_tile_index",max_tile_index,doc.GetAllocator());
    Value a(kArrayType);
                    //x*y
    for(int i = 0; i < (tile_size_x*tile_size_y); i++){
        a.PushBack(tiles.at(i), doc.GetAllocator());
    }
    doc.AddMember("data", a, doc.GetAllocator());
    Document map;
    
    map.SetObject();
    for(auto& t : name_to_position){
        Value tt(t.first.c_str(), map.GetAllocator());
        Value ti(t.second);
        map.AddMember(tt,ti,map.GetAllocator());
    }
    doc.AddMember("mapping", map, doc.GetAllocator());
    
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::ofstream file(path.c_str());
    file << buffer.GetString();
    file.close();
    std::cout << "saved to " << path << std::endl;
    return true;
}
