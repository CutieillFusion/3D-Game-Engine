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
	updateCamera();//This must happen first because transformation depends on camera matrix
	updateModel();
	updateSkyBox();
}

void AppWindow::updateModel()
{
	constant cc;

	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_light_rot_y);

	m_light_rot_y += 0.707f * m_delta_time;

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = m_light_rot_matrix.getZDirection();

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * m_camera_speed) + world_cam.getXDirection() * (m_rightward * m_camera_speed);

	world_cam.setTranslation(new_pos);


	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void AppWindow::updateSkyBox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_sky_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& tex)
{
	//Sets Constant Buffers for Vertex and Pixel Shaders
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps,cb);

	//Sets the Shaders
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	//Sets the Txture of the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, tex);

	//Sets the Vertexs of the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//Sets the Indices of the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	//Draws the Mesh
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\3D-Game-Engine\\Assets\\Textures\\brick.png");
	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\3D-Game-Engine\\Assets\\Textures\\sky.jpg");

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\3D-Game-Engine\\Assets\\Meshes\\suzanne.obj");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\3D-Game-Engine\\Assets\\Meshes\\sphere.obj");

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

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyBoxShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_sky_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	m_sky_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
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

	//Update Transform Matrices
	update();

	//Render Model
	GraphicsEngine::get()->getRenderSystem()->setRasterizeState(false);
	drawMesh(m_mesh, m_vs, m_ps, m_cb, m_wood_tex);
	//Draws Skybox
	GraphicsEngine::get()->getRenderSystem()->setRasterizeState(true);
	drawMesh(m_sky_mesh, m_vs, m_sky_ps, m_sky_cb, m_sky_tex);

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