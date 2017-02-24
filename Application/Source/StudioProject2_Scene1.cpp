#include "StudioProject2_Scene1.h"
#include "StudioProject2_Scene2.h"
#include "DeathScreen.h"
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
#include "HalfMutant.h"
#include <vector>

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34

StudioProject2Scene1::StudioProject2Scene1()
{
}

StudioProject2Scene1::~StudioProject2Scene1()
{
}

void StudioProject2Scene1::Init()
{
	PlayerClass::get_instance();
	/*--------Half Mutant Variable----------*/
	HalfMutant halfmut;
	halfmut.position_hm = Vector3(640.f, -252.2f, 0.f);
	hmvec.push_back(halfmut);
	hmvec[0].init();
	hmvec[0].size_hm = Vector3(1, 1, 1);
	hmvec[0].size_human = Vector3(0.1, 0.1, 0.1);
	/*--------------------------------------*/

	/*----Player & AI & Camera Variables----*/

	EnemyManager::get_instance()->spawnEnemy(Vector3(750.f, -252.2f, 0.f));
	PlayerClass::get_instance()->position_a = Vector3(-15.f,0.f,0.f);
	PlayerClass::get_instance()->restartGame();
	PlayerClass::get_instance()->healthUI();
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

	meshList[GEO_LIGHTBULB] = MeshBuilder::GenerateOBJ("bulb", "OBJ//lighttop.obj");
	meshList[GEO_LIGHTBULB]->textureID = LoadTGA("Image//lighttext.tga");
	meshList[GEO_LIGHTSTAND] = MeshBuilder::GenerateOBJ("lightstand", "OBJ//lightbottom.obj");
	meshList[GEO_LIGHTSTAND]->textureID = LoadTGA("Image//lighttext.tga");

	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//tree.obj");
	meshList[GEO_TREE]->textureID = LoadTGA("Image//tree.tga");

	meshList[GEO_CLUSTERTREE] = MeshBuilder::GenerateOBJ("clustertree", "OBJ//clustertree.obj");
	meshList[GEO_CLUSTERTREE]->textureID = LoadTGA("Image//tree.tga");

	meshList[GEO_DEBRIS] = MeshBuilder::GenerateOBJ("debris", "OBJ//Debri.obj");
	meshList[GEO_DEBRIS]->textureID = LoadTGA("Image//Debri_Texture.tga");
	
	meshList[GEO_SYRINGE] = MeshBuilder::GenerateOBJ("syringe", "OBJ//Scene1//syn.obj");
	meshList[GEO_SYRINGE]->textureID = LoadTGA("Image//crotchtext.tga");

	meshList[GEO_BAT] = MeshBuilder::GenerateOBJ("bat", "OBJ//bat.obj");
	//meshList[GEO_BAT]->textureID = LoadTGA("Image//crotchtext.tga");
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

	EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");

	/*-----------------------------------------------------------------------------*/
	/*-------------------------Human and Half Mutant Loading-----------------------*/
	meshList[GEO_HUMAN] = MeshBuilder::GenerateOBJ("npcHuman", "OBJ//npc.obj");
	meshList[GEO_HUMAN]->textureID = LoadTGA("Image//npctexture.tga");

	meshList[GEO_HM_HEAD] = MeshBuilder::GenerateOBJ("hm_head", "OBJ//Halfmutant//head_hm.obj");
	meshList[GEO_HM_HEAD]->textureID = LoadTGA("Image//hm_facetexture.tga");
	meshList[GEO_HM_BODY] = MeshBuilder::GenerateOBJ("hm_body", "OBJ//Halfmutant//body_hm.obj");
	meshList[GEO_HM_BODY]->textureID = LoadTGA("Image//hm_torsotexture.tga");
	meshList[GEO_HM_RIGHTARM] = MeshBuilder::GenerateOBJ("hm_rArm", "OBJ//Halfmutant//Rightarm_hm.obj");
	meshList[GEO_HM_RIGHTARM]->textureID = LoadTGA("Image//hm_armtexture.tga");
	meshList[GEO_HM_LEFTARM] = MeshBuilder::GenerateOBJ("hm_lArm", "OBJ//Halfmutant//Leftarm_hm.obj");
	meshList[GEO_HM_LEFTARM]->textureID = LoadTGA("Image//hm_armtexture.tga");
	meshList[GEO_HM_RIGHTLEG] = MeshBuilder::GenerateOBJ("hm_rLeg", "OBJ//Halfmutant//Rightleg_hm.obj");
	meshList[GEO_HM_RIGHTLEG]->textureID = LoadTGA("Image//hm_legtexture.tga");
	meshList[GEO_HM_LEFTLEG] = MeshBuilder::GenerateOBJ("hm_lLeg", "OBJ//Halfmutant//Leftleg_hm.obj");
	meshList[GEO_HM_LEFTLEG]->textureID = LoadTGA("Image//hm_legtexture.tga");
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
	meshList[GEO_TRIGGER_SLOPE] = MeshBuilder::GenerateOBJ("Trigger_Slope", "OBJ//TriggerBox.obj");
	meshList[GEO_TRIGGER_SLOPE]->MeshBBox.loadBB("OBJ//TriggerBox.obj");
	meshList[GEO_TRIGGER_SLOPE]->MeshBBox.translate(-12.f, 10.f, 0);
	/*-----------------------------------------------------------------------------*/

	/*-----------------------------Checking BBox-----------------------------------*/
	meshList[GEO_BBOX] = MeshBuilder::GenerateBB("CharBox", PlayerClass::get_instance()->PlayerHitBox.max_, PlayerClass::get_instance()->PlayerHitBox.min_);
	//meshList[GEO_TESTBBOX] = MeshBuilder::GenerateBB("TestBox", EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.max_, EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.min_);
	/*-----------------------------------------------------------------------------*/ 
	
	/*-------------------------Loading Mutant Health----------------------------------*/
	meshList[GEO_M_RHEART] = MeshBuilder::GenerateOBJ("MutantHealthRed", "OBJ//M_HealthRed.obj");
	meshList[GEO_M_RHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
	meshList[GEO_M_BHEART] = MeshBuilder::GenerateOBJ("MutantHealthBlack", "OBJ//M_HealthBlack.obj");
	meshList[GEO_M_BHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
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
	textOccured = 0;
	nexttext = false;
	/*----------------------------------------------------------------------------------*/

	/*---------------------------Initialising Variables---------------------------------*/
	ShortBox_PosX = 0.f;
	TallBox_PosX = 0.f;
	syringeSizeX = 1.f;
	syringeSizeY = 1.f;
	syringeSizeZ = 1.f;

	/*-----Character--------*/
	pressedA = false;
	pressedD = false;
	inmovement = false;
	injump = false;
	attack = false;
	trigger = false;
	grab = false;
	block = false;
	roll = false;
	/*----------------------*/

	//bat
	movebat = 1.f;

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

void StudioProject2Scene1::Update(double dt)
{
	int framespersec = 1 / dt;
	elapsedTime += dt;
	camera.Update(dt, PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y);

	/*-------Half Mutant Functions------------*/
	hmvec[0].movement(dt);
	hmvec[0].transformation();
	/*----------------------------------------*/

	/*-------AI Functions---------------*/

	EnemyManager::get_instance()->EnemyList[0]->update(dt);

	// I spent 10 years trying to fix projectile because I wanted to avoid using erase.
	// Erase won today. Erase, me, 1:0. Shit.

	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++) // in case got error, -- proj when delete
	{
		if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
		{
			for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
			{
				if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] != nullptr)
				{
					EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.translate(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.x,
						(EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.y + 10.f),
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->position_.z);

					if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(meshList[GEO_TRUMP]->MeshBBox) ||
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->displacement() > 300.f)
					{
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
					}
					else if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(PlayerClass::get_instance()->PlayerHitBox) &&
						(elapsedTime > bufferTime_iframe) && (elapsedTime > bufferTime_iframeroll))
					{
						PlayerClass::get_instance()->healthSystem(block);
						bufferTime_iframe = elapsedTime + 0.3f;
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
					}
				}
			}
		}
	}

	/*-------Player Functions------------------*/
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->timeSpent(dt);
	if (!trigger && !otheranims() && !holdanims())
		PlayerClass::get_instance()->facingDirection();

	/*-----------------------------------------*/

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

	if (elapsedTime > 1.1f && !trigger) // This pre-setting ensures animations won't occur at the very start, so animations glitching out will not happen anymore.*
	{									// *I hope.
		inmovement = false;				// so many if statements I could write a philosophy book
		if (Application::IsKeyPressed('A') && !roll)
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox) ||
				pressedD == true)
			{
				PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
				pressedD = false;
				pressedA = true;
				inmovement = true;

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

		if (Application::IsKeyPressed('D') && !roll)
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
				!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox)
				|| pressedA == true)
			{
				PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
				pressedA = false;
				pressedD = true;
				inmovement = true;

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

		if (Application::IsKeyPressed('W') && elapsedTime > bufferTime_Jump)
		{
			bufferTime_Jump = elapsedTime + 0.6f;
			bufferTime_JumpUp = elapsedTime + 0.3f;
		}
		if (Application::IsKeyPressed(VK_LBUTTON) && !attack && !injump && !holdanims())
		{
			bufferTime_attack = elapsedTime + 1.f;

			if ((EnemyManager::get_instance()->EnemyList[0]->get_health() != 0) && PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox))
				EnemyManager::get_instance()->EnemyList[0]->edit_health(-50);
		}

		if (Application::IsKeyPressed('F') && !block && !roll)
			bufferTime_grab = elapsedTime + 0.15f;

		if ((Application::IsKeyPressed(VK_LSHIFT) || Application::IsKeyPressed(VK_RSHIFT)) && !grab && !roll)
			bufferTime_block = elapsedTime + 0.2f;

		if (Application::IsKeyPressed(VK_RBUTTON) && !holdanims())
		{
			bufferTime_roll = elapsedTime + 0.8f;
			bufferTime_iframeroll = elapsedTime + 0.35f;
		}

		/* mutant */

		if (EnemyManager::get_instance()->EnemyList[0]->get_health() > 0)
		{
			if (elapsedTime > bufferTime_attack_M)
			{
				EnemyManager::get_instance()->EnemyList[0]->attack(1, EnemyManager::get_instance()->EnemyList[0]->position_m, EnemyManager::get_instance()->EnemyList[0]->direction_m, dt, block);
				bufferTime_attack_M = elapsedTime + 2.f;
			}
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

		if (pressedA && !trigger &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox))
			PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
		else if (pressedD && !trigger &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_HOUSELEFTWALL]->MeshBBox) &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMP]->MeshBBox))
			PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
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

	if (bufferTime_grab > elapsedTime)
	{
		grab = true;
		et[9] += dt;
	}
	else
		grab = false;

	if (inmovement)
		et[6] += dt;
	else
		et[6] = 0;

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
		PlayerClass::get_instance()->position_a.x += (float)(60.f * dt);

	PlayerClass::get_instance()->PlayerHitBox.loadBB("OBJ//Character//crotch.obj");

	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
		{
			for (unsigned int projectiles = 0; projectiles < EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
			{
				if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles] != nullptr)
					EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.loadBB("OBJ//Scene1//Box_Short.obj");
			}
		}
	}

	if (EnemyManager::get_instance()->EnemyList[0]->get_health() > 0)
		EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj"); // THIS SNEAKY ASS LINE OF CODE RUINED COLLISION FOR THE PAST HOUR OMG.
	// I UNCOMMENTED IT AND OPENED PANDORA'S BOX, WISH ME LUCK.
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

	/*--------------Updates the Full Mutant Kill Count--------*/
	for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() == 0)
		{
			PlayerClass::get_instance()->fm_Killed++;
			EnemyManager::get_instance()->EnemyList[numEnemy]->edit_health(-1);
		}
	}
	/*-------------------------------------------------------*/

	/*---------Change Scene------*/
	if ((PlayerClass::get_instance()->position_a.x > 800 && (EnemyManager::get_instance()->EnemyList[0]->get_health() <= 0)))
	{
		SceneManager::getInstance()->Location = "Inner City";
		SceneManager::getInstance()->changeScene(new StudioProject2Scene2());
	}
	if (PlayerClass::get_instance()->get_health() <= 0)
	{
		SceneManager::getInstance()->changeScene(new DeathScreen());
	}
	/*---------------------------*/
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
			//if (num_anim == 7 && et[num_anim] >= (10.0 / 30.0))
			//	int a = 0;
			//if (et[num_anim] >= (10.0 / 30.0))
			//	int b = 1;
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

	//modelStack.PushMatrix();
	//RenderMesh(meshList[GEO_TESTBBOX], false);
	//modelStack.PopMatrix();

	/*-----------------Mutants (Fuglymon)---------------------*/
	if (EnemyManager::get_instance()->EnemyList[0]->get_health() > 0)
	{
		RenderProjectiles();
		RenderMutant();
	}
	/*-------------------------------------------------------*/

	/*---------------Half Mutant & Human---------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(hmvec[0].position_hm.x, hmvec[0].position_hm.y, hmvec[0].position_hm.z);
	modelStack.Rotate(hmvec[0].hm_LookingDirection, 0, 1, 0);
	modelStack.Scale(hmvec[0].size_hm.x, hmvec[0].size_hm.y, hmvec[0].size_hm.z);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HM_BODY], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HM_HEAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HM_RIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HM_LEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HM_LEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_HM_RIGHTLEG], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
	
	// human
	modelStack.PushMatrix();
	modelStack.Translate(hmvec[0].position_hm.x, hmvec[0].position_hm.y, hmvec[0].position_hm.z);
	modelStack.Rotate(hmvec[0].hm_LookingDirection, 0, 1, 0);
	modelStack.Scale(hmvec[0].size_human.x, hmvec[0].size_human.y, hmvec[0].size_human.z);
	RenderMesh(meshList[GEO_HUMAN], true);
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

	modelStack.PushMatrix();
	modelStack.Scale(syringeSizeX, syringeSizeY, syringeSizeZ);
	RenderMesh(meshList[GEO_SYRINGE], true);
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

	//---------BAT---------
	//don't hurt me :,D i suck at coding QnQ)/
	modelStack.PushMatrix();
	//Batanim(batfly, &modelStack, &et[num_anim], "bat");
	if (PlayerClass::get_instance()->position_a.x >80)	
	{ 
		modelStack.Translate(10.f + (movebat++), -20.f, 40);
	}
	else
	{
		modelStack.Translate(10.f , -20.f, 40);
	}
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_BAT], true);
	modelStack.PopMatrix();
	//---------------------

	/*-----------------Skybox-------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(450, 80, -200);
	modelStack.Scale(1600, 675, 1);
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

	/*------------------HUD-------------------------------------------------*/
	/*----Half mutant count--------*/
	RenderMeshOnScreen(meshList[GEO_HALF_COUNT], 5, 0.5, 8, 8,0);
	RenderTextOnScreen(meshList[GEO_TEXT], hMutantSaved, Color(1, 1, 1), 4, 12.3, -9);
	/*-----------------------------*/
	/*----Full mutant count--------*/
	RenderMeshOnScreen(meshList[GEO_FULL_COUNT], 7, 0.5, 8, 8, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], fMutantKilled, Color(1, 1, 1), 4, 16.3, -9);
	/*-----------------------------*/

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


	/*---------------Text log Rendering--------*/
	RenderTextInteractions();
	/*-----------------------------------------*/
	RenderTextOnScreen(meshList[GEO_TEXT], fps, Color(0, 1, 0), 2, 36, 19);
	RenderTextOnScreen(meshList[GEO_TEXT], "SECLUDED FOREST", Color(0, 1, 0), 2.5, 1.5, -8.5);
	/*----------------------------------------------------------------------------------*/
}

bool StudioProject2Scene1::otheranims()
{
	return (attack || trigger);
}

bool StudioProject2Scene1::holdanims()
{
	return (roll || grab || block);
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
	}
}

void StudioProject2Scene1::RenderMutant()
{
	modelStack.PushMatrix();

	EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.scale(4.f, 2.1f, 1.f);

	EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox.translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x,
		EnemyManager::get_instance()->EnemyList[0]->position_m.y,
		EnemyManager::get_instance()->EnemyList[0]->position_m.z);

	modelStack.Translate(EnemyManager::get_instance()->EnemyList[0]->position_m.x,
						 EnemyManager::get_instance()->EnemyList[0]->position_m.y,
						 EnemyManager::get_instance()->EnemyList[0]->position_m.z);

	modelStack.PushMatrix();

		modelStack.PushMatrix();
		if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == -1)
			modelStack.Rotate(180, 0, 1, 0);
		else if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == 1)
			modelStack.Rotate(0, 0, 1, 0);

			AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_Head");

			RenderMesh(meshList[GEO_MUTANT_HEAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_Head");
		modelStack.Translate(-2, 5, 0);
		RenderMesh(meshList[GEO_M_RHEART], false);
		modelStack.Translate(3, 0, 0);
		if (EnemyManager::get_instance()->EnemyList[0]->get_health() == 100)
			RenderMesh(meshList[GEO_M_RHEART], false);
		else
			RenderMesh(meshList[GEO_M_BHEART], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();
	
	
	if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == -1)
		modelStack.Rotate(180, 0, 1, 0);
	else if (EnemyManager::get_instance()->EnemyList[0]->direction_m.x == 1)
		modelStack.Rotate(0, 0, 1, 0);


	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_LeftArm");

	RenderMesh(meshList[GEO_MUTANT_LEFTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_LeftFeet");

	RenderMesh(meshList[GEO_MUTANT_LEFTFEET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_LeftThigh");

	RenderMesh(meshList[GEO_MUTANT_LEFTTHIGH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_LeftUpperarm");

	RenderMesh(meshList[GEO_MUTANT_LEFTUPPERARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_Neck");

	RenderMesh(meshList[GEO_MUTANT_NECK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_RightArm");

	RenderMesh(meshList[GEO_MUTANT_RIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_RightFeet");

	RenderMesh(meshList[GEO_MUTANT_RIGHTFEET], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_RightThigh");

	RenderMesh(meshList[GEO_MUTANT_RIGHTTHIGH], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_RightUpperarm");

	RenderMesh(meshList[GEO_MUTANT_RIGHTUPPERARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_Mutant(20, &modelStack, &et[20], "Mutant_Torso");

	RenderMesh(meshList[GEO_MUTANT_TORSO], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}