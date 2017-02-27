#include "StudioProject2_MainMenu.h"
#include "StudioProject2_Scene1.h"
#include "StudioProject2_Scene2.h"
#include "DeathScreen.h"
#include "SceneBoss.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Mtx44.h"
#include "Application.h"
#include "EnemyClassManager.h"
#include "Vertex.h"
#include "Utility.h"
#include "shader.hpp"
#include "LoadTGA.h"
#include "Camera.h"
#include "SceneManager.h"

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34


DeathScreen::DeathScreen()
{
}

DeathScreen::~DeathScreen()
{
}

void DeathScreen::Init()
{
	// Init VBO here
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights"); //in case you missed out practical 7
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
		meshList[i] = NULL;

	camera.Init(Vector3(1, 20, 20), Vector3(0, 0, 0), Vector3(0, 1, 0));

	
	/*--------------------------HUD Loading---------------------------------------*/
	meshList[GEO_DEATHSCREEN] = MeshBuilder::GenerateQuad("deathscreen", Color(1, 1, 1));
	meshList[GEO_DEATHSCREEN]->textureID = LoadTGA("Image//deathscreen.tga");
	meshList[GEO_HALF_COUNT] = MeshBuilder::GenerateQuad("hudhalf", Color(1, 1, 1));
	meshList[GEO_HALF_COUNT]->textureID = LoadTGA("Image//halfhud.tga");
	meshList[GEO_FULL_COUNT] = MeshBuilder::GenerateQuad("hudfull", Color(1, 1, 1));
	meshList[GEO_FULL_COUNT]->textureID = LoadTGA("Image//fullhud.tga");
	/*-----------------------------------------------------------------------------*/

	/*--------------------------Text Loading---------------------------------------*/
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//franklingothicheavy.tga");
	/*------------------------------------------------------------------------------*/

	/*---------------------------Initialising Variables---------------------------------*/


	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);
	/*-----------------------------------------------------------------------------*/

}

void DeathScreen::Update(double dt)
{
	int framespersec = 1 / dt;
	//elapsedTime += dt;
	camera.Update(dt);

	/*-----------Updates the FPS to be stated on screen---------*/
	fps = "FPS:" + std::to_string(framespersec);
	hMutantSaved = ":" + std::to_string(PlayerClass::get_instance()->hm_Saved);
	fMutantKilled = ":" + std::to_string(PlayerClass::get_instance()->fm_Killed);
	timer = "Time:" + std::to_string(PlayerClass::get_instance()->timeSpend) + "s";
	deathLocation = "Location:" + SceneManager::getInstance()->Location;
	/*----------------------------------------------------------*/

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xbox;
		const unsigned char *xboxs = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xbox);
		if (Application::IsKeyPressed(VK_RETURN) || GLFW_PRESS == xboxs[7])
		{
			EnemyManager::get_instance()->EnemyList[0]->restartLevel();
			PlayerClass::get_instance()->timeSpend = 0.0f;
			SceneManager::getInstance()->changeScene(new StudioProject2MainMenu());
		}
		if (Application::IsKeyPressed(VK_SPACE) || GLFW_PRESS == xboxs[0])
		{
			if (SceneManager::getInstance()->Location == "Secluded Forest")
			{
				PlayerClass::get_instance()->restartLevel();
				PlayerClass::get_instance()->timeSpend = 0.0f;
				EnemyManager::get_instance()->EnemyList[0]->restartLevel();
				SceneManager::getInstance()->changeScene(new StudioProject2Scene1());
			}
			if (SceneManager::getInstance()->Location == "Inner City")
			{
				PlayerClass::get_instance()->restartLevel();
				SceneManager::getInstance()->changeScene(new StudioProject2Scene2());
			}
		}
	}
	else
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			EnemyManager::get_instance()->EnemyList[0]->restartLevel();
			PlayerClass::get_instance()->timeSpend = 0.0f;
			SceneManager::getInstance()->changeScene(new StudioProject2MainMenu());
		}
		if (Application::IsKeyPressed(VK_SPACE))
		{
			if (SceneManager::getInstance()->Location == "Secluded Forest")
			{
				PlayerClass::get_instance()->restartLevel();
				PlayerClass::get_instance()->timeSpend = 0.0f;
				EnemyManager::get_instance()->EnemyList[0]->restartLevel();
				SceneManager::getInstance()->changeScene(new StudioProject2Scene1());
			}
			if (SceneManager::getInstance()->Location == "Inner City")
			{
				PlayerClass::get_instance()->restartLevel();
				SceneManager::getInstance()->changeScene(new StudioProject2Scene2());
			}
		}
	}
}

void DeathScreen::Render()
{
	// Render VBO here
	Mtx44 MVP;

	// Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	modelStack.LoadIdentity();

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	RenderMeshOnScreen(meshList[GEO_DEATHSCREEN], 0, 0, 80, 60, 0);
	RenderMeshOnScreen(meshList[GEO_FULL_COUNT], 7, 3, 4, 4, 0);
	RenderMeshOnScreen(meshList[GEO_HALF_COUNT], 7, 2, 4, 4, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], fMutantKilled, Color(1, 1, 1), 3, 12, -5.2);
	RenderTextOnScreen(meshList[GEO_TEXT], hMutantSaved, Color(1, 1, 1), 3, 12, -6.8);
	RenderTextOnScreen(meshList[GEO_TEXT], timer, Color(1, 1, 1), 3, 9.2, -4);
	RenderTextOnScreen(meshList[GEO_TEXT], deathLocation, Color(1, 1, 1), 3, 6.5, -3);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press <Space>/<A> to Restart the Level", Color(1, 1, 1), 2, 9, -8);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press <Enter>/<Start> to Return to Main Menu", Color(1, 1, 1), 2, 9, -7);
	RenderTextOnScreen(meshList[GEO_TEXT], "Press <Esc> to Exit Game", Color(1, 1, 1), 2, 9, -9);
	RenderTextOnScreen(meshList[GEO_TEXT], fps, Color(0, 1, 0), 2, 36, 19);
}

void DeathScreen::RenderMesh(Mesh *mesh, bool enableLight)
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

void DeathScreen::RenderText(Mesh* mesh, std::string text, Color color)
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

void DeathScreen::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void DeathScreen::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position)
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

void DeathScreen::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}