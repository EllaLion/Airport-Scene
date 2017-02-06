#include "camera.h"
#include <math.h>

Camera::Camera(sf::Vector3f initPos, sf::Vector3f initLos)
{
	LineOfSight = initLos;
	LineOfSightArrows = initLos;
	Postion = initPos;
	angle = 0.0f;
	upangle = 0.0f;
	height = 0;
}

void Camera::Rotate(int Direction)
{
	float fraction = 0.7f;

	if(Direction == 1) //left
	{ 
		Postion.x += LineOfSightArrows.y * fraction;
		Postion.y -= LineOfSightArrows.x * fraction;
	}

	else if(Direction == 2) //right
	{
		Postion.x -= LineOfSightArrows.y * fraction;
		Postion.y += LineOfSightArrows.x * fraction;
	}

	else if(Direction == 3) //forward
	{
		Postion.x += LineOfSightArrows.x * fraction;
		Postion.y += LineOfSightArrows.y * fraction;
	}
	
	else if(Direction == 4) //backward
	{
		Postion.x -= LineOfSightArrows.x * fraction;
		Postion.y -= LineOfSightArrows.y * fraction;
	}
	else if(Direction == 5) // rotate left
	{
		angle += 0.07f;
		LineOfSight.x = std::sin(angle);
		LineOfSight.y = std::cos(angle);
	}
	else if(Direction == 6) // rotate right
	{
		angle -= 0.07f;
		LineOfSight.x = std::sin(angle);
		LineOfSight.y = std::cos(angle);
	}
	else if(Direction == 7) //up
	{
		Postion.z+=5;
	}
	else if(Direction == 8) //down
	{
		Postion.z-=5;
	}
}
