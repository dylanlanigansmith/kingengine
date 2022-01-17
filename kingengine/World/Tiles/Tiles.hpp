//
//  Tiles.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-10.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef Tiles_hpp
#define Tiles_hpp

#include "includes.hpp"

#include "BaseRenderable.hpp"
class TileManager;

class Tile : public Renderable{
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::IntRect texture_rect;
public:
    sf::Vector2f getPosition();
    Tile(sf::IntRect, sf::Vector2f, sf::Texture*);
    ~Tile();
    void setColor(sf::Color);
    sf::FloatRect getBounds();
    void draw(sf::RenderTarget &target) const override;
    void save(sf::RenderTarget &target) const override;
    RenderOptions::RenderTypes getType() const override;
};

class TileManager : public Renderable{
private:
    std::vector<std::vector<Tile *> > tiles;
    sf::Texture sourceTexture;
    int sizeX;
    int sizeY;
    int rectX;
    int rectY;
    int numX;
    int numY;
    bool initTiles();
public:
    TileManager(int,int,int,int, sf::Texture*);
    ~TileManager();
    void setTileColor(sf::Color, int,int); //x,y
    void draw(sf::RenderTarget &target) const override;
    void save(sf::RenderTarget &target) const override;
    RenderOptions::RenderTypes getType() const override;
    Tile* nearestTileTo(sf::Vector2f);
};

#endif /* Tiles_hpp */
