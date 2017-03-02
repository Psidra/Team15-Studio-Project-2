#include "StudioProject2_SceneBoss.h"
#include "LoadingScreen.h"
#include "DeathScreen.h"
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
#include "EnemyClassManager.h"
#include "EnemyClass.h"
#include "SceneManager.h"
#include "VictoryScreen.h"
#include "Boss.h"
#include <vector>

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34

StudioProject2SceneBoss::StudioProject2SceneBoss()
{
}

StudioProject2SceneBoss::~StudioProject2SceneBoss()
{
}

void StudioProject2SceneBoss::Init()
{
	PlayerClass::get_instance();

	/*----Player & AI & Camera Variables----*/

	//EnemyManager::get_instance()->spawnEnemy(Vector3(750.f, -252.2f, 0.f));
	PlayerClass::get_instance()->position_a = Vector3(-15.f, 0.f, 0.f);
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->manaUI();

	Boss::get_instance()->bossHealthUI();
	Boss::get_instance()->bossInit();
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

	camera.Init(Vector3(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y, PlayerClass::get_instance()->position_a.z + 120),
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
	// GEO_ENVIRONMENT, GEO_GROUND, GEO_LEFTWALL, GEO_RIGHTWALL, GEO_PREVENT, GEO_TRIGGER,
	
	//Light
	meshList[GEO_LIGHT] = MeshBuilder::GenerateOBJ("light", "OBJ//SceneBoss//glowingsphere.obj");
	meshList[GEO_LIGHT]->textureID = LoadTGA("Image//glowingsphere.tga");
	//-----

	meshList[GEO_ENVIRONMENT] = MeshBuilder::GenerateOBJ("Environment", "OBJ//SceneBoss//Bossscene2.obj");
	meshList[GEO_ENVIRONMENT]->textureID = LoadTGA("Image//scenebosstexture.tga");
	meshList[GEO_GROUND] = MeshBuilder::GenerateOBJ("GroundCollision", "OBJ//SceneBoss//bossfloorbb_edit.obj");
	meshList[GEO_LEFTWALL] = MeshBuilder::GenerateOBJ("LeftWall", "OBJ//SceneBoss//bossleftbb.obj");
	meshList[GEO_RIGHTWALL] = MeshBuilder::GenerateOBJ("RightWall", "OBJ//SceneBoss//bossrightbb.obj");
	meshList[GEO_PREVENT] = MeshBuilder::GenerateOBJ("Prevent", "OBJ//Scene1//Box_Tall.obj"); // falling thing when trigger
	meshList[GEO_TRIGGER] = MeshBuilder::GenerateOBJ("Triggered", "OBJ//SceneBoss//bosstrigger.obj");

	meshList[GEO_GROUND]->MeshBBox.loadBB("OBJ//SceneBoss//bossfloorbb_edit.obj");
	meshList[GEO_LEFTWALL]->MeshBBox.loadBB("OBJ//SceneBoss//bossleftbb.obj");
	meshList[GEO_RIGHTWALL]->MeshBBox.loadBB("OBJ//SceneBoss//bossrightbb.obj");
	meshList[GEO_TRIGGER]->MeshBBox.loadBB("OBJ//SceneBoss//bosstrigger.obj");
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

	meshList[GEO_SPIT] = MeshBuilder::GenerateOBJ("Spit", "OBJ//Mutant_Projectile.obj");
	meshList[GEO_SPIT]->textureID = LoadTGA("Image//Mutant_Projectile_Texture.tga");

	//EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");

	/*-----------------------------------------------------------------------------*/

	/*--------------------------Character Loading----------------------------------*/
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

	meshList[GEO_ALEXUS_HEAD] = MeshBuilder::GenerateOBJ("uHead", "OBJ//Stcunt_double//head_.obj");
	meshList[GEO_ALEXUS_HEAD]->textureID = LoadTGA("Image//facehairtext.tga");
	meshList[GEO_ALEXUS_BODY] = MeshBuilder::GenerateOBJ("uBody", "OBJ//Stcunt_double//body_.obj");
	meshList[GEO_ALEXUS_BODY]->textureID = LoadTGA("Image//bodytext.tga");
	meshList[GEO_ALEXUS_CROTCH] = MeshBuilder::GenerateOBJ("uCrotch", "OBJ//Stcunt_double//crotch_.obj");
	meshList[GEO_ALEXUS_CROTCH]->textureID = LoadTGA("Image//crotchtext.tga");
	meshList[GEO_ALEXUS_RIGHTARM] = MeshBuilder::GenerateOBJ("uRightArm", "OBJ//Stcunt_double//right_arm.obj");
	meshList[GEO_ALEXUS_RIGHTARM]->textureID = LoadTGA("Image//rightarmtext.tga");
	meshList[GEO_ALEXUS_LEFTARM] = MeshBuilder::GenerateOBJ("uLeftArm", "OBJ//Stcunt_double//left_arm.obj");
	meshList[GEO_ALEXUS_LEFTARM]->textureID = LoadTGA("Image//leftarmtext.tga");
	meshList[GEO_ALEXUS_RIGHTLEG] = MeshBuilder::GenerateOBJ("uRightLeg", "OBJ//Stcunt_double//right_leg.obj");
	meshList[GEO_ALEXUS_RIGHTLEG]->textureID = LoadTGA("Image//shoetext.tga");
	meshList[GEO_ALEXUS_LEFTLEG] = MeshBuilder::GenerateOBJ("uLeftLeg", "OBJ//Stcunt_double//left_leg.obj");
	meshList[GEO_ALEXUS_LEFTLEG]->textureID = LoadTGA("Image//shoetext.tga");

	meshList[GEO_ALEXUS_RIGHT4ARM] = MeshBuilder::GenerateOBJ("uRight4Arm", "OBJ//Stcunt_double//r_foreArm.obj");
	meshList[GEO_ALEXUS_RIGHT4ARM]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXUS_LEFT4ARM] = MeshBuilder::GenerateOBJ("uLeft4Arm", "OBJ//Stcunt_double//l_foreArm.obj");
	meshList[GEO_ALEXUS_LEFT4ARM]->textureID = LoadTGA("Image//armtext.tga");
	meshList[GEO_ALEXUS_RIGHTTHIGH] = MeshBuilder::GenerateOBJ("uRightLegT", "OBJ//Stcunt_double//right_thigh.obj");
	meshList[GEO_ALEXUS_RIGHTTHIGH]->textureID = LoadTGA("Image//shoetext.tga");
	meshList[GEO_ALEXUS_LEFTTHIGH] = MeshBuilder::GenerateOBJ("uLeftLegT", "OBJ//Stcunt_double//left_thigh.obj");
	meshList[GEO_ALEXUS_LEFTTHIGH]->textureID = LoadTGA("Image//shoetext.tga");

	/*-----------------------------------------------------------------------------*/
	meshList[GEO_BOSSLIFE] = MeshBuilder::GenerateQuad("bosslife", Color(1, 0.843, 0));
	meshList[GEO_ENERGY] = MeshBuilder::GenerateQuad("energy", Color(0, 0, 1));
	meshList[GEO_BLANKENERGY] = MeshBuilder::GenerateQuad("blankenergy", Color(0, 0, 0));
	/*---------------Spells----------*/
	meshList[GEO_LASER_ICON] = MeshBuilder::GenerateQuad("lasericon", Color(1, 1, 1));
	meshList[GEO_LASER_ICON]->textureID = LoadTGA("Image//laser.tga");
	meshList[GEO_LASER_CD] = MeshBuilder::GenerateQuad("laserCDicon", Color(1, 1, 1));
	meshList[GEO_LASER_CD]->textureID = LoadTGA("Image//laser_cooldown.tga");
	meshList[GEO_PROJSHIELD] = MeshBuilder::GenerateQuad("projshieldicon", Color(1, 1, 1));
	meshList[GEO_PROJSHIELD]->textureID = LoadTGA("Image//hardlight.tga");
	meshList[GEO_PROJSHIELD_CD] = MeshBuilder::GenerateQuad("projshieldCDicon", Color(1, 1, 1));
	meshList[GEO_PROJSHIELD_CD]->textureID = LoadTGA("Image//hardlight_cooldown.tga");
	/*-------------------------------*/
	meshList[GEO_LASER] = MeshBuilder::GenerateCylinder("laser", Color(1, 0, 0));

	/*--------------------------Text Loading---------------------------------------*/
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//franklingothicheavy.tga");
	/*-----------------------------------------------------------------------------*/

	/*--------------------------HUD Loading---------------------------------------*/
	meshList[GEO_HALF_COUNT] = MeshBuilder::GenerateQuad("hudhalf", Color(1, 1, 1));
	meshList[GEO_HALF_COUNT]->textureID = LoadTGA("Image//halfhud.tga");
	meshList[GEO_FULL_COUNT] = MeshBuilder::GenerateQuad("hudfull", Color(1, 1, 1));
	meshList[GEO_FULL_COUNT]->textureID = LoadTGA("Image//fullhud.tga");

	/*-------------------------Loading Alexis Health----------------------------------*/
	meshList[GEO_BLANKHEART] = MeshBuilder::GenerateQuad("blankheart", Color(1, 1, 1));
	meshList[GEO_BLANKHEART]->textureID = LoadTGA("Image//heartsb.tga");
	meshList[GEO_ALEXIS_LIFE] = MeshBuilder::GenerateQuad("heart", Color(1, 1, 1));
	meshList[GEO_ALEXIS_LIFE]->textureID = LoadTGA("Image//hearts.tga");
	/*--------------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------Trigger Check-----------------------------------*/
	//meshList[GEO_TRIGGER_SLOPE] = MeshBuilder::GenerateOBJ("Trigger_Slope", "OBJ//TriggerBox.obj");
	//meshList[GEO_TRIGGER_SLOPE]->MeshBBox.loadBB("OBJ//TriggerBox.obj");
	//meshList[GEO_TRIGGER_SLOPE]->MeshBBox.translate(-12.f, 10.f, 0);
	/*-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------*/
	meshList[GEO_BOSSLIFE] = MeshBuilder::GenerateQuad("bosslife", Color(1, 0.843, 0));
	meshList[GEO_ENERGY] = MeshBuilder::GenerateQuad("energy", Color(0, 0, 1));
	meshList[GEO_BLANKENERGY] = MeshBuilder::GenerateQuad("blankenergy", Color(0, 0, 0));
	/*---------------Spells----------*/
	meshList[GEO_LASER_ICON] = MeshBuilder::GenerateQuad("lasericon", Color(1, 1, 1));
	meshList[GEO_LASER_ICON]->textureID = LoadTGA("Image//laser.tga");
	meshList[GEO_LASER_CD] = MeshBuilder::GenerateQuad("laserCDicon", Color(1, 1, 1));
	meshList[GEO_LASER_CD]->textureID = LoadTGA("Image//laser_cooldown.tga");
	meshList[GEO_PROJSHIELD] = MeshBuilder::GenerateQuad("projshieldicon", Color(1, 1, 1));
	meshList[GEO_PROJSHIELD]->textureID = LoadTGA("Image//hardlight.tga");
	meshList[GEO_PROJSHIELD_CD] = MeshBuilder::GenerateQuad("projshieldCDicon", Color(1, 1, 1));
	meshList[GEO_PROJSHIELD_CD]->textureID = LoadTGA("Image//hardlight_cooldown.tga");
	/*-------------------------------*/


	meshList[GEO_BOSS_LARM] = MeshBuilder::GenerateOBJ("Boss_LeftArm", "OBJ//Boss//Boss_LeftArm.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_LJAW] = MeshBuilder::GenerateOBJ("Boss_LeftJaw", "OBJ//Boss//Boss_LeftJaw.obj");
	meshList[GEO_BOSS_LJAW]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_LUARM] = MeshBuilder::GenerateOBJ("Boss_LeftUpperarm", "OBJ//Boss//Boss_LeftUpperarm.obj");
	meshList[GEO_BOSS_LUARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_MHEAD] = MeshBuilder::GenerateOBJ("Boss_MainHead", "OBJ//Boss//Boss_MainHead.obj");
	meshList[GEO_BOSS_MHEAD]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_MJAW] = MeshBuilder::GenerateOBJ("Boss_MainJaw", "OBJ//Boss//Boss_MainJaw.obj");
	meshList[GEO_BOSS_MJAW]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_NECK] = MeshBuilder::GenerateOBJ("Boss_Neck", "OBJ//Boss//Boss_Neck.obj");
	meshList[GEO_BOSS_NECK]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_RARM] = MeshBuilder::GenerateOBJ("Boss_RightArm", "OBJ//Boss//Boss_RightArm.obj");
	meshList[GEO_BOSS_RARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_RJAW] = MeshBuilder::GenerateOBJ("Boss_RightJaw", "OBJ//Boss//Boss_RightJaw.obj");
	meshList[GEO_BOSS_RJAW]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_RUARM] = MeshBuilder::GenerateOBJ("Boss_RightUpperarm", "OBJ//Boss//Boss_RightUpperarm.obj");
	meshList[GEO_BOSS_RUARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG1] = MeshBuilder::GenerateOBJ("Boss_Segment1", "OBJ//Boss//Boss_Segment1.obj");
	meshList[GEO_BOSS_SEG1]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG2] = MeshBuilder::GenerateOBJ("Boss_Segment2", "OBJ//Boss//Boss_Segment2.obj");
	meshList[GEO_BOSS_SEG2]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG3] = MeshBuilder::GenerateOBJ("Boss_Segment3", "OBJ//Boss//Boss_Segment3.obj");
	meshList[GEO_BOSS_SEG3]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG4] = MeshBuilder::GenerateOBJ("Boss_Segment4", "OBJ//Boss//Boss_Segment4.obj");
	meshList[GEO_BOSS_SEG4]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG5] = MeshBuilder::GenerateOBJ("Boss_Segment5", "OBJ//Boss//Boss_Segment5.obj");
	meshList[GEO_BOSS_SEG5]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG6] = MeshBuilder::GenerateOBJ("Boss_Segment6", "OBJ//Boss//Boss_Segment6.obj");
	meshList[GEO_BOSS_SEG6]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_TORSO] = MeshBuilder::GenerateOBJ("Boss_Torso", "OBJ//Boss//Boss_Torso.obj");
	meshList[GEO_BOSS_TORSO]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");

	meshList[GEO_BOSS_INDICATOR] = MeshBuilder::GenerateOBJ("Boss_Mutant_Range", "OBJ//Boss//Boss_Mutant_Range.obj");
	meshList[GEO_SPIKE] = MeshBuilder::GenerateOBJ("Boss_Spike", "OBJ//Boss//Boss_Spike.obj");

	Boss::get_instance()->EnemyHitBox.loadBB("OBJ//Boss//Boss_Torso.obj");

	Boss::get_instance()->Boss_Tail.TailHitBox.loadBB("OBJ//Boss//Boss_Spike.obj");

	meshList[GEO_LASER] = MeshBuilder::GenerateOBJ("laser", "OBJ//lazer.obj");
	meshList[GEO_LASER]->textureID = LoadTGA("Image//lazer.tga");
	meshList[GEO_LASER]->MeshBBox.loadBB("OBJ//lazer.obj");
	// make obj later
	meshList[GEO_SHIELD] = MeshBuilder::GenerateOBJ("proShield", "OBJ//Hardlightshield.obj");
	meshList[GEO_SHIELD]->textureID = LoadTGA("Image//Hardlightshield.tga");
	meshList[GEO_SHIELD]->MeshBBox.loadBB("OBJ//Hardlightshield.obj");
	/*-----------------------------Checking BBox-----------------------------------*/
	meshList[GEO_TESTBBOX] = MeshBuilder::GenerateBB("TestBox", meshList[GEO_SHIELD]->MeshBBox.max_, meshList[GEO_SHIELD]->MeshBBox.min_);
	meshList[GEO_BBOX] = MeshBuilder::GenerateBB("CharBox", PlayerClass::get_instance()->PlayerHitBox.max_, PlayerClass::get_instance()->PlayerHitBox.min_);
	/*-----------------------------------------------------------------------------*/

	/*-------------------------Loading Mutant Health----------------------------------*/
	meshList[GEO_M_RHEART] = MeshBuilder::GenerateOBJ("MutantHealthRed", "OBJ//M_HealthRed.obj");
	meshList[GEO_M_RHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
	meshList[GEO_M_BHEART] = MeshBuilder::GenerateOBJ("MutantHealthBlack", "OBJ//M_HealthBlack.obj");
	meshList[GEO_M_BHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
	/*--------------------------------------------------------------------------------*/

	/*------------------------Initialising Text Variables-------------------------------*/
	bossTextsize = 2;
	alexisTextsize = 0;
	narTextsize = 0;
	insTextsize = 0;
	bufferTime_text = elapsedTime + 3.f;
	pEnter = true;
	nexttext = false;
	/*----------------------------------------------------------------------------------*/

	/*---------------------------Initialising Variables---------------------------------*/
	Unlock = false;

	/*-----Character--------*/
	pressedA = false;
	pressedD = false;
	inmovement = false;
	injump = false;
	attack = false;
	trigger = false;
	block = false;
	roll = false;
	/*----------------------*/
	movespeed = 30.f;

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

void StudioProject2SceneBoss::Update(double dt)
{
	int framespersec = 1 / dt;
	elapsedTime += dt;
	// Lock/Unlock Camera
	if (Application::IsKeyPressed('Y'))
	{
		if (!Unlock && elapsedTime > bufferTime_Unlock)
		{
			Unlock = true;
			bufferTime_Unlock = elapsedTime + 0.5f;
		}
		else if (Unlock && elapsedTime > bufferTime_Unlock)
		{
			Unlock = false;
			bufferTime_Unlock = elapsedTime + 0.5f;
		}
	}

	if (!Unlock)
		camera.Update(dt, PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y + 7);
	else
		camera.UpdateUnlockedCam(dt);

	/*----Player Functions-----*/
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->manaUI();
	PlayerClass::get_instance()->timeSpent(dt);
	PlayerClass::get_instance()->healthRegeneration(elapsedTime);
	PlayerClass::get_instance()->spellUI(elapsedTime);
	if (!otheranims() && !holdanims())
		PlayerClass::get_instance()->facingDirection();

	if (!Boss::get_instance()->magicImmunity)
	{
		PlayerClass::get_instance()->laserBeam(elapsedTime);
		PlayerClass::get_instance()->projectileShield(elapsedTime,dt);
	}


	if (PlayerClass::get_instance()->laserActive == true)
	{
		if (meshList[GEO_LASER]->MeshBBox.collide(Boss::get_instance()->EnemyHitBox))
		{
			Boss::get_instance()->bossHealthSystem(true);
		}
	}
	/*------------------------*/

	/*--------Boss Functions--------------*/
	Boss::get_instance()->update(elapsedTime, block); // so much neater \:D/
	if (trigger)
		Boss::get_instance()->proj_attack(Boss::get_instance()->position_m + Vector3(0, -5.f, 0), Boss::get_instance()->direction_m, elapsedTime);
	Boss::get_instance()->proj_update(true);
	/*------------------------------------*/

	/*-------AI Functions---------------*/
	//EnemyManager::get_instance()->EnemyList[0]->update(dt);

	for (unsigned int projectiles = 0; projectiles < Boss::get_instance()->spit_.size(); projectiles++)
	{
		if (Boss::get_instance()->spit_[projectiles] != nullptr)
		{
			Boss::get_instance()->spit_[projectiles]->projHitBox_.translate(Boss::get_instance()->spit_[projectiles]->position_.x,
				(Boss::get_instance()->spit_[projectiles]->position_.y + 27.f),
				PlayerClass::get_instance()->position_a.z);

			if (Boss::get_instance()->spit_[projectiles]->projHitBox_.collide(meshList[GEO_TRIGGER]->MeshBBox)
				|| Boss::get_instance()->spit_[projectiles]->projHitBox_.collide(meshList[GEO_LEFTWALL]->MeshBBox)
				|| Boss::get_instance()->spit_[projectiles]->projHitBox_.collide(meshList[GEO_RIGHTWALL]->MeshBBox)
				|| Boss::get_instance()->spit_[projectiles]->displacement() > 500.f)
			{
				Boss::get_instance()->spit_.erase(Boss::get_instance()->spit_.begin() + projectiles);
			}
			if (Boss::get_instance()->spit_[projectiles]->projHitBox_.collide(PlayerClass::get_instance()->PlayerHitBox) && (elapsedTime > bufferTime_iframeroll) && (elapsedTime > bufferTime_iframe))
			{
				PlayerClass::get_instance()->healthSystem(block, true);
				bufferTime_iframe = elapsedTime + 0.3f;
				Boss::get_instance()->spit_.erase(Boss::get_instance()->spit_.begin() + projectiles);
			}
		}
	}
	//EnemyManager::get_instance()->EnemyList[0]->update(dt);

	// I spent 10 years trying to fix projectile because I wanted to avoid using erase.
	// Erase won today. Erase, me, 1:0. Shit.

	//for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++) // in case got error, -- proj when delete
	//{
	//	if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
	//	{
	//		for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
	//		{
	//			if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] != nullptr)
	//			{
	//				EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.translate(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.x,
	//					(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.y + 10.f),
	//					EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.z);

	//				if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->displacement() > 300.f)
	//				{
	//					EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
	//				}
	//				else if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(PlayerClass::get_instance()->PlayerHitBox) &&
	//					(elapsedTime > bufferTime_iframe) && (elapsedTime > bufferTime_iframeroll))
	//				{
	//					PlayerClass::get_instance()->healthSystem(block, false);
	//					bufferTime_iframe = elapsedTime + 0.3f;
	//					EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
	//				}
	//			}
	//		}
	//	}
	//}



	/*-----------HUD UPDATES---------*/
	fps = "FPS:" + std::to_string(framespersec);
	fMutantKilled = ":" + std::to_string(PlayerClass::get_instance()->fm_Killed);
	hMutantSaved = ":" + std::to_string(PlayerClass::get_instance()->hm_Saved);
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
	if (!otheranims() || holdanims())
	{
		for (unsigned i = 0; i < 6; i++)
			et[i] = 0;
	}
	if (!holdanims())
	{
		et[7] = 0;
		et[8] = 0;
		et[9] = 0;
	}

	// !PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox)

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xboxButtonCount;
		const unsigned char *xbox = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xboxButtonCount);
		if (elapsedTime > 1.1f) // This pre-setting ensures animations won't occur at the very start, so animations glitching out will not happen anymore.*
		{									// *I hope.
			inmovement = false;				// so many if statements I could write a philosophy book
			if ((GLFW_PRESS == xbox[13] || Application::IsKeyPressed('A')) && !roll)
			{
				if (!trigger)
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LEFTWALL]->MeshBBox)
						|| pressedD == true)
					{
						PlayerClass::get_instance()->position_a.x -= (float)(movespeed * dt);
						pressedD = false;
						pressedA = true;
						inmovement = true;
					}
				}
				else
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox)
						&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTWALL]->MeshBBox)
						|| pressedD == true)
					{
						PlayerClass::get_instance()->position_a.x -= (float)(movespeed * dt);
						pressedD = false;
						pressedA = true;
						inmovement = true;
					}
				}
			}

			if (GLFW_PRESS == xbox[11] || Application::IsKeyPressed('D') && !roll)
			{
				if (!trigger)
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LEFTWALL]->MeshBBox)
						|| pressedA == true)
					{
						PlayerClass::get_instance()->position_a.x += (float)(movespeed * dt);
						pressedA = false;
						pressedD = true;
						inmovement = true;
					}
				}
				else
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox)
						&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTWALL]->MeshBBox)
						|| pressedA == true)
					{
						PlayerClass::get_instance()->position_a.x += (float)(movespeed * dt);
						pressedA = false;
						pressedD = true;
						inmovement = true;
					}
				}
			}

			if ((GLFW_PRESS == xbox[0] || Application::IsKeyPressed('W')) && elapsedTime > bufferTime_Jump)
			{
				bufferTime_Jump = elapsedTime + 0.6f;
				bufferTime_JumpUp = elapsedTime + 0.3f;
			}
			if ((GLFW_PRESS == xbox[2] || Application::IsKeyPressed(VK_LBUTTON)) && !attack && !injump && !holdanims())
			{
				bufferTime_attack = elapsedTime + 1.f;

				//if ((EnemyManager::get_instance()->EnemyList[0]->get_health() != 0) && PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox))
				//	EnemyManager::get_instance()->EnemyList[0]->edit_health(-50);
				
				if ((PlayerClass::get_instance()->PlayerHitBox.collide(Boss::get_instance()->EnemyHitBox)))
						Boss::get_instance()->bossHealthSystem(false);
				
				// add on to this later
			}

			if ((GLFW_PRESS == xbox[3] || Application::IsKeyPressed(VK_LSHIFT) || Application::IsKeyPressed(VK_RSHIFT)) && !roll)
				bufferTime_block = elapsedTime + 0.2f;

			if ((GLFW_PRESS == xbox[1] || Application::IsKeyPressed(VK_RBUTTON)) && !holdanims())
			{
				bufferTime_roll = elapsedTime + 0.8f;
				bufferTime_iframeroll = elapsedTime + 0.35f;
			}

			/* mutant */

			//if (EnemyManager::get_instance()->EnemyList[0]->get_health() > 0)
			//{
			//	if (elapsedTime > bufferTime_attack_M)
			//	{
			//		EnemyManager::get_instance()->EnemyList[0]->attack(1, EnemyManager::get_instance()->EnemyList[0]->position_m, EnemyManager::get_instance()->EnemyList[0]->direction_m, dt, block);

			//		bufferTime_attack_M = elapsedTime + 2.f;
			//	}
			//}
		}
	}
	else
	{
		if (elapsedTime > 1.1f) // This pre-setting ensures animations won't occur at the very start, so animations glitching out will not happen anymore.*
		{									// *I hope.
			inmovement = false;				// so many if statements I could write a philosophy book
			if (Application::IsKeyPressed('A') && !roll)
			{
				if (!trigger)
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LEFTWALL]->MeshBBox)
						|| pressedD == true)
					{
						PlayerClass::get_instance()->position_a.x -= (float)(movespeed * dt);
						pressedD = false;
						pressedA = true;
						inmovement = true;
					}
				}
				else
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox)
						&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTWALL]->MeshBBox)
						|| pressedD == true)
					{
						PlayerClass::get_instance()->position_a.x -= (float)(movespeed * dt);
						pressedD = false;
						pressedA = true;
						inmovement = true;
					}
				}
			}

			if (Application::IsKeyPressed('D') && !roll)
			{
				if (!trigger)
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LEFTWALL]->MeshBBox)
						|| pressedA == true)
					{
						PlayerClass::get_instance()->position_a.x += (float)(movespeed * dt);
						pressedA = false;
						pressedD = true;
						inmovement = true;
					}
				}
				else
				{
					if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox)
						&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTWALL]->MeshBBox)
						|| pressedA == true)
					{
						PlayerClass::get_instance()->position_a.x += (float)(movespeed * dt);
						pressedA = false;
						pressedD = true;
						inmovement = true;
					}
				}
			}

			if (Application::IsKeyPressed('W') && elapsedTime > bufferTime_Jump)
			{
				bufferTime_Jump = elapsedTime + 0.6f;
				bufferTime_JumpUp = elapsedTime + 0.3f;
			}
			if (Application::IsKeyPressed(VK_LBUTTON) && !attack && !injump && !holdanims())
			{
				bufferTime_attack = elapsedTime + 1.f;
			
				if ((PlayerClass::get_instance()->PlayerHitBox.collide(Boss::get_instance()->EnemyHitBox)))
					Boss::get_instance()->bossHealthSystem(false);
				
			/*	if ((EnemyManager::get_instance()->EnemyList[0]->get_health() != 0) && PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox))
					EnemyManager::get_instance()->EnemyList[0]->edit_health(-50);*/
			}

			if ((Application::IsKeyPressed(VK_LSHIFT) || Application::IsKeyPressed(VK_RSHIFT)) && !roll)
				bufferTime_block = elapsedTime + 0.2f;

			if (Application::IsKeyPressed(VK_RBUTTON) && !holdanims())
			{
				bufferTime_roll = elapsedTime + 0.8f;
				bufferTime_iframeroll = elapsedTime + 0.35f;
			}

			/* mutant */

			//if (EnemyManager::get_instance()->EnemyList[0]->get_health() > 0)
			//{
			//	if (elapsedTime > bufferTime_attack_M)
			//	{
			//		EnemyManager::get_instance()->EnemyList[0]->attack(1, EnemyManager::get_instance()->EnemyList[0]->position_m, EnemyManager::get_instance()->EnemyList[0]->direction_m, dt, block);

			//		bufferTime_attack_M = elapsedTime + 2.f;
			//	}
			//}
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
		attack = false;

	if (bufferTime_roll > elapsedTime)
	{
		roll = true;
		et[7] += dt;

		if (pressedA && !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LEFTWALL]->MeshBBox))
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox) && trigger)
				PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
			else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox) && !trigger)
				PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
		}
		else if (pressedD && !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LEFTWALL]->MeshBBox))
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox) && trigger)
				PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
			else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox) && !trigger)
				PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
		}
	}
	else
		roll = false;

	if (bufferTime_block > elapsedTime)
	{
		block = true;
		et[8] += dt;
	}
	else
		block = false;

	if (block)
		movespeed = 17.f;
	else
		movespeed = 30.f;

	if (inmovement && !holdanims())
		et[6] += dt;
	else
		et[6] = 0;

	et[20] += dt;

	if (injump == false)
	{
		if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_GROUND]->MeshBBox))
		{
			//if ((PlayerClass::get_instance()->PlayerHitBox.higherthan(meshList[GEO_BOX_SHORT]->MeshBBox) && // If doing platforms look at this
			//	PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_SHORTTEST]->MeshBBox)) ||
			//	(PlayerClass::get_instance()->PlayerHitBox.higherthan(meshList[GEO_BOX_TALL]->MeshBBox) &&
			//	PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BOX_TALLTEST]->MeshBBox)))
			//{
			//	// do jack shit
			//}
			bufferTime_Jump = elapsedTime + 0.1f; // this fixes a bug I never thought was there in the first place, preventing double jump
			PlayerClass::get_instance()->position_a.y -= (float)(30.f * dt);
		}
	}
	else
	{
		PlayerClass::get_instance()->position_a.y += (float)(30.f * dt);
	}

	if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER]->MeshBBox) && !trigger) // This will only run once.
	{
		PlayerClass::get_instance()->position_a.x += (float)(69 * dt); // huehuehue I'm not 12 I swear
		pressedA = false;
		pressedD = false;
		inmovement = false;
		trigger = true;
	}

	if (trigger && plop < 50)
		plop += 1;

	//if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRIGGER_SLOPE]->MeshBBox))
	//{
	//	bufferTime_trigger_slope = elapsedTime + 11.f;
	//	trigger = true;
	//}

	//if (burrowing)
	//	return 1;
	//else if (spinning)
	//	return 2;
	//else if (tailattacking)
	//	return 3;
	//else
	//	return 0;

	if (Boss::get_instance()->get_action() == 1)
	{
		et[10] = 0;
		et[11] = 0;
		et[12] += dt;
		et[13] = 0;
	}
	else if (Boss::get_instance()->get_action() == 2)
	{
		et[10] = 0;
		et[11] = 0;
		et[12] += dt;
		et[13] = 0;
	}
	else if (Boss::get_instance()->get_action() == 3)
	{
		et[10] = 0;
		et[11] = 0;
		et[12] = 0;
		et[13] += dt;
	}
	else
	{
		if (elapsedTime > bufferTime_attack_M)
			bufferTime_attack_M += 2.f;

		et[12] = 0;
		et[13] = 0;

		if ((elapsedTime + 0.5f < bufferTime_attack_M) && (elapsedTime + 1.5f > bufferTime_attack_M))
		{
			et[11] = 0;
			et[10] += dt;
		}
		else
		{
			et[10] = 0;
			et[11] += dt;
		}
	}

	PlayerClass::get_instance()->PlayerHitBox.loadBB("OBJ//Character//crotch.obj");

	for (unsigned int projectiles = 0; projectiles < Boss::get_instance()->spit_.size(); projectiles++)
	{
		if (Boss::get_instance()->spit_[projectiles] != nullptr)
			Boss::get_instance()->spit_[projectiles]->projHitBox_.loadBB("OBJ//Mutant_Projectile.obj");
	}

	//for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	//{
	//	if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
	//	{
	//		for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
	//		{
	//			if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] != nullptr)
	//				EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.loadBB("OBJ//Mutant_Projectile.obj");
	//		}
	//	}
	//}

	//if (EnemyManager::get_instance()->EnemyList[0]->get_health() > 0)
	//	EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");

	Boss::get_instance()->Boss_Tail.TailHitBox.loadBB("OBJ//Boss//Boss_Spike.obj");
	Boss::get_instance()->EnemyHitBox.loadBB("OBJ//Boss//Boss_Torso.obj");
	/*--------------------------------------------------------*/

	//text&Light interaction
	TextInteraction();
	LightInteraction();

	/*--------------Updates the Full Mutant Kill Count--------*/
	//for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
	//{
	//	if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() == 0)
	//	{
	//		PlayerClass::get_instance()->fm_Killed++;
	//		EnemyManager::get_instance()->EnemyList[numEnemy]->edit_health(-1);
	//	}
	//}
	/*-------------------------------------------------------*/

	/*---------Change Scene------*/
	if (Boss::get_instance()->get_health() <= 0)
		SceneManager::getInstance()->changeScene(new VictoryScreen());
	if (PlayerClass::get_instance()->get_health() <= 0)
		SceneManager::getInstance()->changeScene(new DeathScreen());
	/*---------------------------*/
}


void StudioProject2SceneBoss::Render()
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

	unsigned int num_anim;
	for (num_anim = 0; num_anim < 10;)
	{
		if (et[num_anim] <= 0.f)
			num_anim++;
		else
			break;
	}

	/*-----------------Skybox-------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(450, 80, -200);
	modelStack.Scale(1600, 675, 1);
	RenderMesh(meshList[GEO_SKYBOX], false);
	modelStack.PopMatrix();
	/*------------------------------------------*/
	/*-----------------Main Character (Alexis)---------------------*/
	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	PlayerClass::get_instance()->PlayerHitBox.scale(1.1f, 4.5f, 1.f);
	PlayerClass::get_instance()->PlayerHitBox.translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);
	modelStack.Translate(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y, PlayerClass::get_instance()->position_a.z);
	modelStack.Rotate(PlayerClass::get_instance()->a_LookingDirection, 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Translate(PlayerClass::get_instance()->laserTranslate.x, PlayerClass::get_instance()->laserTranslate.y, PlayerClass::get_instance()->laserTranslate.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(PlayerClass::get_instance()->laserSize.x, PlayerClass::get_instance()->laserSize.y, PlayerClass::get_instance()->laserSize.z);
	RenderMesh(meshList[GEO_LASER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	glBlendFunc(GL_ONE, GL_ONE);
	modelStack.Scale(PlayerClass::get_instance()->ProjShieldSize.x, PlayerClass::get_instance()->ProjShieldSize.y, 
		PlayerClass::get_instance()->ProjShieldSize.z);
	RenderMesh(meshList[GEO_SHIELD], false);
	modelStack.PopMatrix();

	// add in grab animation later
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (num_anim == 7 || num_anim == 8 || num_anim == 9)
	{
		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "pCylinder15");//crotch
		RenderMesh(meshList[GEO_ALEXUS_CROTCH], true);

			modelStack.PushMatrix();
			AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface32");//BODY
			RenderMesh(meshList[GEO_ALEXUS_BODY], true);

				modelStack.PushMatrix();
				AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface9"); // HEAD
				RenderMesh(meshList[GEO_ALEXUS_HEAD], true);

				modelStack.PopMatrix();

				modelStack.PushMatrix();
				AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere17");//ARM WITH SWORD
				RenderMesh(meshList[GEO_ALEXUS_LEFT4ARM], true);

					modelStack.PushMatrix();
					AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface24");//ARM WITH SWORD
					RenderMesh(meshList[GEO_ALEXUS_LEFTARM], true);

					modelStack.PopMatrix();
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere14");//LEFTARM
				RenderMesh(meshList[GEO_ALEXUS_RIGHT4ARM], true);

					modelStack.PushMatrix();
					AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface30");//LEFTARM
					RenderMesh(meshList[GEO_ALEXUS_RIGHTARM], true);

					modelStack.PopMatrix();
				modelStack.PopMatrix();

			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere9");//RIGHT LEG
			RenderMesh(meshList[GEO_ALEXUS_LEFTTHIGH], true);

				modelStack.PushMatrix();
				AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface10");//RIGHT LEG
				RenderMesh(meshList[GEO_ALEXUS_LEFTLEG], true);

				modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere10");//LEFTLEG
			RenderMesh(meshList[GEO_ALEXUS_RIGHTTHIGH], true);

				modelStack.PushMatrix();
				AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface12");//LEFTLEG
				RenderMesh(meshList[GEO_ALEXUS_RIGHTLEG], true);

				modelStack.PopMatrix();
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface9"); // HEAD
		RenderMesh(meshList[GEO_ALEXIS_HEAD], true);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere17");//ARM WITH SWORD
		RenderMesh(meshList[GEO_ALEXIS_LEFTARM], true);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "polySurface32");//BODY
		RenderMesh(meshList[GEO_ALEXIS_BODY], true);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere14");//LEFTARM
		RenderMesh(meshList[GEO_ALEXIS_RIGHTARM], true);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "pCylinder15");//crotch
		RenderMesh(meshList[GEO_ALEXIS_CROTCH], true);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere9");//RIGHT LEG
		RenderMesh(meshList[GEO_ALEXIS_LEFTLEG], true);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(num_anim, &modelStack, &et[num_anim], "pSphere10");//LEFTLEG
		RenderMesh(meshList[GEO_ALEXIS_RIGHTLEG], true);

		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(PlayerClass::get_instance()->position_a.x,
		(PlayerClass::get_instance()->position_a.y + 7.9f), // render collision box
		PlayerClass::get_instance()->position_a.z);		 // i need this
	modelStack.Scale(1.1f, 4.5f, 1.f);										 // if you remove it bad things will happen
	RenderMesh(meshList[GEO_BBOX], false);									 // remove this later when showing actual shit of course
	modelStack.PopMatrix();												 	 // :ok_hand:
	// for some reason I needed to flip translate and scale here to fit with the actual hitbox

	//modelStack.PushMatrix();								// why did i even render this?
	//RenderMesh(meshList[GEO_TRIGGER_SLOPE], false);		// note to self don't use "meshList[GEO_SOMETHING]->MeshBBox.translate(a_PosX, (a_PosY + 8), a_PosZ);" often
	//modelStack.PopMatrix();								// this shit runs every second so smallest translations will move by a lot eventually


	unsigned int num_anim_boss;
	for (num_anim_boss = 10; num_anim_boss < 20;)
	{
		if (et[num_anim_boss] <= 0.f)
			num_anim_boss++;
		else
			break;
	}

	/*------Boss-----*/
	modelStack.PushMatrix();

	Boss::get_instance()->EnemyHitBox.scale(2.f, 3.f, 3.f);
	Boss::get_instance()->EnemyHitBox.translate(Boss::get_instance()->position_m.x, Boss::get_instance()->position_m.y, Boss::get_instance()->position_m.z);

	modelStack.Translate(Boss::get_instance()->position_m.x, Boss::get_instance()->position_m.y, Boss::get_instance()->position_m.z);
	modelStack.Rotate(-70, 0, 1, 0);

		modelStack.PushMatrix();
		AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Torso");
		RenderMesh(meshList[GEO_BOSS_TORSO], true);

			modelStack.PushMatrix();
			AnimCheck_Boss(10, &modelStack, &et[10], "Boss_LeftJaw");
			RenderMesh(meshList[GEO_BOSS_LJAW], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Boss(10, &modelStack, &et[10], "Boss_RightJaw");
			RenderMesh(meshList[GEO_BOSS_RJAW], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Neck");
			RenderMesh(meshList[GEO_BOSS_NECK], true);

				modelStack.PushMatrix();
				AnimCheck_Boss(10, &modelStack, &et[10], "Boss_MainHead");
				RenderMesh(meshList[GEO_BOSS_MHEAD], true);

					modelStack.PushMatrix();
					AnimCheck_Boss(10, &modelStack, &et[10], "Boss_MainJaw");
					RenderMesh(meshList[GEO_BOSS_MJAW], true);

					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Boss(10, &modelStack, &et[10], "Boss_LeftUpperarm");
			RenderMesh(meshList[GEO_BOSS_LUARM], true);

				modelStack.PushMatrix();
				AnimCheck_Boss(10, &modelStack, &et[10], "Boss_LeftArm");
				RenderMesh(meshList[GEO_BOSS_LARM], true);

				modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Boss(10, &modelStack, &et[10], "Boss_RightUpperarm");
			RenderMesh(meshList[GEO_BOSS_RUARM], true);

				modelStack.PushMatrix();
				AnimCheck_Boss(10, &modelStack, &et[10], "Boss_RightArm");
				RenderMesh(meshList[GEO_BOSS_RARM], true);

				modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Segment1");
			RenderMesh(meshList[GEO_BOSS_SEG1], true);

				modelStack.PushMatrix();
				AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Segment2");
				RenderMesh(meshList[GEO_BOSS_SEG2], true);

					modelStack.PushMatrix();
					AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Segment3");
					RenderMesh(meshList[GEO_BOSS_SEG3], true);

						modelStack.PushMatrix();
						AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Segment4");
						RenderMesh(meshList[GEO_BOSS_SEG4], true);

							modelStack.PushMatrix();
							AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Segment5");
							RenderMesh(meshList[GEO_BOSS_SEG5], true);

								modelStack.PushMatrix();
								AnimCheck_Boss(10, &modelStack, &et[10], "Boss_Segment6");
								RenderMesh(meshList[GEO_BOSS_SEG6], true);

								modelStack.PopMatrix(); // do
							modelStack.PopMatrix(); // ti
						modelStack.PopMatrix(); // la
					modelStack.PopMatrix(); // so
				modelStack.PopMatrix(); // fa
			modelStack.PopMatrix(); //mi
		modelStack.PopMatrix(); // re
	modelStack.PopMatrix(); // do

	if (Boss::get_instance()->get_action() == 3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Boss::get_instance()->Boss_Tail.position_t.x, PlayerClass::get_instance()->position_a.y + 2.5f, Boss::get_instance()->Boss_Tail.position_t.z);
		modelStack.Scale(0.1f, 1.f, 0.1f);
		RenderMesh(meshList[GEO_BOSS_INDICATOR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(Boss::get_instance()->Boss_Tail.position_t.x, Boss::get_instance()->Boss_Tail.position_t.y, Boss::get_instance()->Boss_Tail.position_t.z);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderMesh(meshList[GEO_SPIKE], true);
		modelStack.PopMatrix();
	}
	else if (Boss::get_instance()->get_action() == 2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Boss::get_instance()->position_m.x, Boss::get_instance()->position_m.y + 2.5f, Boss::get_instance()->position_m.z);
		modelStack.Scale(0.2f, 1.f, 0.2f);
		RenderMesh(meshList[GEO_BOSS_INDICATOR], true);
		modelStack.PopMatrix();
	}

	/*---------------*/

	modelStack.PushMatrix();
	//modelStack.Translate(Boss::get_instance()->position_m.x, Boss::get_instance()->position_m.y, Boss::get_instance()->position_m.z);
	//modelStack.Scale(2.25f, 3, 3);
	RenderMesh(meshList[GEO_TESTBBOX], false);
	modelStack.PopMatrix();

	/*--------------------Environment------------------------*/

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_ENVIRONMENT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(113, 60 - plop, 0);
	modelStack.Scale(1, 3, 1);
	RenderMesh(meshList[GEO_PREVENT], false);
	modelStack.PopMatrix();

	/*-------------------------------------------------------*/

	/*-----------------Mutants (Fuglymon)---------------------*/
	//unsigned int num_anim_mutant;
	//for (num_anim_mutant = 20; num_anim_mutant <= 30;)
	//{
	//	if (et[num_anim_mutant] <= 0.f)
	//		num_anim_mutant++;
	//	else
	//		break;
	//}

	RenderProjectiles();

	/*if (EnemyManager::get_instance()->EnemyList[0]->get_health() > 0)
	{
		RenderProjectiles();
		RenderMutant(num_anim_mutant);
	}*/
	/*-------------------------------------------------------*/

	/*-----------------Environmental Light Rendering------*/
	RenderLightStands();
	/*----------------------------------------------------*/

	/*----Textbox Rendering--------*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderMeshOnScreen(meshList[GEO_TEXTBOX], 0, 0, 100, 15, 0);
	/*-----------------------------*/

	/*------------------HUD-------------------------------------------------*/
	/*----Half mutant count--------*/
	RenderMeshOnScreen(meshList[GEO_HALF_COUNT], 5, 0.5, 8, 8, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], hMutantSaved, Color(1, 1, 1), 4, 12.3, -9);
	/*-----------------------------*/
	/*----Full mutant count--------*/
	RenderMeshOnScreen(meshList[GEO_FULL_COUNT], 7, 0.5, 8, 8, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], fMutantKilled, Color(1, 1, 1), 4, 16.3, -9);
	/*-----------------------------*/
	/*------Spell HUD-----*/
	RenderMeshOnScreen(meshList[GEO_LASER_ICON], 10, 10,
		PlayerClass::get_instance()->spellHUD.laserReady, PlayerClass::get_instance()->spellHUD.laserReady, 0);
	RenderMeshOnScreen(meshList[GEO_LASER_CD], 10, 10,
		PlayerClass::get_instance()->spellHUD.laserNotReady, PlayerClass::get_instance()->spellHUD.laserNotReady, 0);

	RenderMeshOnScreen(meshList[GEO_PROJSHIELD], 12, 10,
		PlayerClass::get_instance()->spellHUD.projShieldReady, PlayerClass::get_instance()->spellHUD.projShieldReady, 0);
	RenderMeshOnScreen(meshList[GEO_PROJSHIELD_CD], 12, 10,
		PlayerClass::get_instance()->spellHUD.projShieldNotReady, PlayerClass::get_instance()->spellHUD.projShieldNotReady, 0);
	/*--------------------*/
	/*----Heart Rendering----------*/
	float positionXscreen = 2;
	float positionYscreen = 28.5;
	for (int i = 0; i < 10; i++)
	{
		RenderMeshOnScreen(meshList[GEO_ALEXIS_LIFE], positionXscreen, positionYscreen,
			PlayerClass::get_instance()->Hearts.a_heart[i], PlayerClass::get_instance()->Hearts.a_heart[i], 0);
		RenderMeshOnScreen(meshList[GEO_BLANKHEART], positionXscreen, positionYscreen,
			PlayerClass::get_instance()->Hearts.a_blankheart[i], PlayerClass::get_instance()->Hearts.a_blankheart[i], 0);

		positionXscreen += 2;
	}
	/*-----------------------------*/
	float energyX = 7;
	for (int i = 0; i < 10; i++)
	{
		RenderMeshOnScreen(meshList[GEO_ENERGY], energyX, 27,
			PlayerClass::get_instance()->Hearts.a_energy[i], PlayerClass::get_instance()->Hearts.a_energy[i], 0);
		RenderMeshOnScreen(meshList[GEO_BLANKENERGY], energyX, 27,
			PlayerClass::get_instance()->Hearts.a_blankenergy[i], PlayerClass::get_instance()->Hearts.a_blankenergy[i], 0);

		energyX += 1;
	}
	/*--------Boss Life Rendering---*/
	float posXscreen = 6;
	for (int i = 0; i < 30; i++)
	{
		RenderMeshOnScreen(meshList[GEO_BOSSLIFE], posXscreen, 25,
			Boss::get_instance()->bossLife.boss_heart[i], Boss::get_instance()->bossLife.boss_heart[i], 0);

		posXscreen += 0.5;
	}
	/*-----------------------------*/
	/*---------------Text log Rendering--------*/
	RenderTextInteractions();
	/*-----------------------------------------*/
	RenderTextOnScreen(meshList[GEO_TEXT], "ENERGY", Color(0, 0, 1), 2, 3, 17.5);
	RenderTextOnScreen(meshList[GEO_TEXT], "BOSS", Color(1, 1, 0), 2, 3, 15.5);
	RenderTextOnScreen(meshList[GEO_TEXT], fps, Color(0, 1, 0), 2, 36, 19);
	RenderTextOnScreen(meshList[GEO_TEXT], "CAVERN OF TRUTH", Color(0, 0, 1), 2.5, 1.5, -8.5);
	/*----------------------------------------------------------------------------------*/
}

bool StudioProject2SceneBoss::otheranims()
{
	return (attack);
}

bool StudioProject2SceneBoss::holdanims()
{
	return (roll || block);
}

void StudioProject2SceneBoss::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void StudioProject2SceneBoss::RenderProjectiles()
{
	for (unsigned int projectiles = 0; projectiles < Boss::get_instance()->spit_.size(); projectiles++)
	{
		if (Boss::get_instance()->spit_[projectiles] != nullptr)
		{
			modelStack.PushMatrix();
			modelStack.Translate(Boss::get_instance()->spit_[projectiles]->position_.x,
				(Boss::get_instance()->spit_[projectiles]->position_.y + 27.f),
				Boss::get_instance()->spit_[projectiles]->position_.z);
			RenderMesh(meshList[GEO_SPIT], false);
			modelStack.PopMatrix();
		}
	}
}

void StudioProject2SceneBoss::RenderMutant(unsigned int num_anim_mutant)
{
	//modelStack.PushMatrix();

	//EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.scale(4.f, 2.1f, 1.f);

	//EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.y,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.z);

	//modelStack.Translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.y,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.z);

	//if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == -1)
	//	modelStack.Rotate(180, 0, 1, 0);
	//else if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == 1)
	//	modelStack.Rotate(0, 0, 1, 0);

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_Torso");
	//RenderMesh(meshList[GEO_MUTANT_TORSO], true);

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_Neck");
	//RenderMesh(meshList[GEO_MUTANT_NECK], true);

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_Head");
	//RenderMesh(meshList[GEO_MUTANT_HEAD], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == -1)
	//	modelStack.Rotate(-180, 0, 1, 0);

	//AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_Head");
	//modelStack.Translate(-2, 5, 0);
	//RenderMesh(meshList[GEO_M_RHEART], false);
	//modelStack.Translate(3, 0, 0);
	//if (EnemyManager::get_instance()->EnemyList[0]->get_health() == 100)
	//	RenderMesh(meshList[GEO_M_RHEART], false);
	//else
	//	RenderMesh(meshList[GEO_M_BHEART], false);

	//modelStack.PopMatrix();
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_LeftUpperarm");
	//RenderMesh(meshList[GEO_MUTANT_LEFTUPPERARM], true);

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_LeftArm");
	//RenderMesh(meshList[GEO_MUTANT_LEFTARM], true);

	//modelStack.PopMatrix();
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_RightUpperarm");
	//RenderMesh(meshList[GEO_MUTANT_RIGHTUPPERARM], true);

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_RightArm");
	//RenderMesh(meshList[GEO_MUTANT_RIGHTARM], true);

	//modelStack.PopMatrix();
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_LeftThigh");
	//RenderMesh(meshList[GEO_MUTANT_LEFTTHIGH], true);

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_LeftFeet");
	//RenderMesh(meshList[GEO_MUTANT_LEFTFEET], true);

	//modelStack.PopMatrix();
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_RightThigh");
	//RenderMesh(meshList[GEO_MUTANT_RIGHTTHIGH], true);

	//modelStack.PushMatrix();
	//AnimCheck_Mutant(num_anim_mutant, &modelStack, &et[num_anim_mutant], "Mutant_RightFeet");
	//RenderMesh(meshList[GEO_MUTANT_RIGHTFEET], true);

	//modelStack.PopMatrix(); // staircase ayy
	//modelStack.PopMatrix();
	//modelStack.PopMatrix();
	//modelStack.PopMatrix();
}