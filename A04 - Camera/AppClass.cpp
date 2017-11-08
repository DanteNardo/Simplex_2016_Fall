// Resources used:
// https://stackoverflow.com/questions/29451567/how-to-move-in-the-direction-of-the-camera-opengl-c

#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Dante Nardo - dcn5783@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera
	m_pCamera = new MyCamera();
	m_pCamera->SetPositionTargetAndUp(
			vector3(0.0f, 3.0f, 20.0f), //Where my eyes are
			vector3(0.0f, 3.0f, 19.0f), //where what I'm looking at is
			AXIS_Y);					//what is up

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Add objects to the Manager
	for (int j = -50; j < 50; j += 2)
	{
		for (int i = -50; i < 50; i += 2)
		{
			m_pMyMeshMngr->AddConeToRenderList(glm::translate(vector3(i, 0.0f, j)));
		}
	}
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();

	// Set forward and right vectors for camera space movement
	// Forward that shows where we are pointing
	// Right is a vector that points perpendicular to where we are going
	// You get right from the cross product of the forward direction and up direction
	const vector3 UP = AXIS_Y;
	m_pCamera->SetForward(m_pCamera->GetTarget() - m_pCamera->GetPosition());
	m_pCamera->SetRight(glm::cross(UP, m_pCamera->GetForward()));

	// Determine current direction
	// This effectively 
	const float speed = 3.0f;
	vector3 direction = vector3(
		cos(m_pCamera->GetAngleY()) * sin(m_pCamera->GetAngleX()),
		sin(m_pCamera->GetAngleY()),
		cos(m_pCamera->GetAngleY()) * cos(m_pCamera->GetAngleX())
	);

	// Set the camera's position and target
	// Current position = initial + current + direction
	// Target = current position + direction
	m_pCamera->SetPosition(vector3(0, 0.0f, 10.0f) + m_v3Pos + direction);
	m_pCamera->SetTarget(m_pCamera->GetPosition() + direction);
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release model
	SafeDelete(m_pModel);

	//release primitive
	SafeDelete(m_pMesh);

	//release primitive
	SafeDelete(m_pMesh2);

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
