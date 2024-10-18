// in IRTIMER.hpp aggiunto il riferiemento sulla riga del 816 anche per 814


#include <Arduino.h>
#include "global.h"
#include "PinDefinitionsAndMore.h" // Set IR_RECEIVE_PIN for different CPU's

#define NO_LED_FEEDBACK_CODE // saves 92 bytes program memory
// #define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes program memory.
#define EXCLUDE_EXOTIC_PROTOCOLS // saves around 650 bytes program memory if all other protocols are active
#define IR_REMOTE_DISABLE_RECEIVE_COMPLETE_CALLBACK // saves 32 bytes program memory

//#define DECODE_DENON        // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
//#define DECODE_ONKYO        // Decodes only Onkyo and not NEC or Apple
//#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6
//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER
//#define DECODE_FAST
//#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
//#define DECODE_HASH         // special decoder for all protocols
//#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

// #if !defined(RAW_BUFFER_LENGTH)
// #define RAW_BUFFER_LENGTH  200 // this allows usage of 16 bit raw buffer, for RECORD_GAP_MICROS > 20000
// #  endif


#include "IRremote.hpp"

void generateTone();
void handleOverflow();
bool detectLongPress(uint16_t aLongPressDurationMillis);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("START " __FILE__ " from " __DATE__ " " __TIME__ "\r\nUsing library version " VERSION_TINYIR);
  delay(100);
  // In case the interrupt driver crashes on setup, give a clueto the user what's going on.
  Serial.println("Enabling IRin...");

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  // IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);

  Serial.print("Ready protocols: ");
  printActiveIRProtocols(&Serial);
  Serial.println("at pin " STR(IR_RECEIVE_PIN));
  }

void loop() {
  if (IrReceiver.decode()) {
    Serial.println();
    // For less than 16k flash, only print a minimal summary of received data
    IrReceiver.printIRResultMinimal(&Serial);
    // Enable receiving of the next value after decoding and printing the current received data packet
    IrReceiver.resume();

    /*
     * Finally check the received data and perform actions according to the received address and commands
     */
     // if (IrReceiver.decodedIRData.address == 0) {
     //   if (IrReceiver.decodedIRData.command == 0x10) {
     //     // do something
     //     } else if (IrReceiver.decodedIRData.command == 0x11) {
     //       // do something else
     //       }
     //   }

     // Check if repeats of the IR command was sent for more than 1000 ms
    if (detectLongPress(1000)) {
      Serial.print(F("Command 0x"));
      Serial.print(IrReceiver.decodedIRData.command, HEX);
      Serial.println(F(" was repeated for more than 2 seconds"));
      }
    } // if (IrReceiver.decode())
// Your code here

  }  //loop

// BEEP !
void generateTone() {
  // IrReceiver.stopTimer(); // Stop timer consistently before calling tone() or other functions using the timer resource.
  // tone(TONE_PIN, 2200, 8);
  // delay(8);
  // IrReceiver.restartTimer(); // Restart IR timer after timer resource is no longer blocked.
  }

void handleOverflow() {
  Serial.println("Overflow detected");
  Serial.println("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__);
  // see also https://github.com/Arduino-IRremote/Arduino-IRremote#compile-options--macros-for-this-library
  IrReceiver.stopTimer();
  // tone(TONE_PIN, 1100, 10);
  // delay(50);
  // tone(TONE_PIN, 1100, 10);
  delay(50);
  IrReceiver.restartTimer();
  }

// rileva pressioni prolungate
unsigned long sMillisOfFirstReceive;
bool sLongPressJustDetected;
bool detectLongPress(uint16_t aLongPressDurationMillis) {
  if (!sLongPressJustDetected && (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
    //  Here the repeat flag is set (which implies, that command is the same as the previous one)
    if (millis() - aLongPressDurationMillis > sMillisOfFirstReceive) {
      sLongPressJustDetected = true; // Long press here
      }
    } else {
    // No repeat here
    sMillisOfFirstReceive = millis();
    sLongPressJustDetected = false;
    }
  return sLongPressJustDetected; // No long press here
  }



