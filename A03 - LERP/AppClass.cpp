#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Dante Nardo - dcn5783@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
	m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}

	//if there are no segments create 7
	if (m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1 ? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	fTimers = std::vector<float>(m_uOrbits); //store the new timer
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		m_routes.push_back(CreateRoute(fSize, i));
		m_routePositions.push_back(m_routes[i - uSides][0]);
		m_routeIndex.push_back(1);
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
}
route Application::CreateRoute(float a_fSize, uint a_sides)
{
	// Init route
	route r = route(a_sides);

	// Init angle variables
	uint cAngle = 0;
	float aFactor = 360 / a_sides;

	// Iterate through total route points
	for (uint i = 0; i < a_sides; i++) {

		// Create a "circle" with a small amount of points where the distance between the points
		// is an amount of degrees equal to 360 / the amount of sides
		r[i] = vector3(a_fSize * cos(cAngle * PI / 180), a_fSize * sin(cAngle * PI / 180), 0);
		cAngle += aFactor;
	}

	return r;
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	//Get a timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	float dt = m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer
	for (int i = 0; i < fTimers.size(); i++)
		fTimers[i] += dt;

	// Draw shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		// Calculate the current position
		vector3 v3CurrentPos = glm::lerp(m_routePositions[i], m_routes[i][m_routeIndex[i]], 1.0f * fTimers[i]);

		// Set new position and check if we need to change route index
		// If the distance to the next position is very small then set next position
		if (glm::sqrt(
			pow(v3CurrentPos.x - m_routes[i][m_routeIndex[i]].x, 2) + 
			pow(v3CurrentPos.y - m_routes[i][m_routeIndex[i]].y, 2)) < 0.01f) {

			// Set our new position, reset time, increment routeIndex
			m_routePositions[i] = v3CurrentPos;
			fTimers[i] = 0;
			m_routeIndex[i] += 1;
			if (m_routeIndex[i] >= m_routes[i].size())
				m_routeIndex[i] = 0;
		}

		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		// Draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}