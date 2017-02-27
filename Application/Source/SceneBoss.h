#ifndef SceneBoss_H
#define SceneBoss_H
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

class SceneBoss : public Scene
{
public:
	SceneBoss();
	~SceneBoss();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void RenderMesh(Mesh *mesh, bool enableLight);
	virtual void Exit();
	virtual bool otheranims();
	virtual bool holdanims();
	virtual void RenderProjectiles();
	virtual void RenderMutant();
	double et[30];

	enum GEOMETRY_TYPE
	{
		GEO_AXIS,
		GEO_LIGHTBALL,
		GEO_QUAD,
		GEO_BBOX,
		GEO_TESTBBOX,

		//skybox
		GEO_SKYBOX, GEO_GROUND,

		//main character (Alexis)
		GEO_ALEXIS_HEAD, GEO_ALEXIS_BODY, GEO_ALEXIS_RIGHTARM, GEO_ALEXIS_LEFTARM,
		GEO_ALEXIS_CROTCH, GEO_ALEXIS_RIGHTLEG, GEO_ALEXIS_LEFTLEG, GEO_LASER,
		//main character stunt doobl (Alexus) :^)
		GEO_ALEXUS_HEAD, GEO_ALEXUS_BODY, GEO_ALEXUS_RIGHT4ARM, GEO_ALEXUS_LEFT4ARM,
		GEO_ALEXUS_CROTCH, GEO_ALEXUS_RIGHTTHIGH, GEO_ALEXUS_LEFTTHIGH,
		GEO_ALEXUS_RIGHTARM, GEO_ALEXUS_LEFTARM, GEO_ALEXUS_RIGHTLEG, GEO_ALEXUS_LEFTLEG,
		//mutant
		GEO_MUTANT_HEAD, GEO_MUTANT_LEFTARM, GEO_MUTANT_LEFTFEET, GEO_MUTANT_LEFTTHIGH,
		GEO_MUTANT_LEFTUPPERARM, GEO_MUTANT_NECK, GEO_MUTANT_RIGHTARM, GEO_MUTANT_RIGHTFEET,
		GEO_MUTANT_RIGHTTHIGH, GEO_MUTANT_RIGHTUPPERARM, GEO_MUTANT_TORSO, GEO_SPIT,
		GEO_M_RHEART, GEO_M_BHEART,
		//UI Objects
		GEO_BLANKHEART, GEO_ALEXIS_LIFE, GEO_BOSSLIFE, GEO_ENERGY, GEO_BLANKENERGY,
		GEO_TEXT, GEO_LASER_CD, GEO_LASER_ICON, GEO_PROJSHIELD_CD, GEO_PROJSHIELD,
		//Boss
		GEO_BOSS_LARM, GEO_BOSS_LJAW, GEO_BOSS_LUARM, GEO_BOSS_MHEAD, GEO_BOSS_MJAW,
		GEO_BOSS_NECK, GEO_BOSS_RARM, GEO_BOSS_RJAW, GEO_BOSS_RUARM, GEO_BOSS_SEG1,
		GEO_BOSS_SEG2, GEO_BOSS_SEG3, GEO_BOSS_SEG4, GEO_BOSS_SEG5, GEO_BOSS_SEG6,
		GEO_BOSS_TORSO,

		GEO_BOSS_INDICATOR, GEO_SPIKE,

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
	Light light[1];
	MS modelStack, viewStack, projectionStack;
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	void LoadLight();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position);

	void RenderLightStands();

	/*--------------------Text Variables--------------------------*/
	bool pEnter;
	int pressEnterTS;
	/*------------------------------------------------------------*/

	/*-------------Character Variables (a means Alexis)-----------*/ // no a means apple
	bool pressedD;
	bool pressedA;
	bool attack;
	bool block;
	bool roll;
	/*------------------------------------------------------------*/

	std::string fps;

	// TIME
	double elapsedTime;
	double bufferTime_attack;
	double bufferTime_roll;
	double bufferTime_block;
	double bufferTime_iframeroll;
};

#endif 