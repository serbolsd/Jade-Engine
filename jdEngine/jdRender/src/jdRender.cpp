#include "jdRender.h"

namespace jdEngineSDK {
  RenderApi&
  g_Render() {
    return RenderApi::instance();
  }
}
