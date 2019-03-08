//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "AssetLoader.h"
#include "ShaderType.h"

extern void ExitGame();

using namespace DirectX;
using namespace RemotingHostSample;
using Microsoft::WRL::ComPtr;



Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
	AssetLoader::AddSearchPath("c:/");
	AssetLoader::AddSearchPath(".");
	AssetLoader::AddSearchPath("C:\\Users\\Administrator\\source\\repos\\Direct3D Win32 Game1\\x64\\Debug");
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.

	DirectX::FXMVECTOR eye{ 0, 0, 1000 };
	DirectX::FXMVECTOR up{ 0, 1, 0 };
	DirectX::FXMVECTOR target{ 0, 0, 0 };

	DirectX::XMStoreFloat4x4(&m_mvpData.modelMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_mvpData.viewMatrix, DirectX::XMMatrixLookAtLH(eye, target, up));

	m_d3dContext->UpdateSubresource(m_modelConstantBuffer.Get(), 0, nullptr, &m_mvpData, 0, 0);

	m_lightData.diffuseColor = DirectX::XMFLOAT4(1, 0, 0, 1);
	m_lightData.lightDirection = DirectX::XMFLOAT3(1, 1, 0);
	m_lightData.padding = 0;

	m_d3dContext->UpdateSubresource(m_lightConstantBuffer.Get(), 0, nullptr, &m_lightData, 0, 0);

}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;

	// set vertex input buffer
	m_d3dContext->IASetVertexBuffers(0, 1, &m_vertexbuffer, &stride, &offset);
	m_d3dContext->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R32_UINT, // Each index is one 32-bit unsigned integer (short).
		0
	);
	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3dContext->IASetInputLayout(m_inputlayout.Get());

	// Attach the vertex shader.
	m_d3dContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
	);

	// Apply the model constant buffer to the vertex shader.
	m_d3dContext->VSSetConstantBuffers(0, 1, m_modelConstantBuffer.GetAddressOf());

	// Attach the pixel shader.
	m_d3dContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
	);

	m_d3dContext->PSSetConstantBuffers(1, 1, m_lightConstantBuffer.GetAddressOf());

	m_d3dContext->DrawIndexed(model.meshes[0].Indices.size(), 0, 0);

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
        ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
            ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.

	DirectX::XMStoreFloat4x4(&m_mvpData.projectionMatrix,
		DirectX::XMMatrixPerspectiveLH(m_outputWidth, m_outputHeight, 0.1, 1000)
	);

	InitGraphics();
	InitPipeline();
}

void Game::InitGraphics()
{
	model.Load("model.stl", false);

	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(Vertex) * model.meshes[0].Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { &model.meshes[0].Vertices[0], 0, 0 };

	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(&bd, &srd, &m_vertexbuffer)
	);

	D3D11_SUBRESOURCE_DATA indexBufferData = { &model.meshes[0].Indices[0], 0 };
	const CD3D11_BUFFER_DESC indexBufferDesc(model.meshes[0].Indices.size(), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
		)
	);

	const CD3D11_BUFFER_DESC mvpBufferDesc(sizeof(MVPConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
			&mvpBufferDesc,
			nullptr,
			&m_modelConstantBuffer
		)
	);

	const CD3D11_BUFFER_DESC lightBufferDesc(sizeof(LightConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
			&lightBufferDesc,
			nullptr,
			&m_lightConstantBuffer
		)
	);
}

void Game::InitPipeline()
{
	Buffer vsbuffer = AssetLoader::LoadBytes("VertexShader.cso");
	Buffer psbuffer = AssetLoader::LoadBytes("PixelShader.cso");

	DX::ThrowIfFailed(
		m_d3dDevice->CreateVertexShader(vsbuffer.Data(), vsbuffer.Size(), nullptr, &m_vertexShader)
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreatePixelShader(psbuffer.Data(), psbuffer.Size(), nullptr, &m_pixelShader)
	);

	m_d3dContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_d3dContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// create and set the input layout
	m_d3dDevice->CreateInputLayout(ied, ARRAYSIZE(ied), vsbuffer.Data(), vsbuffer.Size(), &m_inputlayout);
	m_d3dContext->IASetInputLayout(m_inputlayout.Get());




}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}