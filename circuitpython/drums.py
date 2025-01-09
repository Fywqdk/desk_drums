# Import libraries

import time
import usb_midi
import adafruit_midi

from adafruit_midi.note_on import NoteOn
from adafruit_midi.note_off import NoteOff

# Initialize midi
midi = adafruit_midi.MIDI(midi_out=usb_midi.ports[1], out_channel=10)

# Function to remap values
def map_range(x, in_max, out_max):
    out = x * out_max // in_max
    if out < 0:
        out = 0
    return out

# Drum class - For analog input like Piezo sensors
class Drum(object):
    def __init__(self, name, note, pin):
        self.name = name
        self.note = note
        self.pin = pin
        self.note_on = False
        self.velocity = 0
        self.trigger_time = 0

    def get_velocity(self, val):
        self.velocity = map_range(val, 32767, 127)

    def play(self, val):
        if not self.note_on:
            self.get_velocity(val)
            self.trigger_time = time.monotonic()
            self.note_on = True
            midi.send(NoteOn(self.note, self.velocity))
            print(f'{self.name} struck with intensity {self.velocity}')

    def stop(self, debounce_time):
        if self.note_on:
            if time.monotonic() - self.trigger_time > debounce_time:
                self.note_on = False
                self.trigger_time = 0
                self.velocity = 0
                midi.send(NoteOff(self.note, 0))
                print(f'{self.name} stopped')

# Pedal class - For digital input like switches
class Pedal(object):
    def __init__(self, name, note, pin):
        self.name = name
        self.note = note
        self.pin = pin
        self.note_on = False
        self.velocity = 127
        self.trigger_time = 0

    def play(self, debounce_time):
        if not self.note_on:
            self.trigger_time = time.monotonic()
            self.note_on = True
            midi.send(NoteOn(self.note, self.velocity))
            print(f'{self.name} hit')
        else:
            if time.monotonic() - self.trigger_time > debounce_time:
            self.note_on = False
            midi.send(NoteOff(self.note, 0))
            print(f'{self.name} stopped')
