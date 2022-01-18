//
//  LightingManager.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-17.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "LightingManager.hpp"
LightingManager::LightingManager(uint32_t w, uint32_t h) :  map_width(w), map_height(h) {
    lights.clear();
    addLightSource(LightSource());
}
LightingManager::~LightingManager(){
    for (auto &light : lights){
        delete light;
    }
    lights.clear();
}

void  LightingManager::addLightSource(LightSource toAdd){
    lights.push_back(new LightSource(toAdd));
   // lights.back()->
}

void LightingManager::createFixedMap(sf::RenderTexture & rt){
    
}
void LightingManager::update(){
    
}
void LightingManager::save(sf::RenderTarget& target) const{
    
}
void LightingManager::draw(sf::RenderTarget& target) const{
   // draw(target, sf::RenderStates(sf::BlendMultiply));
    
      // sunBlend.colorSrcFactor = sf::BlendMode::OneMinusSrcColor;
       //sunBlend.colorEquation = sf::BlendMode::ReverseSubtract;
       //Purple Hue:  sf::BlendMode sunBlend(sf::BlendMode::OneMinusSrcColor, sf::BlendMode::DstColor,sf::BlendMode::ReverseSubtract, sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::ReverseSubtract);
       
       /* Blue Tone
        sf::BlendMode sunBlend = sf::BlendMultiply;
        sunBlend.colorDstFactor = sf::BlendMode::DstColor;
        sunBlend.colorSrcFactor = sf::BlendMode::OneMinusSrcColor;
        sunBlend.colorEquation = sf::BlendMode::Add;
        */
       //sf::RenderStates(sf::BlendMultiply)
}
void LightingManager::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    sf::RenderTexture lightmap;
    /*
     ContextSettings(unsigned int depth = 0, unsigned int stencil = 0,
                unsigned int antialiasing = 0, unsigned int major = 1,
                unsigned int minor = 1, unsigned int attributes = Default, bool sRgb = false)
     */
    if(!lightmap.create(1024,1024, sf::ContextSettings()) )
        return;
    lightmap.clear(sf::Color(0,0,0,200));
    //https://github.com/MiguelMJ/Candle/tree/f05d2dba55d26945cc0e307e3137ca4620d9e8fd
    sf::CircleShape demo;
    demo.setPosition(sf::Vector2f(32*8,32*8));
    demo.setRadius(64);
    demo.setFillColor(sf::Color(250,220,220,150));
    demo.setOutlineThickness(0);
    sf::BlendMode sunBlend = sf::BlendMultiply;
    sunBlend.colorDstFactor = sf::BlendMode::DstColor;
    sunBlend.colorSrcFactor = sf::BlendMode::OneMinusSrcColor;
    sunBlend.colorEquation = sf::BlendMode::Add;
    int points = 100;
    float BASE_RADIUS = 40.0f;
    sf::VertexArray lightShape(sf::TriangleFan, points+2);
           float step = PI*2.f/points;
           lightShape[0].position = {BASE_RADIUS + 1, BASE_RADIUS + 1};
            lightShape[0].color.a = 1;
           for(int i = 1; i < points + 2; i++){
               lightShape[i].position = {
                   (std::sin(step*(i)) + 1) * BASE_RADIUS + 1,
                   (std::cos(step*(i)) + 1) * BASE_RADIUS + 1
               };
               
           }
  //  lightmap.clear(sf::Color::Transparent);
    lightmap.draw(lightShape, sunBlend);
   // lightmap.display();
   

   // setVAColor(lightShape, sf::Color::White);
    
    //lightmap.draw(demo, sunBlend);
    
    lightmap.display();
    lightmap.setSmooth(true);
    target.draw(sf::Sprite(lightmap.getTexture()));
    //The internal sf::Texture of a render-texture is always the same instance, so that it is possible to call this function once and keep a reference to the texture even after it is modified.
    

    
}
void LightingManager::setVAColor(sf::VertexArray& va, const sf::Color& color) const{
        for(size_t i = 0; i < va.getVertexCount(); i++){
            va[i].color = color;
        }
    }
