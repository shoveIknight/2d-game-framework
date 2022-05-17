#include "Renderer.h"

Renderer::Renderer()
{
	gameWindow = NULL;
	gameSurface = NULL;
	gameRenderer = NULL;
	gameTexture = NULL;
}
Renderer::~Renderer()
{
	//delete gameWindow;
	//delete gameSurface;
}
bool Renderer::Init()
{
	bool succsess = true;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "error" << SDL_GetError() << std::endl;
		succsess = false;
	}

	else
	{
		gameWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGTH, SDL_WINDOW_RESIZABLE );

	}
	//SDL_GL_SetSwapInterval(1);
	if (gameWindow == NULL)
	{
		std::cout << "error" << SDL_GetError() << std::endl;
		succsess = false;
	}
	else
	{
		gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED );
		gameSurface = SDL_GetWindowSurface(gameWindow);
		SDL_FillRect(gameSurface, NULL, SDL_MapRGB(gameSurface->format, 0, 0, 0));
	}
	if (gameRenderer == NULL)
	{
		std::cout << "error" << SDL_GetError() << std::endl;
		succsess = false;
	}
	else
	{
		SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			std::cout << "error" << SDL_GetError() << std::endl;
			succsess = false;
		}
	}
	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
	return succsess;
}
SDL_Window* Renderer::getWindow()
{
	return gameWindow;
}
void Renderer::drawRect(Rectangle rect) 
{
	SDL_FRect tmpRect = { rect.x, rect.y, rect.w, rect.h };
	SDL_RenderFillRectF(gameRenderer, &tmpRect);
}
void Renderer::clear()
{

	//SDL_RenderCopy(gameRenderer, gameTexture, NULL, NULL);
	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gameRenderer);
}
void Renderer::gameUpdate() {
	SDL_UpdateWindowSurface(gameWindow);
}

int Renderer::getScreenHeight() {
	gameSurface = SDL_GetWindowSurface(gameWindow);
	return gameSurface->h;
}

int Renderer::getScreenWidth() {
	gameSurface = SDL_GetWindowSurface(gameWindow);
	return gameSurface->w;
}

void Renderer::quit()
{
	SDL_DestroyTexture(gameTexture);
	gameTexture = NULL;
	//SDL_FreeSurface(gameSurface);
	SDL_DestroyWindow(gameWindow);
	gameWindow = NULL;
    gameSurface = NULL;
	IMG_Quit();
	SDL_Quit();
	SDL_DestroyRenderer(gameRenderer);
	gameRenderer = NULL;

}

void Renderer::loadSurface(std::string path)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

	if (loadedSurface == NULL)
	{
		std::cout << "error";

	}
	else
	{
		gameSurface = SDL_ConvertSurface(loadedSurface, gameSurface->format, 0);
		if (gameSurface == NULL)
		{
			std::cout << "error";
		}
		SDL_FreeSurface(loadedSurface);
	}
}

void Renderer::loadTexture(std::string path)
{

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "error";
	}
	else
	{
		//Create texture from surface pixels
		gameTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
		if (gameTexture == NULL)
		{
			std::cout << "error";
		}
		SDL_FreeSurface(loadedSurface);
	}
}
void Renderer::updateRender()
{
	SDL_RenderPresent(gameRenderer);
}

SDL_Renderer* Renderer::getRenderer()
{
	return gameRenderer;
}

void Renderer::setTexture() {
	gameTexture = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
	//SDL_SetRenderTarget(gameRenderer, gameTexture);
}

void Renderer::setTarget()
{
	SDL_SetRenderTarget(gameRenderer, gameTexture);
}

void Renderer::delay(int n)
{
	SDL_Delay(n);
}

void Renderer::setDrawColor(Uint8 R, Uint8 G, Uint8 B, Uint8 A)
{
	SDL_SetRenderDrawColor(gameRenderer, R, G, B, A);
}
