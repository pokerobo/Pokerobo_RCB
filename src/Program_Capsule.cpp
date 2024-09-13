#include "Program_Capsule.h"

bool ProgramCapsule::isTypeOf(byte label) {
  return false;
}

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

int ProgramCapsule::_maxTitleLength = 0;

int ProgramCapsule::getMaxTitleLength() {
  return _maxTitleLength;
}

//-------------------------------------------------------------------------------------------------

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

bool ProgramSticker::isTypeOf(byte label) {
  return label == PROGRAM_TYPE_1;
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

//-------------------------------------------------------------------------------------------------

ProgramPagelet::ProgramPagelet(char* titleOrTemplate, uint16_t index,
        uint16_t minIndex, uint16_t maxIndex) {
  _titleTemplate = titleOrTemplate;
  _minIndex = minIndex;
  _maxIndex = maxIndex;
  _currentIndex = index;
  _currentFocus = _currentIndex;
  _maxTitleLength = max(_maxTitleLength, getTitleLength());
}

bool ProgramPagelet::isTypeOf(byte label) {
  return label == PROGRAM_TYPE_2;
}

char* ProgramPagelet::getTitle(char *buffer) {
  sprintf(buffer + strlen(buffer), _titleTemplate, _currentFocus);
  return buffer;
}

int ProgramPagelet::getTitleLength() {
  return strlen(_titleTemplate) + 5 - 4; // "2^16-1 = 65535" / "%04X"
}

bool ProgramPagelet::hasChanged() {
  return _currentFocus != _currentIndex;
}

bool ProgramPagelet::change() {
  bool changed = _currentFocus != _currentIndex;
  onChanged(_currentIndex, _currentFocus);
  _currentIndex = _currentFocus;
  return changed;
}

uint16_t ProgramPagelet::getIndex() {
  return _currentIndex;
}

void ProgramPagelet::next() {
  if (_currentFocus == _maxIndex) {
    _currentFocus = _minIndex;
  } else {
    _currentFocus += 1;
  }
}

void ProgramPagelet::prev() {
  if (_currentFocus == _minIndex) {
    _currentFocus = _maxIndex;
  } else {
    _currentFocus -= 1;
  }
}

void ProgramPagelet::onChanged(uint16_t currentIndex, uint16_t currentFocus) { }
