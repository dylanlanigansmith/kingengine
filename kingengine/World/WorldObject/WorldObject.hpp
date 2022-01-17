//
//  WorldObject.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-12.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef WorldObject_hpp
#define WorldObject_hpp


#include "includes.hpp"

#include "BaseRenderable.hpp"

class WorldObject : public Renderable{
protected:
    sf::Vector2f position;
    sf::Vector2i position_s;
    sf::RenderStates rs;
    std::string name;
public:
    WorldObject();
    //https://www.cprogramming.com/tutorial/initialization-lists-c++.html
    WorldObject(sf::Vector2f obj_position, std::string obj_name, sf::RenderStates render_state);
    virtual ~WorldObject();
   virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const override;
    virtual void draw(sf::RenderTarget& target) const override;
   virtual void save(sf::RenderTarget& target) const override;
    virtual void update() override;
    inline sf::Vector2f getPositon(){
        return position;}
    RenderOptions::RenderTypes getType() const override;
    
   
};

#endif /* WorldObject_hpp */
