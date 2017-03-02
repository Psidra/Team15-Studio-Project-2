#include "StudioProject2_Scene2.h"
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
#include "HalfMutant.h"
#include <vector>

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34


StudioProject2Scene2::StudioProject2Scene2()
{
}

StudioProject2Scene2::~StudioProject2Scene2()
{
}

void StudioProject2Scene2::Init()
{
	PlayerClass::get_instance();
	/*--------Half Mutant Variable----------*/
	HalfMutant halfmut2;
	halfmut2.position_hm = Vector3(610.f, 10.f, 0.f);
	hmvec.push_back(halfmut2);
	hmvec[0].init();
	hmvec[0].size_hm = Vector3(1, 1, 1);
	hmvec[0].size_human = Vector3(0.1, 0.1, 0.1);
	/*--------------------------------------*/

	/*----Player & AI & Camera Variables----*/
	if (EnemyManager::get_instance()->EnemyList.size() == 0)
	{
		for (unsigned int numenemy = 0; numenemy < 3; numenemy++)
		{
			EnemyManager::get_instance()->spawnEnemy(Vector3(60.f + (numenemy * 10.f), 10.f, 20.f)); // look ma! one line!
		}
	}
	//for (unsigned int numenemy = 1; numenemy < 4; numenemy++)
	//{
	//	for (unsigned int spawnx = 300.f; spawnx < 320.f; spawnx += 5.f)
	//	{
	//		EnemyManager::get_instance()->spawnEnemy(Vector3(spawnx, 10.f, 0.f));
	//	}
	//}

	PlayerClass::get_instance()->position_a = Vector3(-40.f, 10.f, 20.f);
	PlayerClass::get_instance()->init();
	PlayerClass::get_instance()->healthUI();
	/*--------------------------------------*/

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
	meshList[GEO_SKYBOX]->textureID = LoadTGA("Image//CityBG.tga");
	/*-----------------------------------------------------------------------------*/

	/*-----------------Environment Objects Loading---------------------------------*/
	meshList[GEO_LIGHTBULB] = MeshBuilder::GenerateOBJ("bulb", "OBJ//lighttop.obj");
	meshList[GEO_LIGHTBULB]->textureID = LoadTGA("Image//lighttext.tga");
	meshList[GEO_LIGHTSTAND] = MeshBuilder::GenerateOBJ("lightstand", "OBJ//lightbottom.obj");
	meshList[GEO_LIGHTSTAND]->textureID = LoadTGA("Image//lighttext.tga");
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//tree.obj");
	meshList[GEO_TREE]->textureID = LoadTGA("Image//tree.tga");
	meshList[GEO_CLUSTERTREE] = MeshBuilder::GenerateOBJ("clustertree", "OBJ//clustertree.obj");
	meshList[GEO_CLUSTERTREE]->textureID = LoadTGA("Image//tree.tga");

	meshList[GEO_SCENE2] = MeshBuilder::GenerateOBJ("Environment", "OBJ//Scene2//Scene2_Environment.obj");
	meshList[GEO_SCENE2]->textureID = LoadTGA("Image//rocktex.tga");
	meshList[GEO_TRUMPTOWER] = MeshBuilder::GenerateOBJ("SnipingTower", "OBJ//Scene2//TrumpTower.obj");
	meshList[GEO_TRUMPTOWER]->textureID = LoadTGA("Image//Debri_Texture.tga");
	meshList[GEO_TRUMPTOWER]->MeshBBox.loadBB("OBJ//Scene2//TrumpTower.obj");

	meshList[GEO_TRUMPWALL] = MeshBuilder::GenerateOBJ("Wall", "OBJ//Scene2//TrumpWall.obj");
	meshList[GEO_TRUMPWALL]->textureID = LoadTGA("Image//brick.tga");
	meshList[GEO_TRUMPWALL]->MeshBBox.loadBB("OBJ//Scene2//TrumpWall.obj");
	meshList[GEO_TRUMPWALL]->MeshBBox.scale(1.f, 1.f, 1.f);
	meshList[GEO_TRUMPWALL]->MeshBBox.translate(568, -25, 20);

	meshList[GEO_LAMPPOST] = MeshBuilder::GenerateOBJ("Lamp post", "OBJ//Scene2//Lamp_post.obj");
	meshList[GEO_LAMPPOST]->textureID = LoadTGA("Image//rust.tga");
	meshList[GEO_LAMPPOST]->MeshBBox.loadBB("OBJ//Scene2//Lamp_post.obj");
	meshList[GEO_LAMPTRIGGER] = MeshBuilder::GenerateOBJ("Box", "OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_LAMPTRIGGER]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");

	meshList[GEO_LAMPTRIGGER]->MeshBBox.scale(12.4f, 15.f, 30.f);
	meshList[GEO_LAMPTRIGGER]->MeshBBox.translate(541, -7, 10);
	meshList[GEO_LAMPTRIGGER2] = MeshBuilder::GenerateOBJ("Box", "OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_LAMPTRIGGER2]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_LAMPTRIGGER2]->MeshBBox.scale(3.5f, 1.3f, 30.f);
	meshList[GEO_LAMPTRIGGER2]->MeshBBox.translate(576, 67.5f, 10);
	meshList[GEO_LAMPTRIGGER3] = MeshBuilder::GenerateOBJ("Box", "OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_LAMPTRIGGER3]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_LAMPTRIGGER3]->MeshBBox.scale(7.f, 3.f, 30.f);
	meshList[GEO_LAMPTRIGGER3]->MeshBBox.translate(510, 20.f, 10);

	/*meshList[GEO_LAMPTRIGGER]->MeshBBox.scale(14.f, 12.f, 30.f);
	meshList[GEO_LAMPTRIGGER]->MeshBBox.translate(545, 10, 10);*/

	meshList[GEO_BLOCK_ONE] = MeshBuilder::GenerateOBJ("Box", "OBJ//Scene1//Box_Tall.obj"); // praise box_tall
	meshList[GEO_BLOCK_ONE]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_BLOCK_ONE]->MeshBBox.scale(1.f, 2.f, 1.f);
	meshList[GEO_BLOCK_ONE]->MeshBBox.translate(125.f, 10.f, 20.f);

	meshList[GEO_BLOCK_TWO] = MeshBuilder::GenerateOBJ("Box", "OBJ//Scene1//Box_Tall.obj"); // praise box_tall
	meshList[GEO_BLOCK_TWO]->MeshBBox.loadBB("OBJ//Scene1//Box_Tall.obj");
	meshList[GEO_BLOCK_TWO]->MeshBBox.scale(1.f, 2.f, 1.f);
	meshList[GEO_BLOCK_TWO]->MeshBBox.translate(312.f, 10.f, 20.f);

	meshList[GEO_ROPE] = MeshBuilder::GenerateOBJ("Rope", "OBJ//Scene2//Rope.obj");
	meshList[GEO_ROPE]->MeshBBox.loadBB("OBJ//Scene2//Rope.obj");
	meshList[GEO_ROPE]->MeshBBox.scale(0.1, 0.5, 2);
	meshList[GEO_ROPE]->MeshBBox.translate(524, 28, 10);


	//	modelStack.Translate(-130, 8, 0);
	// modelStack.Scale(2, 2.5, 2.5);

	meshList[GEO_DEBRIS1] = MeshBuilder::GenerateOBJ("debris", "OBJ//Debri.obj");
	meshList[GEO_DEBRIS1]->textureID = LoadTGA("Image//Debri_Texture.tga");
	meshList[GEO_DEBRIS1]->MeshBBox.loadBB("OBJ//Debri.obj");
	meshList[GEO_DEBRIS1]->MeshBBox.scale(1, 5, 1.5);
	meshList[GEO_DEBRIS1]->MeshBBox.translate(-130, 8, 0);

	meshList[GEO_DEBRISn] = MeshBuilder::GenerateOBJ("debris", "OBJ//Debri.obj");  //no collision debri
	meshList[GEO_DEBRISn]->textureID = LoadTGA("Image//Debri_Texture.tga");

	meshList[GEO_FLOORBBOX] = MeshBuilder::GenerateOBJ("Floor", "OBJ//Scene2//Scene2_BBox.obj");
	meshList[GEO_FLOORBBOX]->MeshBBox.loadBB("OBJ//Scene2//Scene2_BBox.obj");
	meshList[GEO_FLOORBBOX]->MeshBBox.scale(3, 1, 2);
	meshList[GEO_FLOORBBOX]->MeshBBox.translate(0, -4, 0);

	meshList[GEO_SHELTEROBJ] = MeshBuilder::GenerateOBJ("Shelterobj", "OBJ//Scene2//ShelterObject.obj");
	meshList[GEO_SHELTEROBJ]->textureID = LoadTGA("Image//woodtex.tga");
	meshList[GEO_SHELTEROBJ]->MeshBBox.loadBB("OBJ//Scene2//ShelterObject.obj"); //main body of Shelter obj
	meshList[GEO_SHELTEROBJ]->MeshBBox.translate(20, 16, 20);

	meshList[GEO_MOVEBOX] = MeshBuilder::GenerateOBJ("Box", "OBJ//Scene2//MoveableBox.obj");
	meshList[GEO_MOVEBOX]->textureID = LoadTGA("Image//woodtex.tga");
	meshList[GEO_MOVEBOX]->MeshBBox.loadBB("OBJ//Scene2//MoveableBox.obj");
	meshList[GEO_MOVEBOX]->MeshBBox.translate(480, 8, 10);
	meshList[GEO_MOVEBOX]->MeshBBox.scale(1.f, 1.f, 2.f);
	meshList[GEO_MOVEBOXTEST] = MeshBuilder::GenerateOBJ("Box", "OBJ//Scene2//MoveableBox.obj");
	meshList[GEO_MOVEBOXTEST]->MeshBBox.loadBB("OBJ//Scene2//MoveableBox.obj");
	meshList[GEO_MOVEBOXTEST]->MeshBBox.translate(480, 8, 10);
	meshList[GEO_MOVEBOXTEST]->MeshBBox.scale(1.f, 1.05f, 2.f);


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

	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
		EnemyManager::get_instance()->EnemyList[numenemy]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");

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
	/*---------------------------------------------------------------------------------------------*/
	
	/*-----------------------------Trigger Check-----------------------------------*/

	/*-----------------------------------------------------------------------------*/

	/*-----------------------------Checking BBox-----------------------------------*/
	meshList[GEO_BBOX] = MeshBuilder::GenerateBB("CharBox", PlayerClass::get_instance()->PlayerHitBox.max_, PlayerClass::get_instance()->PlayerHitBox.min_);
	meshList[GEO_TESTBBOX] = MeshBuilder::GenerateBB("TestBox", meshList[GEO_TRUMPWALL]->MeshBBox.max_, meshList[GEO_TRUMPWALL]->MeshBBox.min_);
	meshList[GEO_TESTBBOX2] = MeshBuilder::GenerateBB("TestBox", meshList[GEO_LAMPTRIGGER]->MeshBBox.max_, meshList[GEO_LAMPTRIGGER]->MeshBBox.min_);
	meshList[GEO_TESTBBOX3] = MeshBuilder::GenerateBB("TestBox", meshList[GEO_LAMPTRIGGER3]->MeshBBox.max_, meshList[GEO_LAMPTRIGGER3]->MeshBBox.min_);

	/*-----------------------------------------------------------------------------*/

	/*-------------------------Loading Mutant Health----------------------------------*/
	meshList[GEO_M_RHEART] = MeshBuilder::GenerateOBJ("MutantHealthRed", "OBJ//M_HealthRed.obj");
	meshList[GEO_M_RHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
	meshList[GEO_M_BHEART] = MeshBuilder::GenerateOBJ("MutantHealthBlack", "OBJ//M_HealthBlack.obj");
	meshList[GEO_M_BHEART]->textureID = LoadTGA("Image//Mutant_Health.tga");
	/*--------------------------------------------------------------------------------*/

	/*------------------------Initialising Text Variables-------------------------------*/
	 pEnter = false;
	 nexttext = false;
	 pressEnterTS = 0;

	 preBattleText = false;
	 preBattleTS = 0;

	 preBuildingText = false;
	 preBuildingTS = 0;

	 cautionText = false;
	 cautionTS = 0;

	 crateText = false;
	 crateTS = 0;

	 surroundedText = false;
	 surroundedTS = 0;

	 barbwireText = false;
	 barbwireTS = 0;

	 lamppostText = false;
	 lamppostTS = 0;

	 textOccur = 0;
	/*----------------------------------------------------------------------------------*/

	/*---------------------------Initialising Variables---------------------------------*/

	MoveBox_PosX = 0.f;
	MoveShelterObj_PosX = 0.f;
	
	/*-----Character--------*/
	pressedA = false;
	pressedD = false;
	inmovement = false;
	injump = false;
	attack = false;
	trigger = false;
	grab = false;
	block = false;
	Unlock = false;
	//roll = false;

	/*----------------------*/
	movespeed = 30.f;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);
	/*-----------------------------------------------------------------------------*/

	/*----------------------Light Initialisation-----------------------------------*/
	LoadLight();
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
	/*-------------------------------------------------------------------------------*/
}

void StudioProject2Scene2::Update(double dt)
{
	int framespersec = 1 / dt;
	elapsedTime += dt;


	/*-------Half Mutant Functions------------*/
	hmvec[0].movement(dt);
	hmvec[0].transformation();
	/*----------------------------------------*/

	if (Application::IsKeyPressed('Y') && elapsedTime > bufferTime_Unlock)
	{
		Unlock = !Unlock;
		bufferTime_Unlock = elapsedTime + 0.5f;
	}


	
	if (!Unlock)
		camera.Update(dt, PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y + 7);
	else
		camera.UpdateUnlockedCam2(dt);
	/*-------AI Functions---------------*/

	// I spent 10 years trying to fix projectile because I wanted to avoid using erase.
	// Erase won today. Erase, me, 1:0. Shit.

	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
			EnemyManager::get_instance()->EnemyList[numenemy]->update(dt);
	}

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

					if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(meshList[GEO_FLOORBBOX]->MeshBBox) ||
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(meshList[GEO_TRUMPTOWER]->MeshBBox) ||
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(meshList[GEO_SHELTEROBJ]->MeshBBox) ||
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->displacement() > 300.f)
					{
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
						delete EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles];
						projectiles--;
					}
					else if (EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles]->projHitBox_.collide(PlayerClass::get_instance()->PlayerHitBox) &&
						(elapsedTime > bufferTime_iframe) && (elapsedTime > bufferTime_iframeroll))
					{
						PlayerClass::get_instance()->healthSystem(block, false);
						bufferTime_iframe = elapsedTime + 0.3f;
						EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
						delete EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles];
						projectiles--;
					}
				}
			}
		}
	}

	/*-------Player Functions------------------*/
	PlayerClass::get_instance()->healthUI();
	PlayerClass::get_instance()->healthRegeneration(elapsedTime);
	PlayerClass::get_instance()->timeSpent(dt);
	if (!trigger && !otheranims() && !holdanims())
		PlayerClass::get_instance()->facingDirection();

	/*-----------------------------------------*/

	/*-----------HUD UPDATES---------*/
	fps = "FPS:" + std::to_string(framespersec);
	testposx = "x: " + std::to_string(PlayerClass::get_instance()->position_a.x);
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

	//// !PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[0]->EnemyHitBox)
	//int Connected = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int xboxButtonCount;
		const unsigned char *xbox = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &xboxButtonCount);
		if (elapsedTime > 1.1f && !trigger) // This pre-setting ensures animations won't occur at the very start, so animations glitching out will not happen anymore.*
		{									// *I hope.
			inmovement = false;				// so many if statements I could write a philosophy book
			if ((Application::IsKeyPressed('A') || GLFW_PRESS == xbox[13]) && !roll)
			{
				if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_DEBRIS1]->MeshBBox) &&
					!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 3) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_ONE]->MeshBBox)) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 6) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_TWO]->MeshBBox))
					|| pressedD == true)
				{
					if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER]->MeshBBox) && ClimbLamp)
					{
						PlayerClass::get_instance()->position_a.x -= (float)(13 * dt);
						PlayerClass::get_instance()->position_a.y -= (float)(15 * dt);
					}
					else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER2]->MeshBBox) && ClimbLamp)
					{
						PlayerClass::get_instance()->position_a.x -= (float)(15 * dt);
						PlayerClass::get_instance()->position_a.y += (float)(13.5 * dt);
					}
					else
					{
						PlayerClass::get_instance()->position_a.x -= (float)(movespeed * dt);
					}

					pressedD = false;
					pressedA = true;
					inmovement = true;

					if (grab)
					{
						if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_SHELTEROBJ]->MeshBBox))
						{
							MoveShelterObj_PosX -= (float)(movespeed * dt);
							meshList[GEO_SHELTEROBJ]->MeshBBox.translate(-((float)(movespeed * dt)), 0, 0);
						}
						else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_MOVEBOX]->MeshBBox) && !meshList[GEO_MOVEBOX]->MeshBBox.collide(meshList[GEO_SHELTEROBJ]->MeshBBox))
						{
							MoveBox_PosX -= (float)(movespeed * dt);
							meshList[GEO_MOVEBOX]->MeshBBox.translate(-((float)(movespeed * dt)), 0, 0);
							meshList[GEO_MOVEBOXTEST]->MeshBBox.translate(-((float)(movespeed * dt)), 0, 0);
						}
					}
				}
			}

			if ((Application::IsKeyPressed('D') || GLFW_PRESS == xbox[11]) && !roll)
			{
				if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_DEBRIS1]->MeshBBox) &&
					!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 3) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_ONE]->MeshBBox)) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 6) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_TWO]->MeshBBox))
					|| pressedA == true)
				{
					if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER]->MeshBBox) && ClimbLamp)
					{
						PlayerClass::get_instance()->position_a.x += (float)(13.5 * dt);
						PlayerClass::get_instance()->position_a.y += (float)(16.5 * dt);
					}
					else
					{
						PlayerClass::get_instance()->position_a.x += (float)(movespeed * dt);
					}
					pressedA = false;
					pressedD = true;
					inmovement = true;

					if (grab)
					{
						if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_SHELTEROBJ]->MeshBBox) && !meshList[GEO_SHELTEROBJ]->MeshBBox.collide(meshList[GEO_MOVEBOX]->MeshBBox))
						{
							MoveShelterObj_PosX += (float)(movespeed * dt);
							meshList[GEO_SHELTEROBJ]->MeshBBox.translate(((float)(movespeed * dt)), 0, 0);
						}
						else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_MOVEBOX]->MeshBBox) && !meshList[GEO_MOVEBOX]->MeshBBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox))
						{
							MoveBox_PosX += (float)(movespeed * dt);
							meshList[GEO_MOVEBOX]->MeshBBox.translate(((float)(movespeed * dt)), 0, 0);
							meshList[GEO_MOVEBOXTEST]->MeshBBox.translate(((float)(movespeed * dt)), 0, 0);

						}
					}
				}
			}

			if ((Application::IsKeyPressed('W') || GLFW_PRESS == xbox[0]) && elapsedTime > bufferTime_Jump)
			{
				bufferTime_Jump = elapsedTime + 0.5f;
				bufferTime_JumpUp = elapsedTime + 0.3f;
			}
			if ((Application::IsKeyPressed(VK_LBUTTON) || GLFW_PRESS == xbox[2]) && !attack && !holdanims())
			{
				bufferTime_attack = elapsedTime + 1.f;

				for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
				{
					if ((EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() > 0) && PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[numEnemy]->EnemyHitBox))
						EnemyManager::get_instance()->EnemyList[numEnemy]->edit_health(-50);
				}

				if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_ROPE]->MeshBBox))
					Breakrope++;
			}

			if ((Application::IsKeyPressed('F') || GLFW_PRESS == xbox[5]) && !block && !roll)
				bufferTime_grab = elapsedTime + 0.15f;

			if ((Application::IsKeyPressed(VK_LSHIFT) || Application::IsKeyPressed(VK_RSHIFT) || GLFW_PRESS == xbox[3]) 
				&& !grab && !roll)
				bufferTime_block = elapsedTime + 0.2f;

			if ((Application::IsKeyPressed(VK_RBUTTON)|| GLFW_PRESS == xbox[1]) && !holdanims())
			{
				bufferTime_roll = elapsedTime + 0.8f;
				bufferTime_iframeroll = elapsedTime + 0.35f;
			}

			/* mutant */

			for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
			{
				if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() > 0)
				{
					if (elapsedTime > EnemyManager::get_instance()->EnemyList[numEnemy]->bufferTime_attack_MC)
					{
						EnemyManager::get_instance()->EnemyList[numEnemy]->attack(1, EnemyManager::get_instance()->EnemyList[numEnemy]->position_m, EnemyManager::get_instance()->EnemyList[numEnemy]->direction_m, dt, block);
						EnemyManager::get_instance()->EnemyList[numEnemy]->bufferTime_attack_MC = elapsedTime + 2.f;
					}
				}
			}
		}
	}
	else
	{
		if (elapsedTime > 1.1f && !trigger) // This pre-setting ensures animations won't occur at the very start, so animations glitching out will not happen anymore.*
		{									// *I hope.
			inmovement = false;				// so many if statements I could write a philosophy book
			if (Application::IsKeyPressed('A') && !roll)
			{
				if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_DEBRIS1]->MeshBBox) &&
					!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 3) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_ONE]->MeshBBox)) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 6) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_TWO]->MeshBBox))
					|| pressedD == true)
				{
					if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER]->MeshBBox) && ClimbLamp)
					{
						PlayerClass::get_instance()->position_a.x -= (float)(13.5 * dt);
						PlayerClass::get_instance()->position_a.y -= (float)(16.5 * dt);
					}
					else
					{
						PlayerClass::get_instance()->position_a.x -= (float)(movespeed * dt);
					}

					pressedD = false;
					pressedA = true;
					inmovement = true;

					if (grab)
					{
						if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_SHELTEROBJ]->MeshBBox))
						{
							MoveShelterObj_PosX -= (float)(movespeed * dt);
							meshList[GEO_SHELTEROBJ]->MeshBBox.translate(-((float)(movespeed * dt)), 0, 0);
						}
						else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_MOVEBOX]->MeshBBox) && !meshList[GEO_MOVEBOX]->MeshBBox.collide(meshList[GEO_SHELTEROBJ]->MeshBBox))
						{
							MoveBox_PosX -= (float)(movespeed * dt);
							meshList[GEO_MOVEBOX]->MeshBBox.translate(-((float)(movespeed * dt)), 0, 0);
							meshList[GEO_MOVEBOXTEST]->MeshBBox.translate(-((float)(movespeed * dt)), 0, 0);

						}
					}
				}
			}

			if (Application::IsKeyPressed('D') && !roll)
			{
				if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_DEBRIS1]->MeshBBox) &&
					!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 3) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_ONE]->MeshBBox)) &&
					(PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 6) || !PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_BLOCK_TWO]->MeshBBox))
					|| pressedA == true)
				{
					if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER]->MeshBBox) && ClimbLamp)
					{
						PlayerClass::get_instance()->position_a.x += (float)(13 * dt);		
						PlayerClass::get_instance()->position_a.y += (float)(15 * dt);
					}
					else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER2]->MeshBBox) && ClimbLamp)
					{
						PlayerClass::get_instance()->position_a.x += (float)(15 * dt);
						PlayerClass::get_instance()->position_a.y -= (float)(13.5 * dt);
					}
					else
					{
						PlayerClass::get_instance()->position_a.x += (float)(movespeed * dt);
					}
					

					pressedA = false;
					pressedD = true;
					inmovement = true;

					if (grab)
					{
						if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_SHELTEROBJ]->MeshBBox) && !meshList[GEO_SHELTEROBJ]->MeshBBox.collide(meshList[GEO_MOVEBOX]->MeshBBox))
						{
							MoveShelterObj_PosX += (float)(movespeed * dt);
							meshList[GEO_SHELTEROBJ]->MeshBBox.translate(((float)(movespeed * dt)), 0, 0);
						}
						else if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_MOVEBOX]->MeshBBox) && !meshList[GEO_MOVEBOX]->MeshBBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox))
						{
							MoveBox_PosX += (float)(movespeed * dt);
							meshList[GEO_MOVEBOX]->MeshBBox.translate(((float)(movespeed * dt)), 0, 0);
							meshList[GEO_MOVEBOXTEST]->MeshBBox.translate(((float)(movespeed * dt)), 0, 0);

						}
					}
				}
			}

			if ((PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER3]->MeshBBox)) && ClimbLamp)
			{
				
			}
			else
			{
				if (Application::IsKeyPressed('W') && elapsedTime > bufferTime_Jump)
				{
					bufferTime_Jump = elapsedTime + 0.5f;
					bufferTime_JumpUp = elapsedTime + 0.3f;
				}
			}

			if (Application::IsKeyPressed(VK_LBUTTON) && !attack && !holdanims())
			{
				bufferTime_attack = elapsedTime + 1.f;

				for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
				{
					if ((EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() > 0) && PlayerClass::get_instance()->PlayerHitBox.collide(EnemyManager::get_instance()->EnemyList[numEnemy]->EnemyHitBox))
						EnemyManager::get_instance()->EnemyList[numEnemy]->edit_health(-50);
				}

				if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_ROPE]->MeshBBox))
					Breakrope++;
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

			for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
			{
				if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() > 0)
				{
					if (elapsedTime > EnemyManager::get_instance()->EnemyList[numEnemy]->bufferTime_attack_MC)
					{
						EnemyManager::get_instance()->EnemyList[numEnemy]->attack(1, EnemyManager::get_instance()->EnemyList[numEnemy]->position_m, EnemyManager::get_instance()->EnemyList[numEnemy]->direction_m, dt, block);
						EnemyManager::get_instance()->EnemyList[numEnemy]->bufferTime_attack_MC = elapsedTime + 2.f;
					}
				}
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

		if (pressedA &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_DEBRIS1]->MeshBBox) &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox))
			PlayerClass::get_instance()->position_a.x -= (float)(30.f * dt);
		else if (pressedD &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_DEBRIS1]->MeshBBox) &&
			!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_TRUMPWALL]->MeshBBox))
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

	if (block || grab)
		movespeed = 17.f;
	else
		movespeed = 30.f; 

	if (Breakrope >= 2)
	{
		if (Lamppostrotate > -40.f)
			Lamppostrotate -= 1.f;

		if (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER]->MeshBBox) && ClimbLamp == false)
		{
			//Do nothing WIP
		}
		else
			ClimbLamp = true;
	}

	if (PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 3) && blockone < 30.f)
		blockone += 0.3f;
	else if (PlayerClass::get_instance()->fm_Killed >= (PlayerClass::get_instance()->get_killedstorage() + 6) && blocktwo < 30.f)
		blocktwo += 0.3f;

	if (inmovement && !holdanims())
		et[6] += dt;
	else
		et[6] = 0;

	if (hmvec[0].get_near())
	{
		et[10] = 0;
		et[11] += dt;
	}
	else
	{
		et[10] += dt;
		et[11] = 0;
	}

	if ((!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER]->MeshBBox) && (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER2]->MeshBBox)))
		|| (PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_LAMPTRIGGER]->MeshBBox) && !ClimbLamp))
	{
		if (!injump)
		{
			if (!PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_FLOORBBOX]->MeshBBox))
			{
				if ((PlayerClass::get_instance()->PlayerHitBox.higherthan(meshList[GEO_MOVEBOX]->MeshBBox)) &&
					(PlayerClass::get_instance()->PlayerHitBox.collide(meshList[GEO_MOVEBOXTEST]->MeshBBox)))
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

	for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() > 0)
		{
			if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_action() == 1) // I should have done this earlier.
			{
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[1] = 0;		  // I'm an idiot.
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[3] = 0;									  //               ._ o o
																												  //               \_`-)|_
				if (elapsedTime + 1.5f < EnemyManager::get_instance()->EnemyList[numEnemy]->bufferTime_attack_MC) //            ,""       \ 
				{																								  //          ,"  ## |   o o.
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[0] = 0;								  //        ," ##   ,-\__    `.
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[2] += dt;							  //      ,"       /     `--._;)
				}																							  	  //    ,"     ## /
				else																							  //  ,"   ##    / Heres a giraffe.
				{
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[0] += dt;
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[2] = 0;
				}
			}
			else if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_action() == 2)
			{
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[1] = 0;
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[2] = 0;

				if ((elapsedTime + 0.5f < EnemyManager::get_instance()->EnemyList[numEnemy]->bufferTime_attack_MC) && (elapsedTime + 1.5f < EnemyManager::get_instance()->EnemyList[numEnemy]->bufferTime_attack_MC))
				{
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[0] += dt;
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[3] = 0;
				}
				else
				{
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[0] = 0;
					EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[3] += dt;
				}
			}
			else
			{
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[0] = 0;
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[1] += dt;
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[2] = 0;
				EnemyManager::get_instance()->EnemyList[numEnemy]->M_et[3] = 0;
			}
		}
	}
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

	for (unsigned int numEnemy = 0; numEnemy < EnemyManager::get_instance()->EnemyList.size(); numEnemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numEnemy]->get_health() > 0)
			EnemyManager::get_instance()->EnemyList[numEnemy]->EnemyHitBox.loadBB("OBJ//Mutant_UpdatedOBJ//Mutant_Torso.obj");
	}
	/*--------------------------------------------------------*/

	/*---------Triggers------*/
	//if (bufferTime_trigger_slope > elapsedTime && trigger == true)
	//{
	//	if (elapsedTime < (bufferTime_trigger_slope - 10.99f))
	//		PlayerClass::get_instance()->position_a.y = -3.f;
	//	PlayerClass::get_instance()->position_a.x += (float)(30.f * dt);
	//	PlayerClass::get_instance()->position_a.y -= (float)(3.25f * dt);
	//	if (elapsedTime >(bufferTime_trigger_slope - 10.8f))
	//	{
	//		PlayerClass::get_instance()->position_a.y -= (float)(10.f * dt);
	//	}
	//	if (elapsedTime > (bufferTime_trigger_slope - 8.f))
	//	{
	//		PlayerClass::get_instance()->position_a.y -= (float)(12.5f * dt);
	//	}
	//}
	//else if (bufferTime_trigger_slope < elapsedTime && trigger == true)
	//	trigger = false;
	/*--------------------------------------*/
	TextInteraction();
	LightInteraction();

	/*--------------Updates the Full Mutant Kill Count--------*/
	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() == 0)
		{
			PlayerClass::get_instance()->fm_Killed++;
			EnemyManager::get_instance()->EnemyList[numenemy]->edit_health(-1); // do i even need this
			EnemyManager::get_instance()->EnemyList.erase(EnemyManager::get_instance()->EnemyList.begin() + numenemy);
		}
	}

	if ((PlayerClass::get_instance()->fm_Killed - PlayerClass::get_instance()->get_killedstorage()) == 3 && EnemyManager::get_instance()->EnemyList.size() == 0)
	{
		for (unsigned int numenemy = 0; numenemy < 3; numenemy++)
			EnemyManager::get_instance()->spawnEnemy(Vector3(250.f + (numenemy * 10.f), 10.f, 20.f));
	}
	else if ((PlayerClass::get_instance()->fm_Killed - PlayerClass::get_instance()->get_killedstorage()) == 6 && EnemyManager::get_instance()->EnemyList.size() == 0)
	{
		for (unsigned int numenemy = 0; numenemy < 2; numenemy++)
			EnemyManager::get_instance()->spawnEnemy(Vector3(400.f + (numenemy * 10.f), 10.f, 20.f));
	}
	else if ((PlayerClass::get_instance()->fm_Killed - PlayerClass::get_instance()->get_killedstorage()) == 8 && EnemyManager::get_instance()->EnemyList.size() == 0)
	{
		for (unsigned int numenemy = 0; numenemy < 2; numenemy++)
			EnemyManager::get_instance()->spawnEnemy(Vector3(640.f + (numenemy * 10.f), 10.f, 20.f));
	}
	/*-------------------------------------------------------*/

	/*---------Change Scene------*/
	if ((PlayerClass::get_instance()->position_a.x > 860))
	{
		for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
		{
			for (unsigned int projectiles = EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); EnemyManager::get_instance()->EnemyList[numenemy]->spit_.size(); projectiles++)
			{
				EnemyManager::get_instance()->EnemyList[numenemy]->spit_.erase(EnemyManager::get_instance()->EnemyList[numenemy]->spit_.begin() + projectiles);
				delete EnemyManager::get_instance()->EnemyList[numenemy]->spit_[projectiles];
				projectiles--;
			}
			EnemyManager::get_instance()->EnemyList.erase(EnemyManager::get_instance()->EnemyList.begin() + numenemy);
			delete EnemyManager::get_instance()->EnemyList[numenemy];
			numenemy--;
		}

		SceneManager::getInstance()->Location = "Cavern of Truth";
		SceneManager::getInstance()->changeScene(new LoadingScreen());
	}
	if (PlayerClass::get_instance()->get_health() <= 0)
	{
		SceneManager::getInstance()->changeScene(new DeathScreen());
		for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
		{
			EnemyManager::get_instance()->EnemyList.erase(EnemyManager::get_instance()->EnemyList.begin() + numenemy);
			numenemy--;
		}
	}
}

void StudioProject2Scene2::Render()
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

	/*-----------------Main Character (Alexis)---------------------*/
	modelStack.PushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	PlayerClass::get_instance()->PlayerHitBox.scale(1.1f, 4.5f, 1.f);
	PlayerClass::get_instance()->PlayerHitBox.translate(PlayerClass::get_instance()->position_a.x, (PlayerClass::get_instance()->position_a.y + 7.9f), PlayerClass::get_instance()->position_a.z);
	modelStack.Translate(PlayerClass::get_instance()->position_a.x, PlayerClass::get_instance()->position_a.y, PlayerClass::get_instance()->position_a.z);
	modelStack.Rotate(PlayerClass::get_instance()->a_LookingDirection, 0, 1, 0);

	// add in grab animation later

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
	//RenderMesh(meshList[GEO_BBOX], false);									 // remove this later when showing actual shit of course
	modelStack.PopMatrix();												 	 // :ok_hand:
	// for some reason I needed to flip translate and scale here to fit with the actual hitbox

	//modelStack.PushMatrix();								// why did i even render this?
	//RenderMesh(meshList[GEO_TRIGGER_SLOPE], false);		// note to self don't use "meshList[GEO_SOMETHING]->MeshBBox.translate(a_PosX, (a_PosY + 8), a_PosZ);" often
	//modelStack.PopMatrix();								// this shit runs every second so smallest translations will move by a lot eventually



	/*-----------------Mutants (Fuglymon)---------------------*/
	unsigned int num_anim_mutant;

	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
		{
			for (num_anim_mutant = 0; num_anim_mutant <= 5;)
			{
				if (EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant] <= 0.f)
					num_anim_mutant++;
				else
					break;
			}
		}
	}

	RenderProjectiles();
	RenderMutant(num_anim_mutant);
	/*-------------------------------------------------------*/

	unsigned int num_anim_hmutant;
	for (num_anim_hmutant = 10; num_anim_hmutant < 20;)
	{
		if (et[num_anim_hmutant] <= 0.f)
			num_anim_hmutant++;
		else
			break;
	}

	/*---------------Half Mutant & Human---------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(hmvec[0].position_hm.x, hmvec[0].position_hm.y, hmvec[0].position_hm.z);
	modelStack.Rotate(hmvec[0].hm_LookingDirection, 0, 1, 0);
	modelStack.Scale(hmvec[0].size_hm.x, hmvec[0].size_hm.y, hmvec[0].size_hm.z);

	modelStack.PushMatrix();
	AnimCheck_H_Mutant(num_anim_hmutant, &modelStack, &et[num_anim_hmutant], "body");
	RenderMesh(meshList[GEO_HM_BODY], true);

	modelStack.PushMatrix();
	AnimCheck_H_Mutant(num_anim_hmutant, &modelStack, &et[num_anim_hmutant], "Head");
	RenderMesh(meshList[GEO_HM_HEAD], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_H_Mutant(num_anim_hmutant, &modelStack, &et[num_anim_hmutant], "RightArm");
	RenderMesh(meshList[GEO_HM_RIGHTARM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_H_Mutant(num_anim_hmutant, &modelStack, &et[num_anim_hmutant], "LeftArm");
	RenderMesh(meshList[GEO_HM_LEFTARM], true);

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_H_Mutant(num_anim_hmutant, &modelStack, &et[num_anim_hmutant], "LeftLeg");
	RenderMesh(meshList[GEO_HM_LEFTLEG], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	AnimCheck_H_Mutant(num_anim_hmutant, &modelStack, &et[num_anim_hmutant], "RightLeg");
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

	/*--------------------Environmental Objects--------------*/
	modelStack.PushMatrix();
	modelStack.Translate(0, -5, 0);
	//RenderMesh(meshList[GEO_FLOORBBOX], false); // delete this later
	RenderMesh(meshList[GEO_SCENE2], true);
	modelStack.PopMatrix();

	RenderDebri();
	RenderDebri2();

	RenderObjects();

	modelStack.PushMatrix();
	modelStack.Translate(125.f, (10.f - blockone), 27.f);
	modelStack.Scale(1.f, 5.f, 57.f);
	RenderMesh(meshList[GEO_BLOCK_ONE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(312.f, (10.f - blocktwo), 27.f);
	modelStack.Scale(1.f, 5.f, 57.f);
	RenderMesh(meshList[GEO_BLOCK_TWO], true);
	modelStack.PopMatrix();
	/*-------------------------------------------------------*/
	/*-----------------Skybox-------------------*/
	modelStack.PushMatrix();
	modelStack.Translate(350, 130, -300);
	modelStack.Scale(1500, 300, 1);
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
	RenderMeshOnScreen(meshList[GEO_HALF_COUNT], 5, 0.5, 8, 8, 0);
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
	RenderTextOnScreen(meshList[GEO_TEXT], testposx, Color(0, 1, 0), 2, 35, 18);
	RenderTextOnScreen(meshList[GEO_TEXT], "INNER CITY", Color(0, 1, 1), 2.5, 1.5, -8.5);

}

bool StudioProject2Scene2::otheranims()
{
	return (attack /*|| trigger*/);
}

bool StudioProject2Scene2::holdanims()
{
	return (roll || grab || block);
}

void StudioProject2Scene2::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
		if (meshList[i] != NULL)
			delete meshList[i];

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void StudioProject2Scene2::RenderProjectiles()
{
	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
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
}

void StudioProject2Scene2::RenderMutant(unsigned int num_anim_mutant)
{
	for (unsigned int numenemy = 0; numenemy < EnemyManager::get_instance()->EnemyList.size(); numenemy++)
	{
		if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() > 0)
		{
			modelStack.PushMatrix();

			EnemyManager::get_instance()->EnemyList[numenemy]->EnemyHitBox.scale(4.f, 2.1f, 1.f);

			EnemyManager::get_instance()->EnemyList[numenemy]->EnemyHitBox.translate(EnemyManager::get_instance()->EnemyList[numenemy]->position_m.x,
				EnemyManager::get_instance()->EnemyList[numenemy]->position_m.y,
				EnemyManager::get_instance()->EnemyList[numenemy]->position_m.z);

			modelStack.Translate(EnemyManager::get_instance()->EnemyList[numenemy]->position_m.x,
				EnemyManager::get_instance()->EnemyList[numenemy]->position_m.y,
				EnemyManager::get_instance()->EnemyList[numenemy]->position_m.z);

			if (EnemyManager::get_instance()->EnemyList[numenemy]->direction_m.x == -1)
				modelStack.Rotate(180, 0, 1, 0);
			else if (EnemyManager::get_instance()->EnemyList[numenemy]->direction_m.x == 1)
				modelStack.Rotate(0, 0, 1, 0);

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_Torso");
			RenderMesh(meshList[GEO_MUTANT_TORSO], true);

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_Neck");
			RenderMesh(meshList[GEO_MUTANT_NECK], true);

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_Head");
			RenderMesh(meshList[GEO_MUTANT_HEAD], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();

			if (EnemyManager::get_instance()->EnemyList[numenemy]->direction_m.x == -1)
				modelStack.Rotate(-180, 0, 1, 0);

			AnimCheck_Mutant(0, &modelStack, &et[0], "Mutant_Head");
			modelStack.Translate(-2, 5, 0);
			RenderMesh(meshList[GEO_M_RHEART], false);
			modelStack.Translate(3, 0, 0);


			if (EnemyManager::get_instance()->EnemyList[numenemy]->get_health() == 100)
				RenderMesh(meshList[GEO_M_RHEART], false);
			else
				RenderMesh(meshList[GEO_M_BHEART], false);

			modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_LeftUpperarm");
			RenderMesh(meshList[GEO_MUTANT_LEFTUPPERARM], true);

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_LeftArm");
			RenderMesh(meshList[GEO_MUTANT_LEFTARM], true);

			modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_RightUpperarm");
			RenderMesh(meshList[GEO_MUTANT_RIGHTUPPERARM], true);

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_RightArm");
			RenderMesh(meshList[GEO_MUTANT_RIGHTARM], true);

			modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_LeftThigh");
			RenderMesh(meshList[GEO_MUTANT_LEFTTHIGH], true);

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_LeftFeet");
			RenderMesh(meshList[GEO_MUTANT_LEFTFEET], true);

			modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_RightThigh");
			RenderMesh(meshList[GEO_MUTANT_RIGHTTHIGH], true);

			modelStack.PushMatrix();
			AnimCheck_Mutant(num_anim_mutant, &modelStack, &EnemyManager::get_instance()->EnemyList[numenemy]->M_et[num_anim_mutant], "Mutant_RightFeet");
			RenderMesh(meshList[GEO_MUTANT_RIGHTFEET], true);

			modelStack.PopMatrix(); // staircase ayy
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}
}