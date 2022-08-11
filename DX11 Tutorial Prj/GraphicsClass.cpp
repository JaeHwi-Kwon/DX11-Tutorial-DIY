#include "Stdafx.h"
#include "D3DClass.h"
#include "CameraClass.h"
//#include "ModelClass.h"
//#include "LightClass.h"
//#include "LightShaderClass.h"
#include "TextureShaderClass.h"
#include "BitmapClass.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass() {}


GraphicsClass::GraphicsClass(const GraphicsClass& other) {}


GraphicsClass::~GraphicsClass() {}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {

	m_Direct3D = new D3DClass;
	if (!m_Direct3D) {
		return false;
	}

	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
		FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera) {
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) {
		return false;
	}

	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_Bitmap = new BitmapClass;
	if (!m_Bitmap) {
		return false;
	}

	if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../DX11 Tutorial Prj/data/seafloor.dds",
		400, 400)) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	//m_Model = new ModelClass;
	//if (!m_Model) {
	//	return false;
	//}

	//if (!m_Model->Intialize(m_Direct3D->GetDevice(), "../DX11 Tutorial Prj/data/sphere.txt",
	//	L"../DX11 Tutorial Prj/data/texture.dds")) {
	//	MessageBox(hwnd, L"Could not Initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}

	//m_LightShader = new LightShaderClass;
	//if (!m_LightShader) {
	//	return false;
	//}

	//if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
	//	MessageBox(hwnd, L"Could Not Initialize the color shader object.", L"Error", MB_OK);
	//	return false;
	//}

	//m_Light = new LightClass;
	//if (!m_Light) {
	//	return false;
	//}

	//m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	//m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 0.5f);
	//m_Light->SetSpecularPower(4.0f);

	return true;
}

void GraphicsClass::Shutdown() {

	if (m_Bitmap) {
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	//if (m_Light) {
	//	delete m_Light;
	//	m_Light = 0;
	//}

	//if (m_LightShader) {
	//	m_LightShader->Shutdown();
	//	delete m_LightShader;
	//	m_LightShader = 0;
	//}

	//if (m_Model) {
	//	m_Model->Shutdown();
	//	delete m_Model;
	//	m_Model = 0;
	//}

	if (m_Camera) {
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Frame() {
	static float rotation = 0.0f;

	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
		rotation -= 360.0f;

	return Render(rotation);
}

bool GraphicsClass::Render(float rotation) {
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//worldMatrix = XMMatrixRotationY(rotation);

	//m_Model->Render(m_Direct3D->GetDeviceContext());

	//if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(),
	//	m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
	//	m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
	//	m_Light->GetSpecularColor(),m_Light->GetSpecularPower())) {
	//	return false;
	//}

	m_Direct3D->TurnZBufferOff();

	if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 300, 300)) {
		return false;
	}

	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix,
		orthoMatrix, m_Bitmap->GetTexture())) {
		return false;
	}

	m_Direct3D->TurnZBufferOn();

	m_Direct3D->EndScene();
	return true;
}