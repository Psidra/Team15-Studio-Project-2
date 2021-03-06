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
#include "HalfMutant.h"

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
	virtual bool otheranims();
	virtual bool holdanims();
	virtual void RenderProjectiles();
	virtual void RenderMutant(unsigned int num_anim_mutant);

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
		GEO_ALEXIS_HEAD, GEO_ALEXIS_BODY, GEO_ALEXIS_RIGHTARM, GEO_ALEXIS_LEFTARM,
		GEO_ALEXIS_CROTCH, GEO_ALEXIS_RIGHTLEG, GEO_ALEXIS_LEFTLEG,
		//main character stunt doobl (Alexus) :^)
		GEO_ALEXUS_HEAD, GEO_ALEXUS_BODY, GEO_ALEXUS_RIGHT4ARM, GEO_ALEXUS_LEFT4ARM,
		GEO_ALEXUS_CROTCH, GEO_ALEXUS_RIGHTTHIGH, GEO_ALEXUS_LEFTTHIGH,
		GEO_ALEXUS_RIGHTARM, GEO_ALEXUS_LEFTARM, GEO_ALEXUS_RIGHTLEG, GEO_ALEXUS_LEFTLEG,

		//Environmental Objects
		GEO_HOUSEFLOOR, GEO_HOUSE, GEO_HOUSELEFTWALL, GEO_TEXT, GEO_LIGHTBULB,
		GEO_LIGHTSTAND, GEO_HOUSEFRONT, GEO_BOX_SHORT, GEO_BOX_TALL, GEO_HILL,
		GEO_TRUMP, GEO_FLOOR, GEO_TREE, GEO_BOX_SHORTTEST, GEO_BOX_TALLTEST, GEO_TRUMPTEST,
		GEO_CLUSTERTREE, GEO_DEBRIS, GEO_SYRINGE, GEO_BAT,

		//mutant
		GEO_MUTANT_HEAD, GEO_MUTANT_LEFTARM, GEO_MUTANT_LEFTFEET, GEO_MUTANT_LEFTTHIGH,
		GEO_MUTANT_LEFTUPPERARM, GEO_MUTANT_NECK, GEO_MUTANT_RIGHTARM, GEO_MUTANT_RIGHTFEET,
		GEO_MUTANT_RIGHTTHIGH, GEO_MUTANT_RIGHTUPPERARM, GEO_MUTANT_TORSO, GEO_SPIT,

		// half mutant & npc
		GEO_HUMAN, GEO_HM_HEAD, GEO_HM_BODY, GEO_HM_LEFTARM,
		GEO_HM_RIGHTARM, GEO_HM_LEFTLEG, GEO_HM_RIGHTLEG,

		//UI Objects
		GEO_ALEXIS_LIFE, GEO_BLANKHEART,
		GEO_TEXTBOX, GEO_HALF_COUNT, GEO_FULL_COUNT,

		//Mutant Health
		GEO_M_RHEART, GEO_M_BHEART,

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
	Light light[2];
	MS modelStack, viewStack, projectionStack;
	std::vector<HalfMutant> hmvec;
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	bool Unlock;

	void LoadLight();
	void LoadCharacter();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position);
	void LightInteraction();
	void TextInteraction(); 
	void RenderTrees();
	void RenderLightStands();
	void RenderTextInteractions();

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
	int textOccured;
	bool nexttext;

	/*------------------------------------------------------------*/

	/*-------------Character Variables (a means Alexis)-----------*/ // no a means apple
	bool pressedD;
	bool pressedA;
	bool inmovement;
	bool injump;
	bool attack;
	bool trigger;
	bool grab;
	bool block;
	bool roll;

	double et[30];
	/*  Alexis:
	0 = attack
	6 = walk
	7 = roll
	8 = block
	9 = grab

	Half-Mutant:

	Mutant:
	20 = idle
	21 = walk
	22 = attack
	23 = spit
	*/
	/*------------------------------------------------------------*/

	float ShortBox_PosX;
	float TallBox_PosX;
	float syringeSizeX;
	float syringeSizeY;
	float syringeSizeZ;

	float movespeed;
	float movebat;

	std::string fps;
	std::string hMutantSaved;
	std::string fMutantKilled;

	// TIME
	double elapsedTime;
	double bufferTime_JumpUp;
	double bufferTime_Jump;
	double bufferTime_attack;
	double bufferTime_text;
	double bufferTime_textlog;
	double bufferTime_fmAlexisText;
	double bufferTime_Enter;
	double bufferTime_trigger_slope;	// ten thousand double buffertimes jesus
	double bufferTime_grab;				// there's probably a better way for this but I'm too dumb to know and code it
	double bufferTime_iframe;			// iframe is for damage taken
	double bufferTime_block;			// OLD SPICE ODOUR BODY BLOCKER BLOCKS BACTERIA AND SMELL FOR UP TO 24 HOURS
	double bufferTime_roll;
	double bufferTime_iframeroll;		// I would like to apologise for this monstrocity of buffertimes
	double bufferTime_Unlock;
	double bufferTime_attack_M;
};

#endif 