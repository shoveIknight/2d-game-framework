#include "Animation.h"
#include <iostream>




Animation::Animation(const std::string& path, SDL_Renderer* const& renderer, float rightGap, float leftGap)
{
    rightHitboxAndFrameGap = rightGap;
    leftHitboxAndFrameGap = leftGap;
    char* pathChar = const_cast<char*>(path.c_str());
    frameSurface = IMG_Load(pathChar);
    frameWidth = frameSurface->w;
    frameHeight = frameSurface->h;
    frameTexture = SDL_CreateTextureFromSurface(renderer, frameSurface);
    nextAnimation = nullptr;
    //SDL_FreeSurface(surface);
}





void Animation::setFrame(const std::string& path,  SDL_Renderer* const& renderer , float rightGap, float leftGap)     
    //rightGap is the gap between hitbox's x coordinate and the texture's x coordinate you want when character face right
    //leftGap is the gap between hitbox's x coordinate and the texture's x coordinate you want when character face left
{                                                                                           
    rightHitboxAndFrameGap = rightGap;
    leftHitboxAndFrameGap = leftGap;
    char *pathChar = const_cast<char*>(path.c_str());
    frameSurface = IMG_Load(pathChar);
    frameWidth = frameSurface->w;
    frameHeight = frameSurface->h;
    frameTexture= SDL_CreateTextureFromSurface(renderer, frameSurface);
    //SDL_FreeSurface(surface);
}

void Animation::setNextAnimation(Animation* const &next)
{
    nextAnimation = next;
}

float Animation::getHitboxAndFrameGap(std::string const& facing) const
{
    return facing == "right" ? rightHitboxAndFrameGap : leftHitboxAndFrameGap;
}




float Animation::getFrameWidth() const 
{ 
    return frameWidth; 
}


float Animation::getFrameHeight() const 
{ 
    return frameHeight; 
}

Animation* Animation::getNextAnimation() const 
{
    return nextAnimation; 
}


SDL_Texture* Animation::getCurrentFrame() const 
{ 
    return frameTexture; 
}