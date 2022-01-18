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

//https://bromeon.ch/articles/raii.html


int main(int, char const**)
{
    LogManager logger = LogManager();
  
    logger << "Starting Engine Instance: " << ENGINE_NAME << " " << ENGINE_VERSION << lm::endl;
    
    engine = new GameEngine();
    if(!engine->setup())
        logger.fatal_error("Game Engine Setup Failed");
    else
        logger << "Engine setup successful, Starting Loop" << lm::endl;
    engine->loop();
    if(engine->shouldQuit()){
        
        return EXIT_SUCCESS;
    }
        
   
    return EXIT_FAILURE;
    
    
}



