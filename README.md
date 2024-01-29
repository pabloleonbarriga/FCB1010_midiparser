# MIDI Wah-Wah Pedal Controller

## Overview

This C++ program serves as a MIDI parser designed for the Behringer FCB1010 MIDI Foot Controller, enabling its integration with Helix Native. Upon program launch, the user is prompted to select MIDI input and output ports. Following this selection, the program sends a turn-off signal to all effects in Helix Native. With each press on the foot controller, it alternates between sending MIDI values of 0 and 127 to toggle the effects on and off.

Specifically for the wah-wah effect, the program automatically activates it if the value received from the expression pedal exceeds 4. Conversely, it deactivates the wah-wah effect if the value is below 4. This functionality allows users to control the wah-wah effect seamlessly using the expression pedal, mirroring the behavior of a traditional wah-wah pedal.

## Requirements

- C++ compiler
- RtMidi library (included in the project as "rtmidi")

## Usage

1. Compile the program using a C++ compiler, ensuring the RtMidi library is linked.
2. Run the executable.
3. Choose the MIDI input and output ports when prompted (automatic selection if only one port available).
4. The program initializes by turning off all pedals (MIDI control change messages with a value of 0 for each control number from 0 to 127).
5. Continuously read MIDI input. Press the wah-wah pedal to trigger the effect.

## Code Overview

- Utilizes the `RtMidiOut` and `RtMidiIn` classes from the RtMidi library for MIDI output and input, respectively.
- The `mycallback` function is the MIDI callback that processes incoming messages, specifically detecting MIDI control change messages with control number 27 for the wah-wah pedal.
- Interactive port selection is facilitated by the `chooseMidiPortIn` and `chooseMidiPortOut` functions.

## Dependencies

- [RtMidi](https://www.music.mcgill.ca/~gary/rtmidi/)

## Building and Running

```bash
mkdir build
cd .\build\ 
g++ -std=gnu++11 -D__WINDOWS_MM__ -o midi_parser ..\src\midi_parser.cpp ..\rtmidi\RtMidi.cpp -lwinm
./midi_parser
