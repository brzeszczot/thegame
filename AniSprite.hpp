#pragma once
#include <SFML/Graphics.hpp>

class AniSprite : public sf::Sprite
{
public:
	AniSprite();
		//initialtion list

	AniSprite(const sf::Image& Img, int frameWidth, int frameHeight);
	~AniSprite(void);
	//Get
	sf::IntRect GetFramePosition(int frame);
	int GetFrameCount();
	//set
	void SetFrameSize(int frameWidth, int frameHeight);
	//Sets current frame
	void SetFrame(int frame);
	//sets loop speed in fps
	void SetLoopSpeed(float fps);
	//start looping
	void Play();
	void Play(int start, int end);
	//stop
	void Stop();
	//draw functions
	void Update();
	bool isPlaying;
	int currentFrame;
private:
	sf::Clock clock;
	float fps;
	int loopStart;
	int loopEnd;
	int frameWidth;
	int frameHeight;
};
