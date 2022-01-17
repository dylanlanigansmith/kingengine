//
//  defines.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef defines_h
#define defines_h

#define ENGINE_NAME "KING-ENGINE"
#define ENGINE_VERSION "0.01"

//PLATFORM
#define MAC_ARM64 "mac_arm64"
//#define IOS
//#define MAC_INTEL
#define PLATFORM MAC_ARM64

#define WIN_X 1024
#define WIN_Y 1024
#define WIN_TITLE "Engine"
#define WINOPT_VSYNC true
#define WIN_DEFAULTFONT "pixeboy.ttf"

#define RENDER_LAYERS 3
#define RENDER_LAYERSIZE 32
#define RENDER_BASELAYER 0
#define TILEMANAGER_INDEX 0
#endif /* defines_hpp */

/* Global stuff example
#ifndef WORLDTYPES
#define WORLDTYPES
const char* WorldTypes[] =
{
    "TopView",
    "Ray Trace",
    "Test"
};
#endif
*/
