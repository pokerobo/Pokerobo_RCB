#include "Program_Capsule.h"

uint8_t ProgramCapsule::getId() {
  return 0;
}

char* ProgramCapsule::getTitle() {
  return NULL;
}

char* ProgramCapsule::getTitle(char* buffer) {
  return buffer;
}

int ProgramCapsule::begin() {
  return 0;
}

int ProgramCapsule::check(void* control, void* command) {
  return 0;
}

int ProgramCapsule::close() {
  return 0;
}

//-------------------------------------------------------------------------------------------------

int ProgramCapsule::_maxTitleLength = 0;

int ProgramCapsule::getMaxTitleLength() {
  return _maxTitleLength;
}

ProgramSticker::ProgramSticker(char* title) {
  _title = title;
  _maxTitleLength = max(_maxTitleLength, strlen(_title));
  initialize();
}

ProgramSticker::ProgramSticker(char* titles[PROGRAM_TITLE_PARTS]) {
  initialize(titles);
  _maxTitleLength = max(_maxTitleLength, getTitleLength());
}

void ProgramSticker::initialize(char* titles[PROGRAM_TITLE_PARTS]) {
  for(byte i=0; i<PROGRAM_TITLE_PARTS; i++) {
    if (titles == NULL) {
      _titles[i] = NULL;
      continue;
    }
    _titles[i] = titles[i];
  }
}

char* ProgramSticker::getTitle() {
  return _title;
}

char* ProgramSticker::getTitle(char *buffer) {
  if (buffer == NULL) {
    if (_title != NULL) {
      return _title;
    }
    if (_titles[0] != NULL) {
      return _titles[0];
    }
  }
  if (_title != NULL) {
    strcat(buffer, _title);
    return buffer;
  }
  for(byte i=0; i<PROGRAM_TITLE_PARTS; i++) {
    if (_titles[i] != NULL) {
      strcat(buffer, _titles[i]);
    }
  }
  return buffer;
}

int ProgramSticker::getTitleLength() {
  int total = 0;
  for(byte i=0; i<PROGRAM_TITLE_PARTS; i++) {
    if (_titles[i] == NULL) {
      continue;
    }
    total += strlen(_titles[i]);
  }
  return total;
}
