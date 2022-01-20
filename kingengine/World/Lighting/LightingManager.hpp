//
//  LightingManager.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-17.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef LightingManager_hpp
#define LightingManager_hpp

#include "BaseRenderable.hpp"
#include "LightSource.hpp"

class LightingManager : public Renderable, public sf::Transformable { //debatable
   
public:
    void addLightSource(const LightSource& );
    void createFixedMap(sf::RenderTexture&);
    virtual RenderOptions::RenderTypes getType() const{
        return RenderOptions::WorldObjectManager;
    }
    void update();
    virtual void save(sf::RenderTarget& target) const;
    virtual void draw(sf::RenderTarget& target) const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    LightingManager(uint32_t w, uint32_t h);
    ~LightingManager();
    uint8_t numberOfLights(){
        return lights.size();
    }
    void addEdges(sf::Rect<float>);
    void clearEdges();
    void clearLights();
    void setShadow(float i, float f, float r){
        shadow_falloff = f;
        shadow_intensity = i;
        shadow_radius = r;
    }
    void setDebug(bool t){
        debug = t;
    }
private:
    void castRay(const LightSource &light, sf::RenderTexture& win) const;
    const float PI = 3.1415926f;
    void setVAColor(sf::VertexArray& va, const sf::Color& color) const;
    uint32_t map_width, map_height;
    std::vector<LightSource *> lights;
    std::vector<sf::Rect<float>> edges;
    sf::RenderTexture lightmap;
    sf::Sprite lightmap_sprite;
    float shadow_falloff;
    int shadow_radius;
    float shadow_intensity;
    bool debug = false;
    bool castShadows = true;
};
#endif /* LightingManager_hpp */
