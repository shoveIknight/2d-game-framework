#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

enum AnimType
{
	IdleAnim,
    WalkAnim,
	FallAnim,
	AttackAnim,
	JumpAnim,
	LandAnim,
	JumpEndingAnim,
	TurnAnim,
	WallRideAnim
};

class Animation
{
public:
	
	/**
	* Frame constructor for an animation.
	*
	* @param path Absoulte or relative path of the frame.
	* @param renderer The renderer where you will render the animation.
	* @param rightGap This is the gap between hitbox's x coordinate and the texture's x coordinate you want when character face right.
	* @param leftGap This is the gap between hitbox's x coordinate and the texture's x coordinate you want when character face left.
	*/
	Animation(const std::string& path, SDL_Renderer* const& renderer, float rightGap, float leftGap);

	//default constructor
	Animation() {};
	
	
	Animation* getNextAnimation() const;
	SDL_Texture* getCurrentFrame() const;
	float getFrameWidth() const;
	float getFrameHeight() const;

	/**
	* Frame initilazier for an animation.
	*
	* @param path Absoulte or relative path of the frame.
	* @param renderer The renderer where you will render the animation.
	* @param rightGap This is the gap between hitbox's x coordinate and the texture's x coordinate you want when character face right.
	* @param leftGap This is the gap between hitbox's x coordinate and the texture's x coordinate you want when character face left.
	*/
	void setFrame(const std::string& path, SDL_Renderer* const& renderer, float rightGap, float leftGap);//get rid of this asap

	/**
	* Sets the animation which will come after.
	*
	* @param next Pointer to animation object of the next frame.
	*/
	void setNextAnimation(Animation* const &next);


	/**
	*
	* @param facing The direction currently facing towards.
	* @return The gap there should be between hitbox and the animation frame in floats.
	*/
	float getHitboxAndFrameGap(std::string const& facing) const;





private:
	SDL_Surface* frameSurface = nullptr;
	SDL_Texture* frameTexture = nullptr;
	Animation* nextAnimation = nullptr;
	float leftHitboxAndFrameGap = 0,
		rightHitboxAndFrameGap = 0,
		frameWidth = 0,
		frameHeight = 0;
};

