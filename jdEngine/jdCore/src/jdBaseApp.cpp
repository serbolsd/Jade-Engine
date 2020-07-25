#include "jdBaseApp.h"

namespace jdEngineSDK {
  
  BaseApp::BaseApp()
    : m_wndPosition(-1, -1),
      m_clientSize(640, 480),
      m_wndTitle("jdEngine App"),
    m_fullScreen(false) {
  }

  int32
  BaseApp::run() {
    createWindow();

    initSystems();

    onCreate();

    //Main loop
    float time = 0.0f;
    bool shoudClose = false;
    Event wndEvent;
    while (m_window.isOpen()) {
      while (m_window.pollEvent(wndEvent)) {
        if (Event::Closed == wndEvent.type) {
          m_window.close();
          shoudClose = true;
          break;
        }
        handleWindowEvent(wndEvent);
      }
      if (shoudClose) {
        break;
      }

      update(time);

      render();
    }

    onDestroy();

    destroySystems();

    return 0;
  }

  void 
  BaseApp::onResize(int32 width, int32 height) {
    auto clientSize = m_window.getSize();
    m_clientSize.x = clientSize.x;
    m_clientSize.y = clientSize.y;

    g_graphicsApi().resizeSwapChain(width, height);
  }

  void BaseApp::createWindow() {
    //Agregar windowbase
    if (!m_window.isOpen()) {
      m_window.create(sf::VideoMode(m_clientSize.x, m_clientSize.y),
                       m_wndTitle.c_str(),
                       m_fullScreen ? sf::Style::Fullscreen : sf::Style::Default);
      //m_window.setPosition(sf::Vector2i(m_wndPosition.x, m_wndPosition.y));
      if (-1 != m_wndPosition.x && -1 != m_wndPosition.y)
      {
        m_window.setPosition(sf::Vector2i(m_wndPosition.x, m_wndPosition.y));
      }

      m_windowHasFocus = m_window.hasFocus();
    }
  }

  void
  BaseApp::initSystems() {
    WindowHandle handle = m_window.getSystemHandle();
    HINSTANCE hin;
#ifdef _DEBUG
    hin = LoadLibraryExA("jdDXGraphicApid.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
#else
    hin = LoadLibraryExA("jdDXGraphicApi.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
#endif


    if (!hin)
    {
      return;
    }

    auto createGraphicsApi = reinterpret_cast<graphApi>(GetProcAddress(hin, "createGraphicAPI"));
    if (!createGraphicsApi)
    {
      return;
    }
    GraphicApi::startUp();
    GraphicApi* gsApi = createGraphicsApi();
    g_graphicsApi().setObject(gsApi);

    g_graphicsApi().createDevice();
    g_graphicsApi().createSwapChain(handle, FORMAT::E::FORMAT_R8G8B8A8_UNORM, m_clientSize.x, m_clientSize.y);
    g_graphicsApi().createRenderTargetView(m_clientSize.x, m_clientSize.y);
    
  }

  void 
  BaseApp::destroySystems() {
    g_graphicsApi().shutDown();
  }
  
  void 
  BaseApp::handleWindowEvent(Event& wndEvent) {
    //WindowHandle handle = m_window.getSystemHandle();

    //if (nullptr == ImGui::GetCurrentContext())
    //{
    //  return;
    //}

    switch (wndEvent.type)     
    {
    case wndEvent.Resized:
      onResize(wndEvent.size.width, wndEvent.size.height);
      break;
    case wndEvent.LostFocus:
      m_windowHasFocus = false;
      onLostFocus();
      break;
    case wndEvent.GainedFocus:
      m_windowHasFocus = true;
      onGainedFocus();
      break;
    case wndEvent.TextEntered:
      //io.AddInputCharacterUTF16(static_cast<uint16>(wndEvent.text.unicode));

      onTextEnterd(static_cast<UNICHAR>(wndEvent.text.unicode));
      break;
    case wndEvent.KeyPressed:
      //if (wndEvent.key.code >=0 )
      //{
      //  io.KeysDown[static_cast<int32>(wndEvent.key.code)] = true;
      //}

      onKeyPressed(wndEvent.key.code,
                   wndEvent.key.alt,
                   wndEvent.key.control,
                   wndEvent.key.shift,
                   wndEvent.key.system);
      break;
    case wndEvent.KeyReleased:
      //if (wndEvent.key.code >= 0)
      //{
      //  io.KeysDown[static_cast<int32>(wndEvent.key.code)] = false;
      //}

      onKeyReleased(wndEvent.key.code,
                   wndEvent.key.alt,
                   wndEvent.key.control,
                   wndEvent.key.shift,
                   wndEvent.key.system);
      break;
    case wndEvent.MouseWheelMoved:
      break;
    case wndEvent.MouseWheelScrolled:
      //io.MouseWheel += wndEvent.mouseWheelScroll.delta;

      onMouseWheelScrolled(wndEvent.mouseWheelScroll.wheel,
                           wndEvent.mouseWheelScroll.delta,
                           wndEvent.mouseWheelScroll.x,
                           wndEvent.mouseWheelScroll.y);
      break;
    case wndEvent.MouseButtonPressed:
      //if (!ImGui::IsAnyMouseDown && nullptr == ::GetCapture())
      //{
      //  ::SetCapture(handle);
      //}
      //io.MouseDown[static_cast<int32>(wndEvent.mouseButton.button)] = true;

      onMouseButtonPressed(wndEvent.mouseButton.button, 
                           wndEvent.mouseButton.x, 
                           wndEvent.mouseButton.y);
      break;
    case wndEvent.MouseButtonReleased:
      //if (!ImGui::IsAnyMouseDown && ::GetCapture() == handle)
      //{
      //  ::ReleaseCapture();
      //}
      //io.MouseDown[static_cast<int32>(wndEvent.mouseButton.button)] = false;

      onMouseButtonReleased(wndEvent.mouseButton.button, 
                            wndEvent.mouseButton.x, 
                            wndEvent.mouseButton.y);
      break;
    case wndEvent.MouseMoved:
      onMouseMoved(wndEvent.mouseMove.x, wndEvent.mouseMove.y);
      break;
    case wndEvent.MouseEntered:
      onMouseEntered();
      break;
    case wndEvent.MouseLeft:
      onMouseLeft();
      break;
    case wndEvent.JoystickButtonPressed:
      onJoystickButtonPressed(wndEvent.joystickButton.joystickId,
                              wndEvent.joystickButton.button);
      break;
    case wndEvent.JoystickButtonReleased:
      onJoystickButtonReleased(wndEvent.joystickButton.joystickId,
                               wndEvent.joystickButton.button);
      break;
    case wndEvent.JoystickMoved:
      
      onJoystickMoved(wndEvent.joystickMove.joystickId, 
                      wndEvent.joystickMove.axis, 
                      wndEvent.joystickMove.position);
      break;
    case wndEvent.JoystickConnected:
      onJoystickConnected(wndEvent.joystickConnect.joystickId);
      break;
    case wndEvent.JoystickDisconnected:
      onJoystickDisconnected(wndEvent.joystickConnect.joystickId);
      break;
    case wndEvent.TouchBegan:
      break;
    case wndEvent.TouchMoved:
      break;
    case wndEvent.TouchEnded:
      break;
    case wndEvent.SensorChanged:
      break;
    default:
      break;
    }
    
  }

  void 
  BaseApp::update(const float& deltaTime) {
    onUpdate(deltaTime);
  }

  void 
  BaseApp::render() {
    onRender();
    g_graphicsApi().Present();
  }
}
