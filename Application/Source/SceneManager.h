#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include "Scene.h"

class SceneManager
{
public:
	static SceneManager& getInstance()
	{
		static SceneManager instance;
		return instance;
	}

	void AddScene(Scene *scene);
	void SetNextScene(int sceneID);
	void Update();

private:
	SceneManager() {}
	std::vector<int> Scenes;

	int currSceneID = 0;
	int nextSceneID = 0;
};

#endif