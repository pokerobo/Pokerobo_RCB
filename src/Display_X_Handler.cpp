#include "Display_X_Handler.h"
#include <U8g2lib.h>

void DeviceDisplayHandler::render(DeviceManifest* manifest) {
  U8G2 *_u8g2 = (U8G2*)getU8g2Ref();

  uint8_t _displayWidth = _u8g2->getDisplayWidth();
  uint8_t _displayHeight = _u8g2->getDisplayHeight();
  uint8_t _charHeight = _u8g2->getMaxCharHeight();

  UserInformation _userInformation;
  VendorSignature _vendorSignature;
  DeviceSignature _deviceSignature;

  loadManifest(getDeviceSignatureIndex(), _deviceSignature);
  loadManifest(getVendorSignatureIndex(), _vendorSignature);
  loadManifest(getUserInformationIndex(), _userInformation);

  char* vendorCode;
  if (_vendorSignature.getVendorCodeLength() > 0) {
    vendorCode = _vendorSignature.getVendorCode();
  } else {
    vendorCode = "(unknown vendor)";
  }

  char deviceIdText[15] = "DeviceID: ----"; // 15 ~ 10 + 4 + 1
  if (_deviceSignature.deviceId != 0 && _deviceSignature.deviceId != 0xFFFF) {
    sprintf(deviceIdText + 10, "%04X", _deviceSignature.deviceId);
  }

  char userIdText[13] = "UserID: ----"; // 13 ~ 8 + 4 + 1
  if (_userInformation.userId != 0 && _userInformation.userId != 0xFFFF) {
    sprintf(userIdText + 8, "%04X", _userInformation.userId);
  }

  char usernameText[17] = "Username: "; // 17 ~ 10 + 16 + 1
  strcat(usernameText, _userInformation.getUserNameLength() > 0 ?
      _userInformation.getUserName() : "(unknown)");

  uint8_t _beginTextBlock = _displayHeight/2 - 12;
  uint8_t _titleCharWidth = 6; // u8g2_font_6x13_tf
  uint8_t _textCharWidth = 5; // u8g2_font_5x8_tf
  uint8_t _paddingChars = 2;

  _u8g2->firstPage();
  do {
    _u8g2->setFont(u8g2_font_6x13_tf);
    _u8g2->drawStr((_displayWidth - strlen(vendorCode)*_titleCharWidth)/2,
        _beginTextBlock, vendorCode);

    _u8g2->setFont(u8g2_font_5x8_tf);
    _u8g2->drawStr(_displayWidth/2 - _textCharWidth*(_paddingChars + 9),
        _beginTextBlock + 1*(_charHeight + 2), deviceIdText);
    _u8g2->drawStr(_displayWidth/2 - _textCharWidth*(_paddingChars + 7),
        _beginTextBlock + 2*(_charHeight + 2), userIdText);
    _u8g2->drawStr(_displayWidth/2 - _textCharWidth*(_paddingChars + 9),
        _beginTextBlock + 3*(_charHeight + 2), usernameText);
  } while (_u8g2->nextPage());
}
