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

void StudioProject2Scene2::LoadLight()
{
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(-80, 10, -10); 
	light[0].color.Set(0.251, 0.878, 0.816);
	light[0].power = 5;
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
	light[1].position.Set(-20, 10, -5);
	light[1].color.Set(0.251, 0.878, 0.816);
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
	light[2].color.Set(0.251, 0.878, 0.816);
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

}

void StudioProject2Scene2::RenderMesh(Mesh *mesh, bool enableLight)
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

void StudioProject2Scene2::RenderText(Mesh* mesh, std::string text, Color color)
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

void StudioProject2Scene2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void StudioProject2Scene2::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position)
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

void StudioProject2Scene2::RenderLightStands()
{
	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	modelStack.Translate(-80, 8, -20);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(85, 8, -20);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(205, 8, 45);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(300, 8, -50);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(480, 35, -20);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(676, 8, 55);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(852, 8, -20);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void StudioProject2Scene2::RenderDebri()
{
	modelStack.PushMatrix();
	modelStack.Translate(-130, 8, 0);
	modelStack.Scale(2, 2.5, 2.5);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_DEBRIS1], true);
	modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(-130, 8, 10);
		modelStack.Scale(2, 3, 1);
		modelStack.Rotate(45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-140, 8, 70);
		modelStack.Scale(1.5, 1.5, 2);
		modelStack.Rotate(-45, 1, 0, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(130, 8, -50);
		modelStack.Scale(1, 1, 1);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(150, 8, -60);
		modelStack.Rotate(50, 1, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(30, 8, -50);
		modelStack.Scale(1.5, 1.7, 1);
		modelStack.Rotate(45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-20, 8, -50);
		modelStack.Scale(1.5, 2.5, 1.5);
		modelStack.Rotate(-45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(60, 8, -50);
		modelStack.Scale(1.5, 1.5, 1);
		modelStack.Rotate(45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(80, 8, -50);
		modelStack.Scale(1.5, 1.5, 1);
		modelStack.Rotate(45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(20, 6, 50);
		modelStack.Scale(0.5, 0.4, 0.5);
		modelStack.Rotate(45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(60, 5, 50);
		modelStack.Scale(0.3, 0.3, 0.3);
		modelStack.Rotate(45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(100, 5, 90);
		modelStack.Scale(0.7, 0.7, 0.7);
		//modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-30, 6, 50);
		modelStack.Scale(0.5, 0.4, 0.5);
		modelStack.Rotate(-45, 1, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(230, 6, -80);
	modelStack.Scale(1.6, 1.6, 1.6);
	//modelStack.Rotate(-45, 1, 1, 0);
	RenderMesh(meshList[GEO_DEBRISn], true);
	modelStack.Translate(50, 0, 0);
	RenderMesh(meshList[GEO_DEBRISn], true);
	modelStack.PopMatrix();
		
		modelStack.PushMatrix();
		modelStack.Translate(240, 6, -50);
		modelStack.Scale(1.6, 1.6, 1.6);
		modelStack.Rotate(-45, 1, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();		

		modelStack.PushMatrix();
		modelStack.Translate(220, 5, 80);
		modelStack.Scale(0.6, 0.5, 0.6);
		modelStack.Rotate(-45, 1, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();
		
		modelStack.PushMatrix();
		modelStack.Translate(300, 6, 80);
		modelStack.Scale(1, 0.6, 1);
		modelStack.Rotate(-45, 1, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(350, 6, -70);
	modelStack.Scale(1, 2, 1);
	modelStack.Rotate(-45, 1, 1, 0);
	RenderMesh(meshList[GEO_DEBRISn], true);
	modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(420, 6, -40);
		modelStack.Scale(1, 1.5, 1);
		modelStack.Rotate(-45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(580, 6, -40);
		modelStack.Scale(1, 1.5, 1);
		modelStack.Rotate(-45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.Translate(-50, 0, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(420, 6, 80);
		modelStack.Scale(0.7, 0.4, 0.6);
		modelStack.Rotate(-45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.Translate(-100, 0, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(320, 6, 80);
		modelStack.Scale(0.7, 0.4, 0.6);
		modelStack.Rotate(-45, 0, 1, 0);
		RenderMesh(meshList[GEO_DEBRISn], true);
		modelStack.PopMatrix();
}

void StudioProject2Scene2::RenderDebri2()
{
	modelStack.PushMatrix();
	modelStack.Translate(720, 6, -80);
	modelStack.Scale(2, 2, 2);
	//modelStack.Rotate(-45, 0, 1, 0);
	RenderMesh(meshList[GEO_DEBRISn], true);
	modelStack.Translate(30, 0, 10);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_CLUSTERTREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(520, 6, 80);
	modelStack.Scale(0.7, 0.4, 0.6);
	modelStack.Rotate(-45, 0, 1, 0);
	RenderMesh(meshList[GEO_DEBRISn], true);
	modelStack.PopMatrix();
}

void StudioProject2Scene2::RenderObjects()
{
	modelStack.PushMatrix();
	modelStack.Translate((20 + MoveShelterObj_PosX), 16, 20);
	modelStack.Rotate(-45, 0, 1, 0);
	RenderMesh(meshList[GEO_SHELTEROBJ], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(650, -20, -50);
	RenderMesh(meshList[GEO_TRUMPTOWER], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(565, -20, 20);
	modelStack.Scale(0.5, 1, 1);
	RenderMesh(meshList[GEO_TRUMPWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(510, 0, 10);
	modelStack.Rotate(Lamppostrotate, 0, 0, 1); //initial = -20, slanted fully = -39
	RenderMesh(meshList[GEO_LAMPPOST], true);
	modelStack.PopMatrix();

	if (Breakrope < 2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(505, 7, 10);
		modelStack.Rotate(-39, 0, 0, 1);
		RenderMesh(meshList[GEO_ROPE], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate((480 + MoveBox_PosX), 8, 15);
	RenderMesh(meshList[GEO_MOVEBOX], true);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(meshList[GEO_TESTBBOX], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(meshList[GEO_TESTBBOX2], false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//RenderMesh(meshList[GEO_TESTBBOX3], false);
	//modelStack.PopMatrix();

}

void StudioProject2Scene2::RenderTextInteractions()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis:3 enemies at once, I should be more careful.", Color(1, 1, 1), preBattleTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis:That was a close call, but it's still easy for me.", Color(1, 1, 1), preBuildingTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis:I should stay on guard, I've a feeling there'll", Color(1, 1, 1), cautionTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "be more mutants inside this building.", Color(1, 1, 1), cautionTS, 6, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis:Perhaps I can use this crate to defend", Color(1, 1, 1), crateTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "myself from potential projectiles.", Color(1, 1, 1), crateTS, 6, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis:Seems like I'm surrounded at all sides.", Color(1, 1, 1), surroundedTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "I've no choice but to fight the 3 enemies in front", Color(1, 1, 1), surroundedTS, 6, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis:This is barbwire. I won't be able to climb over", Color(1, 1, 1), barbwireTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "unless I'm looking to come out in pieces", Color(1, 1, 1), barbwireTS, 6, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], "Alexis:This lamppost seems slanted and about to fall.", Color(1, 1, 1), lamppostTS, 1, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Maybe I can get past the wall with that.", Color(1, 1, 1), lamppostTS, 6, -4);

	RenderTextOnScreen(meshList[GEO_TEXT], "Press <Enter> to continue", Color(1, 1, 1), pressEnterTS, 1, -6);
}