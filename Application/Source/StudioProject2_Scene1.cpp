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
	/*----Camera & Camera Variables----*/

	fullMutant.EnemyPos.posX = 750.f;
	fullMutant.EnemyPos.posY = -252.2f;
	fullMutant.EnemyPos.posZ = 0.f;
	enemy.push_back(fullMutant);
	enemy[0].update();
	
	PlayerClass::get_instance()->Coord.posX = -15.f;
	PlayerClass::get_instance()->Coord.posY = 0.f;
	PlayerClass::get_instance()->Coord.posZ = 0.f;

	/*---------------------------------*/

	/*--------Heart Variables----------*/
	a_heart1 = 2;
	a_heart2 = 2;
	a_heart3 = 2;
	a_heart4 = 2;
	a_heart5 = 2;
	heartCounter = 5;
	/*--------------------------------*/

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

	camera.Init(Vector3(PlayerClass::get_instance()->Coord.posX,PlayerClass::get_instance()->Coord.posY,PlayerClass::get_instance()->Coord.posZ + 120),
		Vector3(PlayerClass::get_instance()->Coord.posX, PlayerClass::get_instance()->Coord.posY, PlayerClass::get_instance()->Coord.posZ),
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

	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.loadBB("OBJ//Character//crotch.obj");
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
	meshList[GEO_BBOX] = MeshBuilder::GenerateBB("CharBox", meshList[GEO_ALEXIS_CROTCH]->MeshBBox.max_, meshList[GEO_ALEXIS_CROTCH]->MeshBBox.min_);
	meshList[GEO_TESTBBOX] = MeshBuilder::GenerateBB("TestBox", meshList[GEO_MUTANT_TORSO]->MeshBBox.max_, meshList[GEO_MUTANT_TORSO]->MeshBBox.min_);
	meshList[GEO_TESTBBOX]->MeshBBox.scale(1.f, 2.1f, 1.f);
	meshList[GEO_TESTBBOX]->MeshBBox.translate(600.f, -248.8f, 0);
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
	a_LookingDirection = 90.0f;
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
	camera.Update(dt, PlayerClass::get_instance()->Coord.posX, PlayerClass::get_instance()->Coord.posY);
	enemy[0].movement(dt);
	enemy[0].detection();

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

	HeadDirection();
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
			if (!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_MUTANT_TORSO]->MeshBBox) ||
				pressedD == true)
			{
				PlayerClass::get_instance()->Coord.posX -= (float)(30.f * dt);
				pressedD = false;
				pressedA = true;

				if (grab)
				{
					if (meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_SHORT]->MeshBBox))
					{
						ShortBox_PosX -= (float)(30.f * dt);
						meshList[GEO_BOX_SHORT]->MeshBBox.translate(-((float)(30.f * dt)), 0, 0);
						meshList[GEO_BOX_SHORTTEST]->MeshBBox.translate(-((float)(30.f * dt)), 0, 0);
					}
					else if (meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_TALL]->MeshBBox) && !meshList[GEO_BOX_TALL]->MeshBBox.collide(meshList[GEO_BOX_SHORT]->MeshBBox))
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
			if (!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_MUTANT_TORSO]->MeshBBox) ||
				pressedA == true)
			{
				PlayerClass::get_instance()->Coord.posX += (float)(30.f * dt);
				pressedA = false;
				pressedD = true;

				if (grab)
				{
					if (meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_SHORT]->MeshBBox) && !meshList[GEO_BOX_SHORT]->MeshBBox.collide(meshList[GEO_BOX_TALL]->MeshBBox))
					{
						ShortBox_PosX += (float)(30.f * dt);
						meshList[GEO_BOX_SHORT]->MeshBBox.translate(((float)(30.f * dt)), 0, 0);
						meshList[GEO_BOX_SHORTTEST]->MeshBBox.translate(((float)(30.f * dt)), 0, 0);
					}
					else if (meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_TALL]->MeshBBox) && !meshList[GEO_BOX_TALL]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox))
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
			if (/*!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&*/
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSEFLOOR]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSEFRONT]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_FLOOR]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMPTEST]->MeshBBox) /*&&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox)*/)
			{
				if ((meshList[GEO_ALEXIS_CROTCH]->MeshBBox.higherthan(meshList[GEO_BOX_SHORT]->MeshBBox) &&
					meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_SHORTTEST]->MeshBBox)) ||
					(meshList[GEO_ALEXIS_CROTCH]->MeshBBox.higherthan(meshList[GEO_BOX_TALL]->MeshBBox) &&
					meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_TALLTEST]->MeshBBox)))
				{
					// do jack shit
				}
				else
				{
					bufferTime_Jump = elapsedTime + 0.1f; // this fixes a bug I never thought was there in the first place, preventing double jump
					PlayerClass::get_instance()->Coord.posY -= (float)(30.f * dt);
				}
			}
		}
		else
		{
			PlayerClass::get_instance()->Coord.posY += (float)(30.f * dt);
		}
	}

	if (meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRIGGER_SLOPE]->MeshBBox))
	{
		bufferTime_trigger_slope = elapsedTime + 11.f;
		trigger = true;
	}

	if (meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMPTEST]->MeshBBox))
	{
		PlayerClass::get_instance()->Coord.posX += (float)(30.f * dt);
	}
	
	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.loadBB("OBJ//Character//crotch.obj");
	//meshList[GEO_MUTANT_TORSO]->MeshBBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj"); // THIS SNEAKY ASS LINE OF CODE RUINED COLLISION FOR THE PAST HOUR OMG
	/*--------------------------------------------------------*/
	
	/*----------Health System (Hearts)------*/
	if (Application::IsKeyPressed('V'))
	{
		heartCounter--;
		if (heartCounter == 4)
		{
			a_heart5 = 0;
			a_blankheart5 = 2;
		}
		if (heartCounter == 3)
		{
			a_heart4 = 0;
			a_blankheart4 = 2;
		}
		if (heartCounter == 2)
		{
			a_heart3 = 0;
			a_blankheart3 = 2;
		}
		if (heartCounter == 1)
		{
			a_heart2 = 0;
			a_blankheart2 = 2;
		}
		if (heartCounter == 0)
		{
			a_heart1 = 0;
			a_blankheart1 = 2;
		}
	}
	if (Application::IsKeyPressed('C'))
	{
		heartCounter++;
		if (heartCounter == 5)
		{
			a_blankheart5 = 0;
			a_heart5 = 2;
		}
		if (heartCounter == 4)
		{
			a_blankheart4 = 0;
			a_heart4 = 2;
		}
		if (heartCounter == 3)
		{
			a_blankheart3 = 0;
			a_heart3 = 2;
		}
		if (heartCounter == 2)
		{
			a_blankheart2 = 0;
			a_heart2 = 2;
		}
		if (heartCounter == 1)
		{
			a_blankheart1 = 0;
			a_heart1 = 2;
		}
	}
	
	/*--------------------------------------*/

	/*---------Triggers------*/
	if (bufferTime_trigger_slope > elapsedTime && trigger == true)
	{
		if (elapsedTime < (bufferTime_trigger_slope - 10.99f))
			PlayerClass::get_instance()->Coord.posY = -3.f;
		PlayerClass::get_instance()->Coord.posX += (float)(30.f * dt);
		PlayerClass::get_instance()->Coord.posY -= (float)(3.25f * dt);
		if (elapsedTime > (bufferTime_trigger_slope - 10.8f))
		{
			PlayerClass::get_instance()->Coord.posY -= (float)(10.f * dt);
		}
		if (elapsedTime > (bufferTime_trigger_slope - 8.f))
		{
			PlayerClass::get_instance()->Coord.posY -= (float)(12.5f * dt);
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
	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.scale(1.1f, 4.5f, 1.f);					// This was a mistake tbh
	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.translate(PlayerClass::get_instance()->Coord.posX, (PlayerClass::get_instance()->Coord.posY + 7.9f), PlayerClass::get_instance()->Coord.posZ);	// I should have put the scale in init
	modelStack.Translate(PlayerClass::get_instance()->Coord.posX, PlayerClass::get_instance()->Coord.posY, PlayerClass::get_instance()->Coord.posZ);								// too late for that now
		modelStack.Rotate(a_LookingDirection, 0, 1, 0);

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
	modelStack.Translate(PlayerClass::get_instance()->Coord.posX, (PlayerClass::get_instance()->Coord.posY + 7.9f), PlayerClass::get_instance()->Coord.posZ);	// i need this
	modelStack.Scale(1.1f, 4.5f, 1.f);					// if you remove it bad things will happen
	RenderMesh(meshList[GEO_BBOX], false);				// remove this later when showing actual shit of course
	modelStack.PopMatrix();								// :ok_hand:
														// for some reason I needed to flip translate and scale here to fit with the actual hitbox
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_TRIGGER_SLOPE], false);		// note to self don't use "meshList[GEO_SOMESHIT]->MeshBBox.translate(a_PosX, (a_PosY + 8), a_PosZ);" often
	modelStack.PopMatrix();								// this shit runs every second so smallest translations will move by a lot eventually

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_TESTBBOX], false);			// remove this later when showing actual shit of course
	modelStack.PopMatrix();

	/*-----------------Mutants (Fuglymon)---------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(enemy[0].EnemyPos.posX, enemy[0].EnemyPos.posY, enemy[0].EnemyPos.posZ);

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
	/*-----------------------------------------------------*/

	/*----Textbox Rendering--------*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderMeshOnScreen(meshList[GEO_TEXTBOX], 0, 0, 100, 15, 0);
	/*-----------------------------*/

	/*----Heart Rendering----------*/
	RenderMeshOnScreen(meshList[GEO_HEART], 2, 28.5, a_heart1, a_heart1, 0);
	RenderMeshOnScreen(meshList[GEO_HEART], 4, 28.5, a_heart2, a_heart2, 0);
	RenderMeshOnScreen(meshList[GEO_HEART], 6, 28.5, a_heart3, a_heart3, 0);
	RenderMeshOnScreen(meshList[GEO_HEART], 8, 28.5, a_heart4, a_heart4, 0);
	RenderMeshOnScreen(meshList[GEO_HEART], 10, 28.5, a_heart5, a_heart5, 0);

	RenderMeshOnScreen(meshList[GEO_BLANKHEART], 2, 28.5, a_blankheart1, a_blankheart1, 0);
	RenderMeshOnScreen(meshList[GEO_BLANKHEART], 4, 28.5, a_blankheart2, a_blankheart2, 0);
	RenderMeshOnScreen(meshList[GEO_BLANKHEART], 6, 28.5, a_blankheart3, a_blankheart3, 0);
	RenderMeshOnScreen(meshList[GEO_BLANKHEART], 8, 28.5, a_blankheart4, a_blankheart4, 0);
	RenderMeshOnScreen(meshList[GEO_BLANKHEART], 10, 28.5, a_blankheart5, a_blankheart5, 0);
	/*-----------------------------*/

	/*---------------Text log Rendering--------*/
	
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