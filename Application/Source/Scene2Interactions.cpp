#include "StudioProject2_Scene2.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"
#include "Vertex.h"
#include "Utility.h"
#include "shader.hpp"
#include "LoadTGA.h"
#include "Camera.h"
#include "PlayerClass.h"
#include "Animations.h"
#include "EnemyClass.h"
#include "EnemyClassManager.h"

void StudioProject2Scene2::LightInteraction()
{
}

void StudioProject2Scene2::TextInteraction()
{
	/*-----------------------Text Interaction----------------*/
	// Whenever press Enter is needed to continue [message or interaction]
	if (pEnter == true)
		pressEnterTS = 2;
	else
		pressEnterTS = 0;

	
}
