// openglex-18.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "TextureLoader.h"
#include "ModelReader.h"
#include "camera.h"

#if 0

#define GL_GLEXT_PROTOTYPES 1		// ALLOW USE OF OPEN GL 1.5+ functionality!!!
#define GLEW_STATIC
// this needs to be the first include
#include <SFML\glew.h>		// make 1.5 functions into function pointers - bind at runtime.
#include <SFML\OpenGL.hpp>	// glew BEFORE opengl headers!!!!
#include <SFML\Graphics.hpp>
// put these inside the SFML\include folder
#include <SFML\wglext.h>
#include <SFML\glext.h>
// note weve also added the lib file glew32s.lib

// shader.h included these
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#endif

using namespace std;

// better to use const int rather than define
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;
const float REFRESH_RATE = 0.03f;

GLfloat m_lightPosition[] = {-50.0f, +50.0f, +100.0f, 1.0f};

GLuint m_textureID[2];

GLfloat m_xRotationAngle = 0.0f;
GLfloat m_xAngleIncrement = 0.0f;

GLfloat m_yRotationAngle =  90.0f;
GLfloat m_yAngleIncrement = 0.0f;

std::vector<ModelReader*> m_modelReader;


void DrawUsingVBO(bool drawWithNormals, bool drawWithTexture, GLuint textureBufferID, int i)
{


	glEnableClientState(GL_VERTEX_ARRAY);

	vector<float>& vertices = m_modelReader.at(i)->GetVertices();

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);



	// pointer to normal array
	if (drawWithNormals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);

		vector<float>& normals = m_modelReader.at(i)->GetNormals();

		glNormalPointer(GL_FLOAT, 0, &normals[0]);

	}
	
	if (drawWithTexture)
	{
		glEnable(GL_TEXTURE_2D);	// we are using textures
		vector<float>& textureCoordinates = m_modelReader.at(i)->GetTextureCoordinates();
		glBindTexture(GL_TEXTURE_2D, textureBufferID);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY); //glTexCoordPointer will point to an array
		glTexCoordPointer(2, GL_FLOAT, 0, &textureCoordinates[0]);

	}

	glDrawArrays(GL_TRIANGLES, 0, (unsigned int)vertices.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);

	if (drawWithNormals)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	
	if (drawWithTexture)
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

	}
}

void SetMaterialWhite()
{
	GLfloat materialWhiteAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat materialWhiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 4.0f;
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}

void SetMaterialBlank()
{
	GLfloat materialWhiteAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat materialWhiteSpecular[] = {0.0f, 0.0f, 0.0f, 0.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 400.0f;
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}

void SetMaterialRedShiny()
{
	GLfloat materialRedAmbient[] = {0.2f, 0.0f, 0.0f, 1.0f};
	GLfloat materialRedDiffuse[] = {0.8f, 0.0f, 0.0f, 1.0f};
	GLfloat materialRedSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialRedShininess = 700.0f;						// try 500 for a sharper specular highlight
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialRedAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialRedDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialRedSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialRedShininess);
}


void SetLightPosition(float x, float y, float z)
{
	m_lightPosition[0] = x;
	m_lightPosition[1] = y;
	m_lightPosition[2] = z;
}

void ConfigureLightSources()
{
	GLfloat lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat noLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat lightModelAmbient[] = {0.3f, 0.3f, 0.3f, 1.0};

	// put light behind and above us on left
	SetLightPosition(-50.0f, +50.0f, + 100.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
	glLightfv(GL_LIGHT0, GL_AMBIENT, noLight);			// no ambient light from the source

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);	// use global ambient instead

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// with lighting glColor no longer applies
	// create a default material for the models
	SetMaterialBlank();
}

int main()
{

	// SFML-2.3.2 depth buffering fails unless we create a more specific window - as below
	int depthBits = 24;
	int stencilBits = 8;
	int antiAliasingLevel = 2;
	int majorVersion = 3;
	int minorVersion = 3;

	sf::ContextSettings context(depthBits, stencilBits, antiAliasingLevel, majorVersion, minorVersion)	;
	sf::Window window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "SFML Window", 7U, context);

	// check we got the settings we need
	sf::ContextSettings windowSettings = window.getSettings();


    // Create a clock for measuring time elapsed
    sf::Clock clock;
	
	// always setup OpenGL After the window is configured
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	// draw single sided faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 1.f,2000.f);
	


	TextureLoader* tl = new TextureLoader();
	tl->LoadBMP("images\\Skyboxtexture.bmp", m_textureID[0]);
	tl->LoadBMP("images\\CreatureTexture.bmp", m_textureID[1]);
	tl->LoadBMP("images\\Creature2Texture.bmp", m_textureID[2]);
	tl->LoadBMP("images\\runwayTexture.bmp", m_textureID[3]);
	tl->LoadBMP("images\\towerTexture.bmp", m_textureID[4]);
	tl->LoadBMP("images\\ControlTowerOwn.bmp", m_textureID[5]);
	

	Camera camera(sf::Vector3f (0.0f, -1.0f, 0.0f), sf::Vector3f(0.0f, 5.0f, 0.1f));
	

	ConfigureLightSources();

	m_modelReader.push_back(new ModelReader("models\\SkyBox.obj"));
	m_modelReader.push_back(new ModelReader("models\\Creature.obj"));
	m_modelReader.push_back(new ModelReader("models\\Creature2.obj")); //2
	m_modelReader.push_back(new ModelReader("models\\Runway.obj"));
	m_modelReader.push_back(new ModelReader("models\\tower.obj")); //4
	m_modelReader.push_back(new ModelReader("models\\ControlTower.obj"));


    // Start game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();

			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::A)))
			{
				camera.Rotate(1);
			}

			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::D)))
			{
				camera.Rotate(2);
			}

			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::W)))
			{
				camera.Rotate(3);
			}

			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::S)))
			{
				camera.Rotate(4);
			}
			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::Left)))
			{
				camera.Rotate(5);
			}
			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::Right)))
			{
				camera.Rotate(6);
			}
			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::Up)))
			{
				camera.Rotate(7);
			}
			if((Event.type == sf::Event::KeyPressed && (Event.key.code == sf::Keyboard::Down)))
			{
				camera.Rotate(8);
			}


            // Resize event : adjust viewport
            if (Event.type == sf::Event::Resized)
                  glViewport(0, 0, Event.size.width, Event.size.height);
        }


        window.setActive();

		if(clock.getElapsedTime().asSeconds()>REFRESH_RATE)
		{
			m_xRotationAngle += m_xAngleIncrement;
			m_yRotationAngle += m_yAngleIncrement;

			// Clear colour and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Apply some transformations
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			gluLookAt(camera.Postion.x, camera.Postion.z, camera.Postion.y,
				camera.Postion.x + camera.LineOfSight.x, camera.Postion.z, camera.Postion.y + camera.LineOfSight.y,
					0.0f, 1.0f, 0.0f);/* positive Y up vector */ 

			glPushMatrix();

			glRotatef(m_xRotationAngle,1.f,0.f,0.f);
			glRotatef(m_yRotationAngle,0.f,1.f,0.f);

			SetMaterialWhite();

			// call the rendering code
			DrawUsingVBO(true, true, m_textureID[0], 0);
			DrawUsingVBO(true, true, m_textureID[1], 1);
			DrawUsingVBO(true, true, m_textureID[2], 2);
			DrawUsingVBO(true, true, m_textureID[3], 3);
			DrawUsingVBO(true, true, m_textureID[4], 4);
			DrawUsingVBO(true, true, m_textureID[5], 5);

			// restore untranslated matrix from matrix stack
			glPopMatrix();

			clock.restart();
		    // Finally, display rendered frame on screen
			window.display();
		}
    }
    return EXIT_SUCCESS;
}

