#include "StudioProject2_Scene2.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
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
	if (PlayerClass::get_instance()->position_a.x < 25)
	{
		light[0].position.Set(-80, 8, -15);
		light[1].power = 1;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}
	else if (PlayerClass::get_instance()->position_a.x > 25 && PlayerClass::get_instance()->position_a.x < 120)
	{
		light[0].position.Set(80, 9, -20);
		light[1].power = 0;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}
	else if (PlayerClass::get_instance()->position_a.x > 130 && PlayerClass::get_instance()->position_a.x < 320)
	{
		light[0].position.Set(200, 8, 50);
		light[2].position.Set(300, 8, -40);
		light[2].power = 2;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);

	}
	else if (PlayerClass::get_instance()->position_a.x > 325 && PlayerClass::get_instance()->position_a.x < 600)
	{
		light[0].position.Set(480, 40, -15);
	}
	else if (PlayerClass::get_instance()->position_a.x > 605 && PlayerClass::get_instance()->position_a.x < 750)
	{
		light[0].position.Set(680, 8, 50);
	}
	else if (PlayerClass::get_instance()->position_a.x > 755 && PlayerClass::get_instance()->position_a.x < 1000)
	{
		light[0].position.Set(850, 8, -15);
	}

}

void StudioProject2Scene2::TextInteraction()
{
	/*-----------------------Text Interaction----------------*/
	// Whenever press Enter is needed to continue [message or interaction]
	if (pEnter == true)
		pressEnterTS = 2;
	else
		pressEnterTS = 0;

	if (preBattleText)
		preBattleTS = 2;
	else
		preBattleTS = 0;

	if (preBuildingText)
		preBuildingTS = 2;
	else
		preBuildingTS = 0;

	if (cautionText)
		cautionTS = 2;
	else
		cautionTS = 0;

	if (crateText)
		crateTS = 2;
	else
		crateTS = 0;

	if (surroundedText)
		surroundedTS = 2;
	else
		surroundedTS = 0;

	if (barbwireText)
		barbwireTS = 2;
	else
		barbwireTS = 0;

	if (lamppostText)
		lamppostTS = 2;
	else
		lamppostTS = 0;

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xbox;
		const unsigned char *xboxs = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xbox);

		if ((GLFW_PRESS == xboxs[7] || Application::IsKeyPressed(VK_RETURN)) && (bufferTime_Enter < elapsedTime))
		{
			bufferTime_Enter = elapsedTime + 0.1f;
			nexttext = true;
		}

		if (textOccur == 0 && 
			EnemyManager::get_instance()->EnemyList[0]->position_m.x - PlayerClass::get_instance()->position_a.x < 70)
		{
			textOccur = 1;
			preBattleText = true;
			pEnter = true;
		}

		if (preBattleText)
		{
			if (nexttext)
			{
				preBattleText = false;
				nexttext = false;
				pEnter = false;
			}
			else if (preBuildingText)
				preBattleText = false;
		}

		if (EnemyManager::get_instance()->EnemyList[0]->get_health() <= 0 && 
			EnemyManager::get_instance()->EnemyList[1]->get_health() <= 0 && 
			EnemyManager::get_instance()->EnemyList[2]->get_health() <= 0 && textOccur == 1)
		{
			textOccur = 2;
			bufferTime_preBuilding = elapsedTime + 4.f;
			preBuildingText = true;
			pEnter = true;
		}

		if (preBuildingText && (elapsedTime > bufferTime_preBuilding || nexttext))
		{
			preBuildingText = false;
			nexttext = false;
			cautionText = true;
		}
		
		if (cautionText && (elapsedTime > bufferTime_preBuilding + 4 || nexttext))
		{
			cautionText = false;
			nexttext = false;
			crateText = true;
		}

		if (crateText && (elapsedTime > bufferTime_preBuilding + 8 || nexttext))
		{
			crateText = false;
			pEnter = false;
			nexttext = false;
		}
		

		if (textOccur == 2 && 
			EnemyManager::get_instance()->EnemyList[0]->position_m.x - PlayerClass::get_instance()->position_a.x < 50 &&
			EnemyManager::get_instance()->EnemyList[0]->get_health() > 0 && 
			EnemyManager::get_instance()->EnemyList[1]->get_health() > 0 && 
			EnemyManager::get_instance()->EnemyList[2]->get_health() > 0)
		{
			textOccur = 3;
			surroundedText = true;
			pEnter = true;
		}

		if (surroundedText && (nexttext || EnemyManager::get_instance()->EnemyList[0]->get_health() <= 0 &&
			EnemyManager::get_instance()->EnemyList[1]->get_health() <= 0 &&
			EnemyManager::get_instance()->EnemyList[2]->get_health() <= 0)) 
		{
			surroundedText = false;
			pEnter = false;
			nexttext = false;
		}

		if (textOccur == 3 && PlayerClass::get_instance()->position_a.x > 490)
		{
			textOccur = 4;
			barbwireText = true;
			pEnter = true;
			bufferTime_barbwire = elapsedTime + 4.f;
		}

		if (barbwireText && (nexttext || elapsedTime > bufferTime_barbwire))
		{
			barbwireText = false;
			lamppostText = true;
			nexttext = false;
		}

		if (lamppostText && (nexttext || elapsedTime > bufferTime_barbwire + 4))
		{
			pEnter = false;
			lamppostText = false;
			nexttext = false;
		}
	}
}
