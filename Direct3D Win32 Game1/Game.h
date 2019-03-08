//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "Model.h"
#include "ShaderType.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game() noexcept;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

	void InitGraphics();
	void InitPipeline();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_vertexbuffer;              // the vertex buffer interface
	Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_modelConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_lightConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_inputlayout;          // the input layout interface

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	RemotingHostSample::Model model;
	MVPConstantBuffer m_mvpData;
	LightConstantBuffer m_lightData;
};