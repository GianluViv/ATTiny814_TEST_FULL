//  Pin mapping table for different platforms
//  Platform     IR input    IR output   Tone      Core/Pin schema
//  --------------------------------------------------------------
//  ATtiny814       PA1         PA3       PA5      MegaTinyCore

#define IR_RECEIVE_PIN  PIN_PA2 // 12  era PA1
#define IR_SEND_PIN     PIN_PA4 // 2
// #define TONE_PIN        PIN_PA5 // 3
// #define APPLICATION_PIN PIN_PA4 // 0
// #undef LED_BUILTIN              // No LED available, take the one which is connected to the DAC output
// #define LED_BUILTIN     PIN_PA6 // 4


//  * Helper macro for getting a macro definition as string
#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif
