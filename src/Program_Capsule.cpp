#include "Program_Capsule.h"

ProgramSticker::ProgramSticker(char* title) {
  _title = title;
  initialize();
}

ProgramSticker::ProgramSticker(char* titles[]) {
  initialize(titles);
}

void ProgramSticker::initialize(char* titles[]) {
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
    if (_title[0] != NULL) {
      return _title[0];
    }
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
