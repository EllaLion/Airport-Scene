#pragma once

#include <SFML\Graphics.hpp>

class Camera
{
	public:
		Camera(sf::Vector3f initPos, sf::Vector3f initLos);

		sf::Vector3f LineOfSight;
		sf::Vector3f LineOfSightArrows;
		sf::Vector3f Postion;
		float oldMouseX;
		float oldMouseY;
		float mouseX;
		float mouseY;

		float angle;
		float upangle;
		void Rotate(int direction);
		void Move(int direction);

		int height;
};