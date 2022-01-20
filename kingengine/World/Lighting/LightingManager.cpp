//
//  LightingManager.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-17.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//
#include "GameEngine.hpp"
#include "LightingManager.hpp"
LightingManager::LightingManager(uint32_t w, uint32_t h) :  map_width(w), map_height(h) {
    lights.clear();
    addLightSource(LightSource());
    sf::Rect<float> fr(sf::Vector2f(200,64), sf::Vector2f(128,128));
    
   // edges.push_back(fr);
    if(!lightmap.create(map_width,map_height, sf::ContextSettings()) )
        return;
    lightmap_sprite.setTexture(lightmap.getTexture());
    shadow_radius = 60.f;
    shadow_falloff = 0.2f;
    shadow_intensity = 1.f;
}
LightingManager::~LightingManager(){
    lights.clear();
    std::vector<LightSource*>().swap(lights);
   
}
void LightingManager::clearEdges(){
    edges.clear();
   
}
void LightingManager::clearLights(){
   
    lights.clear();
    std::vector<LightSource*>().swap(lights);
}
void  LightingManager::addLightSource(const LightSource& toAdd){
    lights.push_back(new LightSource(toAdd));
   // lights.back()->
}
void LightingManager::addEdges(sf::Rect<float> rect){
    edges.push_back(rect); 
}
void LightingManager::castRay(const LightSource& light, sf::RenderTexture& win) const{
    using namespace sf;
    sf::Vector2f origin = light.getPosition(); //watch out for get origin later..
    sf::Vector2f viewCenter(engine->getView().getCenter());
    sf::Vector2f viewSize(engine->getView().getSize());

    sf::FloatRect currentViewRect(viewCenter - viewSize / 2.f, viewSize);
    sf::Color light_c = light.color;
    light_c.a = 255;
    //0.1 rad steps
    float falloff = light.falloff;
    float intensity = light.intensity;
    int rad = light.radius;
    
    
   
   
    
    std::vector<Vertex> circle;
    circle.push_back(Vertex(sf::Vector2f(origin), light_c) );
    
    for(float ang = 0; ang <= ( M_PI * 2.f); ang += 0.01f){
        std::vector<Vertex> ray;
        
        
        Vector2f cur_pt;
        Vector2f unit_vec(cos(ang), sin(ang));
        ray.push_back( Vertex(sf::Vector2f(origin), sf::Color::White));
        cur_pt = origin + (unit_vec);
        int itr = 0;
        bool hit = false;
        while( itr < rad && currentViewRect.contains(cur_pt) && !hit){
            for(const auto &r : edges){
                if(r.contains(cur_pt)){
                    sf::Vector2f front = cur_pt;
                    while(r.contains(cur_pt))
                        cur_pt += unit_vec;
                    //CAST SHADOW NOW
                    if(!castShadows){
                        hit = true; break;
                    }
                    sf::Vector2f back = cur_pt;
                    sf::Vector2f shadow =  back - front;
                    int length = sqrt(pow(shadow.x, 2) + pow(shadow.y,2));
                    length = (length < shadow_radius) ? shadow_radius : length;
                    
                    std::vector<Vertex> circle_s;
                    circle_s.push_back(Vertex(back, sf::Color(light_c.r, light_c. g, light_c.b, 255*shadow_falloff) ) );
                    std::vector<Vertex> ray_s;
                    for(float sang = ang - shadow_intensity; sang <= ang+shadow_intensity; sang += 0.01f){
                       
                        int shadow_itr = 0;
                        Vector2f unit_vec_s(cos(sang), sin(sang));
                        Vector2f s_pt = back - unit_vec_s; //UNKNOWN
                        ray_s.push_back( Vertex(sf::Vector2f(s_pt), sf::Color::White));
                        while(shadow_itr < length){
                            s_pt += unit_vec_s;
                            shadow_itr++;
                        }
                        ray_s.push_back( Vertex(sf::Vector2f(s_pt), sf::Color(225, 15, 15, 255*shadow_falloff)));
                        circle_s.push_back(Vertex(s_pt, sf::Color(100,100,100,200*shadow_falloff)));
                    }
                    win.draw(&circle_s[0], circle_s.size(), sf::TriangleFan, RenderStates(sf::BlendAdd)); //<-likely the issue
                    if(debug)
                        win.draw(&ray_s[0], ray_s.size(), sf::Lines);
                    hit = true; itr += 25; break;
                }
            }
            if(!hit)
                cur_pt += unit_vec;
            itr++;
        }
        sf::Color falloff_c = light_c;
        falloff_c.a = 255 - (itr * falloff) + intensity;
        circle.push_back( Vertex(cur_pt, falloff_c));
        ray.push_back( Vertex(sf::Vector2f(cur_pt), falloff_c));
        if(debug)
            win.draw(&ray[0], ray.size(), sf::Lines);
        ray.clear();
    }
    win.draw(&circle[0], circle.size(), sf::TriangleFan, RenderStates(sf::BlendAdd));
}
void LightingManager::createFixedMap(sf::RenderTexture & rt){
    
}
void LightingManager::update(){
    lightmap.clear(sf::Color::Black);
    
    for(const auto &ls : lights){
        castRay(*ls, lightmap);
        
    }
    
    
    
    lightmap.display();
    lightmap.setSmooth(true);
}
void LightingManager::save(sf::RenderTarget& target) const{
    
}
void LightingManager::draw(sf::RenderTarget& target) const{
    
    draw(target, sf::RenderStates(sf::BlendMultiply));
    
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
  
    /*
     ContextSettings(unsigned int depth = 0, unsigned int stencil = 0,
                unsigned int antialiasing = 0, unsigned int major = 1,
                unsigned int minor = 1, unsigned int attributes = Default, bool sRgb = false)
     */
   
    
    target.draw(lightmap_sprite, states);
    //The internal sf::Texture of a render-texture is always the same instance, so that it is possible to call this function once and keep a reference to the texture even after it is modified.
    

    
}
void LightingManager::setVAColor(sf::VertexArray& va, const sf::Color& color) const{
        for(size_t i = 0; i < va.getVertexCount(); i++){
            va[i].color = color;
        }
    }
