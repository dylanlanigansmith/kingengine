//
//  LightSource.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-12.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "LightSource.hpp"




LightSource::LightSource(){
    
}
LightSource::LightSource(const LightSource& copy) : WorldObject(copy.position, copy.name, copy.rs){
    this->setPosition(copy.getPosition());
    color = copy.color;
    intensity = copy.intensity;
    falloff = copy.falloff;
    radius = copy.radius;
    update();
}
LightSource::~LightSource(){
    
}
void LightSource::update(){
    
}
void LightSource::draw(sf::RenderTarget& target) const{
    
}

