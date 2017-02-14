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

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34

bool MouseControl;

StudioProject2Scene1::StudioProject2Scene1()
{
}

StudioProject2Scene1::~StudioProject2Scene1()
{
}

void StudioProject2Scene1::Init()
{
	// Init VBO here
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

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

	meshList[GEO_AXIS] = MeshBuilder::GenerateAxis("reference");


	// Optional if using default

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1.f, 1.f, 1.f), 18, 36, 1.f);

	/*-----------------------------Skybox Loading----------------------------------*/
	/*-----------------------------------------------------------------------------*/

	/*-----------------Environment Objects Loading---------------------------------*/
	meshList[GEO_HOUSE] = MeshBuilder::GenerateOBJ("house", "OBJ//Scene1//House_AllElse.obj");
	meshList[GEO_HOUSEFLOOR] = MeshBuilder::GenerateOBJ("hfloor", "OBJ//Scene1//House_Floor.obj");
	meshList[GEO_HOUSELEFTWALL] = MeshBuilder::GenerateOBJ("hlwall", "OBJ//Scene1//House_Left_Wall.obj");
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("wall", "OBJ//Scene1//TEMP_Hill+Wall.obj");
	/*-----------------------------------------------------------------------------*/

	/*--------------------------Mutants Loading------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	
	/*--------------------------Character Loading----------------------------------*/
	PlayerClass* Alexis = PlayerClass::get_instance();

	meshList[GEO_ALEXIS_HEAD] = MeshBuilder::GenerateOBJ("aHead", "OBJ//Character//facehair.obj");
	meshList[GEO_ALEXIS_HEAD]->textureID = LoadTGA("Image//facehairtext.tga");
	meshList[GEO_ALEXIS_BODY] = MeshBuilder::GenerateOBJ("aBody", "OBJ//Character//body.obj");
	meshList[GEO_ALEXIS_BODY]->textureID = LoadTGA("Image//bodytext.tga");
	meshList[GEO_ALEXIS_CROTCH] = MeshBuilder::GenerateOBJ("aCrotch", "OBJ//Character//crotch.obj");
	meshList[GEO_ALEXIS_CROTCH]->textureID = LoadTGA("Image//crotchtext.tga");
	meshList[GEO_ALEXIS_RIGHTHAND] = MeshBuilder::GenerateOBJ("aRightHand", "OBJ//Character//RightHand.obj");
	meshList[GEO_ALEXIS_RIGHTHAND]->textureID = LoadTGA("Image//righthandtext.tga");
	meshList[GEO_ALEXIS_LEFTHAND] = MeshBuilder::GenerateOBJ("aLeftHand", "OBJ//Character//LeftHand.obj");
	meshList[GEO_ALEXIS_LEFTHAND]->textureID = LoadTGA("Image//lefthandtext.tga");
	meshList[GEO_ALEXIS_RIGHTARM] = MeshBuilder::GenerateOBJ("aRightArm", "OBJ//Character//RightArm.obj");
	meshList[GEO_ALEXIS_RIGHTARM]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXIS_LEFTARM] = MeshBuilder::GenerateOBJ("aLeftArm", "OBJ//Character//LeftArm.obj");
	meshList[GEO_ALEXIS_LEFTARM]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXIS_RIGHTTHIGH] = MeshBuilder::GenerateOBJ("aRightThigh", "OBJ//Character//RightThigh.obj");
	meshList[GEO_ALEXIS_RIGHTTHIGH]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXIS_LEFTTHIGH] = MeshBuilder::GenerateOBJ("aLeftThigh", "OBJ//Character//LeftThigh.obj");
	meshList[GEO_ALEXIS_LEFTTHIGH]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXIS_RIGHTLEG] = MeshBuilder::GenerateOBJ("aRightLeg", "OBJ//Character//RightLeg.obj");
	meshList[GEO_ALEXIS_RIGHTLEG]->textureID = LoadTGA("Image//shoetext.tga");
	meshList[GEO_ALEXIS_LEFTLEG] = MeshBuilder::GenerateOBJ("aLeftLeg", "OBJ//Character//LeftLeg.obj");
	meshList[GEO_ALEXIS_LEFTLEG]->textureID = LoadTGA("Image//shoetext.tga");
	/*-----------------------------------------------------------------------------*/

	/*--------------------------Text Loading---------------------------------------*/
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//franklingothicheavy.tga");
	/*-----------------------------------------------------------------------------*/

	/*------------------------Initialising Text Variables-------------------------------*/
	spawnTS = 2;

	syringeTriggedText = false;
	syringeTriggedTS = 0;

	boxTriggedText = false;
	boxTriggedTS = 0;
	boxTriggedText_Two = false;
	boxTriggedTS_two = 0;

	hmTriggeredText = false;
	hmTriggedTS = 0;

	hm_to_alexis = false;
	hm_to_alexisTS = 0;

	alexis_to_hm = false;
	alexis_to_hmTS = 0;

	alexis_beside_hm = false;
	alexis_beside_hmTS = 0;

	postProjectileThrownText = false;
	postProjectileThrownTS = 0;

	fm_triggedText = false;
	fm_triggedTS = 0;

	alexisText = false;
	alexisTS = 0;

	guideText = false;
	guideTS = 0;

	/*----------------------------------------------------------------------------------*/

	/*---------------------------Initialising Variables---------------------------------*/
	MouseControl = false;


	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);
	/*-----------------------------------------------------------------------------*/

	/*----------------------Light Initialisation-----------------------------------*/
	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(1.f, 105, -56.5f);
	light[0].color.Set(1, 1, 1);
	light[0].power = 7;
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

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	/*-------------------------------------------------------------------------------*/
}

void StudioProject2Scene1::Update(double dt)
{
	int framespersec = 1 / dt;
	//elapsedTime += dt;
	camera.Update(dt);

	/*-----------Updates the FPS to be stated on screen---------*/
	fps = "FPS:" + std::to_string(framespersec);
	/*----------------------------------------------------------*/

	if (Application::IsKeyPressed(VK_1))
		glEnable(GL_CULL_FACE);

	if (Application::IsKeyPressed(VK_2))
		glDisable(GL_CULL_FACE);

	if (Application::IsKeyPressed(VK_3))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (Application::IsKeyPressed(VK_4))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//float LSPEED = 10.f;
	//if (Application::IsKeyPressed('I'))
	//	light[0].position.z -= (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('K'))
	//	light[0].position.z += (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('J'))
	//	light[0].position.x -= (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('L'))
	//	light[0].position.x += (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('O'))
	//	light[0].position.y -= (float)(LSPEED * dt);
	//if (Application::IsKeyPressed('P'))
	//	light[0].position.y += (float)(LSPEED * dt);

	/*-----------------------Text Interaction----------------*/
	//if () [Movement will remove the spawn text]
	//{
	//}

	//if () [When near syringe, then make text appear]
	//{
	//}

	//if () [Get coordinates beside box]
	//{
	//	  boxTriggedTS = 2;
	//    boxTriggedText = true;
	//}

	if (boxTriggedText == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			boxTriggedText_Two = true;
			boxTriggedTS = 0;
			boxTriggedTS_two = 2;
			boxTriggedText = false;
		}
	}

	//if () [When alexis at wall, he sees the half mutant and text gets triggered]
	//{ hmTriggeredText = true;
	//}

	if (hmTriggeredText == true) 
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			hm_to_alexis = true;
			hmTriggedTS = 0;
			hm_to_alexisTS = 2;
			hmTriggeredText = false;
		}
	}

	if (hm_to_alexis == true)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			alexis_to_hm = true;
			hm_to_alexisTS = 0;
			alexis_to_hmTS = 2;
			hm_to_alexis = false;
		}
	}

	//if (alexis_to_hm == true && [insert coordinates (right beside half mutant)])
	//{
	//      alexis_beside_hm = true;
	//		alexis_to_hmTS = 0;
	//		alexis_beside_hmTS = 2;
	//		alexis_to_hm = false;
	//}

	//if (alexis_beside_hm == true)
	//{
	//   if (Application::IsKeyPressed((VK_RETURN))
	//   {
	//		alexis_beside_hmTS = 0;
	//		alexis_beside_hm = false;
	//   }
	//}

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

void StudioProject2Scene1::text()
{
	//fill in when necessary
}

void StudioProject2Scene1::Render()
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

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_BODY], false);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_HEAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_LEFTARM], false);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_LEFTHAND], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_RIGHTARM], false);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_RIGHTHAND], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_CROTCH], false);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_LEFTTHIGH], false);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_LEFTLEG], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix(); 

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_RIGHTTHIGH], false);
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ALEXIS_RIGHTLEG], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_WALL], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOUSELEFTWALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOUSEFLOOR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOUSE], false);
	modelStack.PopMatrix();

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

	RenderTextOnScreen(meshList[GEO_TEXT], fps, Color(0, 1, 0), 2, 36, 19);
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

void StudioProject2Scene1::RenderSkybox()
{
	// Person in charge of implementing skybox, pls type the codes (rotate,transform,scale) here
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

void StudioProject2Scene1::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}