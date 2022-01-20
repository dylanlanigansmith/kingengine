//
//  LightSource.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-12.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef LightSource_hpp
#define LightSource_hpp


#include "WorldObject.hpp"
class LightingManager;

class LightSource : public WorldObject {
    sf::Color color;
    float intensity;
    float falloff;
    float radius;
  
    std::vector<sf::Vertex> vertices;
    sf::PrimitiveType type;
public:
    friend class LightingManager;
    LightSource();
    LightSource(const LightSource&);
    virtual ~LightSource();
    virtual void update() override;
    virtual void draw(sf::RenderTarget& target) const override;
    void setFalloff(float f){
        falloff =f;
    }
    void setColor(sf::Color c){
        color = c;
    }
    void setIntensity(float i){
        intensity =i;
    }
    void setRadius(float r){
        radius  =r ;
    }
    
};


#endif /* LightSource_hpp */
