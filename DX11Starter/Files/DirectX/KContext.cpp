#pragma once
#include "KContext.h"
#include "Vertex.h"
#include <iostream>





// For the DirectX Math library
using namespace NDirectX;
using namespace Graphic;
using namespace Graphic::NScene;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
KContext::KContext(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	
	// Initialize fields
	//vertexBuffer = 0;
	//indexBuffer = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
KContext::~KContext()
{
	/*
	Mesh classes destroy themselves
	*/
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void KContext::Init()
{
	
	m_renderContexts.push_back({ "example00","Created for demo purpose.", Graphic::GraphicMain(), Scene() });
	for (auto it = m_renderContexts.begin(); it != m_renderContexts.end(); it++) {
		if (!it->main.init(this->device, this->context, this->width, this->height)) {
			std::cout << "GraphicMain failed to init" << std::endl;
		}
		it->scene.loadExample00();
	}

	world.objs.push_back(World::Object());
	world.objs.push_back(World::Object());
	world.objs.push_back(World::Object());
	world.objs.push_back(World::Object());

	//LoadShaders();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//m_renderContexts.begin()->main.mesh00 = &*triangle; //TODO you should delete this line
	
	//NImGui::UIMain::example_texture = m_renderContexts.begin()->main.m_renderTextures[Graphic::RENDER_TYPE::DEFFERED_DIFFUSE];
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void KContext::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your KContext here - user input, move objects, AI, etc.
// --------------------------------------------------------
void KContext::Update(float deltaTime, float totalTime)
{
	world.update(deltaTime);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();


	float x, y, speed(.5f* deltaTime), dis_camerMove(1.3f*deltaTime);
	int count = 0;
	
	XMVECTOR dir;
	if (GetAsyncKeyState('W') & 0x8000) {
		testingCamera.setPos(testingCamera.m_pos + (Vector3)(Vector3(0, 0, 1* dis_camerMove)* testingCamera.m_rotation));
		/* Do something useful */
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		testingCamera.setPos(testingCamera.m_pos + (Vector3)(Vector3(0, 0, -1 * dis_camerMove)* testingCamera.m_rotation));
		/* Do something useful */
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		testingCamera.setPos(testingCamera.m_pos + (Vector3)(Vector3(-1 * dis_camerMove, 0, 0 )* testingCamera.m_rotation));
		/* Do something useful */
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		testingCamera.setPos(testingCamera.m_pos + (Vector3)(Vector3(1* dis_camerMove, 0, 0 )* testingCamera.m_rotation));
		/* Do something useful */
	}
}
// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void KContext::Draw(float deltaTime, float totalTime)
{
	for (auto it = m_renderContexts.begin(); it != m_renderContexts.end(); it++) {
		it->main.render(this->device, this->context, it->scene);
	}
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	context->OMSetRenderTargets(1, &this->backBufferRTV, depthStencilView);
	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;


	DirectX::XMFLOAT4X4 worldMatrix_temp;
	int count = 0;
	

	context->OMSetRenderTargets(1,&this-> backBufferRTV, depthStencilView);
	if (m_ui) m_ui->render();
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void KContext::OnMouseDown(WPARAM buttonState, int x, int y)
{

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void KContext::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void KContext::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...


}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void KContext::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}