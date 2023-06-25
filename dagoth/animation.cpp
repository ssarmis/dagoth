#include "animation.hpp"

void Animation::Tick(r32 delta){
    // use the delta
    timer += delta;
    if( ((u32)(timer * 1000) > frameInterval)){
        ++currentTexture;
        currentTexture %= framesAmount;
        timer = 0;
    }
}