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

void StudioProject2Scene1::LoadLight()
{
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(-80, 10, -10); // 40 10 -20
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
}

void StudioProject2Scene1::LoadCharacter()
{
	meshList[GEO_ALEXIS_HEAD] = MeshBuilder::GenerateOBJ("aHead", "OBJ//Character//facehair.obj");
	meshList[GEO_ALEXIS_HEAD]->textureID = LoadTGA("Image//facehairtext.tga");
	meshList[GEO_ALEXIS_BODY] = MeshBuilder::GenerateOBJ("aBody", "OBJ//Character//body.obj");
	meshList[GEO_ALEXIS_BODY]->textureID = LoadTGA("Image//bodytext.tga");
	meshList[GEO_ALEXIS_CROTCH] = MeshBuilder::GenerateOBJ("aCrotch", "OBJ//Character//crotch.obj");
	meshList[GEO_ALEXIS_CROTCH]->textureID = LoadTGA("Image//crotchtext.tga");
	meshList[GEO_ALEXIS_RIGHTARM] = MeshBuilder::GenerateOBJ("aRightArm", "OBJ//Character//rightarm.obj");
	meshList[GEO_ALEXIS_RIGHTARM]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXIS_LEFTARM] = MeshBuilder::GenerateOBJ("aLeftArm", "OBJ//Character//leftarm.obj");
	meshList[GEO_ALEXIS_LEFTARM]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXIS_RIGHTLEG] = MeshBuilder::GenerateOBJ("aRightLeg", "OBJ//Character//rightleg.obj");
	meshList[GEO_ALEXIS_RIGHTLEG]->textureID = LoadTGA("Image//shoetext.tga");
	meshList[GEO_ALEXIS_LEFTLEG] = MeshBuilder::GenerateOBJ("aLeftLeg", "OBJ//Character//leftleg.obj");
	meshList[GEO_ALEXIS_LEFTLEG]->textureID = LoadTGA("Image//shoetext.tga");

	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.loadBB("OBJ//Character//crotch.obj");
}

void StudioProject2Scene1::RenderMesh(Mesh *mesh, bool enableLight)
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

void StudioProject2Scene1::RenderText(Mesh* mesh, std::string text, Color color)
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

void StudioProject2Scene1::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void StudioProject2Scene1::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position)
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

void StudioProject2Scene1::RenderLightStands()
{
	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(50, 0, -20);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(-20, 0, -5);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(-60, -10, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(135, -18, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(240, -86, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(340, -165, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(450, -253, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(580, -253, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(680, -253, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.Translate(780, -253, -10);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_LIGHTSTAND], true);
	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	RenderMesh(meshList[GEO_LIGHTBULB], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void StudioProject2Scene1::RenderTrees()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -20, -60);
	modelStack.Scale(5, 5, 5);
	for (unsigned i = 0; i < 3; ++i)
	{
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.Translate(-10, 0, 0);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, -30, -120);
	modelStack.Scale(5, 5, 5);
	for (unsigned i = 0; i < 3; ++i)
	{
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.Translate(-10, 0, 0);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(440, -256, 20);
	modelStack.Scale(4, 4, 4);
	modelStack.Rotate(-180, 0, 1, 0);
	for (unsigned i = 0; i < 5; ++i)
	{
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.Translate(-20, 0, 0);
	}
	modelStack.PopMatrix();

	//Cluster
	modelStack.PushMatrix();
	modelStack.Translate(70, -11, -70);
	modelStack.Scale(4.5, 4.5, 4.5);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_CLUSTERTREE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, -256, -80);
	modelStack.Scale(4.5, 4.5, 4.5);
	modelStack.Rotate(180, 0, 1, 0);
	for (unsigned i = 0; i < 10; ++i)
	{
		RenderMesh(meshList[GEO_CLUSTERTREE], true);
		modelStack.Translate(-28, 0, 0);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(370, -256, -130);
	modelStack.Scale(5, 5, 5);
	modelStack.Rotate(180, 0, 1, 0);
	for (unsigned i = 0; i < 10; ++i)
	{
		RenderMesh(meshList[GEO_CLUSTERTREE], true);
		modelStack.Translate(-20, 0, 0);
	}
	modelStack.PopMatrix();
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