//
//  BaseRenderable.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-10.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "BaseRenderable.hpp"

Renderable::Renderable(){
    render_type = RenderOptions::DevType;
}




void Renderable::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    //states.texture = etc
    //draw(vertexarray, states)
    
    target.draw(sprite, states); //MODES
}
void Renderable::draw(sf::RenderTarget& target) const{
    target.draw(sprite); //MODES
}

RenderOptions::RenderTypes Renderable::getType() const{
    return render_type;
}

void Renderable::setType(RenderOptions::RenderTypes toSet){
    render_type = toSet;
}

void Renderable::update(){
    //relax, have a couple
    
}
