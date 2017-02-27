#include "SceneBoss.h"
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
#include "SceneManager.h"
#include "Boss.h"
#include "DeathScreen.h"
#include "VictoryScreen.h"
#include <vector>

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34

SceneBoss::SceneBoss()
{
}

SceneBoss::~SceneBoss()
{
}

void SceneBoss::Init()
{
	PlayerClass::get_instance();
	PlayerClass::get_instance()->a_LookingDirection = -90.f;
	PlayerClass::get_instance()->position_a = Vector3(50,0,0);
	/*-----------Hearts Initialisation-----*/
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->manaUI();

	Boss::get_instance()->bossHealthUI();
	Boss::get_instance()->bossInit();
	/*-------------------------------------*/
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

	camera.Init(Vector3(90, 180, 10), Vector3(10, 0, 10), Vector3(0, 1, 0));

	meshList[GEO_AXIS] = MeshBuilder::GenerateAxis("reference");
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1.f, 1.f, 1.f), 18, 36, 1.f);

	/*-----------------------------Skybox Loading----------------------------------*/
	//meshList[GEO_SKYBOX] = MeshBuilder::GenerateQuad("skybox", Color(1, 1, 1));
	//meshList[GEO_SKYBOX]->textureID = LoadTGA("Image//SkyBG.tga");
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1));
	/*-----------------------------------------------------------------------------*/

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

	Boss::get_instance()->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");

	meshList[GEO_SPIT] = MeshBuilder::GenerateOBJ("Spit", "OBJ//Scene1//Box_Short.obj"); //box short placeholder for spit projectile
	/*-------------------------------------------------------------*/
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
	/*--------------------------Text Loading---------------------------------------*/
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//franklingothicheavy.tga");
	/*-----------------------------------------------------------------------------*/

	
	/*-------------------------Loading Mutant Health----------------------------------*/
	meshList[GEO_M_RHEART] = MeshBuilder::GenerateOBJ("MutantHealthRed", "OBJ//M_HealthRed.obj");
	meshList[GEO_M_RHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
	meshList[GEO_M_BHEART] = MeshBuilder::GenerateOBJ("MutantHealthBlack", "OBJ//M_HealthBlack.obj");
	meshList[GEO_M_BHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
	/*--------------------------------------------------------------------------------*/

	/*------------------------------HUD Loading -------------------------------------*/
	/*-------------------------Loading Alexis Health--------------------*/
	meshList[GEO_BLANKHEART] = MeshBuilder::GenerateQuad("blankheart", Color(1, 1, 1));
	meshList[GEO_BLANKHEART]->textureID = LoadTGA("Image//heartsb.tga");
	meshList[GEO_ALEXIS_LIFE] = MeshBuilder::GenerateQuad("heart", Color(1, 1, 1));
	meshList[GEO_ALEXIS_LIFE]->textureID = LoadTGA("Image//hearts.tga");
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
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_LUARM] = MeshBuilder::GenerateOBJ("Boss_LeftUpperarm", "OBJ//Boss//Boss_LeftUpperarm.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_MHEAD] = MeshBuilder::GenerateOBJ("Boss_MainHead", "OBJ//Boss//Boss_MainHead.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_MJAW] = MeshBuilder::GenerateOBJ("Boss_MainJaw", "OBJ//Boss//Boss_MainJaw.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_NECK] = MeshBuilder::GenerateOBJ("Boss_Neck", "OBJ//Boss//Boss_Neck.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_RARM] = MeshBuilder::GenerateOBJ("Boss_RightArm", "OBJ//Boss//Boss_RightArm.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_RJAW] = MeshBuilder::GenerateOBJ("Boss_RightJaw", "OBJ//Boss//Boss_RightJaw.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_RUARM] = MeshBuilder::GenerateOBJ("Boss_RightUpperarm", "OBJ//Boss//Boss_RightUpperarm.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG1] = MeshBuilder::GenerateOBJ("Boss_Segment1", "OBJ//Boss//Boss_Segment1.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG2] = MeshBuilder::GenerateOBJ("Boss_Segment2", "OBJ//Boss//Boss_Segment2.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG3] = MeshBuilder::GenerateOBJ("Boss_Segment3", "OBJ//Boss//Boss_Segment3.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG4] = MeshBuilder::GenerateOBJ("Boss_Segment4", "OBJ//Boss//Boss_Segment4.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG5] = MeshBuilder::GenerateOBJ("Boss_Segment5", "OBJ//Boss//Boss_Segment5.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_SEG6] = MeshBuilder::GenerateOBJ("Boss_Segment6", "OBJ//Boss//Boss_Segment6.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");
	meshList[GEO_BOSS_TORSO] = MeshBuilder::GenerateOBJ("Boss_Torso", "OBJ//Boss//Boss_Torso.obj");
	meshList[GEO_BOSS_LARM]->textureID = LoadTGA("Image//Boss_Mutant_Texture.tga");

	meshList[GEO_BOSS_INDICATOR] = MeshBuilder::GenerateOBJ("Boss_Mutant_Range", "OBJ//Boss//Boss_Mutant_Range.obj");
	meshList[GEO_SPIKE] = MeshBuilder::GenerateOBJ("Boss_Spike", "OBJ//Boss//Boss_Spike.obj");

	Boss::get_instance()->Boss_Tail.TailHitBox.loadBB("OBJ//Boss//Boss_Spike.obj");

	meshList[GEO_LASER] = MeshBuilder::GenerateCylinder("laser", Color(1, 0, 0));
	/*--------------------------------------------------------------------------------*/
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);

	/*----------------------Light Initialisation-----------------------------------*/
	LoadLight();
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	/*-------------------------------------------------------------------------------*/
}

void SceneBoss::Update(double dt)
{
	int framespersec = 1 / dt;
	elapsedTime += dt;

	/*--------Boss Functions--------------*/
	Boss::get_instance()->bossHealthSystem(elapsedTime);
	Boss::get_instance()->bossHealthUI();
	Boss::get_instance()->stateManager();
	Boss::get_instance()->dmgOvertime(elapsedTime);
	Boss::get_instance()->burrowTeleportation(elapsedTime);
	Boss::get_instance()->tailAttack(elapsedTime, block);
	//Boss::get_instance()->spinAttack(elapsedTime, false);
	/*------------------------------------*/

	/*-------AI Functions---------------*/
	//EnemyManager::get_instance()->EnemyList[0]->update(dt);

	//// I spent 10 years trying to fix projectile because I wanted to avoid using erase.
	//// Erase won today. Erase, me, 1:0. Shit.

	//for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++) // in case got error, -- proj when delete
	//{
	//	for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
	//	{
	//		if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] != nullptr)
	//		{
	//			EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.translate(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.x,
	//				(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.y + 10.f),
	//				EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.z);

	//			if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(meshList[GEO_TRUMP]->MeshBBox) || EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->displacement() > 300.f)
	//			{
	//				EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
	//			}
	//			else if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(PlayerClass::get_instance()->PlayerHitBox) &&
	//				(elapsedTime > bufferTime_iframe) && (elapsedTime > bufferTime_iframeroll))
	//			{
	//				PlayerClass::get_instance()->healthSystem(block);
	//				bufferTime_iframe = elapsedTime + 0.3f;
	//				EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
	//			}
	//		}
	//	}
	//}

	/*-------Player Functions------------------*/
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->manaUI();
	PlayerClass::get_instance()->timeSpent(dt);
	PlayerClass::get_instance()->bossFightFacingDirection();
	PlayerClass::get_instance()->spellUI(elapsedTime);
	

	if (Boss::get_instance()->magicImmunity == false)
	{
		PlayerClass::get_instance()->laserBeam(elapsedTime);
		//PlayerClass::get_instance()->projectileShield(elapsedTime);
	}
	/*-----------------------------------------*/
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

	/*----------Player Movement and Collisions------------*/
	if ((Application::IsKeyPressed('A') && Application::IsKeyPressed('D')) ||
		(Application::IsKeyPressed('W') && Application::IsKeyPressed('S')) || 
		roll == true)
	{
	}
	else
	{
		if (Application::IsKeyPressed('W'))
		{
			PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
		}
		if (Application::IsKeyPressed('S'))
		{
			PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
		}
		if (Application::IsKeyPressed('A'))
		{
			PlayerClass::get_instance()->position_a.z += (float)(30.f * dt);
		}
		if (Application::IsKeyPressed('D'))
		{
			PlayerClass::get_instance()->position_a.z -= (float)(30.f * dt);
		}
	}

	if (Application::IsKeyPressed(VK_LBUTTON) && !attack && !holdanims())
	{
		bufferTime_attack = elapsedTime + 1.f;

		//if ((EnemyManager::get_instance()->EnemyList[0]->get_health() != 0) && PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox))
		//	EnemyManager::get_instance()->EnemyList[0]->edit_health(-50);
	}

	if ((Application::IsKeyPressed(VK_LSHIFT) || Application::IsKeyPressed(VK_RSHIFT)) && !roll)
		bufferTime_block = elapsedTime + 0.2f;

	if (Application::IsKeyPressed(VK_RBUTTON) && !holdanims())
	{
		bufferTime_roll = elapsedTime + 0.8f;
		bufferTime_iframeroll = elapsedTime + 0.35f;
	}
	/*-----------------------------------------------*/
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

	et[20] += dt;		// This is for me to see if the idleanim is running at all

	PlayerClass::get_instance()->PlayerHitBox.loadBB("OBJ//Character//crotch.obj");

	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
		{
			if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] != nullptr)
				EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.loadBB("OBJ//Scene1//Box_Short.obj");
		}
	}

	et[10] += dt;

	//if (Boss::get_instance()->get_action() == 1)
	//{
	//	et[10] = 0;
	//	et[11] += dt;
	//	et[12] = 0;
	//	et[13] = 0;
	//	et[14] = 0;
	//	et[15] = 0;
	//}
	//else
	//{
	//	et[10] += dt;
	//	et[11] = 0;
	//	et[12] = 0;
	//	et[13] = 0;
	//	et[14] = 0;
	//	et[15] = 0;
	//}

	//if (EnemyManager::get_instance()->EnemyList[0]->get_action() == 1)
	//{
	//	et[20] = 0;
	//	et[21] = 0;
	//	et[22] += dt;
	//	et[23] = 0;

	//	if (elapsedTime + 1.5f < bufferTime_attack_M)
	//	{
	//		et[20] = 0;
	//		et[22] += dt;
	//	}
	//	else
	//	{
	//		et[22] = 0;
	//		et[20] += dt;
	//	}
	//}
	//else if (EnemyManager::get_instance()->EnemyList[0]->get_action() == 2)
	//{
	//	et[21] = 0;
	//	et[22] = 0;
	//	if ((elapsedTime + 0.5f < bufferTime_attack_M) && (elapsedTime + 1.5f > bufferTime_attack_M))
	//	{
	//		et[23] = 0;
	//		et[20] += dt;
	//	}
	//	else
	//	{
	//		et[20] = 0;
	//		et[23] += dt;
	//	}
	//}
	//else
	//{
	//	et[20] = 0;
	//	et[21] += dt;
	//	et[22] = 0;
	//	et[23] = 0;
	//}

	//EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj"); // THIS SNEAKY ASS LINE OF CODE RUINED COLLISION FOR THE PAST HOUR OMG.
	// I UNCOMMENTED IT AND OPENED PANDORA'S BOX, WISH ME LUCK.

	Boss::get_instance()->Boss_Tail.TailHitBox.loadBB("OBJ//Boss//Boss_Spike.obj");

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

	/*-------------Scene Change--------*/
	if (Boss::get_instance()->get_health() <= 0)
		SceneManager::getInstance()->changeScene(new VictoryScreen()); 
	if (PlayerClass::get_instance()->get_health() <= 0)
		SceneManager::getInstance()->changeScene(new DeathScreen());
	/*---------------------------------*/
}

void SceneBoss::Render()
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

	/*modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(130, 130, 130);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();

	unsigned int num_anim;
	for (num_anim = 0; num_anim < 30;)
	{
		if (et[num_anim] <= 0.f)
			num_anim++;
		else
			break;
	}

	/*-----------------Main Character (Alexis)---------------------*/
	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	PlayerClass::get_instance()->PlayerHitBox.scale(1.1f, 4.5f, 1.f);
	PlayerClass::get_instance()->PlayerHitBox.translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);
	modelStack.Translate(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y, PlayerClass::get_instance()->position_a.z);
	modelStack.Rotate(PlayerClass::get_instance()->a_LookingDirection, 0, 1, 0);

	// add in grab animation later
	modelStack.PushMatrix();
	modelStack.Translate(PlayerClass::get_instance()->laserTranslate.x, PlayerClass::get_instance()->laserTranslate.y, PlayerClass::get_instance()->laserTranslate.z);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(PlayerClass::get_instance()->laserSize.x, PlayerClass::get_instance()->laserSize.y, PlayerClass::get_instance()->laserSize.z);
	RenderMesh(meshList[GEO_LASER], false);
	modelStack.PopMatrix();
	if (num_anim != 7)
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
	else
	{
		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pCylinder15");//crotch
		RenderMesh(meshList[GEO_ALEXUS_CROTCH], true);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface32");//BODY
		RenderMesh(meshList[GEO_ALEXUS_BODY], true);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface9"); // HEAD
		RenderMesh(meshList[GEO_ALEXUS_HEAD], true);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere17");//ARM WITH SWORD
		RenderMesh(meshList[GEO_ALEXUS_LEFT4ARM], true);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface24");//ARM WITH SWORD
		RenderMesh(meshList[GEO_ALEXUS_LEFTARM], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere14");//LEFTARM
		RenderMesh(meshList[GEO_ALEXUS_RIGHT4ARM], true);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface30");//LEFTARM
		RenderMesh(meshList[GEO_ALEXUS_RIGHTARM], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere9");//RIGHT LEG
		RenderMesh(meshList[GEO_ALEXUS_LEFTTHIGH], true);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface10");//RIGHT LEG
		RenderMesh(meshList[GEO_ALEXUS_LEFTLEG], true);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere10");//LEFTLEG
		RenderMesh(meshList[GEO_ALEXUS_RIGHTTHIGH], true);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface12");//LEFTLEG
		RenderMesh(meshList[GEO_ALEXUS_RIGHTLEG], true);

		modelStack.PopMatrix();

		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();

	//modelStack.PushMatrix();							// render collision box
	//modelStack.Translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);	// i need this
	//modelStack.Scale(1.1f, 4.5f, 1.f);					// if you remove it bad things will happen
	//RenderMesh(meshList[GEO_BBOX], false);				// remove this later when showing actual shit of course
	//modelStack.PopMatrix();								// :ok_hand:
	// for some reason I needed to flip translate and scale here to fit with the actual hitbox

	//modelStack.PushMatrix();
	//modelStack.Translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);	// i need this
	//RenderMesh(meshList[GEO_TESTBBOX], false);
	//modelStack.PopMatrix();

	/*-----------------Mutants (Fuglymon)---------------------*/
	RenderProjectiles();
	RenderMutant();
	/*-------------------------------------------------------*/

	/*------Boss-----*/
	modelStack.PushMatrix();

	Boss::get_instance()->EnemyHitBox.translate(Boss::get_instance()->position_m.x, Boss::get_instance()->position_m.y, Boss::get_instance()->position_m.z);

	modelStack.Translate(Boss::get_instance()->position_m.x, Boss::get_instance()->position_m.y, Boss::get_instance()->position_m.z);

		modelStack.PushMatrix();
		AnimCheck_Boss(0, &modelStack, &et[10], "Boss_Torso");
		RenderMesh(meshList[GEO_BOSS_TORSO], true);

			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_BOSS_LJAW], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_BOSS_RJAW], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_BOSS_NECK], true);

				modelStack.PushMatrix();
				RenderMesh(meshList[GEO_BOSS_MHEAD], true);

					modelStack.PushMatrix();
					RenderMesh(meshList[GEO_BOSS_MJAW], true);

					modelStack.PopMatrix();
				modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_BOSS_LUARM], true);

				modelStack.PushMatrix();
				RenderMesh(meshList[GEO_BOSS_LARM], true);

				modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_BOSS_RUARM], true);

				modelStack.PushMatrix();
				RenderMesh(meshList[GEO_BOSS_RARM], true);

				modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_BOSS_SEG1], true);

				modelStack.PushMatrix();
				RenderMesh(meshList[GEO_BOSS_SEG2], true);

					modelStack.PushMatrix();
					RenderMesh(meshList[GEO_BOSS_SEG3], true);

						modelStack.PushMatrix();
						RenderMesh(meshList[GEO_BOSS_SEG4], true);

							modelStack.PushMatrix();
							RenderMesh(meshList[GEO_BOSS_SEG5], true);

								modelStack.PushMatrix();
								RenderMesh(meshList[GEO_BOSS_SEG6], true);

								modelStack.PopMatrix();
							modelStack.PopMatrix();
						modelStack.PopMatrix();
					modelStack.PopMatrix();
				modelStack.PopMatrix(); // whee
			modelStack.PopMatrix();
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	if (Boss::get_instance()->get_action() == 3)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Boss::get_instance()->Boss_Tail.position_t.x, PlayerClass::get_instance()->position_a.y + 2.5f, Boss::get_instance()->Boss_Tail.position_t.z);
		modelStack.Scale(0.1f, 0.1f, 0.1f);
		RenderMesh(meshList[GEO_BOSS_INDICATOR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(Boss::get_instance()->Boss_Tail.position_t.x, Boss::get_instance()->Boss_Tail.position_t.y, Boss::get_instance()->Boss_Tail.position_t.z);
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		RenderMesh(meshList[GEO_SPIKE], true);
		modelStack.PopMatrix();
	}

	/*---------------*/

	/*-----------------Skybox-------------------*/
	/*modelStack.PushMatrix();
	modelStack.Translate(450, 80, -200);
	modelStack.Scale(1600, 675, 1);
	RenderMesh(meshList[GEO_SKYBOX], false);
	modelStack.PopMatrix();*/
	/*------------------------------------------*/

	/*-----------------Environmental Light Rendering------*/
	RenderLightStands();
	/*----------------------------------------------------*/

	/*----Heart and Energy Bar Rendering----------*/
	float positionXscreen = 2;
	for (int i = 0; i < 10; i++)
	{
		RenderMeshOnScreen(meshList[GEO_ALEXIS_LIFE], positionXscreen, 28.5,
			PlayerClass::get_instance()->Hearts.a_heart[i], PlayerClass::get_instance()->Hearts.a_heart[i], 0);
		RenderMeshOnScreen(meshList[GEO_BLANKHEART], positionXscreen, 28.5,
			PlayerClass::get_instance()->Hearts.a_blankheart[i], PlayerClass::get_instance()->Hearts.a_blankheart[i], 0);

		positionXscreen += 2;
	}

	float energyX = 7;
	for (int i = 0; i < 10; i++)
	{
		RenderMeshOnScreen(meshList[GEO_ENERGY], energyX, 27,
			PlayerClass::get_instance()->Hearts.a_energy[i], PlayerClass::get_instance()->Hearts.a_energy[i], 0);
		RenderMeshOnScreen(meshList[GEO_BLANKENERGY], energyX, 27,
			PlayerClass::get_instance()->Hearts.a_blankenergy[i], PlayerClass::get_instance()->Hearts.a_blankenergy[i], 0);

		energyX += 1;
	}
	/*-----------------------------*/
	/*--------Boss Life Rendering---*/
	float posXscreen = 6;
	for (int i = 0; i < 30; i++)
	{
		RenderMeshOnScreen(meshList[GEO_BOSSLIFE], posXscreen, 1,
			Boss::get_instance()->bossLife.boss_heart[i], Boss::get_instance()->bossLife.boss_heart[i], 0);

		posXscreen += 0.5;
	}
	/*-----------------------------*/
	/*------Spell HUD-----*/
	RenderMeshOnScreen(meshList[GEO_LASER_ICON], 10, 0.5,
		PlayerClass::get_instance()->spellHUD.laserReady, PlayerClass::get_instance()->spellHUD.laserReady, 0);
	RenderMeshOnScreen(meshList[GEO_LASER_CD], 10, 0.5,
		PlayerClass::get_instance()->spellHUD.laserNotReady, PlayerClass::get_instance()->spellHUD.laserNotReady, 0);

	RenderMeshOnScreen(meshList[GEO_PROJSHIELD], 12, 0.5,
		PlayerClass::get_instance()->spellHUD.projShieldReady, PlayerClass::get_instance()->spellHUD.projShieldReady, 0);
	RenderMeshOnScreen(meshList[GEO_PROJSHIELD_CD],12, 0.5,
		PlayerClass::get_instance()->spellHUD.projShieldNotReady, PlayerClass::get_instance()->spellHUD.projShieldNotReady, 0);
	/*--------------------*/
	RenderTextOnScreen(meshList[GEO_TEXT], "ENERGY", Color(0, 0, 1), 2, 3, 17.5);
	RenderTextOnScreen(meshList[GEO_TEXT], "BOSS", Color(1, 1, 0), 2, 3, -8.5);
	/*------------------------------*/
	RenderTextOnScreen(meshList[GEO_TEXT], fps, Color(0, 1, 0), 2, 36, 19);
}


bool SceneBoss::otheranims()
{
	return (attack);
}

bool SceneBoss::holdanims()
{
	return (roll || block);

}

void SceneBoss::RenderMutant()
{
	//modelStack.PushMatrix();
	//EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.y,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.z);

	//modelStack.Translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.y,
	//	EnemyManager::get_instance()->EnemyList[0]->position_m.z);

	//modelStack.PushMatrix();

	//modelStack.PushMatrix();
	//if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == -1)
	//	modelStack.Rotate(180, 0, 1, 0);
	//else if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == 1)
	//	modelStack.Rotate(0, 0, 1, 0);

	//IdleAnim_M(&modelStack, &et[20], "Mutant_Head");

	//RenderMesh(meshList[GEO_MUTANT_HEAD], true);
	//modelStack.PopMatrix();
	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_Head");
	//modelStack.Translate(-2, 5, 0);
	////if (EnemyManager::get_instance()->EnemyList[0]->healthsystem())
	//RenderMesh(meshList[GEO_M_RHEART], false);
	//modelStack.Translate(3, 0, 0);
	////if (attack)
	////	RenderMesh(meshList[GEO_M_BHEART], false);
	////else
	//RenderMesh(meshList[GEO_M_RHEART], false);
	//modelStack.PopMatrix();
	//modelStack.PopMatrix();


	//if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == -1)
	//	modelStack.Rotate(180, 0, 1, 0);
	//else if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == 1)
	//	modelStack.Rotate(0, 0, 1, 0);


	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_LeftArm");

	//RenderMesh(meshList[GEO_MUTANT_LEFTARM], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_LeftFeet");

	//RenderMesh(meshList[GEO_MUTANT_LEFTFEET], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_LeftThigh");

	//RenderMesh(meshList[GEO_MUTANT_LEFTTHIGH], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_LeftUpperarm");

	//RenderMesh(meshList[GEO_MUTANT_LEFTUPPERARM], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_Neck");

	//RenderMesh(meshList[GEO_MUTANT_NECK], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_RightArm");

	//RenderMesh(meshList[GEO_MUTANT_RIGHTARM], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_RightFeet");

	//RenderMesh(meshList[GEO_MUTANT_RIGHTFEET], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_RightThigh");

	//RenderMesh(meshList[GEO_MUTANT_RIGHTTHIGH], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_RightUpperarm");

	//RenderMesh(meshList[GEO_MUTANT_RIGHTUPPERARM], true);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//IdleAnim_M(&modelStack, &et[20], "Mutant_Torso");

	//RenderMesh(meshList[GEO_MUTANT_TORSO], true);
	//modelStack.PopMatrix();

	//modelStack.PopMatrix();
}

void SceneBoss::RenderProjectiles()
{
	/*for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
		{
			if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] != nullptr)
			{
				modelStack.PushMatrix();
				modelStack.Translate(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.x,
					(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.y + 10.f),
					EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.z);
				RenderMesh(meshList[GEO_SPIT], false);
				modelStack.PopMatrix();
			}
		}
	}*/
}

void SceneBoss::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}






