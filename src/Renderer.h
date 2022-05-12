#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "Math.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGTH 720
class Renderer
{
public:

	Renderer();
	~Renderer();
	bool Init();
	SDL_Window* getWindow();
	void drawRect(Rectangle rect);
	void clear();
	void gameUpdate();
	int getScreenHeight();
	int getScreenWidth();
	void quit();
	void loadSurface(std::string path);
	void loadTexture(std::string path);
	void updateRender();
	SDL_Renderer* getRenderer();
	void setTexture(); 
	void setTarget();
	void delay(int );
	void setDrawColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A);

private:
	//const int screen_width = 1280;
	//const int SCREEN_HEIGTH = 960;
	SDL_Window* gameWindow;
	SDL_Surface* gameSurface;
	SDL_Renderer* gameRenderer;
	SDL_Texture* gameTexture;
};

