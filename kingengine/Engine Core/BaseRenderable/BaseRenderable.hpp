//
//  BaseRenderable.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-10.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef BaseRenderable_hpp
#define BaseRenderable_hpp

#include "includes.hpp"
#include "defines.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


namespace RenderOptions{
    enum RenderTypes {
        DevType = 0, //undeclared/test stuff
        WorldObject,
        WorldObjectManager,
        Entity,
        Player,
    };
}



//https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Drawable.php#a90d2c88bba9b035a0844eccb380ef631
/*
 virtual void sf::Drawable::draw (RenderTarget &target,
 RenderStates states)const
 (protected pure virtual) (friend class RenderTarget)
 
 Draw the object to a render target.

 This is a pure virtual function that has to be implemented by
 the derived class to define how the drawable should be drawn.

 Parameters
 target    Render target to draw to
 states    Current render states
 */


//BASE RENDERABLE
//MUST IMPLEMENT:
/*
 ~Deconstructor
 
 draw(target) 
 
 update() //called in physics / game section, deltaTime
 
 */
class Renderable : public sf::Drawable{
    //https://www.tutorialspoint.com/cplusplus/cpp_inheritance.htm
protected:
    sf::Texture texture;
    sf::Sprite sprite; //placeholder
    int index;
    RenderOptions::RenderTypes render_type;
    sf::RenderStates render_state;
public:
    Renderable();
    
    virtual ~Renderable() = default;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void draw(sf::RenderTarget& target) const = 0;
    virtual void save(sf::RenderTarget& target) const = 0;
    virtual void update();
    virtual RenderOptions::RenderTypes getType() const = 0;
    virtual void setType(RenderOptions::RenderTypes);
};

#endif /* BaseRenderable_hpp */
