#ifndef StudioProject2Scene1_H
#define StudioProject2Scene1_H
#include <string>
#include <vector>
#include "Scene.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Camera4_SideScroller.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"
#include "EnemyClass.h"

class StudioProject2Scene1 : public Scene
{
public:
	StudioProject2Scene1();
	~StudioProject2Scene1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void RenderMesh(Mesh *mesh, bool enableLight);
	virtual void Exit();
	std::vector<EnemyClass> enemy;

	enum GEOMETRY_TYPE
	{
		GEO_AXIS,
		GEO_LIGHTBALL,
		GEO_QUAD,
		GEO_BBOX,
		GEO_TESTBBOX,

		//skybox
		GEO_SKYBOX,

		//main character (Alexis)
		GEO_ALEXIS_HEAD, GEO_ALEXIS_BODY, GEO_ALEXIS_RIGHTARM, GEO_ALEXIS_LEFTARM, GEO_ALEXIS_RIGHTHAND, GEO_ALEXIS_LEFTHAND,
		GEO_ALEXIS_CROTCH, GEO_ALEXIS_RIGHTTHIGH, GEO_ALEXIS_RIGHTLEG, GEO_ALEXIS_LEFTTHIGH, GEO_ALEXIS_LEFTLEG,
		//Environmental Objects
		GEO_HOUSEFLOOR, GEO_HOUSE, GEO_HOUSELEFTWALL, GEO_TEXT, GEO_LIGHTBULB,
		GEO_LIGHTSTAND, GEO_HOUSEFRONT, GEO_BOX_SHORT, GEO_BOX_TALL, GEO_HILL,
		GEO_TRUMP, GEO_FLOOR, GEO_TREE, GEO_BOX_SHORTTEST, GEO_BOX_TALLTEST, GEO_TRUMPTEST,

		//UI Objects
		GEO_HEART, GEO_BLANKHEART,
		GEO_TEXTBOX,

		//Triggers
		GEO_TRIGGER_SLOPE,

		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_LIGHT4_POSITION,
		U_LIGHT4_COLOR,
		U_LIGHT4_POWER,
		U_LIGHT4_KC,
		U_LIGHT4_KL,
		U_LIGHT4_KQ,
		U_LIGHT4_TYPE,
		U_LIGHT4_SPOTDIRECTION,
		U_LIGHT4_COSCUTOFF,
		U_LIGHT4_COSINNER,
		U_LIGHT4_EXPONENT,

		U_LIGHT5_POSITION,
		U_LIGHT5_COLOR,
		U_LIGHT5_POWER,
		U_LIGHT5_KC,
		U_LIGHT5_KL,
		U_LIGHT5_KQ,
		U_LIGHT5_TYPE,
		U_LIGHT5_SPOTDIRECTION,
		U_LIGHT5_COSCUTOFF,
		U_LIGHT5_COSINNER,
		U_LIGHT5_EXPONENT,

		U_LIGHT6_POSITION,
		U_LIGHT6_COLOR,
		U_LIGHT6_POWER,
		U_LIGHT6_KC,
		U_LIGHT6_KL,
		U_LIGHT6_KQ,
		U_LIGHT6_TYPE,
		U_LIGHT6_SPOTDIRECTION,
		U_LIGHT6_COSCUTOFF,
		U_LIGHT6_COSINNER,
		U_LIGHT6_EXPONENT,

		U_LIGHT7_POSITION,
		U_LIGHT7_COLOR,
		U_LIGHT7_POWER,
		U_LIGHT7_KC,
		U_LIGHT7_KL,
		U_LIGHT7_KQ,
		U_LIGHT7_TYPE,
		U_LIGHT7_SPOTDIRECTION,
		U_LIGHT7_COSCUTOFF,
		U_LIGHT7_COSINNER,
		U_LIGHT7_EXPONENT,

		U_LIGHTENABLED,
		U_NUMLIGHTS, 

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

private:
	Mesh* meshList[NUM_GEOMETRY];
	//Camera2 camera;
	Camera4 camera;
	Light light[8];
	MS modelStack, viewStack, projectionStack;
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	void LoadLight();
	void LoadCharacter();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position);
	void LightInteraction();
	void TextInteraction(); 
	void HeadDirection();

	/*--------Render Mutants Function-----------*/
	void RenderHalfMutant();
	void RenderFullMutant();
	/*------------------------------------------*/

	// Naming Convention : Trigged == Triggered; TS == TextSize; hm == half-mutant; fm == full-mutant
	/*--------------------Text Variables--------------------------*/
	int spawnTS; // all text size 2
	int pressEnterTS;

	bool syringeTriggedText; 
	int	syringeTriggedTS;

	bool boxTriggedText; 
	int boxTriggedTS;
	bool boxTriggedText_Two; 
	int boxTriggedTS_two;

	bool hmTriggeredText; 
	int hmTriggedTS;

	bool hm_to_alexis; 
	int hm_to_alexisTS;

	bool alexis_to_hm; 
	int alexis_to_hmTS;

	bool alexis_beside_hm; 
	int alexis_beside_hmTS;

	bool postProjectileThrownText; 
	int postProjectileThrownTS;

	bool fm_triggedText; 
	int fm_triggedTS;

	bool alexisText;  
	int alexisTS;

	bool guideText; 
	int guideTS;

	bool pEnter;
	int textOccurStorage;
	int textOccured;
	bool nexttext;

	/*------------------------------------------------------------*/

	/*-------------Character Variables (a means Alexis)-----------*/
	float a_LookingDirection;
	bool pressedD;
	bool pressedA;
	bool injump;
	bool infall;
	bool attack;
	bool trigger;
	bool grab;

	float a_PosX;
	float a_PosY;
	float a_PosZ;

	float a_RotationLeftLeg;
	float a_RotationRightLeg;
	/*------------------------------------------------------------*/

	/*----------------Health System UI variables (hearts)---------*/
	float a_heart1;
	float a_heart2;
	float a_heart3;
	float a_heart4;
	float a_heart5;
	float a_blankheart1;
	float a_blankheart2;
	float a_blankheart3;
	float a_blankheart4;
	float a_blankheart5;

	int heartCounter;
	/*------------------------------------------------------------*/
	std::string fps;
	
	// TIME
	double elapsedTime;
	double bufferTime_JumpUp;
	double bufferTime_Jump;
	double bufferTime_attack;
	double bufferTime_text;
	double bufferTime_trigger_slope;


};

#endif 