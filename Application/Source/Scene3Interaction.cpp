#include "StudioProject2_Scene3.h"
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

//void StudioProject2Scene3::LightInteraction()
//{
//	if (PlayerClass::get_instance()->position_a.x < 25)
//	{
//		light[0].position.Set(-80, 8, -15);
//		light[1].power = 1;
//		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
//	}
//	else if (PlayerClass::get_instance()->position_a.x > 25 && PlayerClass::get_instance()->position_a.x < 120)
//	{
//		light[0].position.Set(80, 9, -20);
//		light[1].power = 0;
//		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
//	}
//	else if (PlayerClass::get_instance()->position_a.x > 130 && PlayerClass::get_instance()->position_a.x < 320)
//	{
//		light[0].position.Set(200, 8, 50);
//		light[2].position.Set(300, 8, -40);
//		light[2].power = 2;
//		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
//		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
//
//	}
//	else if (PlayerClass::get_instance()->position_a.x > 325 && PlayerClass::get_instance()->position_a.x < 600)
//	{
//		light[0].position.Set(480, 40, -15);
//		light[2].power = 0;
//		//glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
//	}
//	else if (PlayerClass::get_instance()->position_a.x > 605 && PlayerClass::get_instance()->position_a.x < 750)
//	{
//		light[0].position.Set(680, 8, 50);
//		light[0].power = 2;
//		//glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
//	}
//	else if (PlayerClass::get_instance()->position_a.x > 755 && PlayerClass::get_instance()->position_a.x < 1000)
//	{
//		light[0].position.Set(850, 8, -15);
//	}
//
//}

void StudioProject2Scene3::Renderbb()
{
	modelStack.PushMatrix();
	//modelStack.Translate(-130, 8, 0);
	//modelStack.Scale(2, 2.5, 2.5);
	RenderMesh(meshList[GEO_BACKCAVE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(-130, 8, 0);
	//modelStack.Scale(2, 2.5, 2.5);
	RenderMesh(meshList[GEO_BIGLEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(-130, 8, 0);
	//modelStack.Scale(2, 2.5, 2.5);
	RenderMesh(meshList[GEO_RIGHTCAVE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(-130, 8, 0);
	//modelStack.Scale(2, 2.5, 2.5);
	RenderMesh(meshList[GEO_BIGRIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(-130, 8, 0);
	//modelStack.Scale(2, 2.5, 2.5);
	RenderMesh(meshList[GEO_SMALLRIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//modelStack.Translate(-130, 8, 0);
	//modelStack.Scale(2, 2.5, 2.5);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
}