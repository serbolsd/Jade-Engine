#include "jdGraphicApi.h"

namespace jdEngineSDK {
  GraphicApi& 
  g_graphicsApi() {
    return GraphicApi::instance();
  }
}

