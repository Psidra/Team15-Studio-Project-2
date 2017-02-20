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
	if (PlayerClass::get_instance()->position_a.x < -17.f || PlayerClass::get_instance()->position_a.x > -8.f) //Moving away from the initial Spawn Point will make Spawn Message disappear
	{
		spawnTS = 0;
	}
	/*-------------------------------------*/

	/*-----------Syringe Text-------------*/
	if (PlayerClass::get_instance()->position_a.x > 10.f && PlayerClass::get_instance()->position_a.x < 20.f && textOccured == textOccurStorage)
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
	if (PlayerClass::get_instance()->position_a.x > 450.f && PlayerClass::get_instance()->position_a.x < 500.f && textOccured > textOccurStorage) //When near box, text appears
	{
		textOccurStorage = textOccured + 1; // tOS > tO
		nexttext = false;
		pEnter = true;
		boxTriggedText = true;

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

	if (PlayerClass::get_instance()->position_a.x > 550 && PlayerClass::get_instance()->position_a.x < 600 && textOccurStorage > textOccured)
	{
		hmTriggeredText = true;
		pEnter = true;
		textOccured = textOccurStorage;
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

	if (PlayerClass::get_instance()->position_a.x > 600 && PlayerClass::get_instance()->position_a.x < 630 && textOccured == textOccurStorage)
	{
		alexis_beside_hm = true;
		pEnter = true;
		textOccured = textOccurStorage + 1;  //tO > tOS
	}

	if (alexis_beside_hm == true && nexttext == true)
	{
		pEnter = false;
		alexis_beside_hm = false;
		nexttext = false;
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
