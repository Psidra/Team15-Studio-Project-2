#include "StudioProject2_SceneBoss.h"
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

void StudioProject2SceneBoss::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	//to do: scale and translate accordingly
	modelStack.Scale(sizex, sizey, position);
	modelStack.Translate(x + 0.5f, y + 0.5f, 0);

	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void StudioProject2SceneBoss::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.7f, 10, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

}

void StudioProject2SceneBoss::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void StudioProject2SceneBoss::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void StudioProject2SceneBoss::LightInteraction()
{
	//first light

	if (PlayerClass::get_instance()->position_a.x < 20)
	{
		light[0].position.Set(-2, -1, 30);
		light[1].power = 1;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}
	else if (PlayerClass::get_instance()->position_a.x > 24 && PlayerClass::get_instance()->position_a.x < 100)
	{
		light[0].position.Set(90, 4, 30);
		light[1].power = 0;
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	}

	else if (PlayerClass::get_instance()->position_a.x > 104 && PlayerClass::get_instance()->position_a.x < 220)
	{
		light[0].position.Set(182, 4, 30);
		light[2].position.Set(272, 4, 30);
		light[2].power = 2;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);

	}
	else if (PlayerClass::get_instance()->position_a.x > 225 && PlayerClass::get_instance()->position_a.x < 600)
	{
		light[0].position.Set(272, 4, 30);
		light[2].power = 0;
		//glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	}
	//else if (PlayerClass::get_instance()->position_a.x > 34 && PlayerClass::get_instance()->position_a.x < 40)
	//{
	//	light[1].position.Set(182, 4, 30);
	//	light[2].power = 1;
	//	glUniform1f(m_parameters[U_LIGHT1_POWER], light[2].power);
	//}
}

void StudioProject2SceneBoss::RenderLightStands()
{
	glBlendFunc(GL_ONE, GL_ONE);
	modelStack.PushMatrix();
	modelStack.Translate(-2, -1, 30);
	modelStack.Scale(3, 3, 3);

	for (int i = 0; i < 4; i++)
	{
		RenderMesh(meshList[GEO_LIGHT], true);

		modelStack.Translate(30, 0, 0);
	}
	modelStack.PopMatrix();

}

void StudioProject2SceneBoss::LoadLight()
{
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(-2, 4, 30);
	light[0].color.Set(1.000, 0.947, 0.200);
	light[0].power = 3;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(50, 4, 30);
	light[1].color.Set(1.000, 0.947, 0.200);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	light[2].type = Light::LIGHT_POINT;
	light[2].position.Set(-20, 10, -5);
	light[2].color.Set(1.000, 0.947, 0.200);
	light[2].power = 1;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);
/*
	light[3].type = Light::LIGHT_POINT;
	light[3].position.Set(-20, 10, -5);
	light[3].color.Set(1.000, 0.947, 0.200);
	light[3].power = 1;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = cos(Math::DegreeToRadian(45));
	light[3].cosInner = cos(Math::DegreeToRadian(30));
	light[3].exponent = 3.f;
	light[3].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

	light[4].type = Light::LIGHT_POINT;
	light[4].position.Set(-20, 10, -5);
	light[4].color.Set(1.000, 0.947, 0.200);
	light[4].power = 1;
	light[4].kC = 1.f;
	light[4].kL = 0.01f;
	light[4].kQ = 0.001f;
	light[4].cosCutoff = cos(Math::DegreeToRadian(45));
	light[4].cosInner = cos(Math::DegreeToRadian(30));
	light[4].exponent = 3.f;
	light[4].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
	glUniform4fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);
*/
}

void StudioProject2SceneBoss::TextInteraction()
{
	if (pEnter == true) //press enter to continue
		pressEnterTS = 2;
	else
		pressEnterTS = 0;

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xbox;
		const unsigned char * xboxs = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xbox);

		if ((GLFW_PRESS == xboxs[7] || Application::IsKeyPressed(VK_RETURN)) && (bufferTime_Enter < elapsedTime))
		{
			bufferTime_Enter = elapsedTime + 0.1f;
			nexttext = true;
		}

		if ((elapsedTime > bufferTime_text || nexttext == true) && bossTextsize == 2)
		{
			bossTextsize = 0;
			alexisTextsize = 2;
			nexttext = false;
		}

		if ((elapsedTime > bufferTime_text + 3 || nexttext == true) && alexisTextsize == 2)
		{
			alexisTextsize = 0;
			narTextsize = 2;
			nexttext = false;
		}

		if ((elapsedTime > bufferTime_text + 6 || nexttext == true) && narTextsize == 2)
		{
			narTextsize = 0;
			insTextsize = 2;
			nexttext = false;
		}

		if (insTextsize == 2 && (elapsedTime > bufferTime_text + 9 || nexttext == true))
		{
			pEnter = false;
			insTextsize = 0;
			nexttext = false; 
		}
	}
	else
	{
		if (Application::IsKeyPressed(VK_RETURN) && (bufferTime_Enter < elapsedTime))
		{
			bufferTime_Enter = elapsedTime + 0.1f;
			nexttext = true;
		}

		if ((elapsedTime > bufferTime_text || nexttext == true) && bossTextsize == 2)
		{
			bossTextsize = 0;
			alexisTextsize = 2;
			nexttext = false;
		}

		if ((elapsedTime > bufferTime_text + 3 || nexttext == true) && alexisTextsize == 2)
		{
			alexisTextsize = 0;
			narTextsize = 2;
			nexttext = false;
		}

		if ((elapsedTime > bufferTime_text + 6 || nexttext == true) && narTextsize == 2)
		{
			narTextsize = 0;
			insTextsize = 2;
			nexttext = false;
		}

		if (insTextsize == 2 && (elapsedTime > bufferTime_text + 9 || nexttext == true))
		{
			pEnter = false;
			insTextsize = 0;
			nexttext = false;
		}
	}
	



	
}

void StudioProject2SceneBoss::RenderTextInteractions()
{
	//nar = narrator (since the text is in second person pov)
	//ins = instruction (because....well, self-explanitory.)

	RenderTextOnScreen(meshList[GEO_TEXT], "???: HHRRRRRRRNNNGG", Color(1, 1, 1), bossTextsize, 1, -3); //mesh,text,color,size,position
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis: I must be close to the Source.", Color(1, 1, 1), alexisTextsize, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "You inject some kind of serum into your body.", Color(1, 1, 1), narTextsize, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Power coursed through your veins. You're ready to fight.", Color(1, 1, 1), narTextsize, 1, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press <Q>/<LB> for Laser Attack", Color(1, 1, 1), insTextsize, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press <R>/<RB> for Hard Light Shield", Color(1, 1, 1), insTextsize, 1, -4);

	RenderTextOnScreen(meshList[GEO_TEXT], "Press <Enter> to continue", Color(1, 1, 1), pressEnterTS, 1, -6);
}