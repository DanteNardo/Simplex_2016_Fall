#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Dante Nardo - dcn5783@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	// Init the meshes
	m_meshes = std::vector<MyMesh*>();
	for (int i = 0; i < 46; i++) {
		MyMesh* newMesh = new MyMesh();
		newMesh->GenerateCube(1, vector3(0, 0, 0)); // Gotta make sure it has a sexy black
		m_meshes.push_back(newMesh);
	}

	// =========================================================================================================
	// ======================================== Hardcode Mesh Positions ========================================
	// =========================================================================================================

	#pragma region Hardcoded Mesh Positions
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	static float value = 0.0f;
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(1.0f, 1.0f, 1.0f));
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 1.0f, 1.0f));
	matrix4 m4Model = m4Scale * m4Translate;

	// Antenna
	m_meshes[0]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 4, 0)));
	m_meshes[1]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 4, 0)));
	m_meshes[2]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, 3, 0)));
	m_meshes[3]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, 3, 0)));

	// Top of Head
	m_meshes[4]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 2, 0)));
	m_meshes[5]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, 2, 0)));
	m_meshes[6]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, 2, 0)));
	m_meshes[7]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, 2, 0)));
	m_meshes[8]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, 2, 0)));
	m_meshes[9]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, 2, 0)));
	m_meshes[10]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 2, 0)));

	// Eye Level
	m_meshes[11]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 4, 1, 0)));
	m_meshes[12]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 1, 0)));
	m_meshes[13]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, 1, 0)));
	m_meshes[14]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, 1, 0)));
	m_meshes[15]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, 1, 0)));
	m_meshes[16]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 1, 0)));
	m_meshes[17]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 4, 1, 0)));

	// Mid Body
	m_meshes[18]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 5, 0, 0)));
	m_meshes[19]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 4, 0, 0)));
	m_meshes[20]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 0, 0)));
	m_meshes[21]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, 0, 0)));
	m_meshes[22]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, 0, 0)));
	m_meshes[23]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, 0, 0)));
	m_meshes[24]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, 0, 0)));
	m_meshes[25]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, 0, 0)));
	m_meshes[26]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 0, 0)));
	m_meshes[27]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 4, 0, 0)));
	m_meshes[28]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 5, 0, 0)));

	// Arms and Legs 1
	m_meshes[29]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 5, -1, 0)));
	m_meshes[30]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, -1, 0)));
	m_meshes[31]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, -1, 0)));
	m_meshes[32]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, -1, 0)));
	m_meshes[33]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, -1, 0)));
	m_meshes[34]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, -1, 0)));
	m_meshes[35]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, -1, 0)));
	m_meshes[36]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, -1, 0)));
	m_meshes[37]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 5, -1, 0)));

	// Arms and Legs 2
	m_meshes[38]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 5, -2, 0)));
	m_meshes[39]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, -2, 0)));
	m_meshes[40]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, -2, 0)));
	m_meshes[41]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 5, -2, 0)));

	// Arms and Legs 3
	m_meshes[42]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, -3, 0)));
	m_meshes[43]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, -3, 0)));
	m_meshes[44]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, -3, 0)));
	m_meshes[45]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, -3, 0)));
	#pragma endregion

	// =========================================================================================================
	// =========================================================================================================
	// =========================================================================================================
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	static float value = 0.0f;
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(1.0f, 1.0f, 1.0f));
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	// =========================================================================================================
	// ======================================== Mesh Render Translating ========================================
	// =========================================================================================================

	#pragma region Mesh Render Translating

	// Antenna
	m_meshes[0]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 4, 0)));
	m_meshes[1]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 4, 0)));
	m_meshes[2]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, 3, 0)));
	m_meshes[3]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, 3, 0)));

	// Top of Head
	m_meshes[4]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 2, 0)));
	m_meshes[5]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, 2, 0)));
	m_meshes[6]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, 2, 0)));
	m_meshes[7]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, 2, 0)));
	m_meshes[8]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, 2, 0)));
	m_meshes[9]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, 2, 0)));
	m_meshes[10]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 2, 0)));

	// Eye Level
	m_meshes[11]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 4, 1, 0)));
	m_meshes[12]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 1, 0)));
	m_meshes[13]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, 1, 0)));
	m_meshes[14]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, 1, 0)));
	m_meshes[15]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, 1, 0)));
	m_meshes[16]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 1, 0)));
	m_meshes[17]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 4, 1, 0)));

	// Mid Body
	m_meshes[18]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 5, 0, 0)));
	m_meshes[19]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 4, 0, 0)));
	m_meshes[20]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, 0, 0)));
	m_meshes[21]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, 0, 0)));
	m_meshes[22]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, 0, 0)));
	m_meshes[23]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, 0, 0)));
	m_meshes[24]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, 0, 0)));
	m_meshes[25]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, 0, 0)));
	m_meshes[26]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, 0, 0)));
	m_meshes[27]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 4, 0, 0)));
	m_meshes[28]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 5, 0, 0)));

	// Arms and Legs 1
	m_meshes[29]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 5, -1, 0)));
	m_meshes[30]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, -1, 0)));
	m_meshes[31]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, -1, 0)));
	m_meshes[32]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, -1, 0)));
	m_meshes[33]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 0, -1, 0)));
	m_meshes[34]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, -1, 0)));
	m_meshes[35]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, -1, 0)));
	m_meshes[36]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, -1, 0)));
	m_meshes[37]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 5, -1, 0)));

	// Arms and Legs 2
	m_meshes[38]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 5, -2, 0)));
	m_meshes[39]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 3, -2, 0)));
	m_meshes[40]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 3, -2, 0)));
	m_meshes[41]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 5, -2, 0)));

	// Arms and Legs 3
	m_meshes[42]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 2, -3, 0)));
	m_meshes[43]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value - 1, -3, 0)));
	m_meshes[44]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 1, -3, 0)));
	m_meshes[45]->Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(value + 2, -3, 0)));
	#pragma endregion

	// =========================================================================================================
	// =========================================================================================================
	// =========================================================================================================
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	for (int i = 0; i < m_meshes.size(); i++) {
		SafeDelete(m_meshes[i]);
	}

	//release GUI
	ShutdownGUI();
}