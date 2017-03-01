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

void StudioProject2SceneBoss::RenderLightStands()
{
	glBlendFunc(GL_ONE, GL_ONE);
	modelStack.PushMatrix();
	modelStack.Translate(10, 1, -30);
	modelStack.Scale(3, 3, 3);

	for (int i = 0; i < 6; i++)
	{
		RenderMesh(meshList[GEO_LIGHT], true);

		modelStack.Translate(30, 0, 0);
	}
	modelStack.PopMatrix();
	
	
}

void StudioProject2SceneBoss::LoadLight()
{

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