#include <HID-Project.h>
#include <HID-Settings.h>

// pins to which the buttons are connected
const uint8_t previous = 14;
const uint8_t play = 16;
const uint8_t next = 10;

// debounce and timer initiated
const uint8_t debounce_time = 20;
uint32_t timer0 = 0;
uint32_t timer1 = 0;
uint32_t timer2 = 0;

bool playBtnPressed = false;

unsigned long switchLoopTimer = 0;
unsigned long mouseLoopDelay = 0;
bool inMouseLoop = false;
int8_t dir = 1;

void setup() {
  Serial.begin(115200);
  Consumer.begin();
  pinMode(previous, INPUT);
  pinMode(play, INPUT);
  pinMode(next, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Setup complete");
}

void loop() {
  if (inMouseLoop) {
    mouseLoop();
    return;
  }
  // check previous button
  if (!digitalRead(previous)) {
    if (millis() - timer2 > debounce_time) { // pull-up button -> digitalread = FALSE if pressed
      Consumer.write(MEDIA_PREVIOUS);
      timer2 = millis();
      Serial.println("Prev pressed - OK");
    } else {
      timer2 = millis();
    }
  }

    // check next button
  if (!digitalRead(next)) {
    if (millis() - timer0 > debounce_time) { // pull-up button -> digitalread = FALSE if pressed
      Consumer.write(MEDIA_NEXT);
      timer0 = millis();
      Serial.println("Next pressed - OK");
    } else {
      timer0 = millis();
    }
  }
  

  // check play/pause button
  if (!digitalRead(play)) {
    if (millis() - switchLoopTimer >= 2000 && switchLoopTimer > 0) { // check if pressed for 2 sec
      // goto mouseloop
      Serial.println("Switching to mouseloop!");
      inMouseLoop = true;
      timer1 = millis();
      switchLoopTimer = 0;
    }
    else if (millis() - timer1 > debounce_time) { // check if pressed outside debounce
      playBtnPressed = true;
      if (switchLoopTimer == 0) { // set inital value for slt
        switchLoopTimer = millis();
      }
      timer1 = millis();
      Serial.println("Play btn pressed");
    }
    else{
      timer1 = millis();
    }
  } else if (playBtnPressed) { // btn released
    Serial.println("Media controlled");
    Consumer.write(MEDIA_PLAY_PAUSE);
    playBtnPressed = false;
    timer1 = millis();
    switchLoopTimer = 0;
  } 
  else{
    switchLoopTimer = 0;
    
  }
}

void mouseLoop() {
  if (millis() - mouseLoopDelay >= 1500 + random(0, 500))
  {
    int8_t height = 0;
    int8_t deltay = 0;
    for (uint8_t i = 0; i <= 20; i++) {
      deltay = random(-5, 5);
      height += deltay;
      if (height > 10) {
        deltay = -10;
        height += deltay;
      }
      else if (height < -10) {
        deltay = 10;
        height += deltay;
      }
      Mouse.move(-i * dir, deltay, 0);
      if (!checkPlayBtnPressed()) return;
      delay(10);
    }
    dir = - dir;
    mouseLoopDelay = millis();
  }
  if (!checkPlayBtnPressed()) return;
}

bool checkPlayBtnPressed()
{
  if (!digitalRead(play)) {
    if (millis() - timer1 >= debounce_time) {
      inMouseLoop = false;
      playBtnPressed = false;
      Serial.println("Exiting mouse loop");
      switchLoopTimer = 0;
      timer1 = millis();
      return inMouseLoop;
    } 
    else {
      timer1 = millis();
    }
  }
  return inMouseLoop;
}
