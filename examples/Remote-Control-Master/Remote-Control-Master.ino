#include <Pokerobo_RCB.h>

const uint8_t address = 1;

JoystickHandler joystickHandler;
MovingDisplayHandler displayHandler;
MovingMessageSerializer messageSerializer;
MovingCommandResolver commandResolver(true);
MovingCommandPacket commandBuffer;

RF24Tranceiver rf24Tranceiver(&displayHandler, &messageSerializer);
ProgramSelector programSelector(&displayHandler, &joystickHandler);

void setup() {
  Serial.begin(57600);

  joystickHandler.begin();
  displayHandler.begin();

  rf24Tranceiver.begin(RF24_TX, address);

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

  //---------------------------------------------------------------------------
  #ifdef __SAMPLE_PROGRAM_DEVICE_INFORMATION__
  titleDef[0] = "Device", titleDef[1] = " ", titleDef[2] = "Information", titleDef[3] = NULL;
  programSelector.add(new ProgramDeviceInfo(titleDef, &displayHandler));
  #endif

  programSelector.begin();
}

void loop() {
  programSelector.check();
}
