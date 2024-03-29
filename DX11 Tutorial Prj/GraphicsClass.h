#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1;

class D3DClass;
class CameraClass;
class TextureShaderClass;
class BitmapClass;
//class ModelClass;
//class LightClass;
//class LightShaderClass;

class GraphicsClass {
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	BitmapClass* m_Bitmap = nullptr;
	//ModelClass* m_Model = nullptr;
	//LightShaderClass* m_LightShader = nullptr;
	//LightClass* m_Light = nullptr;
};