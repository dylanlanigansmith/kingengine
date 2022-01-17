//TO FIX RESOURCE PATHS: xattr -cr kingengine.app
#include "includes.hpp"
#include "defines.hpp"

#include "GameEngine.hpp"
#include "AssetManager.hpp"


// Here is a small helper for you! Have a look.






/*
//MAIN FUNCTION WHEN .APP CLICKED
// \brief: hell yeah brother, lets giver
 
 so the engine should look like this:
 ->main()-> checks base stuff, loads some files maybe, starts engine class, calls engine->run, waits for it to quit
 engine -> base window, render, optimizing, debug, clocks, MODES
 world->tiling, render of entitys, big vector of em,
 player -> special class, should inherit base of something idk
 drawable-> inherit sfml class duh, then
            -> worldobject: streetlights, trees, houses with triggers, stuff that doesnt move basically
            -> entity: anything that updates
            
 https://github.com/SFML/SFML/wiki/Tutorial%3A-Basic-Game-Engine
^some good ideas that u were on the right track of before 
*/
//#define OLDMAIN
#ifndef OLDMAIN
//https://bromeon.ch/articles/raii.html
extern GameEngine* engine;

int main(int, char const**)
{
    std::cout << "Starting Engine Instance: " << ENGINE_NAME << " " << ENGINE_VERSION << std::endl;
    
    
    engine = new GameEngine();
    engine->setup();
    engine->loop();
    if(engine->shouldQuit())
        return EXIT_SUCCESS;
    
    return EXIT_FAILURE;
    
    
}



#endif
#ifdef OLDMAIN
#include "Player.hpp"
#include "World.hpp"
#include "Tile.hpp"
//someday this will be run from an app and a bunch of data, that just imports the engine!

   // sunBlend.colorEquation = sf::BlendMode::Add;
    
    
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
    

   
#endif
