#ifndef __POKEROBO_RCB_DISPLAY_X_HANDLER_H__
#define __POKEROBO_RCB_DISPLAY_X_HANDLER_H__

#include "Display_Handler.h"
#include <Pokerobo_ABC.h>
#include <Pokerobo_Dev.h>

class DeviceDisplayHandler: public DisplayHandler {
  public:
    void render(DeviceManifest *manifest);
};

#endif
