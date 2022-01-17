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


class LightSource : public WorldObject{
    sf::Color color;
    float intensity;
    
public:
    LightSource();
    LightSource(const LightSource&);
    virtual ~LightSource();
    virtual void update() override;
    virtual void draw(sf::RenderTarget& target) const override;
};

class LightingManager : public WorldObject { //debatable
    std::vector<LightSource *> lights;
    
public:
    void addLightSource(LightSource);
    void createFixedMap(sf::RenderTexture&);
    
    LightingManager();
    ~LightingManager();
    
};
#endif /* LightSource_hpp */