# MIDI Desk Drums

## Overview
MIDI Desk Drums is a compact, USB-powered drum kit designed to fit right on your desk. Built using Arduino, piezo sensors, and 3D-printed components, this project enables you to create a fully functional MIDI drum kit that's portable, responsive, and works seamlessly with music software like GarageBand, Ableton, and Logic Pro.

This repository contains all the code, schematics, and 3D models you need to build your own MIDI Desk Drums. Whether you're a music enthusiast, DIY maker, or just curious about MIDI technology, this project is perfect for adding a personal touch to your music-making setup!

[![MIDI Desk Drums Demo](https://img.youtube.com/vi/3wSPyhD2FfM/0.jpg)](https://youtu.be/3wSPyhD2FfM)

## Features
- **Compact and Desk-Friendly Design**: The drum kit is designed to fit on a standard desk or table, making it accessible and convenient.
- **MIDI Compatibility**: Sends MIDI signals over USB, compatible with various DAWs (Digital Audio Workstations).
- **Dynamic Response**: Adjusts the volume based on hit strength, providing realistic drumming feedback.
- **Arduino-Powered**: Uses an Arduino Leonardo to read sensor inputs and convert them to MIDI signals.
- **Debouncing Logic**: Avoids multiple signals from a single hit, thanks to a custom debounce algorithm.

## Components
- **Arduino Leonardo** (or similar, with MIDI support)
- **Piezo Sensors** for detecting drum hits
- **3D-Printed Plates and Drumsticks** (files included)
- **Desk Mat** for mounting and cushioning
- **Universal Glue** and **Hot Glue** for assembly
- **Ribbon cable""

## Getting Started
1. **Hardware Assembly**: Follow the schematic provided to connect piezo sensors to the Arduino. Use the 3D-printed templates to hold the drum pads in place on the desk mat.
2. **Software Setup**: Upload the Arduino code to the Arduino Leonardo to enable MIDI functionality.
3. **Configure in DAW**: Connect your Arduino to your computer, and open a MIDI-compatible software like GarageBand or Ableton to map each pad to a specific drum sound.

## Wiring Schematic
![MIDI Drums Wiring](https://github.com/cybercraftics/desk_drums/blob/main/schematics/MIDI_drums_wiring.png)

## License
This project is licensed under the MIT License.
