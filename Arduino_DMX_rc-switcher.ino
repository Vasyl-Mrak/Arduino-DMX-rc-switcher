#include <ShiftDisplay.h>                      // Including necessary libraries
#include <GyverButton.h>
#include <EEPROM.h>
#include <DMXSerial.h>
#include <RCSwitch.h>

#define BUTTON_PLUS 8                          // Buttons for changing the DMX channel
#define BUTTON_MINUS 9
#define BUTTON_ENTER 10
GButton butt1(BUTTON_PLUS);                    // Initializing button objects
GButton butt2(BUTTON_MINUS);
GButton butt3(BUTTON_ENTER);

int x1 = 0;                                    // Variables for display
int x2 = 0;
int x3 = 1;
String x4 = "d001";
int dmx_ch = 1;                                // Initial DMX channel

const int LATCH_PIN = 6;                       // Pins for library operation
const int CLOCK_PIN = 7;
const int DATA_PIN = 5;
const DisplayType DISPLAY_TYPE = COMMON_ANODE; // Display connection type
const int DISPLAY_SIZE = 4;                    // Number of digits on display

ShiftDisplay display(LATCH_PIN, CLOCK_PIN, DATA_PIN, DISPLAY_TYPE, DISPLAY_SIZE);
RCSwitch mySwitch = RCSwitch(); 
unsigned long lastActiveTime = 0;              // Timer variable

void setup() {      
  mySwitch.enableTransmit(11);                 // RC transmitter setup
  mySwitch.setProtocol(1);
  mySwitch.setPulseLength(173); 
  DMXSerial.init(DMXReceiver);                 // DMX receiver initialization
  butt1.setStepTimeout(50);                    // Button settings
  butt2.setStepTimeout(50);
  butt1.setClickTimeout(100);
  butt2.setClickTimeout(100);
  butt3.setTimeout(2000);
  pinMode(BUTTON_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_MINUS, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);
  dmx_ch = EEPROM.read(1) + EEPROM.read(2) + EEPROM.read(3);   // Reading values from memory
}

void loop() {
  unsigned long currentTime = millis();                    // Current time
  unsigned long lastPacket = DMXSerial.noDataSince();      // Last DMX packet
  butt1.tick();                                            // Button press handling
  butt2.tick();
  butt3.tick();
  
  if (butt1.isClick()) {                                  // Check for "+" button press
    lastActiveTime = currentTime;                          // Update last active time
    dmx_ch = dmx_ch + 1;                                   // Increment DMX channel by one
    if (dmx_ch > 512) dmx_ch = 1;                          // Check for maximum value
  }

  if (butt2.isClick()) {                                   // Check for "-" button press
    lastActiveTime = currentTime;
    dmx_ch = dmx_ch - 1;
    if (dmx_ch < 1) dmx_ch = 512;                          // Check for minimum value
  }

  if (butt1.isStep()) {                                    // Check for "+" button hold
    lastActiveTime = currentTime;
    dmx_ch = dmx_ch + 1;
    if (dmx_ch > 512) dmx_ch = 1;
  }

  if (butt2.isStep()) {                                    // Check for "-" button hold
    lastActiveTime = currentTime;
    dmx_ch = dmx_ch - 1;
    if (dmx_ch < 1) dmx_ch = 512;
  }

  if (butt3.isStep()) {                                    // Save DMX channel to memory
    lastActiveTime = currentTime;
    save();
  }

  if (currentTime - lastActiveTime > 10000) {              // Waiting time for sleep mode
    display.clear();
  } else {
    display_dmx();                                         // Display the selected DMX channel
  }
  
  if (DMXSerial.read(dmx_ch) > 127) {                      // Send radio signal depending on DMX level
    mySwitch.sendTriState("FFFFFFFF1000");
    lastActiveTime = currentTime;
  }
    
  if (DMXSerial.read(dmx_ch + 1) > 127) {
    mySwitch.sendTriState("FFFFFFFF0100");
    lastActiveTime = currentTime;
  }
    
  if (DMXSerial.read(dmx_ch + 2) > 127) {
    mySwitch.sendTriState("FFFFFFFF0010");
    lastActiveTime = currentTime;
  }
  
  if (DMXSerial.read(dmx_ch + 3) > 127) {
    mySwitch.sendTriState("FFFFFFFF0001");
    lastActiveTime = currentTime;
  }
}

void display_dmx() {                                       // Display the selected DMX channel
  x1 = (dmx_ch / 10) / 10;
  x2 = (dmx_ch / 10) % 10;
  x3 = dmx_ch - (x1 * 100) - (x2 * 10);
  x4 = "d" + String(x1) + String(x2) + String(x3);
  display.set(x4);
  display.update();
}

void save() {                                               // Save DMX channel to memory
  display.set("SAVE");                                      // Display "SAVE" on the display
  display.show(2000);                                       // Show on display for 2 seconds
  if (dmx_ch <= 255) {
    EEPROM.write(1, dmx_ch);
    EEPROM.write(2, 0);
    EEPROM.write(3, 0);
  }
  if (dmx_ch > 255 && dmx_ch <= 510) {
    EEPROM.write(1, 255);
    EEPROM.write(2, dmx_ch - 255);
    EEPROM.write(3, 0);
  }
  if (dmx_ch > 510) {
    EEPROM.write(1, 255);
    EEPROM.write(2, 255);
    EEPROM.write(3, dmx_ch - 510);
  }
  
  delay(2000);
}
