#include "StudioProject2_Scene1.h"
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

void StudioProject2Scene1::LightInteraction()
{
	if (PlayerClass::get_instance()->position_a.x < 25)
	{
		light[0].position.Set(-80, 10, -10);
		light[1].power = 1;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}
	else if (PlayerClass::get_instance()->position_a.x > 25 && PlayerClass::get_instance()->position_a.x < 90)
	{
		light[0].position.Set(40, 10, -20);
		light[1].power = 0;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}
	else if (PlayerClass::get_instance()->position_a.x > 90 && PlayerClass::get_instance()->position_a.x < 200)
	{
		light[0].position.Set(130, 0, -10);
	}
	else if (PlayerClass::get_instance()->position_a.x > 200 && PlayerClass::get_instance()->position_a.x < 300)
	{
		light[0].position.Set(235, -75, -10);
	}
	else if (PlayerClass::get_instance()->position_a.x > 300 && PlayerClass::get_instance()->position_a.x < 400)
	{
		light[0].position.Set(330, -155, 10);
	}
	else if (PlayerClass::get_instance()->position_a.x > 400 && PlayerClass::get_instance()->position_a.x < 550)
	{
		light[0].position.Set(440, -243, -10);
	}
	else if (PlayerClass::get_instance()->position_a.x > 550 && PlayerClass::get_instance()->position_a.x < 650)
	{
		light[0].position.Set(570, -243, -10);
	}
	else if (PlayerClass::get_instance()->position_a.x > 650 && PlayerClass::get_instance()->position_a.x < 750)
	{
		light[0].position.Set(670, -243, -10);
	}
	else if (PlayerClass::get_instance()->position_a.x > 750 && PlayerClass::get_instance()->position_a.x < 850)
	{
		light[0].position.Set(770, -243, -10);
	}
}

void StudioProject2Scene1::TextInteraction()
{

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xboxButtonsCount;
		const unsigned char *xboxcontroller = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xboxButtonsCount);
		/*-----------------------Text Interaction----------------*/
		// Whenever press Enter is needed to continue [message or interaction]
		if (pEnter == true)
			pressEnterTS = 2;
		else
			pressEnterTS = 0;

		/*-------------------Spawn Text--------*/
		if (PlayerClass::get_instance()->position_a.x < -17.f || PlayerClass::get_instance()->position_a.x > -8.f) //Moving away from the initial Spawn Point will make Spawn Message disappear
		{
			spawnTS = 0;
		}
		/*-------------------------------------*/

		/*-----------Syringe Text-------------*/
		if (PlayerClass::get_instance()->position_a.x > 10.f && PlayerClass::get_instance()->position_a.x < 20.f && textOccured == 0)
		{	// Make syringe text appear once only when near syringe
			syringeTriggedText = true;
			pEnter = true;
			textOccured = 1;
		}

		if (syringeTriggedText == true)
			syringeTriggedTS = 2;
		else
			syringeTriggedTS = 0;

		if (syringeTriggedText == true && (nexttext == true || boxTriggedText == true))
		{
			syringeSizeX = 0.1f;
			syringeSizeY = 0.1f;
			syringeSizeZ = 0.1f;
			syringeTriggedText = false;
			pEnter = false;
			nexttext = false;
		}
		/*--------------------------------------*/

		/*------------Box Text--------------------*/
		if (PlayerClass::get_instance()->position_a.x > 450.f && PlayerClass::get_instance()->position_a.x < 500.f && textOccured == 1) //When near box, text appears
		{
			textOccured = 2;
			bufferTime_text = elapsedTime + 5.f;
			nexttext = false;
			pEnter = true;
			boxTriggedText = true;

		}


		if (boxTriggedText == true)
			boxTriggedTS = 2;
		else
			boxTriggedTS = 0;

		if ((GLFW_PRESS == xboxcontroller[7] || Application::IsKeyPressed(VK_RETURN)) && (bufferTime_Enter < elapsedTime))
		{
			bufferTime_Enter = elapsedTime + 0.1f;
			nexttext = true;
		}

		if (boxTriggedText == true && (nexttext == true || elapsedTime > bufferTime_text))
		{
			boxTriggedText = false;
			boxTriggedText_Two = true;
			nexttext = false;
		}

		if (boxTriggedText_Two == true)
			boxTriggedTS_two = 2;
		else
			boxTriggedTS_two = 0;

		if (boxTriggedText_Two == true && (nexttext == true || hmTriggeredText == true))
		{
			pEnter = false;
			boxTriggedText_Two = false;
			nexttext = false;
		}
		/*----------------------------------------------*/

		if (hmvec[0].position_hm.x - PlayerClass::get_instance()->position_a.x < 30 && textOccured == 2)
		{
			hmTriggeredText = true;
			bufferTime_textlog = elapsedTime + 5.f;
			pEnter = true;
			textOccured = 3;
		}

		if (hmTriggeredText == true)
			hmTriggedTS = 2;
		else
			hmTriggedTS = 0;

		if (hmTriggeredText == true && (nexttext == true || bufferTime_textlog < elapsedTime))
		{
			hmTriggeredText = false;
			hm_to_alexis = true;
			nexttext = false;
		}

		if (hm_to_alexis == true)
			hm_to_alexisTS = 2;
		else
			hm_to_alexisTS = 0;


		if (hm_to_alexis == true && (nexttext == true || elapsedTime > bufferTime_textlog + 5.f))
		{
			hm_to_alexis = false;
			alexis_to_hm = true;
			nexttext = false;
		}

		if (alexis_to_hm == true)
			alexis_to_hmTS = 2;
		else
			alexis_to_hmTS = 0;

		if (alexis_to_hm == true && (nexttext == true || alexis_beside_hm == true))
		{
			alexis_to_hm = false;
			pEnter = false;
			nexttext = false;
		}
		/*----------------------------------------------------*/

		if (alexis_beside_hm == true)
			alexis_beside_hmTS = 2;
		else
			alexis_beside_hmTS = 0;

		if (hmvec[0].position_hm.x - PlayerClass::get_instance()->position_a.x < 3 && textOccured == 3)
		{
			alexis_beside_hm = true;
			pEnter = true;
			textOccured = 4;
		}

		if (alexis_beside_hm == true && (nexttext == true || postProjectileThrownText == true))
		{
			pEnter = false;
			alexis_beside_hm = false;
			nexttext = false;
		}

		if (postProjectileThrownText == true)
			postProjectileThrownTS = 2;
		else
			postProjectileThrownTS = 0;

		if (EnemyManager::get_instance()->EnemyList[0]->position_m.x - PlayerClass::get_instance()->position_a.x < 60
			&& textOccured == 4)
		{
			postProjectileThrownText = true;
			pEnter = true;
			textOccured = 5;
		}

		if (postProjectileThrownText == true && (nexttext == true || fm_triggedText == true))
		{
			pEnter = false;
			postProjectileThrownText = false;
			nexttext = false;
		}

		if (fm_triggedText == true)
			fm_triggedTS = 2;
		else
			fm_triggedTS = 0;

		if (textOccured == 5 && EnemyManager::get_instance()->EnemyList[0]->position_m.x - PlayerClass::get_instance()->position_a.x < 40)
		{
			fm_triggedText = true;
			bufferTime_fmAlexisText = elapsedTime + 5.f;
			pEnter = true;
			textOccured = 6;
		}

		if (alexisText == true)
			alexisTS = 2;
		else
			alexisTS = 0;

		if (fm_triggedText == true && (nexttext == true || elapsedTime > bufferTime_fmAlexisText))
		{
			fm_triggedText = false;
			alexisText = true;
			nexttext = false;
		}

		if (guideText == true)
			guideTS = 2;
		else
			guideTS = 0;

		if (alexisText == true && (nexttext == true || elapsedTime > bufferTime_fmAlexisText + 5.f))
		{
			alexisText = false;
			guideText = true;
			nexttext = false;
		}

		if (guideText == true && (nexttext == true || elapsedTime > bufferTime_fmAlexisText + 10.f))
		{
			pEnter = false;
			guideText = false;
			nexttext = false;
		}
		/*-------------------------------------------------------------------*/
	}
	else
	{
		/*-----------------------Text Interaction----------------*/
		// Whenever press Enter is needed to continue [message or interaction]
		if (pEnter == true)
			pressEnterTS = 2;
		else
			pressEnterTS = 0;

		/*-------------------Spawn Text--------*/
		if (PlayerClass::get_instance()->position_a.x < -17.f || PlayerClass::get_instance()->position_a.x > -8.f) //Moving away from the initial Spawn Point will make Spawn Message disappear
		{
			spawnTS = 0;
		}
		/*-------------------------------------*/

		/*-----------Syringe Text-------------*/
		if (PlayerClass::get_instance()->position_a.x > 10.f && PlayerClass::get_instance()->position_a.x < 20.f && textOccured == 0)
		{	// Make syringe text appear once only when near syringe
			syringeTriggedText = true;
			pEnter = true;
			textOccured = 1;
		}

		if (syringeTriggedText == true)
			syringeTriggedTS = 2;
		else
			syringeTriggedTS = 0;

		if (syringeTriggedText == true && (nexttext == true || boxTriggedText == true))
		{
			syringeSizeX = 0.1f;
			syringeSizeY = 0.1f;
			syringeSizeZ = 0.1f;
			syringeTriggedText = false;
			pEnter = false;
			nexttext = false;
		}
		/*--------------------------------------*/

		/*------------Box Text--------------------*/
		if (PlayerClass::get_instance()->position_a.x > 450.f && PlayerClass::get_instance()->position_a.x < 500.f && textOccured == 1) //When near box, text appears
		{
			textOccured = 2;
			bufferTime_text = elapsedTime + 5.f;
			nexttext = false;
			pEnter = true;
			boxTriggedText = true;

		}


		if (boxTriggedText == true)
			boxTriggedTS = 2;
		else
			boxTriggedTS = 0;

		if (Application::IsKeyPressed(VK_RETURN) && (bufferTime_Enter < elapsedTime))
		{
			bufferTime_Enter = elapsedTime + 0.1f;
			nexttext = true;
		}

		if (boxTriggedText == true && (nexttext == true || elapsedTime > bufferTime_text))
		{
			boxTriggedText = false;
			boxTriggedText_Two = true;
			nexttext = false;
		}

		if (boxTriggedText_Two == true)
			boxTriggedTS_two = 2;
		else
			boxTriggedTS_two = 0;

		if (boxTriggedText_Two == true && (nexttext == true || hmTriggeredText == true))
		{
			pEnter = false;
			boxTriggedText_Two = false;
			nexttext = false;
		}
		/*----------------------------------------------*/

		if (hmvec[0].position_hm.x - PlayerClass::get_instance()->position_a.x < 30 && textOccured == 2)
		{
			hmTriggeredText = true;
			bufferTime_textlog = elapsedTime + 5.f;
			pEnter = true;
			textOccured = 3;
		}

		if (hmTriggeredText == true)
			hmTriggedTS = 2;
		else
			hmTriggedTS = 0;

		if (hmTriggeredText == true && (nexttext == true || bufferTime_textlog < elapsedTime))
		{
			hmTriggeredText = false;
			hm_to_alexis = true;
			nexttext = false;
		}

		if (hm_to_alexis == true)
			hm_to_alexisTS = 2;
		else
			hm_to_alexisTS = 0;


		if (hm_to_alexis == true && (nexttext == true || elapsedTime > bufferTime_textlog + 5.f))
		{
			hm_to_alexis = false;
			alexis_to_hm = true;
			nexttext = false;
		}

		if (alexis_to_hm == true)
			alexis_to_hmTS = 2;
		else
			alexis_to_hmTS = 0;

		if (alexis_to_hm == true && (nexttext == true || alexis_beside_hm == true))
		{
			alexis_to_hm = false;
			pEnter = false;
			nexttext = false;
		}
		/*----------------------------------------------------*/

		if (alexis_beside_hm == true)
			alexis_beside_hmTS = 2;
		else
			alexis_beside_hmTS = 0;

		if (hmvec[0].position_hm.x - PlayerClass::get_instance()->position_a.x < 3 && textOccured == 3)
		{
			alexis_beside_hm = true;
			pEnter = true;
			textOccured = 4;
		}

		if (alexis_beside_hm == true && (nexttext == true || postProjectileThrownText == true))
		{
			pEnter = false;
			alexis_beside_hm = false;
			nexttext = false;
		}

		if (postProjectileThrownText == true)
			postProjectileThrownTS = 2;
		else
			postProjectileThrownTS = 0;

		if (EnemyManager::get_instance()->EnemyList[0]->position_m.x - PlayerClass::get_instance()->position_a.x < 60
			&& textOccured == 4)
		{
			postProjectileThrownText = true;
			pEnter = true;
			textOccured = 5;
		}

		if (postProjectileThrownText == true && (nexttext == true || fm_triggedText == true))
		{
			pEnter = false;
			postProjectileThrownText = false;
			nexttext = false;
		}

		if (fm_triggedText == true)
			fm_triggedTS = 2;
		else
			fm_triggedTS = 0;

		if (textOccured == 5 && EnemyManager::get_instance()->EnemyList[0]->position_m.x - PlayerClass::get_instance()->position_a.x < 40)
		{
			fm_triggedText = true;
			bufferTime_fmAlexisText = elapsedTime + 5.f;
			pEnter = true;
			textOccured = 6;
		}

		if (alexisText == true)
			alexisTS = 2;
		else
			alexisTS = 0;

		if (fm_triggedText == true && (nexttext == true || elapsedTime > bufferTime_fmAlexisText))
		{
			fm_triggedText = false;
			alexisText = true;
			nexttext = false;
		}

		if (guideText == true)
			guideTS = 2;
		else
			guideTS = 0;

		if (alexisText == true && (nexttext == true || elapsedTime > bufferTime_fmAlexisText + 5.f))
		{
			alexisText = false;
			guideText = true;
			nexttext = false;
		}

		if (guideText == true && (nexttext == true || elapsedTime > bufferTime_fmAlexisText + 10.f))
		{
			pEnter = false;
			guideText = false;
			nexttext = false;
		}
		/*-------------------------------------------------------------------*/
	}
}

void StudioProject2Scene1::RenderTextInteractions()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "Press A or D to move around. Walk over to the syringe", Color(1, 1, 1), spawnTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "This is the power to revolutionise the world!", Color(1, 1, 1), syringeTriggedTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "The wall is too high, I need to use the box to scale it.", Color(1, 1, 1), boxTriggedTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press F to grab box and move it to the wall", Color(1, 1, 1), boxTriggedTS_two, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press W to jump", Color(1, 1, 1), boxTriggedTS_two, 1, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "You see a half mutant.", Color(1, 1, 1), hmTriggedTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Perfect, you get to test your newly created cure.", Color(1, 1, 1), hmTriggedTS, 1, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "You called out to him. He turns and replies,", Color(1, 1, 1), hmTriggedTS, 1, -5);
	RenderTextOnScreen(meshList[GEO_TEXT], "Half Mutant: Get away from me!", Color(1, 1, 1), hm_to_alexisTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "I don't want to cause bloodshed...", Color(1, 1, 1), hm_to_alexisTS, 10, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis: Don't worry, I've created a cure. Trust me.", Color(1, 1, 1), alexis_to_hmTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press F to cure half-mutant.", Color(1, 1, 1), alexis_beside_hmTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press Right Click to dodge-roll projectiles", Color(1, 1, 1), postProjectileThrownTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Hold Left Shift to block", Color(1, 1, 1), postProjectileThrownTS, 1, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Full-mutants cannot be brought back to humanity.", Color(1, 1, 1), fm_triggedTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "They must be killed to advance.", Color(1, 1, 1), fm_triggedTS, 1, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "My apologies.", Color(1, 1, 1), alexisTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press Left Click to swing your sword", Color(1, 1, 1), guideTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "and attack when nearby.", Color(1, 1, 1), guideTS, 1, -4);

	RenderTextOnScreen(meshList[GEO_TEXT], "Press <Enter> to continue", Color(1, 1, 1), pressEnterTS, 1, -6);
}