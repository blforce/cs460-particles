//#pragma once

#include "Scene.h"


CScene::CScene()
{
	camera.x = 0.0f;
	camera.y = 1.0f;
	camera.z = 10.0f;

	particleSystem = CParticleSystem();

	environment = CEnvironment();
}

CScene::CScene(CEnvironment env)
{
	CScene();

	environment = env;
}


void CScene::Render(void)
{
	particleSystem.Render(&environment, camera);
}


void CScene::Update(void)
{
	particleSystem.Spawn();
}


CScene::~CScene(void)
{
}
