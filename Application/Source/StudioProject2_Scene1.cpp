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
#include "EnemyClassManager.h"
#include "EnemyClass.h"
#include <vector>

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
	PlayerClass::get_instance();
	/*----Player & AI & Camera Variables----*/

	EnemyManager::get_instance()->spawnEnemy(Vector3(750.f, -252.2f, 0.f));
	PlayerClass::get_instance()->position_a = Vector3(-15.f,0.f,0.f);

	/* Hearts size (User Interface) Initialisation--------------*/
	PlayerClass::get_instance()->Hearts.heartCounter = PlayerClass::get_instance()->get_health() / 10;

	if (PlayerClass::get_instance()->Hearts.heartCounter == 10) // full health then just do this
	{
		for (int i = 0; i < 10; i++) 
		{
			PlayerClass::get_instance()->Hearts.a_heart[i] = 2;
			PlayerClass::get_instance()->Hearts.a_blankheart[i] = 0;
		}
	}
	else // if not full health, shuld init the size of the blankhearts and red hearts
	{
		for (int i = 0; i < PlayerClass::get_instance()->Hearts.heartCounter; i++)
		{
			PlayerClass::get_instance()->Hearts.a_heart[i] = 2;
			PlayerClass::get_instance()->Hearts.a_blankheart[i] = 0;
		}
		for (int i = PlayerClass::get_instance()->Hearts.heartCounter; i < 10; i++)
		{
			PlayerClass::get_instance()->Hearts.a_heart[i] = 0;
			PlayerClass::get_instance()->Hearts.a_blankheart[i] = 2;
		}
	}
	/*-------------------------------------------------------------------------------*/
	// Init VBO here
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); // was enabled but disable it for skybox and leg model
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
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights"); //in case you missed out practical 7
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

	camera.Init(Vector3(PlayerClass::get_instance()->position_a.x,PlayerClass::get_instance()->position_a.y,PlayerClass::get_instance()->position_a.z + 120),
		Vector3(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y, PlayerClass::get_instance()->position_a.z),
		Vector3(0, 1, 0));

	meshList[GEO_AXIS] = MeshBuilder::GenerateAxis("reference");
	
	// Optional if using default

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1.f, 1.f, 1.f), 18, 36, 1.f);

	/*-----------------------------Skybox Loading----------------------------------*/
	meshList[GEO_SKYBOX] = MeshBuilder::GenerateQuad("skybox", Color(1, 1, 1));
	meshList[GEO_SKYBOX]->textureID = LoadTGA("Image//SkyBG.tga");
	/*-----------------------------------------------------------------------------*/

	/*-----------------Environment Objects Loading---------------------------------*/
	meshList[GEO_HOUSE] = MeshBuilder::GenerateOBJ("house", "OBJ//Scene1//House_AllElse.obj");
	meshList[GEO_HOUSE]->textureID = LoadTGA("Image//housetexture.tga");
	meshList[GEO_HOUSEFLOOR] = MeshBuilder::GenerateOBJ("hfloor", "OBJ//Scene1//House_Floor.obj");
	meshList[GEO_HOUSEFLOOR]->textureID = LoadTGA("Image//housetexture.tga");
	meshList[GEO_HOUSELEFTWALL] = MeshBuilder::GenerateOBJ("hlwall", "OBJ//Scene1//House_Left_Wall.obj");
	meshList[GEO_HOUSELEFTWALL]->textureID = LoadTGA("Image//housetexture.tga");
	meshList[GEO_HOUSEFRONT] = MeshBuilder::GenerateOBJ("HouseFront", "OBJ//Scene1//House_Front.obj"); // No texture for this its hidden beneath ground
	meshList[GEO_BOX_SHORT] = MeshBuilder::GenerateOBJ("Box_Short", "OBJ//Scene1//Box_Short.obj");
	meshList[GEO_BOX_TALL] = MeshBuilder::GenerateOBJ("Box_Tall", "OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_HILL] = MeshBuilder::GenerateOBJ("Hill", "OBJ//Scene1//Hill.obj");
	meshList[GEO_HILL]->textureID = LoadTGA("Image//hills.tga");
	meshList[GEO_FLOOR] = MeshBuilder::GenerateOBJ("Floor", "OBJ//Scene1//Floor.obj");
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//ground.tga");
	meshList[GEO_TRUMP] = MeshBuilder::GenerateOBJ("Trump", "OBJ//Scene1//Trump.obj");
	meshList[GEO_BOX_SHORTTEST] = MeshBuilder::GenerateOBJ("Box_Short", "OBJ//Scene1//Box_Short.obj"); // THE TEST NAME AT THE END
	meshList[GEO_BOX_TALLTEST] = MeshBuilder::GenerateOBJ("Box_Tall", "OBJ//Scene1//Box_Tall.obj");	   // INDICATES I NEED IT TO TEST FOR
	meshList[GEO_TRUMPTEST] = MeshBuilder::GenerateOBJ("Trump", "OBJ//Scene1//Trump.obj");			   // COLLISION SO YOU CAN STAND ON IT

	meshList[GEO_HOUSEFLOOR]->MeshBBox.loadBB("OBJ//Scene1//House_Floor.obj");
	meshList[GEO_HOUSELEFTWALL]->MeshBBox.loadBB("OBJ//Scene1//House_Left_Wall.obj");
	meshList[GEO_HOUSEFRONT]->MeshBBox.loadBB("OBJ//Scene1//House_Front.obj");
	meshList[GEO_BOX_SHORT]->MeshBBox.loadBB("OBJ//Scene1//Box_Short.obj");
	meshList[GEO_BOX_SHORT]->MeshBBox.scale(1, 1.5f, 1);
	meshList[GEO_BOX_SHORT]->MeshBBox.translate(490, -250.f, 0);
	meshList[GEO_BOX_SHORTTEST]->MeshBBox.loadBB("OBJ//Scene1//Box_Short.obj");
	meshList[GEO_BOX_SHORTTEST]->MeshBBox.scale(1, 1.7f, 1);
	meshList[GEO_BOX_SHORTTEST]->MeshBBox.translate(490, -250.f, 0);
	meshList[GEO_BOX_TALL]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_BOX_TALL]->MeshBBox.scale(1, 1.6f, 1);
	meshList[GEO_BOX_TALL]->MeshBBox.translate(500, -250.f, 0);
	meshList[GEO_BOX_TALLTEST]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_BOX_TALLTEST]->MeshBBox.scale(1, 1.7f, 1);
	meshList[GEO_BOX_TALLTEST]->MeshBBox.translate(500, -250.f, 0);
	meshList[GEO_FLOOR]->MeshBBox.loadBB("OBJ//Scene1//Floor.obj");
	meshList[GEO_FLOOR]->MeshBBox.scale(1.5f, 1, 1);
	meshList[GEO_FLOOR]->MeshBBox.translate(100, -253.5f, 0);
	meshList[GEO_TRUMP]->MeshBBox.loadBB("OBJ//Scene1//Trump.obj");
	meshList[GEO_TRUMP]->MeshBBox.scale(1, 1.5f, 1);
	meshList[GEO_TRUMP]->MeshBBox.translate(550, -250.f, 0);
	meshList[GEO_TRUMPTEST]->MeshBBox.loadBB("OBJ//Scene1//Trump.obj");
	meshList[GEO_TRUMPTEST]->MeshBBox.scale(0.75f, 1.7f, 1);
	meshList[GEO_TRUMPTEST]->MeshBBox.translate(550.1f, -250.f, 0);

	meshList[GEO_LIGHTBULB] = MeshBuilder::GenerateOBJ("bulb", "OBJ//Scene1//lighttop.obj");
	meshList[GEO_LIGHTBULB]->textureID = LoadTGA("Image//lighttext.tga");
	meshList[GEO_LIGHTSTAND] = MeshBuilder::GenerateOBJ("lightstand", "OBJ//Scene1//lightbottom.obj");
	meshList[GEO_LIGHTSTAND]->textureID = LoadTGA("Image//lighttext.tga");

	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//tree.obj");
	meshList[GEO_TREE]->textureID = LoadTGA("Image//tree.tga");

	meshList[GEO_CLUSTERTREE] = MeshBuilder::GenerateOBJ("clustertree", "OBJ//clustertree.obj");
	meshList[GEO_CLUSTERTREE]->textureID = LoadTGA("Image//tree.tga");

	meshList[GEO_DEBRIS] = MeshBuilder::GenerateOBJ("debris", "OBJ//Debri.obj");
	meshList[GEO_DEBRIS]->textureID = LoadTGA("Image//Debri_Texture.tga");
	/*-----------------------------------------------------------------------------*/

	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("textbox", Color(0, 0, 0));
	/*--------------------------Mutants Loading------------------------------------*/
	meshList[GEO_MUTANT_HEAD] = MeshBuilder::GenerateOBJ("aHead", "OBJ//Mutant_UpdatedOBJ//Mutant_Head.obj");
	meshList[GEO_MUTANT_HEAD]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_LEFTARM] = MeshBuilder::GenerateOBJ("aBody", "OBJ//Mutant_UpdatedOBJ//Mutant_LeftArm.obj");
	meshList[GEO_MUTANT_LEFTARM]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_LEFTFEET] = MeshBuilder::GenerateOBJ("aCrotch", "OBJ//Mutant_UpdatedOBJ//Mutant_LeftFeet.obj");
	meshList[GEO_MUTANT_LEFTFEET]->textureID = LoadTGA("Image//Mutant_Texture.tga");;
	meshList[GEO_MUTANT_LEFTTHIGH] = MeshBuilder::GenerateOBJ("aRightArm", "OBJ//Mutant_UpdatedOBJ//Mutant_LeftThigh.obj");
	meshList[GEO_MUTANT_LEFTTHIGH]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_LEFTUPPERARM] = MeshBuilder::GenerateOBJ("aLeftArm", "OBJ//Mutant_UpdatedOBJ//Mutant_LeftUpperarm.obj");
	meshList[GEO_MUTANT_LEFTUPPERARM]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_NECK] = MeshBuilder::GenerateOBJ("aRightLeg", "OBJ//Mutant_UpdatedOBJ//Mutant_Neck.obj");
	meshList[GEO_MUTANT_NECK]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_RIGHTARM] = MeshBuilder::GenerateOBJ("aLeftLeg", "OBJ//Mutant_UpdatedOBJ//Mutant_RightArm.obj");
	meshList[GEO_MUTANT_RIGHTARM]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_RIGHTFEET] = MeshBuilder::GenerateOBJ("aLeftLeg", "OBJ//Mutant_UpdatedOBJ//Mutant_RightFeet.obj");
	meshList[GEO_MUTANT_RIGHTFEET]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_RIGHTTHIGH] = MeshBuilder::GenerateOBJ("aLeftLeg", "OBJ//Mutant_UpdatedOBJ//Mutant_RightThigh.obj");
	meshList[GEO_MUTANT_RIGHTTHIGH]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_RIGHTUPPERARM] = MeshBuilder::GenerateOBJ("aLeftLeg", "OBJ//Mutant_UpdatedOBJ//Mutant_RightUpperarm.obj");
	meshList[GEO_MUTANT_RIGHTUPPERARM]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_MUTANT_TORSO] = MeshBuilder::GenerateOBJ("aLeftLeg", "OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");
	meshList[GEO_MUTANT_TORSO]->textureID = LoadTGA("Image//Mutant_Texture.tga");
	meshList[GEO_SPIT] = MeshBuilder::GenerateOBJ("Spit", "OBJ//Scene1//Box_Short.obj"); //box short placeholder for spit projectile



	//meshList[GEO_MUTANT_TORSO]->MeshBBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");
	//meshList[GEO_MUTANT_TORSO]->MeshBBox.scale(1.f, 2.1f, 1.f);
	//meshList[GEO_MUTANT_TORSO]->MeshBBox.translate(750.f, -248.8f, 0); // y + 1.2f. EG: if i want y at -250, it'd be -250 + 1.2 = 248.8

	/*-----------------------------------------------------------------------------*/
	
	/*--------------------------Character Loading----------------------------------*/
	LoadCharacter();
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

	PlayerClass::get_instance()->PlayerHitBox.loadBB("OBJ//Character//crotch.obj");

	/*-----------------------------------------------------------------------------*/

	/*--------------------------Text Loading---------------------------------------*/
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//franklingothicheavy.tga");
	/*-----------------------------------------------------------------------------*/

	/*-----------------------------Trigger Check-----------------------------------*/
	meshList[GEO_TRIGGER_SLOPE] = MeshBuilder::GenerateOBJ("Trigger_Slope", "OBJ//TriggerBox.obj");
	meshList[GEO_TRIGGER_SLOPE]->MeshBBox.loadBB("OBJ//TriggerBox.obj");
	meshList[GEO_TRIGGER_SLOPE]->MeshBBox.translate(-12.f, 10.f, 0);
	/*-----------------------------------------------------------------------------*/

	/*-----------------------------Checking BBox-----------------------------------*/
	meshList[GEO_BBOX] = MeshBuilder::GenerateBB("CharBox", PlayerClass::get_instance()->PlayerHitBox.max_, PlayerClass::get_instance()->PlayerHitBox.min_);
	/*-----------------------------------------------------------------------------*/ 
	
	/*-------------------------Loading Hearts-----------------------------------------*/
	meshList[GEO_HEART] = MeshBuilder::GenerateQuad("heart", Color(1, 0, 0));
	meshList[GEO_BLANKHEART] = MeshBuilder::GenerateQuad("blankheart", Color(0, 0, 0));
	/*--------------------------------------------------------------------------------*/

	/*------------------------Initialising Text Variables-------------------------------*/
	spawnTS = 2;
	pressEnterTS = 0;

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

	pEnter = false;
	textOccurStorage = 0;
	textOccured = 0;
	nexttext = false;
	/*----------------------------------------------------------------------------------*/

	/*---------------------------Initialising Variables---------------------------------*/
	MouseControl = false;
	ShortBox_PosX = 0.f;
	TallBox_PosX = 0.f;
	bufferTime_JumpUp = elapsedTime - 1.f;
	bufferTime_Jump = elapsedTime - 1.f;
	bufferTime_attack = elapsedTime - 1.f;
	bufferTime_text = elapsedTime - 1.f;
	bufferTime_trigger_slope = elapsedTime - 1.f;
	bufferTime_grab = elapsedTime - 1.f;

	/*-----Character--------*/
	pressedA = false;
	pressedD = false;
	injump = false;
	infall = true;
	attack = false;
	trigger = false;
	grab = false;
	/*----------------------*/

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);
	/*-----------------------------------------------------------------------------*/

	/*----------------------Light Initialisation-----------------------------------*/
	LoadLight();
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	/*-------------------------------------------------------------------------------*/
}

double et[30];
/*  Alexis:
	0 = attack
	1 = idle

	Half-Mutant:

	Mutant:
	20 = idle
*/
void StudioProject2Scene1::Update(double dt)
{
	int framespersec = 1 / dt;
	elapsedTime += dt;
	camera.Update(dt, PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y);

	/*-------AI Functions---------------*/
	RenderProjectiles();
	EnemyManager::get_instance()->EnemyList[0]->update(dt);
	/*-------Player Functions------------------*/
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->facingDirection();
	/*-----------------------------------------*/

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

	/*------------------------------Collision Check------------------------------*/
	if (!otheranims())
	{
		for (unsigned i = 0; i < 9; i++)
			et[i] = 0;
	}

	if (elapsedTime > 1.1f) // This pre-setting ensures animations won't occur at the very start, so animations glitching out will not happen anymore.*
	{						// *I hope.

		if (Application::IsKeyPressed('A') && !trigger)
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox)  ||
				pressedD == true)
			{
				PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
				pressedD = false;
				pressedA = true;

				if (grab)
				{
					if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_SHORT]->MeshBBox))
					{
						ShortBox_PosX -= (float)(30.f * dt);
						meshList[GEO_BOX_SHORT]->MeshBBox.translate(-((float)(30.f * dt)), 0, 0);
						meshList[GEO_BOX_SHORTTEST]->MeshBBox.translate(-((float)(30.f * dt)), 0, 0);
					}
					else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_TALL]->MeshBBox) && !meshList[GEO_BOX_TALL]->MeshBBox.collide(meshList[GEO_BOX_SHORT]->MeshBBox))
					{
						TallBox_PosX -= (float)(30.f * dt);
						meshList[GEO_BOX_TALL]->MeshBBox.translate(-((float)(30.f * dt)), 0, 0);
						meshList[GEO_BOX_TALLTEST]->MeshBBox.translate(-((float)(30.f * dt)), 0, 0);
					}
				}
			}
		}
		if (Application::IsKeyPressed('D') && !trigger)
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox) 
				 ||	pressedA == true)
			{
				PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
				pressedA = false;
				pressedD = true;

				if (grab)
				{
					if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_SHORT]->MeshBBox) && !meshList[GEO_BOX_SHORT]->MeshBBox.collide(meshList[GEO_BOX_TALL]->MeshBBox))
					{
						ShortBox_PosX += (float)(30.f * dt);
						meshList[GEO_BOX_SHORT]->MeshBBox.translate(((float)(30.f * dt)), 0, 0);
						meshList[GEO_BOX_SHORTTEST]->MeshBBox.translate(((float)(30.f * dt)), 0, 0);
					}
					else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_TALL]->MeshBBox) && !meshList[GEO_BOX_TALL]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox))
					{
						TallBox_PosX += (float)(30.f * dt);
						meshList[GEO_BOX_TALL]->MeshBBox.translate(((float)(30.f * dt)), 0, 0);
						meshList[GEO_BOX_TALLTEST]->MeshBBox.translate(((float)(30.f * dt)), 0, 0);
					}
				}
			}
		}
		if (Application::IsKeyPressed('W') && (bufferTime_Jump < elapsedTime) && !trigger)
		{
			bufferTime_Jump = elapsedTime + 0.6f;
			bufferTime_JumpUp = elapsedTime + 0.3f;
		}
		if (Application::IsKeyPressed(VK_LBUTTON) && (bufferTime_attack < elapsedTime) && !trigger)
		{
			bufferTime_attack = elapsedTime + 1;

			EnemyManager::get_instance()->EnemyList[0]->attack(true, 1, EnemyManager::get_instance()->EnemyList[0]->position_m, EnemyManager::get_instance()->EnemyList[0]->direction_m, dt);
			EnemyManager::get_instance()->EnemyList[0]->spit_[0]->projHitBox_.loadBB("OBJ//Scene1//Box_Short.obj");
			meshList[GEO_TESTBBOX] = MeshBuilder::GenerateBB("TestBox", EnemyManager::get_instance()->EnemyList[0]->spit_[0]->projHitBox_.max_, EnemyManager::get_instance()->EnemyList[0]->spit_[0]->projHitBox_.min_);
		}
		if (Application::IsKeyPressed('F'))
		{
			bufferTime_grab = elapsedTime + 0.15f;
		}
	}

	if (bufferTime_JumpUp > elapsedTime)
		injump = true;
	else
		injump = false;

	if (bufferTime_attack > elapsedTime)
	{
		attack = true;
		et[0] += dt;
	}
	else
	{
		attack = false;
	}

	if (bufferTime_grab > elapsedTime)
	{
		grab = true;
		et[1] += dt;
	}
	else
	{
		grab = false;
	}

	et[20] += dt;		// This is for me to see if the idleanim is running at all

	if (!trigger)
	{
		if (injump == false)
		{
			if (/*!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&*/
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSEFLOOR]->MeshBBox) &&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSEFRONT]->MeshBBox) &&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_FLOOR]->MeshBBox) &&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPTEST]->MeshBBox) /*&&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox)*/)
			{
				if ((PlayerClass::get_instance()->PlayerHitBox.higherthan(meshList[GEO_BOX_SHORT]->MeshBBox) &&
					PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_SHORTTEST]->MeshBBox)) ||
					(PlayerClass::get_instance()->PlayerHitBox.higherthan(meshList[GEO_BOX_TALL]->MeshBBox) &&
					PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_TALLTEST]->MeshBBox)))
				{
					// do jack shit
				}
				else
				{
					bufferTime_Jump = elapsedTime + 0.1f; // this fixes a bug I never thought was there in the first place, preventing double jump
					PlayerClass::get_instance()->position_a.y -= (float)(30.f * dt);
				}
			}
		}
		else
		{
			PlayerClass::get_instance()->position_a.y += (float)(30.f * dt);
		}
	}

	if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER_SLOPE]->MeshBBox))
	{
		bufferTime_trigger_slope = elapsedTime + 11.f;
		trigger = true;
	}

	if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPTEST]->MeshBBox) && !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox))
	{
		PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
	}
	
	PlayerClass::get_instance()->PlayerHitBox.loadBB("OBJ//Character//crotch.obj");

	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
		{
			PlayerClass::get_instance()->PlayerHitBox.loadBB("OBJ//Character//crotch.obj");

			EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.loadBB("OBJ//Scene1//Box_Short.obj");

			meshList[GEO_TESTBBOX]->MeshBBox.loadBB("OBJ//Scene1//Box_Short.obj");
		}
	}

	//meshList[GEO_MUTANT_TORSO]->MeshBBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj"); // THIS SNEAKY ASS LINE OF CODE RUINED COLLISION FOR THE PAST HOUR OMG
	/*--------------------------------------------------------*/
	
	

	/*---------Triggers------*/
	if (bufferTime_trigger_slope > elapsedTime && trigger == true)
	{
		if (elapsedTime < (bufferTime_trigger_slope - 10.99f))
			PlayerClass::get_instance()->position_a.y = -3.f;
		PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
		PlayerClass::get_instance()->position_a.y -= (float)(3.25f * dt);
		if (elapsedTime > (bufferTime_trigger_slope - 10.8f))
		{
			PlayerClass::get_instance()->position_a.y -= (float)(10.f * dt);
		}
		if (elapsedTime > (bufferTime_trigger_slope - 8.f))
		{
			PlayerClass::get_instance()->position_a.y -= (float)(12.5f * dt);
		}
	}
	else if (bufferTime_trigger_slope < elapsedTime && trigger == true)
		trigger = false;
	/*--------------------------------------*/
	TextInteraction();
	LightInteraction();
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

	Position lightPosition1_cameraspace = viewStack.Top() * light[1].position;
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition1_cameraspace.x);

	/*modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();*/

	/*-----------------Main Character (Alexis)---------------------*/
	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	PlayerClass::get_instance()->PlayerHitBox.scale(1.1f, 4.5f, 1.f);					// This was a mistake tbh
	PlayerClass::get_instance()->PlayerHitBox.translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);	// I should have put the scale in init
	modelStack.Translate(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y, PlayerClass::get_instance()->position_a.z);								// too late for that now
		modelStack.Rotate(PlayerClass::get_instance()->a_LookingDirection, 0, 1, 0);

		// add in grab animation later

		modelStack.PushMatrix();
		AttackAnim(attack, &modelStack, &et[0], "polySurface9"); // HEAD

		RenderMesh(meshList[GEO_ALEXIS_HEAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AttackAnim(attack, &modelStack, &et[0], "pSphere17");//ARM WITH SWORD

		RenderMesh(meshList[GEO_ALEXIS_LEFTARM], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AttackAnim(attack, &modelStack, &et[0], "polySurface32");//BODY

		RenderMesh(meshList[GEO_ALEXIS_BODY], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AttackAnim(attack, &modelStack, &et[0], "pSphere14");//LEFTARM

		RenderMesh(meshList[GEO_ALEXIS_RIGHTARM], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AttackAnim(attack, &modelStack, &et[0], "pCylinder15");//crotch

		RenderMesh(meshList[GEO_ALEXIS_CROTCH], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AttackAnim(attack, &modelStack, &et[0], "pSphere9");//RIGHT LEG

		RenderMesh(meshList[GEO_ALEXIS_LEFTLEG], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AttackAnim(attack, &modelStack, &et[0], "pSphere10");//LEFTLEG

		RenderMesh(meshList[GEO_ALEXIS_RIGHTLEG], true);
		modelStack.PopMatrix();

	modelStack.PopMatrix();

	modelStack.PushMatrix();							// render collision box
	modelStack.Translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);	// i need this
	modelStack.Scale(1.1f, 4.5f, 1.f);					// if you remove it bad things will happen
	RenderMesh(meshList[GEO_BBOX], false);				// remove this later when showing actual shit of course
	modelStack.PopMatrix();								// :ok_hand:
														// for some reason I needed to flip translate and scale here to fit with the actual hitbox
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_TRIGGER_SLOPE], false);		// note to self don't use "meshList[GEO_SOMESHIT]->MeshBBox.translate(a_PosX, (a_PosY + 8), a_PosZ);" often
	modelStack.PopMatrix();								// this shit runs every second so smallest translations will move by a lot eventually

	modelStack.PushMatrix();
	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
		{
			meshList[GEO_TESTBBOX]->MeshBBox.translate(EnemyManager::get_instance()->EnemyList[0]->spit_[0]->position_.x,
				(EnemyManager::get_instance()->EnemyList[0]->spit_[0]->position_.y + 10.f),
				EnemyManager::get_instance()->EnemyList[0]->spit_[0]->position_.z);
				RenderMesh(meshList[GEO_TESTBBOX], false);			// remove this later when showing actual shit of course
		}
	}
	modelStack.PopMatrix();

	/*-----------------Mutants (Fuglymon)---------------------*/
	RenderProjectiles();

	RenderMutant();
	/*-------------------------------------------------------*/

	/*-------------------------------------------------------*/
	
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOUSELEFTWALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOUSEFLOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HOUSE], true);
	modelStack.PopMatrix();

	/*--------------------trees rendering-----------------*/
	RenderTrees();
	/*-----------------------------------*/

	/*--------Debris------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(600, -253, 40);
	modelStack.Scale(0.2, 0.2, 0.2);
	RenderMesh(meshList[GEO_DEBRIS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -253, 40);
	modelStack.Scale(0.2, 0.2, 0.2);
	modelStack.Rotate(-45, 0, 1, 0);
	RenderMesh(meshList[GEO_DEBRIS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(510, -253, -20);
	modelStack.Scale(0.2, 0.2, 0.2);
	modelStack.Rotate(-105, 0, 1, 0);
	RenderMesh(meshList[GEO_DEBRIS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(515, -253, -20);
	modelStack.Scale(0.2, 0.2, 0.2);
	modelStack.Rotate(-80, 0, 1, 0);
	RenderMesh(meshList[GEO_DEBRIS], true);
	modelStack.PopMatrix();

	/*----------------------------------------------------*/

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HILL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate((490 + ShortBox_PosX), -250.f, 0);
	modelStack.Scale(1.f, 1.5f, 1.f);
	RenderMesh(meshList[GEO_BOX_SHORT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate((500 + TallBox_PosX), -250.f, 0);
	modelStack.Scale(1.f, 1.5f, 1.f);
	RenderMesh(meshList[GEO_BOX_TALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100.f, -255.f, 0);
	modelStack.Scale(1.5f, 1.f, 1.f);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(550.f, -250.f, 0);
	modelStack.Scale(1.f, 1.5f, 1.f);
	RenderMesh(meshList[GEO_TRUMP], true);
	modelStack.PopMatrix();

	/*-----------------Skybox-------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(50, 170, -200);
	modelStack.Scale(1000, 500, 500);
	RenderMesh(meshList[GEO_SKYBOX], false);
	modelStack.PopMatrix();
	/*------------------------------------------*/

	/*-----------------Environmental Light Rendering------*/
	RenderLightStands();
	/*----------------------------------------------------*/

	/*----Textbox Rendering--------*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderMeshOnScreen(meshList[GEO_TEXTBOX], 0, 0, 100, 15, 0);
	/*-----------------------------*/

	/*----Heart Rendering----------*/
		float positionXscreen = 2;
		float positionYscreen = 28.5;
		for (int i = 0; i < 10; i++)
		{
			RenderMeshOnScreen(meshList[GEO_HEART], positionXscreen, positionYscreen,
				PlayerClass::get_instance()->Hearts.a_heart[i], PlayerClass::get_instance()->Hearts.a_heart[i], 0);
			RenderMeshOnScreen(meshList[GEO_BLANKHEART], positionXscreen, positionYscreen,
				PlayerClass::get_instance()->Hearts.a_blankheart[i], PlayerClass::get_instance()->Hearts.a_blankheart[i], 0);

			positionXscreen += 2;
		}
	/*-----------------------------*/

	/*---------------Text log Rendering--------*/
		RenderTextInteractions();
	/*-----------------------------------------*/
	RenderTextOnScreen(meshList[GEO_TEXT], fps, Color(0, 1, 0), 2, 36, 19);
}

bool StudioProject2Scene1::otheranims()
{
	return (injump || infall || attack || trigger || grab);
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

void StudioProject2Scene1::RenderProjectiles()
{
	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
		{
			if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(meshList[GEO_TRUMP]->MeshBBox) || EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->displacement() > 300.f)
			{
				EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->~Projectile();
			}
			else if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(PlayerClass::get_instance()->PlayerHitBox))
			{
				// take damage
				EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->~Projectile();
			}
			else
			{
				if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] == nullptr)
					break;
				modelStack.PushMatrix();
				modelStack.Translate(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.x,
									 (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.y + 10.f),
									 EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.z);
				RenderMesh(meshList[GEO_SPIT], false);
				modelStack.PopMatrix();
			}

			EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.translate(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.x,
				(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.y + 10.f),
				EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.z);
		}
	}
}

void StudioProject2Scene1::RenderMutant()
{
	modelStack.PushMatrix();
	EnemyManager* enemies = EnemyManager::get_instance();

	modelStack.Translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x,
						 EnemyManager::get_instance()->EnemyList[0]->position_m.y,
						 EnemyManager::get_instance()->EnemyList[0]->position_m.z);

	if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == -1)
		modelStack.Rotate(180, 0, 1, 0);
	else if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == 1)
		modelStack.Rotate(0, 0, 1, 0);

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_Head");

	RenderMesh(meshList[GEO_MUTANT_HEAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_LeftArm");

	RenderMesh(meshList[GEO_MUTANT_LEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_LeftFeet");

	RenderMesh(meshList[GEO_MUTANT_LEFTFEET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_LeftThigh");

	RenderMesh(meshList[GEO_MUTANT_LEFTTHIGH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_LeftUpperarm");

	RenderMesh(meshList[GEO_MUTANT_LEFTUPPERARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_Neck");

	RenderMesh(meshList[GEO_MUTANT_NECK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_RightArm");

	RenderMesh(meshList[GEO_MUTANT_RIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_RightFeet");

	RenderMesh(meshList[GEO_MUTANT_RIGHTFEET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_RightThigh");

	RenderMesh(meshList[GEO_MUTANT_RIGHTTHIGH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_RightUpperarm");

	RenderMesh(meshList[GEO_MUTANT_RIGHTUPPERARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	IdleAnim_M(&modelStack, &et[20], "Mutant_Torso");

	RenderMesh(meshList[GEO_MUTANT_TORSO], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}