#include "StudioProject2_Scene3.h"
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
#include "DeathScreen.h"
#include "VictoryScreen.h"
#include <vector>

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34

StudioProject2Scene3::StudioProject2Scene3()
{
}

StudioProject2Scene3::~StudioProject2Scene3()
{
}

void StudioProject2Scene3::Init()
{
	PlayerClass::get_instance();
	PlayerClass::get_instance()->a_LookingDirection = -90.f;
	PlayerClass::get_instance()->position_a = Vector3(90, 0, -40);
	/*-----------Hearts Initialisation-----*/
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->manaUI();

	//Boss::get_instance()->bossHealthUI();
	//Boss::get_instance()->bossInit();
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
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	glUseProgram(m_programID);
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


	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
		meshList[i] = NULL;

	camera.Init(Vector3(90, 120, 10), Vector3(10, 0, 10), Vector3(0, 1, 0));

	meshList[GEO_AXIS] = MeshBuilder::GenerateAxis("reference");
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1.f, 1.f, 1.f), 18, 36, 1.f);

	/*-----------------------------Skybox/ground Loading----------------------------------*/
	meshList[GEO_SKYBOX3] = MeshBuilder::GenerateOBJ("skyboxscene3", "OBJ//Scene3//scene3skybox.obj");
	meshList[GEO_SKYBOX3]->textureID = LoadTGA("Image//skyboxs3.tga");
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1));

	meshList[GEO_GROUND3] = MeshBuilder::GenerateOBJ("caveground", "OBJ//Scene3//scene3.obj");
	meshList[GEO_GROUND3]->textureID = LoadTGA("Image//scene3.tga");
	/*-----------------------------------------------------------------------------*/
	
	//Textbox
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

	//EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");

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
	meshList[GEO_BBOX] = MeshBuilder::GenerateBB("CharBox", PlayerClass::get_instance()->PlayerHitBox.max_, PlayerClass::get_instance()->PlayerHitBox.min_);

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
	/*--------------------------------------------------------------------------------*/
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);
	/*----------------------Light Initialisation-----------------------------------*/
	LoadLight();
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
 	/*-------------------------------------------------------------------------------*/
	/*-----------------------------------Bound box-----------------------------------*/
	meshList[GEO_BACKCAVE] = MeshBuilder::GenerateOBJ("GEO_BACKCAVE", "OBJ//Scene3//bbscene3//backcave.obj");
	meshList[GEO_BACKCAVE]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//backcave.obj");

	meshList[GEO_BIGLEFT] = MeshBuilder::GenerateOBJ("GEO_BIGLEFT","OBJ//Scene3//bbscene3//bigleft.obj");
	meshList[GEO_BIGLEFT]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//bigleft.obj");

	meshList[GEO_BIGRIGHT] = MeshBuilder::GenerateOBJ("GEO_BIGRIGHT", "OBJ//Scene3//bbscene3//bigRIGHT.obj");
	meshList[GEO_BIGRIGHT]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//bigright.obj");
	
	meshList[GEO_FRONT] = MeshBuilder::GenerateOBJ("GEO_FRONT", "OBJ//Scene3//bbscene3//front.obj");
	meshList[GEO_FRONT]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//front.obj");
	
	//meshList[GEO_LEFTCAVE]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//leftcave.obj");
	
	meshList[GEO_RIGHTCAVE] = MeshBuilder::GenerateOBJ("GEO_RIGHTCAVE", "OBJ//Scene3//bbscene3//rightcave.obj");
	//meshList[GEO_RIGHTCAVE]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//rightcave.obj");

	meshList[GEO_TESTBBOX] = MeshBuilder::GenerateBB("TestBox", meshList[GEO_RIGHTCAVE]->MeshBBox.max_, meshList[GEO_RIGHTCAVE]->MeshBBox.min_);

	//meshList[GEO_SMALLLEFT]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//smallleft.obj");
	
	meshList[GEO_SMALLRIGHT] = MeshBuilder::GenerateOBJ("GEO_SMALLRIGHT", "OBJ//Scene3//bbscene3//smallright.obj");
	meshList[GEO_SMALLRIGHT]->MeshBBox.loadBB("OBJ//Scene3//bbscene3//smallright.obj");
	/*-------------------------------------------------------------------------------*/

	pressedA = false;
	pressedD = false;
	pressedW = false;
	pressedS = false;
	inmovement = false;
	attack = false;
	block = false;
	roll = false;
}

void StudioProject2Scene3::Update(double dt)
{
	int framespersec = 1 / dt;
	elapsedTime += dt;
	camera.UpdateTopdown(dt, PlayerClass::get_instance()->position_a.x + 70.f, PlayerClass::get_instance()->position_a.z /*+ 7*/);

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
	PlayerClass::get_instance()->spellUI(elapsedTime);
	/*-----------------------------------------*/

	/*-----------HUD UPDATES---------*/
	//fps = "FPS:" + std::to_string(framespersec);
	//fMutantKilled = ":" + std::to_string(PlayerClass::get_instance()->fm_Killed);
	//hMutantSaved = ":" + std::to_string(PlayerClass::get_instance()->hm_Saved);
	/*----------------------------------------------------------*/

	/*----------------------------------------------------------*/
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
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BACKCAVE]->MeshBBox)
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BIGLEFT]->MeshBBox)
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTCAVE]->MeshBBox)
				|| pressedS == true)
			{
				pressedA = false;
				pressedS = false;
				pressedD = false;
				pressedW = true;
				PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
			}
		}
		if (Application::IsKeyPressed('S'))
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BACKCAVE]->MeshBBox)
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BIGLEFT]->MeshBBox)
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTCAVE]->MeshBBox)
				|| pressedW == true)
			{
				pressedA = false;
				pressedS = true;
				pressedD = false;
				pressedW = false;
				PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
			}
		}
		if (Application::IsKeyPressed('A'))
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BACKCAVE]->MeshBBox)
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BIGLEFT]->MeshBBox)
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTCAVE]->MeshBBox)
				|| pressedD == true)
			{
				pressedA = true;
				pressedS = false;
				pressedD = false;
				pressedW = false;
				PlayerClass::get_instance()->position_a.z += (float)(30.f * dt);
			}
		}
		if (Application::IsKeyPressed('D'))
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BACKCAVE]->MeshBBox) 
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BIGLEFT]->MeshBBox)
				&& !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_RIGHTCAVE]->MeshBBox)
				|| pressedA == true)
			{
				pressedA = false;
				pressedS = false;
				pressedD = true;
				pressedW = false;
				PlayerClass::get_instance()->position_a.z -= (float)(30.f * dt);
			}
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

	et[0] += dt;

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
	if (PlayerClass::get_instance()->get_health() <= 0)
		SceneManager::getInstance()->changeScene(new DeathScreen());
	/*---------------------------------*/
}

void StudioProject2Scene3::Render()
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
	//modelStack.Rotate(90, 0, 1, 0);
	//modelStack.Scale(130, 130, 130);
	RenderMesh(meshList[GEO_GROUND3], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_TESTBBOX], false);
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
	PlayerClass::get_instance()->PlayerHitBox.translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y), PlayerClass::get_instance()->position_a.z);
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
		RenderMesh(meshList[GEO_ALEXUS_CROTCH], false);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface32");//BODY
		RenderMesh(meshList[GEO_ALEXUS_BODY], false);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface9"); // HEAD
		RenderMesh(meshList[GEO_ALEXUS_HEAD], false);

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere17");//ARM WITH SWORD
		RenderMesh(meshList[GEO_ALEXUS_LEFT4ARM], false);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface24");//ARM WITH SWORD
		RenderMesh(meshList[GEO_ALEXUS_LEFTARM], false);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere14");//LEFTARM
		RenderMesh(meshList[GEO_ALEXUS_RIGHT4ARM], false);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface30");//LEFTARM
		RenderMesh(meshList[GEO_ALEXUS_RIGHTARM], false);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere9");//RIGHT LEG
		RenderMesh(meshList[GEO_ALEXUS_LEFTTHIGH], false);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface10");//RIGHT LEG
		RenderMesh(meshList[GEO_ALEXUS_LEFTLEG], false);

		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "pSphere10");//LEFTLEG
		RenderMesh(meshList[GEO_ALEXUS_RIGHTTHIGH], false);

		modelStack.PushMatrix();
		AnimCheck(7, &modelStack, &et[7], "polySurface12");//LEFTLEG
		RenderMesh(meshList[GEO_ALEXUS_RIGHTLEG], false);

		modelStack.PopMatrix();

		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();

	modelStack.PushMatrix();							// render collision box
	modelStack.Translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);	// i need this
	modelStack.Scale(1.1f, 4.5f, 1.f);					// if you remove it bad things will happen
	RenderMesh(meshList[GEO_BBOX], false);				// remove this later when showing actual shit of course
	modelStack.PopMatrix();								// :ok_hand:

	//modelStack.PushMatrix();
	//modelStack.Translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);	// i need this
	//RenderMesh(meshList[GEO_TESTBBOX], false);
	//modelStack.PopMatrix();

	/*-----------------Mutants (Fuglymon)---------------------*/
	RenderProjectiles();
	RenderMutant();
	/*-------------------------------------------------------*/

	//-----------------------------RENDER BOUND BOX
	Renderbb();

	/*-----------------Skybox-------------------*/
	//modelStack.PushMatrix();
	////modelStack.Translate(450, 80, -200);
	//modelStack.Scale(0.8, 0.8, 0.8);
	//RenderMesh(meshList[GEO_SKYBOX3], false);
	//modelStack.PopMatrix();
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
	/*-----------------------------*

	/*----Textbox Rendering--------*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderMeshOnScreen(meshList[GEO_TEXTBOX], 0, 0, 100, 15, 0);
	/*-----------------------------*/

	/*------Spell HUD-----*/
	RenderMeshOnScreen(meshList[GEO_LASER_ICON], 10, 0.5,
		PlayerClass::get_instance()->spellHUD.laserReady, PlayerClass::get_instance()->spellHUD.laserReady, 0);
	RenderMeshOnScreen(meshList[GEO_LASER_CD], 10, 0.5,
		PlayerClass::get_instance()->spellHUD.laserNotReady, PlayerClass::get_instance()->spellHUD.laserNotReady, 0);

	RenderMeshOnScreen(meshList[GEO_PROJSHIELD], 12, 0.5,
		PlayerClass::get_instance()->spellHUD.projShieldReady, PlayerClass::get_instance()->spellHUD.projShieldReady, 0);
	RenderMeshOnScreen(meshList[GEO_PROJSHIELD_CD], 12, 0.5,
		PlayerClass::get_instance()->spellHUD.projShieldNotReady, PlayerClass::get_instance()->spellHUD.projShieldNotReady, 0);
	/*--------------------*/
	RenderTextOnScreen(meshList[GEO_TEXT], "ENERGY", Color(0, 0, 1), 2, 3, 17.5);
	/*------------------------------*/
	RenderTextOnScreen(meshList[GEO_TEXT], fps, Color(0, 1, 0), 2, 36, 19);

}


bool StudioProject2Scene3::otheranims()
{
	return (attack);
}

bool StudioProject2Scene3::holdanims()
{
	return (roll || block);

}

void StudioProject2Scene3::RenderMutant()
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

void StudioProject2Scene3::RenderProjectiles()
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

void StudioProject2Scene3::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}






