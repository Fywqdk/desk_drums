# MIDI Desk Drums - Circuit Python edition

Original work by [cybercraftics](https://github.com/cybercraftics): [MIDI Desk Drums](https://github.com/cybercraftics/desk_drums)

I am much more comfortable with python and have some spare [Waveshare RP2040 Zero](https://www.waveshare.com/wiki/RP2040-Zero), and then it would be a fun project to port it to Circuitpython 

Todo:
- Test full analog drum setup
  - Waiting for 3D printer and desk mats in mail
- Create schematic for electronics
  - Current build relies on 2x ADS1115 16bit 4-channel ADC which are querried over I2C for input
- Test Pedal class
  - Haven't had time, a Pedal is printed though.
- Complete the full build and post pictures.

---

## Overview
MIDI Desk Drums is a compact, USB-powered drum kit designed to fit right on your desk. Built using Arduino, piezo sensors, and 3D-printed components, this project enables you to create a fully functional MIDI drum kit that's portable, responsive, and works seamlessly with music software like GarageBand, Ableton, and Logic Pro.

This repository contains all the code, schematics, and 3D models you need to build your own MIDI Desk Drums. Whether you're a music enthusiast, DIY maker, or just curious about MIDI technology, this project is perfect for adding a personal touch to your music-making setup!

### Video:

[![MIDI Desk Drums Demo](https://img.youtube.com/vi/3wSPyhD2FfM/0.jpg)](https://youtu.be/3wSPyhD2FfM)

## Features

![Instructables Image](https://content.instructables.com/FGG/X3QP/M3HNDZE6/FGGX3QPM3HNDZE6.png?auto=webp&frame=1&width=1024&height=1024&fit=bounds&md=MjAyNC0xMS0xNSAwMTozMTo1MC4w)

- **Compact and Desk-Friendly Design**: The drum kit is designed to fit on a standard desk or table, making it accessible and convenient.
- **MIDI Compatibility**: Sends MIDI signals over USB, compatible with various DAWs (Digital Audio Workstations).
- **Dynamic Response**: Adjusts the volume based on hit strength, providing realistic drumming feedback.
- **Arduino-Powered**: Uses an Arduino Leonardo to read sensor inputs and convert them to MIDI signals.
- **Debouncing Logic**: Avoids multiple signals from a single hit, thanks to a custom debounce algorithm.

## Building Instructions
https://www.instructables.com/Desk-Mat-to-MIDI-Drums/


## Wiring Schematic
![MIDI Drums Wiring](https://github.com/cybercraftics/desk_drums/blob/main/schematics/MIDI_drums_wiring.png)

## License
This project is licensed under the MIT License.
