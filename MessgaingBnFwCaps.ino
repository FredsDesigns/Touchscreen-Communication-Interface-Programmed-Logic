#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS     10
#define TFT_DC     9
#define TS_CS      6

#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
XPT2046_Touchscreen ts(TS_CS);

const int MAX_MESSAGES = 5;
String messages[MAX_MESSAGES];
int messageCount = 0;
int shift = 0;

String name = ""; // Declare name globally

void setup() {
  Serial.begin(9600);
  
  tft.begin();
  tft.setRotation(3); // Adjust rotation if needed (lengthwise is 2, width is 3)
  
  // Clear the screen
  tft.fillScreen(ILI9341_WHITE);
  // Set text color to white
  tft.setTextColor(ILI9341_BLACK);
  // Set text size
  tft.setTextSize(2);
  
  // Initialize touchscreen
  ts.begin();

  // Prompt user to enter their name
  tft.setCursor(0, 0);
  tft.print("     Enter your name:");
  
  // Initialize a flag to keep track of whether the name has been entered
  bool nameEntered = false;

  while (!nameEntered) {
    // Check if data is available to read from serial
    if (Serial.available() > 0) {
      // Read the incoming byte
      char incomingByte = Serial.read();
      // Check if the byte is a newline character (Enter key)
      if (incomingByte == '\n') {
        // If the name is not empty, set the nameEntered flag to true and exit the loop
        if (name.length() > 0) {
          nameEntered = true;
        }
      } else {
        // Append the received character to the name string
        name += incomingByte;
      }
    }
  }

  // Clear the screen
  tft.fillScreen(ILI9341_WHITE);
  
  // Display "Connected with [name]" message
  tft.setCursor(0, 0);
  tft.print("Connected with " + name);
  tft.fillRect(0, 20, tft.width(), 20 * MAX_MESSAGES, ILI9341_WHITE);
  tft.fillRect(0, 20 * (MAX_MESSAGES + 1), tft.width(), tft.height() - 20 * (MAX_MESSAGES + 1), ILI9341_RED);
  tft.fillRect(0, 20 * (MAX_MESSAGES + 1), tft.width(), tft.height() - 44 * (MAX_MESSAGES + 1), 0xFC0E);
  
  // Draw keyboard buttons
  drawKeyboard();
}

// Store incoming serial messages separately
String serialMessage = "";

void loop() {
  // Check for touchscreen input
  if (ts.touched()) {
    // Handle touchscreen input
    handleTouchscreenInput();
  }
  
  // Check for incoming serial data
  while (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();
    // Check if the byte is a newline character (Enter key)
    if (incomingByte == '\n') {
      // If the serial message is not empty, add it to the messages array
      if (serialMessage.length() > 0) {
        addMessage(serialMessage);
        // Clear the serial message buffer
        serialMessage = "";
        // Update display after message is received
        updateDisplay();
      }
    } else {
      // Append the received character to the serial message buffer
      serialMessage += incomingByte;
    }
  }
}

void addMessage(String newMessage) {
  // Shift existing messages up
  for (int i = 0; i < MAX_MESSAGES - 1; i++) {
    messages[i] = messages[i + 1];
  }
  
  // Add new message at the end
  messages[MAX_MESSAGES - 2] = newMessage;
}

void handleTouchscreenInput() {
  // Process touchscreen input
  TS_Point p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  
 if (p.y > 158 && p.x >= 300 && p.y <=195) {
    // Handle backspace action
    handleBackspace();
    return; // Exit the function early to prevent executing other actions
  }

  // Print coordinates to serial monitor, you can uncomment this if you want to see where a user is tapping!
  //Serial.print("X: ");
  //Serial.print(p.x);
  //Serial.print(", Y: ");
  //Serial.println(p.y);
  
  // Check if touch is within the send button area
  if (p.y <= 158 && p.x >= 300) {
    // Send the current message over serial
    Serial.println(messages[MAX_MESSAGES - 1]);
    // Clear the current message
    messages[MAX_MESSAGES - 1] = "";
    // Update display after message is sent
    updateDisplay();
  } else {
    // Determine which letter was pressed
    int buttonWidth = tft.width() / 13; // 13 letters in the alphabet
    int buttonHeight = 40;
    int letterIndex = p.x / buttonWidth;
    if (letterIndex >= 0 && letterIndex < 13 && p.y <= 158 && p.x < 300 && shift == 0) {
      char letter = letterIndex + '`';
      // Append the pressed letter to the current message
      messages[MAX_MESSAGES - 1] += letter;
      // Update display after letter is added
      updateDisplay();
      
    }
else if (letterIndex >= 0 && letterIndex < 13 && p.y <= 158 && p.x < 300 && shift == 1) {
      char letter = letterIndex + '@';
      // Append the pressed letter to the current message
      messages[MAX_MESSAGES - 1] += letter;
      // Update display after letter is added
      updateDisplay();


    } else if (letterIndex >= 0 && letterIndex <= 13 && p.y > 158 && p.x < 300 && p.y <= 195 && shift == 0) {
      char letter = letterIndex + 'l';
      // Append the pressed letter to the current message
      messages[MAX_MESSAGES - 1] += letter;
      // Update display after letter is added
      updateDisplay();
    } else if (letterIndex >= 0 && letterIndex <= 13 && p.y > 158 && p.x < 300 && p.y <= 195 && shift == 1) {
      char letter = letterIndex + 'L';
      // Append the pressed letter to the current message
      messages[MAX_MESSAGES - 1] += letter;
      // Update display after letter is added
      updateDisplay();
    }  
    
else if (letterIndex >= 0 && letterIndex <= 2 && p.y > 195 && p.x <= 71 && shift == 0) {
      char letter = letterIndex + 'x';
      // Append the pressed letter to the current message
      messages[MAX_MESSAGES - 1] += letter;
      // Update display after letter is added
      updateDisplay();
    } else if (letterIndex >= 0 && letterIndex <= 2 && p.y > 195 && p.x <= 71 && shift == 1) {
      char letter = letterIndex + 'X';
      // Append the pressed letter to the current message
      messages[MAX_MESSAGES - 1] += letter;
      // Update display after letter is added
      updateDisplay();
    }
    
     else if (p.y > 195 && p.x > 137 && p.x < 300) {
      handleSymbolInput(p); // Handle symbol input
    } else if (p.y > 195 && p.x > 71 && p.x <= 137){
      messages[MAX_MESSAGES - 1] += ' ';
      // Update display after letter is added
      updateDisplay();
    } else if (p.x > 279 && p.y > 195 && shift == 0){
      shift = 1;
      Serial.println("Shift = 1");
      delay(1000);
      drawKeyboard();
    } else if (p.x > 279 && p.y > 195 && shift == 1){
      shift = 0;
      Serial.println("Shift = 0");
      delay(1000);
      drawKeyboard();
    } 
  }
}

// Function to handle addition of symbols
void addSymbol(char symbol) {
  messages[MAX_MESSAGES - 1] += symbol;
  // Update display after symbol is added
  updateDisplay();
}

void handleSymbolInput(TS_Point p) {
  // Determine which symbol was pressed based on the regions
  if (p.y > 195) {
     if (p.x > 137 && p.x <= 160) { // Symbol: "!"
      addSymbol('!');
    } else if (p.x > 160 && p.x <= 185 ) { // Symbol: "?"
      addSymbol('?');
    } else if (p.x > 185 && p.x <= 208) { // Symbol: ","
      addSymbol(',');
    } else if (p.x > 208 && p.x <= 232) { // Symbol: "."
      addSymbol('.');
    } else if (p.x > 232 && p.x <= 254) { // Symbol: "("
      addSymbol('(');
    } else if (p.x > 254 && p.x <= 279) { // Symbol: "/"
      addSymbol(')');
  }   
 }
}

void handleBackspace() {
  // Get the current message
  String currentMessage = messages[MAX_MESSAGES - 1];
  
  // Check if the message is not empty
  if (currentMessage.length() > 0) {
    // Remove the last character from the message
    currentMessage.remove(currentMessage.length() - 1);
    // Update the message in the messages array
    messages[MAX_MESSAGES - 1] = currentMessage;
    // Update the display
    updateDisplay();
  }
}

void updateDisplay() {
  // Clear the area beneath the messages
  tft.fillRect(0, 20, tft.width(), 20 * MAX_MESSAGES, ILI9341_WHITE);
  tft.fillRect(0, 20 * (MAX_MESSAGES + 1), tft.width(), tft.height() - 44 * (MAX_MESSAGES + 1), 0xFC0E);

  // Display "Connected with [name]" message
  tft.setCursor(0, 0);
  tft.print("Connected with " + name);

  // Display messages
  for (int i = 0; i < MAX_MESSAGES; i++) {
    tft.setCursor(0, 20 * (i + 1));
    tft.print(messages[i]);
  }
}

void drawKeyboard() {
  // Define keyboard button dimensions
  int buttonWidth = tft.width() / 13; // 13 letters in the alphabet
  int buttonHeight = 40;
  
char letter = 'a';
if(shift == 1) {
  letter = 'A';
}
  for (int i = 0; i < 12; i++) {
    tft.fillRect(i * buttonWidth, 20 * (MAX_MESSAGES + 1), buttonWidth, buttonHeight, ILI9341_WHITE);
    tft.drawRect(i * buttonWidth, 20 * (MAX_MESSAGES + 1), buttonWidth, buttonHeight, ILI9341_BLACK);
    tft.setCursor(i * buttonWidth + 10, 20 * (MAX_MESSAGES + 1) + 10);
    tft.print(letter);
    letter++;
  }

letter = 'm';
if(shift == 1) {
  letter = 'M';
}
  for (int i = 0; i < 12; i++) {
    tft.fillRect(i * buttonWidth, 20 * (MAX_MESSAGES + 3), buttonWidth, buttonHeight, ILI9341_WHITE);
    tft.drawRect(i * buttonWidth, 20 * (MAX_MESSAGES + 3), buttonWidth, buttonHeight, ILI9341_BLACK);
    tft.setCursor(i * buttonWidth + 10, 20 * (MAX_MESSAGES + 3) + 10);
    tft.print(letter);
    letter++;
  }
letter = 'y';
if(shift == 1) {
  letter = 'Y';
}
  for (int i = 0; i < 2; i++) {
    tft.fillRect(i * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth, buttonHeight, ILI9341_WHITE);
    tft.drawRect(i * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth, buttonHeight, ILI9341_BLACK);
    tft.setCursor(i * buttonWidth + 10, 20 * (MAX_MESSAGES + 5) + 10);
    tft.print(letter);
    letter++;
  }
  
//spacebar
    tft.fillRect(2 * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth * 3, buttonHeight, ILI9341_WHITE);
    tft.drawRect(2 * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth * 3, buttonHeight, ILI9341_BLACK);
    tft.setCursor(2 * buttonWidth + 10, 20 * (MAX_MESSAGES + 5) + 10);
    tft.print(' ');

     String symbols = "!?,.()";
  for (int i = 5; i < symbols.length()+5; i++) {
    tft.fillRect(i * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth, buttonHeight, ILI9341_WHITE);
    tft.drawRect(i * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth, buttonHeight, ILI9341_BLACK);
    tft.setCursor(i * buttonWidth + 10, 20 * (MAX_MESSAGES + 5) + 10);
    tft.print(symbols[i-5]);
  }
  int sendButtonWidth = tft.width() / 10;
  int sendButtonHeight = 40;
  tft.fillRect(tft.width() - sendButtonWidth, 20 * (MAX_MESSAGES + 3), sendButtonWidth, sendButtonHeight, ILI9341_RED);
  tft.drawRect(tft.width() - sendButtonWidth, 20 * (MAX_MESSAGES + 3), sendButtonWidth, sendButtonHeight, ILI9341_BLACK);
  tft.setCursor(tft.width() - sendButtonWidth + 15, 20 * (MAX_MESSAGES + 3) + 10);
  tft.print("<"); // Print left arrow
  tft.setCursor(tft.width() - sendButtonWidth + 5 , 20 * (MAX_MESSAGES + 3) + 10);
  tft.print("<"); 
  tft.setTextColor(ILI9341_BLACK);

  // Draw send button
  tft.fillRect(tft.width() - sendButtonWidth, 20 * (MAX_MESSAGES + 1), sendButtonWidth, sendButtonHeight, 0x07E0); // Green color
  tft.drawRect(tft.width() - sendButtonWidth, 20 * (MAX_MESSAGES + 1), sendButtonWidth, sendButtonHeight, ILI9341_BLACK);
  tft.setCursor(tft.width() - sendButtonWidth + 10, 20 * (MAX_MESSAGES + 1) + 10);
  tft.print("^");
   tft.setCursor(tft.width() - sendButtonWidth + 10, 20 * (MAX_MESSAGES + 1.2) + 10);
   tft.print("I");
  tft.setTextColor(ILI9341_BLACK);

    // Draw shift button
  tft.fillRect(11 * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth*2.36, sendButtonHeight, 0x8410);
  if(shift == 0){
  tft.fillRect(11 * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth*2.36, sendButtonHeight/4, ILI9341_WHITE);
  } //Gray color
  tft.drawRect(11 * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth*2.36, sendButtonHeight, ILI9341_BLACK);
  tft.setCursor(tft.width() - sendButtonWidth, 20 * (MAX_MESSAGES + 5.5) + 10);
  tft.print("^");
  tft.setTextColor(ILI9341_BLACK);

  if(shift == 1) {
    tft.fillRect(11 * buttonWidth, 20 * (MAX_MESSAGES + 5), buttonWidth*2.36, sendButtonHeight/4, ILI9341_YELLOW); //Gray color
  }
}
