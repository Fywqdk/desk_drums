#include "MIDIUSB.h"

#define PIEZO_THRESHOLD 10        // Minimum piezo value to trigger a note
#define PIEZO_SPIKE_DETECT 300    // Value to detect a sharp spike
#define NUM_OF_DRUMS 7            // Number of drums

const byte MIDI_CH = 0;           // MIDI channel
bool noteOnFlag[NUM_OF_DRUMS] = {false};  // To track if each piezo note is already on
unsigned long lastTriggerTime[NUM_OF_DRUMS] = {0};  // Last time each piezo note was triggered

byte piezoPin[NUM_OF_DRUMS] = {A0, A1, A2, A3, A4, A5, A6};  // Pins for each piezo sensor
byte piezoNotes[NUM_OF_DRUMS] = {51, 47, 48, 38, 49, 42, 41};  // MIDI notes for each drum

const byte pedalPin = 2;         // Pin for the pedal switch
const byte bassDrumNote = 36;    // MIDI note for bass drum
bool pedalPressed = false;       // To track pedal state

void setup() {
  Serial.begin(9600);
  
  // Speed up the analogRead process
  ADCSRA = (ADCSRA & 0b11111000) | 0x04;  // Set prescaler to 16MHz/32 = 500kHz
  
  // Setup piezo pins
  for (int i = 0; i < NUM_OF_DRUMS; i++) {
    pinMode(piezoPin[i], INPUT);
  }
  
  // Setup pedal pin
  pinMode(pedalPin, INPUT_PULLUP);  // Use internal pull-up resistor
}

void loop() {
  for (int i = 0; i < NUM_OF_DRUMS; i++) {
    handlePiezo(i);    // Check piezo sensor state for each drum
  }
  handlePedals();       // Check pedal state for the bass drum
}

// Handle the piezo sensor readings and send MIDI signals for each drum
void handlePiezo(int drumIndex) {
  int piezoReading = analogRead(piezoPin[drumIndex]);

  if (piezoReading > 50) {
    Serial.print("Drum index: ");
    Serial.print(drumIndex);
    Serial.print(" note: ");
    Serial.println(piezoNotes[drumIndex]);
  }

  // Check for a sharp spike to detect a tap (instant response)
  if (piezoReading > PIEZO_SPIKE_DETECT && !noteOnFlag[drumIndex]) {
    byte velocity = map(piezoReading, PIEZO_THRESHOLD, 1023, 1, 127);  // Map piezo value to MIDI velocity
    noteOn(MIDI_CH, piezoNotes[drumIndex], velocity);  // Send Note On message for each drum
    Serial.print("Drum ");
    Serial.print(drumIndex);
    Serial.print(" Note On - Velocity: ");
    Serial.println(velocity);
    noteOnFlag[drumIndex] = true;  // Mark that the note is currently on
    lastTriggerTime[drumIndex] = millis();  // Store the time of the tap
  }

  // If piezo has been triggered, send Note Off after a short delay
  if (noteOnFlag[drumIndex] && millis() - lastTriggerTime[drumIndex] > 90) {  // Send Note Off after 90 ms
    noteOff(MIDI_CH, piezoNotes[drumIndex], 0);  // Send Note Off message
    Serial.print("Drum ");
    Serial.print(drumIndex);
    Serial.println(" Note Off");
    noteOnFlag[drumIndex] = false;  // Reset the flag
  }
}

// Handle the pedal input and send MIDI signal for bass drum
void handlePedals() {
  bool currentPedalState = digitalRead(pedalPin) == LOW;  // Check if pedal is pressed

  if (currentPedalState && !pedalPressed) {  // If pedal is pressed and wasn't previously
    noteOn(MIDI_CH, bassDrumNote, 127);  // Send Note On for bass drum with max velocity
    Serial.println("Bass Drum Note On - Pedal Pressed");
    pedalPressed = true;  // Set pedal state to pressed
  } else if (!currentPedalState && pedalPressed) {  // If pedal is released
    noteOff(MIDI_CH, bassDrumNote, 0);  // Send Note Off for bass drum
    Serial.println("Bass Drum Note Off - Pedal Released");
    pedalPressed = false;  // Reset pedal state
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