//
//  WorldObject.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-12.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "WorldObject.hpp"


WorldObject::WorldObject(){
    name = "worldobjectundef";
}
WorldObject::WorldObject(sf::Vector2f obj_position, std::string obj_name, sf::RenderStates render_state){
    position = obj_position;
    name = obj_name;
    rs = render_state;
    
    
    
}
WorldObject::~WorldObject(){
    
}
    
void WorldObject::draw(sf::RenderTarget &target, sf::RenderStates states)const{
    
}
void WorldObject::draw(sf::RenderTarget &target) const{
    
}
RenderOptions::RenderTypes WorldObject::getType() const{
    return RenderOptions::WorldObject;
}

void WorldObject::save(sf::RenderTarget &target) const{
    
}

void WorldObject::update(){
    
}
