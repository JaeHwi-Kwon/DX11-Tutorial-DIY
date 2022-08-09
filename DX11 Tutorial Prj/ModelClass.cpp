#include "Stdafx.h"
#include "TextureClass.h"
#include "ModelClass.h"

#include <fstream>
using namespace std;

ModelClass::ModelClass() {}


ModelClass::ModelClass(const ModelClass& other) {}


ModelClass::~ModelClass() {}


bool ModelClass::Intialize(ID3D11Device* device,char* modelFilename, WCHAR*textureFilename) {
	if (!LoadModel(modelFilename)) {
		return false;
	}

	if (!InitializeBuffers(device)) { 
		return false; 
	}
	return LoadTexture(device, textureFilename);
}

void ModelClass::Shutdown() {
	ReleaseTexture();

	ShutdownBuffers();

	ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount() {
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture() {
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device) {

	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}

	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices) {
		return false;
	}

	// 정점을 시계방향으로 배치해줘야 그림이 그려지는 거 중요.
	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer))) {
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer))) {
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
	
	return true;
}

void ModelClass::ShutdownBuffers() {
	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename) {
	m_Texture = new TextureClass;
	if (!m_Texture) {
		return false;
	}

	return m_Texture->Initialize(device, filename);
}

void ModelClass::ReleaseTexture() {
	if (m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

bool ModelClass::LoadModel(char* filename) {
	ifstream fin;
	fin.open(filename);

	if (fin.fail()) {
		return false;
	}

	char input = 0;
	fin.get(input);
	while (input != ':') {
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];
	if (!m_model) {
		return false;
	}

	fin.get(input);
	while (input != ':') {
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (int i = 0; i < m_vertexCount; i++) {
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel() {
	if (m_model) {
		delete[] m_model;
		m_model = 0;
	}
}