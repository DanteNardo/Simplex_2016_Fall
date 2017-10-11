#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// =============================================================================
	// =============================== MY CONE CODE ================================
	// =============================================================================
	#pragma region ConeCode

	//				WHAT ARE CONE
	// A cone is a circle with multiple points on it
	// that all converge at a certain height with an
	// x position equal to the center of the circle

	//				HOW MAKE CONE
	// Generate an amount of points equal to 360
	// divided by amount of subdivisions and then
	// connect them to one center point

	//			    CREATE CIRCLE
	// Set starting degree angle and then continue
	// creating points by rotating until 360
	std::vector<vector3> points = std::vector<vector3>();
	int angle = 360 / a_nSubdivisions;
	int cAngle = 0;
	while (cAngle < 360) {
		points.push_back(vector3(a_fRadius * cos(cAngle * PI / 180), 0,
								 a_fRadius * sin(cAngle * PI / 180)));
		cAngle += angle;
	}

	//			 DEFINE CENTER POINTS
	// Set top point at center of circle and equal to
	// the height value given as a param and bottom at 0
	vector3 bottom = vector3(0, 0, 0);
	vector3 top = vector3(0, a_fHeight, 0);

	//				CREATE TRIS
	// Iterate through points and create a tri every
	// other point from two circle points to top
	int next = 0;
	for (int i = 0; i < points.size(); i++) {

		// Determine next index
		next = (i + 1 >= points.size()) ? 0 : i + 1;

		// Connect tris to bottom
		AddTri(points[next], bottom, points[i]);
		
		// Connect tris to top
		AddTri(points[i], top, points[next]);
	}
	#pragma endregion
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// =============================================================================
	// ============================ MY CYLINDER CODE ===============================
	// =============================================================================
	#pragma region CylinderCode

	//			  WHAT ARE CYLINDER
	// A cylinder is two circles that connect each
	// point vertically to a point on the other circle.

	//			  HOW MAKE CYLINDER
	// Generate a circle with a bottom face, and a
	// circle with a top face. Connect every vertical
	// point on the circle.

	//			 CREATE BOTH CIRCLES
	// Set starting degree angle and then continue
	// creating points by rotating until 360
	std::vector<vector3> bCircle = std::vector<vector3>();
	std::vector<vector3> tCircle = std::vector<vector3>();
	int angle = 360 / a_nSubdivisions;
	int cAngle = 0;
	while (cAngle < 360) {
		bCircle.push_back(vector3(a_fRadius * cos(cAngle * PI / 180), 0,
								  a_fRadius * sin(cAngle * PI / 180)));
		tCircle.push_back(vector3(a_fRadius * cos(cAngle * PI / 180), a_fHeight,
								  a_fRadius * sin(cAngle * PI / 180)));
		cAngle += angle;
	}

	//			 CREATE BOTH CENTERS
	// Bottom face and top face both need a center point
	vector3 bCenter = vector3(0, 0, 0);
	vector3 tCenter = vector3(0, a_fHeight, 0);

	//		  CREATE TRI AND QUAD FACES
	// Set starting degree angle and then continue
	// creating points by rotating until 360
	int next = 0;
	for (int i = 0; i < bCircle.size(); i++) {

		// Determine next index
		next = (i + 1 >= bCircle.size()) ? 0 : i + 1;

		// Create bottom and top faces
		AddTri(bCircle[next], bCenter, bCircle[i]);
		AddTri(tCircle[i], tCenter, tCircle[next]);

		// Connect bottom circle to top circle
		AddQuad(bCircle[next], bCircle[i], tCircle[next], tCircle[i]);
	}
	#pragma endregion
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// =============================================================================
	// ============================== MY TUBE CODE =================================
	// =============================================================================
	#pragma region TubeCode

	//			  WHAT ARE TUBE
	// A tube is two cylinders where a smaller cylinder
	// "cuts out" a portion of a larger cylinder

	//			  HOW MAKE CYLINDER
	// Generate two bottom circles, one with smaller radius.
	// Generate two top circles, one with smaller radius.
	// Create quad walls between both inner circles.
	// Create quad walls between both outer circles.

	//			 CREATE ALL CIRCLES
	// Set starting degree angle and then continue
	// creating points by rotating until 360
	std::vector<vector3> bICircle = std::vector<vector3>(); // bottom inner circle
	std::vector<vector3> tICircle = std::vector<vector3>(); // top    inner circle
	std::vector<vector3> bOCircle = std::vector<vector3>(); // bottom outer circle
	std::vector<vector3> tOCircle = std::vector<vector3>(); // top    outer circle
	int angle = 360 / a_nSubdivisions;
	int cAngle = 0;
	while (cAngle < 360) {
		bICircle.push_back(vector3(a_fInnerRadius * cos(cAngle * PI / 180), 0,
								   a_fInnerRadius * sin(cAngle * PI / 180)));
		bOCircle.push_back(vector3(a_fOuterRadius * cos(cAngle * PI / 180), 0,
								   a_fOuterRadius * sin(cAngle * PI / 180)));
		tICircle.push_back(vector3(a_fInnerRadius * cos(cAngle * PI / 180), a_fHeight,
								   a_fInnerRadius * sin(cAngle * PI / 180)));
		tOCircle.push_back(vector3(a_fOuterRadius * cos(cAngle * PI / 180), a_fHeight,
								   a_fOuterRadius * sin(cAngle * PI / 180)));
		cAngle += angle;
	}

	//		  CREATE TRI AND QUAD FACES
	// Set starting degree angle and then continue
	// creating points by rotating until 360
	int next = 0;
	for (int i = 0; i < bICircle.size(); i++) {

		// Determine next index
		next = (i + 1 >= bICircle.size()) ? 0 : i + 1;

		// Connect bottom inner and bottom outer circles
		AddQuad(bICircle[next], bICircle[i], bOCircle[next], bOCircle[i]);

		// Connect top inner and top outer circles
		AddQuad(tICircle[i], tICircle[next], tOCircle[i], tOCircle[next]);

		// Connect bottom inner circle to top inner circle
		AddQuad(bICircle[i], bICircle[next], tICircle[i], tICircle[next]);

		// Connect bottom outer circle to top outer circle
		AddQuad(bOCircle[next], bOCircle[i], tOCircle[next], tOCircle[i]);
	}
	#pragma endregion
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// =============================================================================
	// ============================== MY TORUS CODE ================================
	// =============================================================================

	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// =============================================================================
	// ============================== MY SPHERE CODE ===============================
	// =============================================================================
	#pragma region SphereCode

	//			  WHAT ARE SPHERE
	// A sphere is a bunch of layered circles with 

	//			  HOW MAKE SPHERE
	// Generate a bunch of horizontal circles whose 
	// radius and y position are determined by a
	// vertical circle that we "create" by keeping
	// track of an incremental representation of the
	// angles of the points on that vertical circle.
	// Then we make two bottom faces similar to how
	// we did it with a cone.

	//			 CREATE ALL CIRCLES
	// Set starting degree angle and then continue
	// creating points by rotating until 360
	std::vector<std::vector<vector3>> circles = std::vector<std::vector<vector3>>();
	float radius = 1.0f;
	float lower = -0.1f;
	float higher = 0.1f;

	// rAngle and crAngle are used to determine the height
	// positions of the numerous circle bands
	int rAngle = 90 / (a_nSubdivisions);
	int crAngle = 0;

	// Set initial radius, lower, and higher values
	radius  = cos(crAngle * PI / 180) * a_fRadius;
	lower = -(sin(crAngle * PI / 180) * a_fRadius);
	higher  = sin(crAngle * PI / 180) * a_fRadius;

	// Create an amount of circles equal to subdivisions
	for (int i = 0; i < a_nSubdivisions - 1; i++) {

		// Generate two circles
		std::vector<vector3> lCircle = std::vector<vector3>(); // lower circle
		std::vector<vector3> hCircle = std::vector<vector3>(); // higher circle
		int angle = 360 / a_nSubdivisions;
		int cAngle = 0;
		while (cAngle < 360) {
			vector3 lPoint = vector3(radius * cos(cAngle * PI / 180), lower,
								     radius * sin(cAngle * PI / 180));
			vector3 hPoint = vector3(radius * cos(cAngle * PI / 180), higher,
								     radius * sin(cAngle * PI / 180));
			lCircle.push_back(lPoint);
			hCircle.push_back(hPoint);
			cAngle += angle;
		}

		// Radius is determined by width (thus cos)
		// Lower and higher are determined by height (thus sin)
		crAngle += rAngle;
		radius  = cos(crAngle * PI / 180) * a_fRadius;
		lower = -(sin(crAngle * PI / 180) * a_fRadius);
		higher  = sin(crAngle * PI / 180) * a_fRadius;
		circles.push_back(lCircle);
		circles.push_back(hCircle);
	}

	//		  CREATE TRI AND QUAD FACES
	// Create the center band of the sphere first
	// then alternate between upper and lower bands.
	// Afterwards, create bottom and then top faces.
	int nextJ = 0;
	int nextK = 0;
	bool even = true;
	for (int j = 0; j < circles.size(); j++) {
		for (int k = 0; k < circles[j].size(); k++) {

			// Create middle odd circle band and then branch out
			if (j == 0) {

				// Determine next indexes
				nextJ = j + 1;
				nextK = (k + 1 >= circles[j].size()) ? 0 : k + 1;
				AddQuad(circles[j][k], circles[j][nextK], circles[j + 1][k], circles[j + 1][nextK]);
				AddQuad(circles[j][k], circles[j][nextK], circles[j + 2][k], circles[j + 2][nextK]);
			}

			// Create every other band
			else {

				// Determine next indexes
				nextJ = (j + 2 >= circles.size()) ? 0 : j + 2;
				nextK = (k + 1 >= circles[j].size()) ? 0 : k + 1;

				if (even) AddQuad(circles[nextJ][nextK], circles[nextJ][k], circles[j][nextK], circles[j][k]);
				else	  AddQuad(circles[j][nextK], circles[j][k], circles[nextJ][nextK], circles[nextJ][k]);
			}
		}

		// Boolean to easily keep track of true or false
		// to draw quads facing the right way
		even = (even == false) ? true : false;
	}

	//		CREATE BOTTOM AND TOP FACES
	// Select the last two circles and create tris
	// to the two generated points
	crAngle += rAngle;
	radius  = cos(crAngle * PI / 180) * a_fRadius;
	lower = -(sin(crAngle * PI / 180) * a_fRadius);
	higher  = sin(crAngle * PI / 180) * a_fRadius;
	vector3 bottom = vector3(radius * cos(crAngle * PI / 180), lower,
							 radius * sin(crAngle * PI / 180));
	vector3 top =	 vector3(radius * cos(crAngle * PI / 180), higher,
							 radius * sin(crAngle * PI / 180));

	// Iterate through the last two circles and create bottom and top faces
	// The first loop is the bottom
	int next = 0;
	int index = circles.size() - 1;
	for (int l = 0; l < circles[index].size(); l++) {
		next = (l + 1 >= circles[index].size()) ? 0 : l + 1;
		AddTri(circles[index][next], circles[index][l], top);
	}
	// The second loop is the top
	next = 0;
	index--;
	for (int m = 0; m < circles[index].size(); m++) {
		next = (m + 1 >= circles[index].size()) ? 0 : m + 1;
		AddTri(circles[index][m], circles[index][next], bottom);
	}

	#pragma endregion
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}