#include "MIDIUSB.h"

#define PIEZO_PIN A0
#define PIEZO_THRESHOLD 20  // Minimum piezo value to trigger a note
#define PIEZO_SPIKE_DETECT 300  // Value to detect a sharp spike
#define NUM_OF_BUTTONS 3  // Number of buttons

const byte MIDI_CH = 0;   // MIDI channel
const byte MIDI_NOTE = 40;  // Example MIDI note (Middle C)
bool noteOnFlag = false;  // To track if the piezo note is already on
unsigned long lastTriggerTime = 0;  // Last time the piezo note was triggered

byte buttonPin[NUM_OF_BUTTONS] = {2, 4, 11};  // Button pins
byte buttonState[NUM_OF_BUTTONS] = {0};       // Current state of buttons
byte buttonPreviousState[NUM_OF_BUTTONS] = {0};  // Previous state of buttons
unsigned long lastPressTime[NUM_OF_BUTTONS] = {0};  // Debounce timer for each button
byte buttonNotes[NUM_OF_BUTTONS] = {36, 38, 40};  // MIDI note numbers for the buttons

void setup() {
  Serial.begin(9600);
  
  // Speed up the analogRead process
  // Adjust ADC prescaler to speed up analogRead from default (16MHz / 128) to faster (16MHz / 32)
  ADCSRA = (ADCSRA & 0b11111000) | 0x04;  // Set prescaler to 16MHz/32 = 500kHz (higher speed, less resolution)
  
  // Setup button pins
  for (int i = 0; i < NUM_OF_BUTTONS; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  
  pinMode(PIEZO_PIN, INPUT);
}

void loop() {
  handleButtons();  // Check button states
  handlePiezo();    // Check piezo sensor state
}

// Handle the piezo sensor readings and send MIDI signals
void handlePiezo() {
  int piezoReading = analogRead(PIEZO_PIN);

  // Check for a sharp spike to detect a tap (instant response)
  if (piezoReading > PIEZO_SPIKE_DETECT && !noteOnFlag) {
    byte velocity = map(piezoReading, PIEZO_THRESHOLD, 1023, 1, 127);  // Map piezo value to MIDI velocity
    noteOn(MIDI_CH, MIDI_NOTE, velocity);  // Send Note On message
    Serial.print("Piezo Note On - Velocity: ");
    Serial.println(velocity);
    noteOnFlag = true;  // Mark that the note is currently on
    lastTriggerTime = millis();  // Store the time of the tap
  }

  // If piezo has been triggered, send Note Off after a short delay
  if (noteOnFlag && millis() - lastTriggerTime > 90) {  // Send Note Off after 50 ms
    noteOff(MIDI_CH, MIDI_NOTE, 0);  // Send Note Off message
    Serial.println("Piezo Note Off");
    noteOnFlag = false;  // Reset the flag
  }
}

// Handle button presses and send MIDI signals
void handleButtons() {
  unsigned long currentTime = millis();

  for (int i = 0; i < NUM_OF_BUTTONS; i++) {
    buttonState[i] = digitalRead(buttonPin[i]);

    // Check for a button press (LOW state with debouncing)
    if (buttonState[i] == LOW && buttonPreviousState[i] == HIGH && currentTime - lastPressTime[i] > 100) {
      noteOn(MIDI_CH, buttonNotes[i], 127);  // Send Note On message for the button
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(": Pressed");

      lastPressTime[i] = currentTime;  // Update the last press time
    }

    // Check for button release (transition to HIGH)
    if (buttonState[i] == HIGH && buttonPreviousState[i] == LOW) {
      noteOff(MIDI_CH, buttonNotes[i], 0);  // Send Note Off message for the button
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(": Released");
    }

    // Update previous state
    buttonPreviousState[i] = buttonState[i];
  }
}

// Send MIDI Note On message
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();  // Ensure the message is sent
}

// Send MIDI Note Off message
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}
