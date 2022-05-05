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
	Vector4D m_light_position = Vector4D(0, 1, 0, 0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
};

//Current Stuck @ 27:59 Ep 24 
AppWindow::AppWindow()
{
}

void AppWindow::render()
{
	//Clears the Render Target 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	//Sets the Viewport of the Render Target
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//Update Transform Matrices
	update();

	//Render Model
	drawMesh(m_mesh, m_mat);

	//Draws Skybox
	drawMesh(m_sky_mesh, m_sky_mat);

	m_swap_chain->present(true);

	//Sets the Delta Times
	m_old_delta = m_new_delta;
	m_new_delta = (long)::GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_time += m_delta_time;
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

	m_light_rot_y += (0.707f / 2) * m_delta_time;

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	float dist_from_origin = 1.0f;

	cc.m_light_position = Vector4D(cos(m_light_rot_y * 4) * dist_from_origin, 1.0f, sin(m_light_rot_y * 4), 1.0f);

	cc.m_light_radius = m_light_radius;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;

	m_mat->setData(&cc, sizeof(constant));
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

	m_sky_mat->setData(&cc, sizeof(constant));
}

void AppWindow::drawMesh(const MeshPtr& mesh, const MaterialPtr& material)
{
	GraphicsEngine::get()->setMaterial(material);
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

	m_play_state = true;
	InputSystem::get()->showCursor(false);

	m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\3D-Game-Engine\\Assets\\Textures\\wall.jpg");


	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\3D-Game-Engine\\Assets\\Textures\\stars_map.jpg");
	
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\3D-Game-Engine\\Assets\\Meshes\\scene.obj");
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\3D-Game-Engine\\Assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));


	m_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	std::cout << "1";
	m_mat->addTexture(m_wall_tex);
	std::cout << "2";
	m_mat->setCullMode(CULL_MODE_BACK);
	std::cout << "3";



	m_sky_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxShader.hlsl");
	m_sky_mat->addTexture(m_sky_tex);
	m_mat->setCullMode(CULL_MODE_FRONT);
}

void AppWindow::onUpdate()
{
	//Updates the Window
	Window::onUpdate();

	//Updates the Input
	InputSystem::get()->update();

	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
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

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
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
	else if (key == 'O')
	{
		//Decrease Radius
		m_light_radius -= 1.0f * m_delta_time;
	}
	else if (key == 'P')
	{
		//Increase Radius
		m_light_radius += 1.0f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{
	//Resets Direction
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G') 
	{
		m_play_state = m_play_state ? false : true;
		InputSystem::get()->showCursor(!m_play_state);
	}
	else if (key == 'F')
	{
		m_fullscreen_state = m_fullscreen_state ? false : true;
		RECT size_screen = this->getSizeScreen();

		m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) 
	{
		return;
	}

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