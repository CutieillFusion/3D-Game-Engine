#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
};


AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	//Updating Time
	constant cc;
	//cc.m_time = (long)::GetTickCount64();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	Matrix4x4 temp;
	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_light_rot_y);

	m_light_rot_y += 0.707f / 4 * m_delta_time;

	cc.m_light_direction = m_light_rot_matrix.getZDirection();

	m_delta_scale += m_delta_time / 0.55f;

	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * m_camera_speed) + world_cam.getXDirection() * (m_rightward * m_camera_speed);

	world_cam.setTranslation(new_pos);

	cc.m_camera_position = new_pos;

	m_world_cam = world_cam;

	world_cam.inverse();

	cc.m_view = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\3D-Game-Engine\\Assets\\Textures\\asteroid.jpg");

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\3D-Game-Engine\\Assets\\Meshes\\statue.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	//cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	//Updates the Window
	Window::onUpdate();

	//Updates the Input
	InputSystem::get()->update();

	//Clears the Render Target 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	//Sets the Viewport of the Render Target
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//Update Camera Movement
	update();

	//Sets Constant Buffers for Vertex and Pixel Shaders
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//Sets the Shaders
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//Sets the Txture of the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_tex);

	//Sets the Vertexs of the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());
	//Sets the Indices of the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

	//Draws the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
	m_swap_chain->present(true);

	//Sets the Delta Times
	m_old_delta = m_new_delta;
	m_new_delta = (long)::GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	//Starts Accepting Input
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	//Stops Accepting Input
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	//Should be a Switch Statement
	if (key == 'W')
	{
		//Forward
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//Backward
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//Left
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//Right
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	//Resets Direction
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	//Rotates the Camera by the Delta Mouse Position
	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	//Sets the cursor to the Middle of the Window
	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_camera_speed = 0.04f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_camera_speed = 0.02f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_camera_speed = 0.01f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_camera_speed = 0.02f;
}