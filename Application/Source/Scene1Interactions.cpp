#include "StudioProject2_Scene1.h"
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

void StudioProject2Scene1::LightInteraction()
{
	if (a_PosX < 10)
	{
		light[2].power = 5;
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	}
	else
	{
		light[2].power = 0;
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	}

	if (a_PosX < 25)
	{
		light[1].power = 2;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}
	else
	{
		light[1].power = 0;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}

	if (a_PosX > 25 && a_PosX < 90)
	{
		light[0].power = 5;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}
	else
	{
		light[0].power = 0;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}

	if (a_PosX > 90 && a_PosX < 190)
	{
		light[3].power = 5;
		glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	}
	else
	{
		light[3].power = 0;
		glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	}

	if (a_PosX > 210 && a_PosX < 290)
	{
		light[4].power = 5;
		glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	}
	else
	{
		light[4].power = 0;
		glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	}

	if (a_PosX > 300 && a_PosX < 380)
	{
		light[5].power = 5;
		glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	}
	else
	{
		light[5].power = 0;
		glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	}

	if (a_PosX > 400 && a_PosX < 530)
	{
		light[6].power = 5;
		glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
	}
	else
	{
		light[6].power = 0;
		glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
	}

	if (a_PosX > 550 && a_PosX < 630)
	{
		light[7].power = 5;
		glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
	}
	else
	{
		light[7].power = 0;
		glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
	}
}

void StudioProject2Scene1::TextInteraction()
{
	/*-----------------------Text Interaction----------------*/
	// Whenever press Enter is needed to continue [message or interaction]
	if (pEnter == true)
		pressEnterTS = 2;
	else
		pressEnterTS = 0;

	/*-------------------Spawn Text--------*/
	if (a_PosX < -17.f || a_PosX > -8.f) //Moving away from the initial Spawn Point will make Spawn Message disappear
	{
		spawnTS = 0;
	}
	/*-------------------------------------*/

	/*-----------Syringe Text-------------*/
	if (a_PosX > 10.f && a_PosX < 20.f && textOccured == textOccurStorage)
	{	// Make syringe text appear once only when near syringe
		syringeTriggedText = true;
		pEnter = true;
	}

	if (syringeTriggedText == true)
		syringeTriggedTS = 2;
	else
		syringeTriggedTS = 0;

	if (syringeTriggedText == true && nexttext == true)
	{
		textOccured = textOccurStorage + 1; // tO > tOS
		syringeTriggedText = false;
		pEnter = false;
		nexttext = false;
	}
	/*--------------------------------------*/

	/*------------Box Text--------------------*/
	if (a_PosX > 450.f && a_PosX < 500.f && textOccured > textOccurStorage) //When near box, text appears
	{
		pEnter = true;
		boxTriggedText = true;
		textOccurStorage = textOccured + 1; // tO < tOS
	}


	if (boxTriggedText == true)
		boxTriggedTS = 2;
	else
		boxTriggedTS = 0;

	if (Application::IsKeyPressed(VK_RETURN) && (bufferTime_text < elapsedTime))
	{
		bufferTime_text = elapsedTime + 0.1f;
		nexttext = true;
	}

	if (boxTriggedText == true && nexttext == true)
	{
		boxTriggedText = false;
		boxTriggedText_Two = true;
		nexttext = false;
	}

	if (boxTriggedText_Two == true)
		boxTriggedTS_two = 2;
	else
		boxTriggedTS_two = 0;

	if (boxTriggedText_Two == true && nexttext == true)
	{
		pEnter = false;
		boxTriggedText_Two = false;
		nexttext = false;
	}
	/*----------------------------------------------*/

	/*------Half Mutant conversation with Alexis-----*/
	if (a_PosX < 540 && a_PosX > 570 && textOccured < textOccurStorage)
	{	//When alexis at wall, he sees the half mutant and text gets triggered
		textOccured = textOccurStorage; // tO == tOS
		hmTriggeredText = true;
		pEnter = true;
	}

	if (hmTriggeredText == true)
		hmTriggedTS = 2;
	else
		hmTriggedTS = 0;

	if (hmTriggeredText == true && nexttext == true)
	{
		hmTriggeredText = false;
		hm_to_alexis = true;
		nexttext = false;
	}

	if (hm_to_alexis == true)
		hm_to_alexisTS = 2;
	else
		hm_to_alexisTS = 0;


	if (hm_to_alexis == true && nexttext == true)
	{
		hm_to_alexis = false;
		alexis_to_hm = true;
		nexttext = false;
	}

	if (alexis_to_hm == true)
		alexis_to_hmTS = 2;
	else
		alexis_to_hmTS = 0;

	if (alexis_to_hm == true && nexttext == true)
	{
		alexis_to_hm = false;
		pEnter = false;
		nexttext = false;
		//Since cannot get back to the other side of the wall, 
		//the syringe text wont be reachable , so reuse the tO == tOS condition for next text
	}
	/*----------------------------------------------------*/

	if (alexis_beside_hm == true)
		alexis_beside_hmTS = 2;
	else
		alexis_beside_hmTS = 0;


	if (a_PosX < 620 && a_PosX > 650 && textOccured == textOccurStorage)
	{
		alexis_beside_hm = true;
		pEnter = true;
	}

	if (alexis_beside_hm == true && nexttext == true)
	{
		pEnter = false;
		alexis_beside_hm = false;
		nexttext = false;
		textOccured = textOccurStorage + 1; // tO > tOS
	}


	//if () [Right after projectile barely missed]
	//{  need a boolean to set true 
	//   when projectile is thrown
	//	 postProjectileThrownText = true;
	//	 postProjectileThrownTS = 2;
	//}

	//if (postProjectileThrownTS == 2)
	//{
	//	if (Application::IsKeyPressed((VK_RETURN))
	//   {
	//		postProjectileThrownTS = 0;
	//   }
	//}

	//if (postProjectileThrownText == true
	//     && [insert coordinates of character.x or character.z when fm and char distance is less than 10])
	//{
	//	 fm_triggedText = true;
	//   fm_triggedTS = 2;
	//   postProjectileThrownText = false;
	//}

	//if (fm_triggedText == true)
	//{
	//   if (Application::IsKeyPressed((VK_RETURN))
	//   {
	//		alexisText = true;
	//		fm_triggedTS = 0;
	//      alexisTS = 2;
	//		fm_triggedText = false;
	//   }
	//}

	//if (alexisText == true)
	//{
	//    if (Application::IsKeyPressed((VK_RETURN))
	//   {
	//		guideText = true;
	//		alexisTS = 0;
	//		guideTS = 2;
	//		alexisText = false;
	//   }
	//}
	/*-------------------------------------------------------------------*/

}

void StudioProject2Scene1::HeadDirection()
{
	if ((pressedA == true && a_LookingDirection == 90.f) ||
		(pressedD == true && a_LookingDirection == -90.f))
	{
		a_LookingDirection *= -1;
	}
}