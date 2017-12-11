#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 900;
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;

	// Minimum and maximum vector3 values to store the "size" of the world's AABB
	vector3 min = vector3(std::numeric_limits<float>::max(), 
						  std::numeric_limits<float>::max(), 
						  std::numeric_limits<float>::max());
	vector3 max = vector3(std::numeric_limits<float>::min(), 
						  std::numeric_limits<float>::min(),
						  std::numeric_limits<float>::min());
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);

			// Update world min and max
			if (v3Position.x < min.x) min.x = v3Position.x;
			if (v3Position.y < min.y) min.y = v3Position.y;
			if (v3Position.z < min.z) min.z = v3Position.z;
			if (v3Position.x > max.x) max.x = v3Position.x;
			if (v3Position.y > max.y) max.y = v3Position.y;
			if (v3Position.z > max.z) max.z = v3Position.z;
		}
	}

	// Buffer because object width is not taken into account
	float buffer = 1;
	min.x = min.x - buffer;
	min.y = min.y - buffer;
	min.z = min.z - buffer;
	max.x = max.x + buffer;
	max.y = max.y + buffer;
	max.z = max.z + buffer;

	// Create Octree
	m_uOctantLevels = 5;
	m_pRootAABB = new AABB(min, max);
	m_pRoot = new MyOctant(m_uOctantLevels, m_pRootAABB, m_pEntityMngr, m_pMeshMngr);
	m_pRoot->CreateTree();
	m_pEntityMngr->Update();
	m_pRoot->CheckForCollisions();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager and use Octree to check for collisions
	m_pEntityMngr->Update();
	m_pRoot->CheckForCollisions();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Display octree
	m_pRoot->Display();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();

	SafeDelete(m_pRoot);
	SafeDelete(m_pRootAABB);
}