#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
}
void MyEntityManager::Release(void)
{
	ReleaseInstance();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	for (int i = 0; i < sizeof(m_entityList); i++) {
		delete m_pInstance->m_entityList[i];
		m_pInstance->m_entityList[i] = nullptr;
	}

	delete m_pInstance;
	m_pInstance = nullptr;
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	for (int i = 0; i < sizeof(m_entityList); i++)
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			return m_entityList[i]->GetModel();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	for (int i = 0; i < sizeof(m_entityList); i++)
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			return m_entityList[i]->GetRigidBody();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	for (int i = 0; i < sizeof(m_entityList); i++)
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			return m_entityList[i]->GetModelMatrix();
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	for (int i = 0; i < sizeof(m_entityList); i++)
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			m_entityList[i]->SetModelMatrix(a_m4ToWorld);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{

}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* newEntity = new MyEntity(a_sFileName, a_sUniqueID);
	m_entityList.push_back(newEntity);
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	m_entityList.erase(m_entityList.begin() + a_uIndex);
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int index = 0;
	for (int i = 0; i < sizeof(m_entityList); i++) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID) {
			index = i;
			break;
		}
	}
	m_entityList.erase(m_entityList.begin() + index);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	for (int i = 0; i < sizeof(m_entityList); i++)
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			m_entityList[i]->AddToRenderList(a_bRigidBody);
}