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
	vertexShader = 0;
	pixelShader = 0;

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
	delete vertexShader;
	delete pixelShader;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void KContext::Init()
{
	//temporary world information 
	CreateMatrices();
	CreateBasicGeometry();
	
	m_renderContexts.push_back({ "example00","Created for demo purpose.", Graphic::GraphicMain(), Scene() });
	for (auto it = m_renderContexts.begin(); it != m_renderContexts.end(); it++) {
		if (!it->main.init(this->device, this->context, this->width, this->height)) {
			std::cout << "GraphicMain failed to init" << std::endl;
		}
		it->scene.loadExample00();
	}

	vertexShader	= *m_renderContexts.begin()->main.m_shadersVert[Graphic::RENDER_TYPE::DEFAULT].get();
	pixelShader		= *m_renderContexts.begin()->main.m_shadersFrag[Graphic::RENDER_TYPE::DEFAULT].get();
	world.objs.push_back(World::Object());
	world.objs.push_back(World::Object());
	world.objs.push_back(World::Object());
	world.objs.push_back(World::Object());

	//LoadShaders();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	renderTexture.Initialize(this->device, width, height);
	//m_renderContexts.begin()->main.mesh00 = &*triangle; //TODO you should delete this line
	
	//NImGui::UIMain::example_texture = m_renderContexts.begin()->main.m_renderTextures[Graphic::RENDER_TYPE::DEFFERED_DIFFUSE];
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void KContext::LoadShaders()
{
	vertexShader = new Graphic::SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new Graphic::SimpleFragmentShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void KContext::CreateMatrices()
{
	// Set up world matrix
	// - In an actual KContext, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//   an identity matrix.  This is just to show that HLSL expects a different
	//   matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual KContext, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void KContext::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	/*
	
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +.5f, +0.0f), red },
		{ XMFLOAT3(+.5f, -.50f, +0.0f), blue },
		{ XMFLOAT3(-.5f, -.50f, +0.0f), green },
	};
	Vertex verticesSquare[] =
	{
		{ XMFLOAT3(-0.5f, +0.5f, +0.0f), red },
		{ XMFLOAT3(+0.5f, +0.5f, +0.0f), blue },
		{ XMFLOAT3(+0.5f, -.5f, +0.0f), green },
		{ XMFLOAT3(-0.5f, -.5f, +0.0f), green },
	};
	Vertex verticesDiamond[] =
	{
		{ XMFLOAT3(-0.2f, +0.0f, +0.0f), red },
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), blue },
		{ XMFLOAT3(+0.2f, 0.0f, +0.0f), green },
		{ XMFLOAT3(-0.0f, -.5f, +0.0f), green },
	};
	*/

	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +.5f, +0.0f) },
		{ XMFLOAT3(+.5f, -.50f, +0.0f) },
		{ XMFLOAT3(-.5f, -.50f, +0.0f) },
	};
	Vertex verticesSquare[] =
	{
		{ XMFLOAT3(-0.5f, +0.5f, +0.0f) },
		{ XMFLOAT3(+0.5f, +0.5f, +0.0f) },
		{ XMFLOAT3(+0.5f, -.5f, +0.0f) },
		{ XMFLOAT3(-0.5f, -.5f, +0.0f) },
	};
	Vertex verticesDiamond[] =
	{
		{ XMFLOAT3(-0.2f, +0.0f, +0.0f) },
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f) },
		{ XMFLOAT3(+0.2f, 0.0f, +0.0f) },
		{ XMFLOAT3(-0.0f, -.5f, +0.0f) },
	};
	int indices[] = { 0, 1, 2 };
	int indicesSquare[] = { 0, 1, 2 ,2,3,0};
	this->triangle = std::unique_ptr<Graphic::Mesh>(new Graphic::Mesh(device, vertices, 3, indices, 3));
	this->square = std::unique_ptr<Graphic::Mesh>(new Graphic::Mesh(device, verticesSquare, 4, indicesSquare, 6));
	this->diamond = std::unique_ptr<Graphic::Mesh>(new Graphic::Mesh(device, verticesDiamond, 4, indicesSquare, 6));


	//Graphic::Mesh mesh01(device, vertices, 3, indices, 3);
	//this->vertexBuffer = triangle->getBufferVertex();
	//this->indexBuffer = triangle->getBufferIndex();

}

void KContext::renderMesh(Graphic::Mesh* mesh)
{
	if (mesh == NULL) return;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
	context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
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
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
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
		it->main.render(this->context, this->depthStencilView, it->scene);
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
	//renderTexture.SetRenderTarget(context, this->depthStencilView);
	//renderTexture.ClearRenderTarget(context, this->depthStencilView, 0, 1, 0, 0);
	/*
	XMVECTOR pos = XMVector4Transform(XMVectorSet(0,0,0,1), XMLoadFloat4x4(&world.cam.pos));
	XMVECTOR dir = XMVector4Transform(XMVectorSet(0, 0, 1, 1) , XMLoadFloat4x4(&world.cam.rotation));
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	*/


	
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.

	//testingCamera.getProjectionMatrix(this->width,this->height,0.1,100)
	

	/*
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	XMFLOAT4X4 dummy;
	XMMATRIX mat_view = testingCamera.getViewMatrix();
	XMMATRIX P_new = testingCamera.getProjectionMatrix(
		width ,height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance

	//XMStoreFloat4x4(&projectionMatrix, P); // Transpose for HLSL!

	
	std::cout << "ViewMatrix\n";
	//projectionMatrix[2] *= -1;
	for (int i = 0; i < 4; i++) {
		std::cout << "[";
		for (int j = 0; j < 4; j++) {
			std::cout << viewMatrix.m[i][j];
			if (j != 3)std::cout << ",";
		}
		std::cout << "]\n";
	}
	
	
	std::cout << "New view\n";

	//mat_view.r[2] *= -1;
	XMStoreFloat4x4(&dummy, XMMatrixTranspose(mat_view)); // Transpose for HLSL!
	for (int i = 0; i < 4; i++) {
		std::cout << "[";
		for (int j = 0; j < 4; j++) {
			std::cout << dummy.m[i][j];
			if (j != 3)std::cout << ",";
		}
		std::cout << "]\n";
	}

	//projectionMatrix[2] *= -1;
	XMStoreFloat4x4(&projectionMatrix, P_new); // Transpose for HLSL!
	
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P_normal)); // Transpose for HLSL!
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P_new)); // Transpose for HLSL!


	P_new.r[2] *= -1;
	*/
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P_new)); // Transpose for HLSL!
	//XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(mat_view)); // Transpose for HLSL!
	vertexShader->SetMatrix4x4("projection", projectionMatrix);


	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(testingCamera.getProjectionMatrix(this->width, this->height, 0.1, 100)));
	//XMStoreFloat4x4(&projectionMatrix, testingCamera.getProjectionMatrix(this->width, this->height, 0.1, 100));
	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER

	DirectX::XMFLOAT4X4 worldMatrix_temp;
	int count = 0;
	for (auto it = world.objs.begin(); it != world.objs.end(); it++) {
		XMStoreFloat4x4(&worldMatrix_temp, XMMatrixTranspose(XMLoadFloat4x4(&it->pos))); // Transpose for HLSL!
		vertexShader->SetMatrix4x4("world", worldMatrix_temp);
		vertexShader->CopyAllBufferData();
		switch (count++) {
		default:
		case 0:
			renderMesh(square.get()); break;
		case 1:
			renderMesh(diamond.get()); break;
		case 2:
			renderMesh(triangle.get()); break;
		}
	}

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
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

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


	float scale = .001f;
	float disX = x - prevMousePos.x, disY = y - prevMousePos.y;
	if (disX*disX + disY*disY < 5000) {

		XMMATRIX rotation = XMLoadFloat4x4(&world.cam.rotation);
		XMStoreFloat4x4(&world.cam.rotation, rotation * XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 1), disX*scale)); // Transpose for HLSL!
		rotation = XMLoadFloat4x4(&world.cam.rotation);
		XMStoreFloat4x4(&world.cam.rotation, rotation * XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 1), disY*scale)); // Transpose for HLSL!
		
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
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