#include <SDL.h>
#include <iostream>
#include "Renderer.h"
#include "SideScroller.h"

void gameLoop()
{
	SideScroller newSideScroller;
	bool gameLoop = false,
		jumping = false,
		mouseHeldDown = false;
	SDL_Event e;
	newSideScroller.Init();
	while (!gameLoop) {

		newSideScroller.update();

		const Uint8* keyboard = SDL_GetKeyboardState(NULL);

		if (keyboard[SDL_SCANCODE_RIGHT])
		{
			newSideScroller.goRight();
		}
		else if (keyboard[SDL_SCANCODE_LEFT])
		{
			newSideScroller.goLeft();

		}
		else
		{
			newSideScroller.resetXVelocity();
		}

		if (keyboard[SDL_SCANCODE_UP])
		{
			newSideScroller.jump();
			jumping = true;
		}
		else if (jumping)
		{
			newSideScroller.setJumpConditionFalse();
			jumping = false;
		}
		if (keyboard[SDL_SCANCODE_UP])
		{
			newSideScroller.jump();
			jumping = true;
		}
		if (keyboard[SDL_SCANCODE_F])
		{
			newSideScroller.attack();
		}


		while (SDL_PollEvent(&e) != 0)
		{

			if (e.type == SDL_QUIT)
			{
				break;
				gameLoop = true;
			}

			else if (e.type == SDL_KEYDOWN)
			{

				if (e.key.keysym.sym == SDLK_DOWN)
				{
					newSideScroller.set_draw_hitboxes_true();
				}
				if (e.key.keysym.sym == SDLK_F10)
				{
					newSideScroller.pauseOrUnPause();
				}
				if (e.key.keysym.sym == SDLK_F4)
				{
					newSideScroller.editorModeToggle();
				}
				if (e.key.keysym.sym == SDLK_F1)
				{
					newSideScroller.increaseScale();
				}
				if (e.key.keysym.sym == SDLK_F2)
				{
					newSideScroller.decreaseScale();
				}
				if (e.key.keysym.sym == SDLK_F3)
				{
					newSideScroller.resetScale();
				}
				if (e.key.keysym.sym == SDLK_DELETE)
				{
					newSideScroller.deleteSelectedEntity();
				}
				if (e.key.keysym.sym == SDLK_F5)
				{
					newSideScroller.setEntityDynamic();
				}
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					gameLoop = true;
				}

			}
			if (mouseHeldDown && e.type == SDL_MOUSEMOTION)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				//std::cout << "X: " << x << "Y: " << y << std::endl;
				newSideScroller.slideEntity(x, y);
			}
			if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				//std::cout << "X: " << x << "Y: " << y << std::endl;
				newSideScroller.newStaticEntity(x, y);
				mouseHeldDown = true;
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
			{
				mouseHeldDown = false;
			}
		}
	}

	newSideScroller.Quit();

	return;
}


int main(int argc, char* args[])
{

	gameLoop();
	//SideScrollerMain newSideScroller;
	//newSideScroller.start_sideScroller();
	return 0;
}