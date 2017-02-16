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
#include "LoadATOM.h"

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
	/*----Camera & Camera Variables----*/
	a_PosX = -15.f;
	a_PosY = 0.f;
	a_PosZ = 0.f;
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

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

	m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
	m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
	m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
	m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
	m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
	m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
	m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
	m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
	m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
	m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
	m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");

	m_parameters[U_LIGHT6_POSITION] = glGetUniformLocation(m_programID, "lights[6].position_cameraspace");
	m_parameters[U_LIGHT6_COLOR] = glGetUniformLocation(m_programID, "lights[6].color");
	m_parameters[U_LIGHT6_POWER] = glGetUniformLocation(m_programID, "lights[6].power");
	m_parameters[U_LIGHT6_KC] = glGetUniformLocation(m_programID, "lights[6].kC");
	m_parameters[U_LIGHT6_KL] = glGetUniformLocation(m_programID, "lights[6].kL");
	m_parameters[U_LIGHT6_KQ] = glGetUniformLocation(m_programID, "lights[6].kQ");
	m_parameters[U_LIGHT6_TYPE] = glGetUniformLocation(m_programID, "lights[6].type");
	m_parameters[U_LIGHT6_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[6].spotDirection");
	m_parameters[U_LIGHT6_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[6].cosCutoff");
	m_parameters[U_LIGHT6_COSINNER] = glGetUniformLocation(m_programID, "lights[6].cosInner");
	m_parameters[U_LIGHT6_EXPONENT] = glGetUniformLocation(m_programID, "lights[6].exponent");

	m_parameters[U_LIGHT7_POSITION] = glGetUniformLocation(m_programID, "lights[7].position_cameraspace");
	m_parameters[U_LIGHT7_COLOR] = glGetUniformLocation(m_programID, "lights[7].color");
	m_parameters[U_LIGHT7_POWER] = glGetUniformLocation(m_programID, "lights[7].power");
	m_parameters[U_LIGHT7_KC] = glGetUniformLocation(m_programID, "lights[7].kC");
	m_parameters[U_LIGHT7_KL] = glGetUniformLocation(m_programID, "lights[7].kL");
	m_parameters[U_LIGHT7_KQ] = glGetUniformLocation(m_programID, "lights[7].kQ");
	m_parameters[U_LIGHT7_TYPE] = glGetUniformLocation(m_programID, "lights[7].type");
	m_parameters[U_LIGHT7_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[7].spotDirection");
	m_parameters[U_LIGHT7_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[7].cosCutoff");
	m_parameters[U_LIGHT7_COSINNER] = glGetUniformLocation(m_programID, "lights[7].cosInner");
	m_parameters[U_LIGHT7_EXPONENT] = glGetUniformLocation(m_programID, "lights[7].exponent");

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

	camera.Init(Vector3(a_PosX, a_PosY, a_PosZ + 120), Vector3(a_PosX, a_PosY, a_PosZ), Vector3(0, 1, 0));

	meshList[GEO_AXIS] = MeshBuilder::GenerateAxis("reference");
	
	// Optional if using default

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1.f, 1.f, 1.f), 18, 36, 1.f);

	/*-----------------------------Skybox Loading----------------------------------*/
	meshList[GEO_SKYBOX] = MeshBuilder::GenerateQuad("skybox", Color(1, 1, 1));
	meshList[GEO_SKYBOX]->textureID = LoadTGA("Image//SkyBG.tga");
	/*-----------------------------------------------------------------------------*/

	/*-----------------Environment Objects Loading---------------------------------*/
	meshList[GEO_HOUSE] = MeshBuilder::GenerateOBJ("house", "OBJ//Scene1//House_AllElse.obj");
	meshList[GEO_HOUSEFLOOR] = MeshBuilder::GenerateOBJ("hfloor", "OBJ//Scene1//House_Floor.obj");
	meshList[GEO_HOUSELEFTWALL] = MeshBuilder::GenerateOBJ("hlwall", "OBJ//Scene1//House_Left_Wall.obj");
	meshList[GEO_HOUSEFRONT] = MeshBuilder::GenerateOBJ("HouseFront", "OBJ//Scene1//House_Front.obj");
	meshList[GEO_BOX_SHORT] = MeshBuilder::GenerateOBJ("Box_Short", "OBJ//Scene1//Box_Short.obj");
	meshList[GEO_BOX_TALL] = MeshBuilder::GenerateOBJ("Box_Tall", "OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_HILL] = MeshBuilder::GenerateOBJ("Hill", "OBJ//Scene1//Hill.obj");
	meshList[GEO_FLOOR] = MeshBuilder::GenerateOBJ("Floor", "OBJ//Scene1//Floor.obj");
	meshList[GEO_TRUMP] = MeshBuilder::GenerateOBJ("Trump", "OBJ//Scene1//Trump.obj");

	meshList[GEO_HOUSEFLOOR]->MeshBBox.loadBB("OBJ//Scene1//House_Floor.obj");
	meshList[GEO_HOUSELEFTWALL]->MeshBBox.loadBB("OBJ//Scene1//House_Left_Wall.obj");
	meshList[GEO_HOUSEFRONT]->MeshBBox.loadBB("OBJ//Scene1//House_Front.obj");
	meshList[GEO_BOX_SHORT]->MeshBBox.loadBB("OBJ//Scene1//Box_Short.obj");
	meshList[GEO_BOX_TALL]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_FLOOR]->MeshBBox.loadBB("OBJ//Scene1//Floor.obj");
	meshList[GEO_FLOOR]->MeshBBox.translate(50, -253.75, 0);
	meshList[GEO_TRUMP]->MeshBBox.loadBB("OBJ//Scene1//Trump.obj");

	meshList[GEO_LIGHTBULB] = MeshBuilder::GenerateOBJ("bulb", "OBJ//Scene1//lighttop.obj");
	meshList[GEO_LIGHTBULB]->textureID = LoadTGA("Image//lighttext.tga");
	meshList[GEO_LIGHTSTAND] = MeshBuilder::GenerateOBJ("lightstand", "OBJ//Scene1//lightbottom.obj");
	meshList[GEO_LIGHTSTAND]->textureID = LoadTGA("Image//lighttext.tga");

	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//tree.obj");
	meshList[GEO_TREE]->textureID = LoadTGA("Image//tree.tga");
	/*-----------------------------------------------------------------------------*/

	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("textbox", Color(0, 0, 0));
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
	meshList[GEO_TESTBBOX] = MeshBuilder::GenerateBB("TestBox", meshList[GEO_FLOOR]->MeshBBox.max_, meshList[GEO_FLOOR]->MeshBBox.min_);
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

	/*-----Character--------*/
	a_LookingDirection = 90.0f;
	pressedA = false;
	pressedD = false;
	injump = false;
	infall = true;
	attack = false;
	trigger = false;
	/*----------------------*/
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);
	/*-----------------------------------------------------------------------------*/

	/*----------------------Light Initialisation-----------------------------------*/
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(45, 20, -20);
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
	light[2].position.Set(-70, 10, -10);
	light[2].color.Set(0.251, 0.878, 0.816);
	light[2].power = 5;
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

	light[3].type = Light::LIGHT_POINT;
	light[3].position.Set(130, 0, -10);
	light[3].color.Set(0.251, 0.878, 0.816);
	light[3].power = 5;
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
	light[4].position.Set(235, -75, -10);
	light[4].color.Set(0.251, 0.878, 0.816);
	light[4].power = 5;
	light[4].kC = 1.f;
	light[4].kL = 0.01f;
	light[4].kQ = 0.001f;
	light[4].cosCutoff = cos(Math::DegreeToRadian(45));
	light[4].cosInner = cos(Math::DegreeToRadian(30));
	light[4].exponent = 3.f;
	light[4].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);

	light[5].type = Light::LIGHT_POINT;
	light[5].position.Set(330, -155, -10);
	light[5].color.Set(0.251, 0.878, 0.816);
	light[5].power = 5;
	light[5].kC = 1.f;
	light[5].kL = 0.01f;
	light[5].kQ = 0.001f;
	light[5].cosCutoff = cos(Math::DegreeToRadian(45));
	light[5].cosInner = cos(Math::DegreeToRadian(30));
	light[5].exponent = 3.f;
	light[5].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
	glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], light[5].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], light[5].cosInner);
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);

	light[6].type = Light::LIGHT_POINT;
	light[6].position.Set(440, -243, -10);
	light[6].color.Set(0.251, 0.878, 0.816);
	light[6].power = 5;
	light[6].kC = 1.f;
	light[6].kL = 0.01f;
	light[6].kQ = 0.001f;
	light[6].cosCutoff = cos(Math::DegreeToRadian(45));
	light[6].cosInner = cos(Math::DegreeToRadian(30));
	light[6].exponent = 3.f;
	light[6].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT6_TYPE], light[6].type);
	glUniform3fv(m_parameters[U_LIGHT6_COLOR], 1, &light[6].color.r);
	glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
	glUniform1f(m_parameters[U_LIGHT6_KC], light[6].kC);
	glUniform1f(m_parameters[U_LIGHT6_KL], light[6].kL);
	glUniform1f(m_parameters[U_LIGHT6_KQ], light[6].kQ);
	glUniform1f(m_parameters[U_LIGHT6_COSCUTOFF], light[6].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT6_COSINNER], light[6].cosInner);
	glUniform1f(m_parameters[U_LIGHT6_EXPONENT], light[6].exponent);

	light[7].type = Light::LIGHT_POINT;
	light[7].position.Set(540, -243, -10);
	light[7].color.Set(0.251, 0.878, 0.816);
	light[7].power = 5;
	light[7].kC = 1.f;
	light[7].kL = 0.01f;
	light[7].kQ = 0.001f;
	light[7].cosCutoff = cos(Math::DegreeToRadian(45));
	light[7].cosInner = cos(Math::DegreeToRadian(30));
	light[7].exponent = 3.f;
	light[7].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT7_TYPE], light[7].type);
	glUniform3fv(m_parameters[U_LIGHT7_COLOR], 1, &light[7].color.r);
	glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
	glUniform1f(m_parameters[U_LIGHT7_KC], light[7].kC);
	glUniform1f(m_parameters[U_LIGHT7_KL], light[7].kL);
	glUniform1f(m_parameters[U_LIGHT7_KQ], light[7].kQ);
	glUniform1f(m_parameters[U_LIGHT7_COSCUTOFF], light[7].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT7_COSINNER], light[7].cosInner);
	glUniform1f(m_parameters[U_LIGHT7_EXPONENT], light[7].exponent);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 8);
	/*-------------------------------------------------------------------------------*/
}
double et = 0.0;
void StudioProject2Scene1::Update(double dt)
{
	static float rotationDirection = 1.0f;
	int framespersec = 1 / dt;
	elapsedTime += dt;
	camera.Update(dt, a_PosX, a_PosY);

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

	float LSPEED = 10.f;
	if (Application::IsKeyPressed('I'))
		light[3].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[3].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[3].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[3].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[3].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[3].position.y += (float)(LSPEED * dt);

	/*-----------------------Text Interaction----------------*/
	// Whenever press Enter is needed to continue [message or interaction]
	if (pEnter == true)
		pressEnterTS = 2;
	else
		pressEnterTS = 0;

	/*-------------------Spawn Text--------*/
	if (a_PosX < -17.f || a_PosX > -8.f) //Moving away from the initial Spawn Point will make Spawn Message disappear
	{
		spawnTS = 0;
	}
	/*-------------------------------------*/

	/*-----------Syringe Text-------------*/
	if (a_PosX > 10.f && a_PosX < 20.f && textOccured == textOccurStorage)
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
	if (a_PosX > 50.f && a_PosX < 70.f && textOccured > textOccurStorage) //When near box, text appears
	{
	    pEnter = true;
	    boxTriggedText = true;
		textOccurStorage = textOccured + 1; // tO < tOS
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

	/*------Half Mutant conversation with Alexis-----*/
	if (a_PosX < 170 && a_PosX > 160 && textOccured < textOccurStorage) 
	{	//When alexis at wall, he sees the half mutant and text gets triggered
		textOccured = textOccurStorage; // tO == tOS
		hmTriggeredText = true;
		pEnter = true;
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


	if (a_PosX < 200 && a_PosX > 190 && textOccured == textOccurStorage)
	{
	    alexis_beside_hm = true;
		pEnter = true;
	}

	if (alexis_beside_hm == true && nexttext == true)
	{
		pEnter = false;
		alexis_beside_hm = false;
		nexttext = false;
		textOccured = textOccurStorage + 1; // tO > tOS
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

	/*----------Character Movement-------------------*/

	if ((pressedA == true && a_LookingDirection == 90.f) ||
		(pressedD == true && a_LookingDirection == -90.f))
	{
		a_LookingDirection *= -1;
	}

	if ((a_RotationRightLeg > 30) || (a_RotationRightLeg < -30))
	{
		rotationDirection *= -1;
	}

	if ((!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D')) || 
		(Application::IsKeyPressed('A') && Application::IsKeyPressed('D')))
	{
		a_RotationLeftLeg = 0.0f;
		a_RotationRightLeg = 0.0f;
	}
	/*-----------------------------------------------*/

	/*------------------------------Collision Check------------------------------*/
	if (Application::IsKeyPressed('A') && !trigger)
	{
		if (!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
			!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox) ||
			pressedD == true)
		{
			a_PosX -= (float)(30.f * dt);
			a_RotationLeftLeg += (float)(rotationDirection * 5.0f * dt);
			a_RotationRightLeg -= (float)(rotationDirection * 5.0f * dt);
			pressedD = false;
			pressedA = true;
		}
	}
	if (Application::IsKeyPressed('D') && !trigger)
	{
		if (!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
			!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox) ||
			pressedA == true)
		{
			a_PosX += (float)(30.f * dt);
			a_RotationLeftLeg += (float)(rotationDirection * 15.0f * dt);
			a_RotationRightLeg -= (float)(rotationDirection * 15.0f * dt);
			pressedA = false;
			pressedD = true;
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

	if (bufferTime_JumpUp > elapsedTime)
		injump = true;
	else
		injump = false;

	if (bufferTime_attack > elapsedTime)
	{
		attack = true;
		et += dt;
	}
	else
	{
		attack = false;
		et = 0;
	}

	if (!trigger)
	{
		if (injump == false)
		{
			if (!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSEFLOOR]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_HOUSEFRONT]->MeshBBox) &&
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_FLOOR]->MeshBBox) && 
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_SHORT]->MeshBBox) && 
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_BOX_TALL]->MeshBBox) && 
				!meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRUMP]->MeshBBox))
				a_PosY -= (float)(30.f * dt);
		}
		else
		{
			a_PosY += (float)(30.f * dt);
		}
	}

	if (meshList[GEO_ALEXIS_CROTCH]->MeshBBox.collide(meshList[GEO_TRIGGER_SLOPE]->MeshBBox))
	{
		bufferTime_trigger_slope = elapsedTime + 11.f;
		trigger = true;
	}

	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.loadBB("OBJ//Character//crotch.obj");
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
			a_PosY = -3.f;
		a_PosX += (float)(30.f * dt);
		a_PosY -= (float)(3.25f * dt);
		if (elapsedTime > (bufferTime_trigger_slope - 10.8f))
		{
			a_PosY -= (float)(10.f * dt);
		}
		if (elapsedTime > (bufferTime_trigger_slope - 8.f))
		{
			a_PosY -= (float)(12.5f * dt);
		}
	}
	else if (bufferTime_trigger_slope < elapsedTime && trigger == true)
		trigger = false;
	/*--------------------------------------*/
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

	Position lightPosition1_cameraspace = viewStack.Top() * light[1].position;
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition1_cameraspace.x);

	Position lightPosition2_cameraspace = viewStack.Top() * light[2].position;
	glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition2_cameraspace.x);

	Position lightPosition3_cameraspace = viewStack.Top() * light[3].position;
	glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition3_cameraspace.x);

	Position lightPosition4_cameraspace = viewStack.Top() * light[4].position;
	glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition4_cameraspace.x);

	Position lightPosition5_cameraspace = viewStack.Top() * light[5].position;
	glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition5_cameraspace.x);

	Position lightPosition6_cameraspace = viewStack.Top() * light[6].position;
	glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition6_cameraspace.x);

	Position lightPosition7_cameraspace = viewStack.Top() * light[7].position;
	glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition7_cameraspace.x);

	modelStack.PushMatrix();
	modelStack.Translate(light[3].position.x, light[3].position.y, light[3].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	/*-----------------Skybox-------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(50, 170, -200);
	modelStack.Scale(1000, 500, 500);
	RenderMesh(meshList[GEO_SKYBOX], false);
	modelStack.PopMatrix();
	/*------------------------------------------*/

	/*-----------------Main Character (Alexis)---------------------*/
	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.scale(1.1f, 4.3f, 1.f);
	meshList[GEO_ALEXIS_CROTCH]->MeshBBox.translate(a_PosX, (a_PosY + 8), a_PosZ);
		modelStack.Translate(a_PosX, a_PosY, a_PosZ);
		modelStack.Rotate(a_LookingDirection, 0, 1, 0);


		modelStack.PushMatrix();
		if (attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "polySurface9");//HEAD
		else if (!attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "polySurface9");//HEAD

		RenderMesh(meshList[GEO_ALEXIS_HEAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		if (attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere17");//ARM WITH SWORD
		else if (!attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere17");//ARM WITH SWORD

		RenderMesh(meshList[GEO_ALEXIS_LEFTARM], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		if (attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "polySurface32");//BODY
		else if (!attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "polySurface32");//BODY

		RenderMesh(meshList[GEO_ALEXIS_BODY], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		if (attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere14");//LEFTARM
		else if (!attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere14");//LEFTARM

		RenderMesh(meshList[GEO_ALEXIS_RIGHTARM], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		if (attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pCylinder15");//crotch
		else if (!attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pCylinder15");//crotch

		RenderMesh(meshList[GEO_ALEXIS_CROTCH], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		if (attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere9");//RIGHT LEG
		else if (!attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere9");//RIGHT LEG

		RenderMesh(meshList[GEO_ALEXIS_LEFTLEG], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		if (attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere10");//LEFTLEG
		else if (!attack)
			LoadAtom("ATOM//attack.atom", &modelStack, &et, "pSphere10");//LEFTLEG

		RenderMesh(meshList[GEO_ALEXIS_RIGHTLEG], true);
		modelStack.PopMatrix();

	modelStack.PopMatrix();

	modelStack.PushMatrix();							// render collision box
	modelStack.Translate(a_PosX, (a_PosY + 8), a_PosZ);	// i need this
	modelStack.Scale(1.1f, 4.3f, 1.f);					// if you remove it bad things will happen
	RenderMesh(meshList[GEO_BBOX], false);				// remove this later when showing actual shit of course
	modelStack.PopMatrix();								// :ok_hand:
														// for some reason I needed to flip translate and scale here to fit with the actual hitbox
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_TRIGGER_SLOPE], false);		// note to self don't use "meshList[GEO_SOMESHIT]->MeshBBox.translate(a_PosX, (a_PosY + 8), a_PosZ);" often
	modelStack.PopMatrix();								// this shit runs every second so smallest translations will move by a lot eventually

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_TESTBBOX], false);			// remove this later when showing actual shit of course
	modelStack.PopMatrix();
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
	modelStack.Translate(0, -12, -60);
	modelStack.Scale(5, 5, 5);
	for (unsigned i = 0; i < 3; ++i)
	{
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.Translate(-10, 0, 0);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, -24, -120);
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

	/*modelStack.PushMatrix();
	RenderMesh(meshList[GEO_BOX_SHORT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_BOX_TALL], true);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(50.f, -255.f, 0);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	RenderMesh(meshList[GEO_TRUMP], true);
	modelStack.PopMatrix();*/

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
	modelStack.Translate(550, -253, -10);
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