#ifndef StudioProject2Scene1_H
#define StudioProject2Scene1_H
#include <string>
#include "Scene.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Camera4_SideScroller.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"

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
	virtual void text();

	enum GEOMETRY_TYPE
	{
		GEO_AXIS,
		GEO_LIGHTBALL,
		GEO_TEXTBOX,
		GEO_QUAD,
		GEO_BBOX,

		//skybox
		GEO_LEFT, GEO_RIGHT, GEO_TOP, GEO_BOTTOM, GEO_FRONT, GEO_BACK,

		//main character (Alexis)
		GEO_ALEXIS_HEAD, GEO_ALEXIS_BODY, GEO_ALEXIS_RIGHTARM, GEO_ALEXIS_LEFTARM, GEO_ALEXIS_RIGHTHAND, GEO_ALEXIS_LEFTHAND,
		GEO_ALEXIS_CROTCH, GEO_ALEXIS_RIGHTTHIGH, GEO_ALEXIS_RIGHTLEG, GEO_ALEXIS_LEFTTHIGH, GEO_ALEXIS_LEFTLEG,

		//Environmental Objects
		GEO_HOUSEFLOOR, GEO_HOUSE, GEO_HOUSELEFTWALL, GEO_WALL,
		GEO_TEXT,
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

	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, int position);

	// Naming Convention : Trigged == Triggered; TS == TextSize; hm == half-mutant; fm == full-mutant
	/*--------------------Text Variables--------------------------*/
	int spawnTS; // all text size 2

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
	/*------------------------------------------------------------*/

	/*-------------Character Variables (a means Alexis)-----------*/
	float a_LookingDirection;
	bool pressedD;
	bool pressedA;
	bool inmovement;
	bool injump;

	float a_PosX;
	float a_PosY;
	float a_PosZ;

	float a_RotationLeftLeg;
	float a_RotationRightLeg;
	/*------------------------------------------------------------*/
	std::string fps;
	
	// TIME
	double elapsedTime;
	double bufferTime;
};

#endif 