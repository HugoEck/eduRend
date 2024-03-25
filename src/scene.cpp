
#include "Scene.h"
#include "QuadModel.h"
#include "C:\Users\hugo\OneDrive\Desktop\Datorgrafik\eduRend\Cube.h"
#include "OBJModel.h"

Scene::Scene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	m_dxdevice(dxdevice),
	m_dxdevice_context(dxdevice_context),
	m_window_width(window_width),
	m_window_height(window_height)
{ }

void Scene::OnWindowResized(
	int new_width,
	int new_height)
{
	m_window_width = new_width;
	m_window_height = new_height;
}

OurTestScene::OurTestScene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	Scene(dxdevice, dxdevice_context, window_width, window_height)
{ 
	InitTransformationBuffer();
	InitLightCameraBuffer();
	// + init other CBuffers
}

//
// Called once at initialization
//
void OurTestScene::Init()
{
	m_camera = new Camera(
		45.0f * fTO_RAD,		// field-of-view (radians)
		(float)m_window_width / m_window_height,	// aspect ratio
		1.0f,					// z-near plane (everything closer will be clipped/removed)
		500.0f);				// z-far plane (everything further will be clipped/removed)

	// Move camera to (0,0,5)
	m_camera->MoveTo({ 0, 0, 5 });

	// Create objects
	m_quad = new QuadModel(m_dxdevice, m_dxdevice_context);
	m_cube = new Cube(m_dxdevice, m_dxdevice_context);
	m_sponza = new OBJModel("assets/crytek-sponza/sponza.obj", m_dxdevice, m_dxdevice_context);
	m_sphere1 = new OBJModel("assets/sphere/sphere.obj", m_dxdevice, m_dxdevice_context);
	m_sphere2 = new OBJModel("assets/sphere/sphere.obj", m_dxdevice, m_dxdevice_context);
	m_sphere3 = new OBJModel("assets/sphere/sphere.obj", m_dxdevice, m_dxdevice_context);

	// Initialize cube map filenames
	const char* cube_filenames[6] = {
		"cubemaps/brightday/posx.png",
		"cubemaps/brightday/negx.png",
		"cubemaps/brightday/posy.png",
		"cubemaps/brightday/negy.png",
		"cubemaps/brightday/posz.png",
		"cubemaps/brightday/negz.png"
	};

	// Load cube map texture
	HRESULT hr = LoadCubeTextureFromFile(m_dxdevice, cube_filenames, &cube_texture);
	if (SUCCEEDED(hr))
		std::cout << "Cubemap loaded successfully" << std::endl;
	else
		std::cout << "Cubemap failed to load" << std::endl;

	InitializeSamplerState(
		D3D11_FILTER_MIN_MAG_MIP_LINEAR, // Filter type
		D3D11_TEXTURE_ADDRESS_WRAP,     // Address mode for U coordinate
		D3D11_TEXTURE_ADDRESS_MIRROR,     // Address mode for V coordinate
		D3D11_TEXTURE_ADDRESS_CLAMP,      // Address mode for W coordinate
		16                                // Anisotropy level
	);

	InitializeCubeMapSamplerState(
		D3D11_FILTER_MIN_MAG_MIP_LINEAR, // Filter type
		D3D11_TEXTURE_ADDRESS_CLAMP,     // Address mode for U coordinate
		D3D11_TEXTURE_ADDRESS_CLAMP,     // Address mode for V coordinate
		D3D11_TEXTURE_ADDRESS_CLAMP,      // Address mode for W coordinate
		16                                // Anisotropy level
	);
}

//
// Called every frame
// dt (seconds) is time elapsed since the previous frame
//
void OurTestScene::Update(
	float dt,
	const InputHandler& input_handler)
{
	// Basic camera control
	if (input_handler.IsKeyPressed(Keys::Up) || input_handler.IsKeyPressed(Keys::W))
		m_camera->Move({ 0.0f, 0.0f, -m_camera_velocity * dt });
	if (input_handler.IsKeyPressed(Keys::Down) || input_handler.IsKeyPressed(Keys::S))
		m_camera->Move({ 0.0f, 0.0f, m_camera_velocity * dt });
	if (input_handler.IsKeyPressed(Keys::Right) || input_handler.IsKeyPressed(Keys::D))
		m_camera->Move({ m_camera_velocity * dt, 0.0f, 0.0f });
	if (input_handler.IsKeyPressed(Keys::Left) || input_handler.IsKeyPressed(Keys::A))
		m_camera->Move({ -m_camera_velocity * dt, 0.0f, 0.0f });

	//Camera rotation when space is pressed
	if (input_handler.IsKeyPressed(Keys::Space))
		m_camera->RotateWithMouse(input_handler, 0.2f, dt);

	// Now set/update object transformations
	// This can be done using any sequence of transformation matrices,
	// but the T*R*S order is most common; i.e. scale, then rotate, and then translate.
	// If no transformation is desired, an identity matrix can be obtained 
	// via e.g. Mquad = linalg::mat4f_identity; 

	// Quad model-to-world transformation
	m_quad_transform = mat4f::translation(0, 0, 0) *			// No translation
		mat4f::rotation(-m_angle, 0.0f, 1.0f, 0.0f) *	// Rotate continuously around the y-axis
		mat4f::scaling(1.5, 1.5, 1.5);				// Scale uniformly to 150%

	// Sponza model-to-world transformation
	m_sponza_transform = mat4f::translation(0, -5, 0) *		 // Move down 5 units
		mat4f::rotation(fPI / 2, 0.0f, 1.0f, 0.0f) * // Rotate pi/2 radians (90 degrees) around y
		mat4f::scaling(1.0f);						 // The scene is quite large so scale it down to 5%

	m_cube_transform = mat4f::translation(0, 0, 0) *			// No translation
		mat4f::rotation(0.0f, 0.0f, 0.0f, 0.0f) *	// Rotate continuously around the y-axis
		mat4f::scaling(300.0f, 300.0f, 300.0f);

	//Sphere hierarchy 1 to 3. Another rotation added for orbit on 2 and 3.
	m_sphere1_transform = mat4f::translation(3, 2, 0) *
		mat4f::rotation(-m_angle, 0.0f, 1.0f, 0.0f) *
		mat4f::scaling(1.0, 1.0, 1.0);

	m_sphere2_transform = mat4f::translation(3, 0, 0) * 
		mat4f::rotation(-m_angle, 0.0f, 1.0f, 0.0f) * 
		mat4f::rotation(0.0f, 0.0f, 0.0f, 0.0f) * 
		mat4f::scaling(0.5, 0.5, 0.5);

	m_sphere3_transform = mat4f::translation(4, 0, 0) * 
		mat4f::rotation(-m_angle, 0.0f, 1.0f, 0.0f) * 
		mat4f::rotation(0.0f, 0.0f, 0.0f, 0.0f) * 
		mat4f::scaling(0.25, 0.25, 0.25);

	// Increment the rotation angle.
	m_angle += m_angular_velocity * dt;

	// Print fps
	m_fps_cooldown -= dt;
	if (m_fps_cooldown < 0.0)
	{
		std::cout << "fps " << (int)(1.0f / dt) << std::endl;
//		printf("fps %i\n", (int)(1.0f / dt));
		m_fps_cooldown = 2.0;
	}
}

//
// Called every frame, after update
//
void OurTestScene::Render()
{
	// Bind transformation_buffer to slot b0 of the VS
	m_dxdevice_context->VSSetConstantBuffers(0, 1, &m_transformation_buffer);

	// Bind the light and camera position buffer to the pixel shader
	m_dxdevice_context->PSSetConstantBuffers(0, 1, &m_light_camera_buffer);

	// Bind the sampler state to the pixel shader
	m_dxdevice_context->PSSetSamplers(0, 1, &m_samplerState);

	m_dxdevice_context->PSSetSamplers(1, 1, &m_cubeMapSamplerState);

	// Set cube map texture in pixel shader
	const unsigned cube_slot = 2; // Choose a suitable slot for the cube map texture
	m_dxdevice_context->PSSetShaderResources(cube_slot, 1, &cube_texture.TextureView);

	// Obtain the matrices needed for rendering from the camera
	m_view_matrix = m_camera->WorldToViewMatrix();
	m_projection_matrix = m_camera->ProjectionMatrix();

	// Load matrices + the Quad's transformation to the device and render it

	// Update and render sphere1
	UpdateTransformationBuffer(m_sphere1_transform, m_view_matrix, m_projection_matrix);
	m_sphere1->Render();

	// Update and render sphere2
	UpdateTransformationBuffer(m_sphere1_transform * m_sphere2_transform, m_view_matrix, m_projection_matrix);
	m_sphere2->Render();

	// Update and render sphere3
	UpdateTransformationBuffer(m_sphere1_transform * m_sphere2_transform * m_sphere3_transform, m_view_matrix, m_projection_matrix);
	m_sphere3->Render();

	// Load matrices + Sponza's transformation to the device and render it
	UpdateTransformationBuffer(m_sponza_transform, m_view_matrix, m_projection_matrix);
	m_sponza->Render();

	UpdateTransformationBuffer(m_cube_transform, m_view_matrix, m_projection_matrix);
	m_cube->Render();
	UpdateLightCameraBuffer();

}

void OurTestScene::Release()
{
	SAFE_DELETE(m_quad);
	SAFE_DELETE(m_cube);
	SAFE_DELETE(m_sphere1); 
	SAFE_DELETE(m_sphere2); 
	SAFE_DELETE(m_sphere3);
	SAFE_DELETE(m_sponza);
	SAFE_DELETE(m_camera);

	SAFE_RELEASE(m_transformation_buffer);
	SAFE_RELEASE(m_light_camera_buffer);
	SAFE_RELEASE(m_samplerState);
	SAFE_RELEASE(m_cubeMapSamplerState);
	// + release other CBuffers
}

void OurTestScene::OnWindowResized(
	int new_width,
	int new_height)
{
	if (m_camera)
		m_camera->SetAspect(float(new_width) / new_height);

	Scene::OnWindowResized(new_width, new_height);
}

void OurTestScene::InitTransformationBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC matrixBufferDesc = { 0 };
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(TransformationBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	ASSERT(hr = m_dxdevice->CreateBuffer(&matrixBufferDesc, nullptr, &m_transformation_buffer));
}

void OurTestScene::UpdateTransformationBuffer(
	mat4f ModelToWorldMatrix,
	mat4f WorldToViewMatrix,
	mat4f ProjectionMatrix)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_transformation_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	TransformationBuffer* matrixBuffer = (TransformationBuffer*)resource.pData;
	matrixBuffer->ModelToWorldMatrix = ModelToWorldMatrix;
	matrixBuffer->WorldToViewMatrix = WorldToViewMatrix;
	matrixBuffer->ProjectionMatrix = ProjectionMatrix;
	m_dxdevice_context->Unmap(m_transformation_buffer, 0);
}

void OurTestScene::InitLightCameraBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC lightCameraBufferDesc = {};
	lightCameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightCameraBufferDesc.ByteWidth = sizeof(LightCameraBuffer);
	lightCameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightCameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightCameraBufferDesc.MiscFlags = 0;
	lightCameraBufferDesc.StructureByteStride = 0;
	ASSERT(hr = m_dxdevice->CreateBuffer(&lightCameraBufferDesc, nullptr, &m_light_camera_buffer));
}

void OurTestScene::UpdateLightCameraBuffer()
{
	// Map the resource buffer, obtain a pointer and then write the camera and light positions to it
	D3D11_MAPPED_SUBRESOURCE resource;
	m_dxdevice_context->Map(m_light_camera_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	LightCameraBuffer* bufferData = (LightCameraBuffer*)resource.pData;
	bufferData->cameraPosition = vec4f(m_camera->GetPosition(), 1.0f);
	// Set light position accordingly
	bufferData->lightPosition = vec4f(0.0f, 20.0f, 0.0f, 1.0f);
	m_dxdevice_context->Unmap(m_light_camera_buffer, 0);
}

void OurTestScene::InitializeSamplerState(
	D3D11_FILTER filter,
	D3D11_TEXTURE_ADDRESS_MODE addressU,
	D3D11_TEXTURE_ADDRESS_MODE addressV,
	D3D11_TEXTURE_ADDRESS_MODE addressW,
	UINT maxAnisotropy)
{
	HRESULT hr;

	// Sampler state description
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = addressU;
	samplerDesc.AddressV = addressV;
	samplerDesc.AddressW = addressW;
	samplerDesc.MaxAnisotropy = maxAnisotropy;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; // Default comparison function
	samplerDesc.MinLOD = 0; // Default minimum level-of-detail
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // Default maximum level-of-detail

	// Create the sampler state
	ASSERT(hr = m_dxdevice->CreateSamplerState(&samplerDesc, &m_samplerState));
}

void OurTestScene::InitializeCubeMapSamplerState(
	D3D11_FILTER filter,
	D3D11_TEXTURE_ADDRESS_MODE addressU,
	D3D11_TEXTURE_ADDRESS_MODE addressV,
	D3D11_TEXTURE_ADDRESS_MODE addressW,
	UINT maxAnisotropy)
{
	HRESULT hr;

	// Sampler state description
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = addressU;
	samplerDesc.AddressV = addressV;
	samplerDesc.AddressW = addressW;
	samplerDesc.MaxAnisotropy = maxAnisotropy;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; // Default comparison function
	samplerDesc.MinLOD = 0; // Default minimum level-of-detail
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // Default maximum level-of-detail

	// Create the sampler state
	ASSERT(hr = m_dxdevice->CreateSamplerState(&samplerDesc, &m_cubeMapSamplerState));
}