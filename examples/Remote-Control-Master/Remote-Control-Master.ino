#include <Pokerobo_RCB_master.h>

const uint8_t address = 1;

MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
MovingCommandResolver commandResolver(true);
MovingCommandPacket commandBuffer;

RF24Tranceiver rf24Tranceiver;
JoystickHandler joystickHandler;
ProgramSelector programSelector;

void setup() {
  Serial.begin(57600);

  displayHandler.begin();

  rf24Tranceiver.set(&displayHandler);
  rf24Tranceiver.set(&messageSerializer);
  rf24Tranceiver.begin(RF24_TX, address);

  joystickHandler.begin();

  programSelector.set(&displayHandler);
  programSelector.set(&joystickHandler);

  const char* titleDef[PROGRAM_TITLE_PARTS] = { "Car RC ", "Dashboard", NULL, NULL };

  programSelector.add(new ProgramTransmitter(titleDef,
      &commandResolver, &displayHandler, &rf24Tranceiver, address));

  //---------------------------------------------------------------------------
  titleDef[1] = "RX:", titleDef[2] = "default", titleDef[3] = NULL;
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 0));

  //---------------------------------------------------------------------------
  titleDef[1] = "RX:", titleDef[2] = "185809", titleDef[3] = NULL;

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "01";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 1));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "02";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 2));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "03";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 3));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "04";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 4));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "05";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 5));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "06";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 6));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "07";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 7));
  
  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "08";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 8));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "09";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 9));

  /*titleDef[1] = "RX:", titleDef[2] = "185809", */titleDef[3] = "10";
  programSelector.add(new ProgramReceiver(titleDef, &rf24Tranceiver, 10));

  #ifdef __EXTENTION__
  #endif

  //---------------------------------------------------------------------------
  titleDef[1] = "TX:", titleDef[2] = "default", titleDef[3] = NULL;
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 0));

  //---------------------------------------------------------------------------
  titleDef[1] = "TX:", titleDef[2] = "185809", titleDef[3] = NULL;

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "07";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 7));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "08";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 8));

  #ifdef __SAMPLE_PROGRAM_TRANSMITTERS__
  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "01";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 1));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "02";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 2));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "03";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 3));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "04";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 4));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "05";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 5));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "06";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 6));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "07";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 7));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "08";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 8));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "09";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 9));

  /*titleDef[1] = "TX:", titleDef[2] = "185809", */titleDef[3] = "10";
  programSelector.add(new ProgramTransmitter(titleDef,
      &commandBuffer, &commandResolver, &displayHandler, &rf24Tranceiver, 10));
  #endif

  programSelector.begin(PROGRAM_MODE_TESTER);
}

void loop() {
  programSelector.check();
}
