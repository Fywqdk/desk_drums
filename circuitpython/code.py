# Import libraries

# Core
import board
import digitalio
import busio
import pwmio
import time

# Midi
import usb_midi
import adafruit_midi

# Adafruit ADC library
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

# Adafruit Midi library functions
from adafruit_midi.note_on import NoteOn
from adafruit_midi.note_off import NoteOff
from adafruit_midi.pitch_bend import PitchBend
from adafruit_midi.control_change import ControlChange

# Drum and Pedal classes
from drums import Drum, Pedal

# Initialize midi
midi = adafruit_midi.MIDI(midi_out=usb_midi.ports[1], out_channel=10)

# Create the I2C bus
i2c = busio.I2C(board.GP3, board.GP2) # SCL, SDA

# Create ADC objects using the I2C bus
ads1 = ADS.ADS1115(i2c, data_rate=860, address=int(0x48))
ads2 = ADS.ADS1115(i2c, data_rate=860, address=int(0x4A))

# Global values
spike_detect = 1200
debounce_time = 0.1

# Drum set configuration
hihat = Drum("Hi-hat", 42, AnalogIn(ads1, ADS.P0))
snare = Drum("Lilletromme", 38, AnalogIn(ads1, ADS.P1))
left_tom = Drum("Venstre Tom", 41, AnalogIn(ads1, ADS.P2))
right_tom = Drum("Højre Tom", 47, AnalogIn(ads1, ADS.P3))
floor_tom = Drum("Gulv Tom", 48, AnalogIn(ads2, ADS.P0))
ride_cymbal = Drum("Ride bækken", 51, AnalogIn(ads2, ADS.P1))
crash_cymbal = Drum("Crash bækken", 49, AnalogIn(ads2, ADS.P2))
splash_cymbal = Drum("Splash bækken", 55, AnalogIn(ads2, ADS.P3))

# Collect drums in lists
drums = [hihat, snare, left_tom, right_tom, floor_tom, crash_cymbal, splash_cymbal, ride_cymbal]
pedals = []

# Start loop
while True:
    
    # Check and handle drums for input
    for i in range(len(drums)):
        val = drums[i].pin.value
        if val > spike_detect:
            drums[i].play(val)
        else:
            drums[i].stop(debounce_time)
 
    # Check and handle pedals for input 
    for i in range(len(pedals)):
        if pedals[i].pin:
            pedals[i].play(debounce_time)
        
    time.sleep(0.01)
