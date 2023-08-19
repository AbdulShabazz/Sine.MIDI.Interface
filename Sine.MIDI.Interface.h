#pragma once

// Sine.MIDI.Interface.cpp : This file contains the 'main' function. Program execution begins and ends there. //

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

int main() {
    // Enumerate MIDI output devices //
    unsigned int numDevs = midiOutGetNumDevs();

    if (numDevs == 0) {
        std::cout << "No MIDI output devices found!" << "\n";
        return 0;
    }

    // MIDI output device information //
    struct midi_object_struct {
		std::uint64_t nID;
		MIDIOUTCAPS moc;
	};

    // Print the names of all MIDI output devices //
    std::vector<midi_object_struct> devices(numDevs);

    // Enumerate the MIDI output devices //
    for (unsigned int i = 0; i < numDevs; i++) {
        MIDIOUTCAPS moc;
        if (midiOutGetDevCaps(i, &moc, sizeof(MIDIOUTCAPS)) == MMSYSERR_NOERROR) {
            std::cout << "Device " << i << ": " << &moc.szPname << "\n";
            midi_object_struct midi_object;
            midi_object.nID = i;
            midi_object.moc = moc;
            devices.push_back(midi_object);
        }
    }

    // Open the first MIDI output device //
    HMIDIOUT hMidiOut = NULL;
    const MMRESULT& mm_open_const_result = midiOutOpen(&hMidiOut, devices.at(0).nID, 0, 0, CALLBACK_NULL);
    if (mm_open_const_result != MMSYSERR_NOERROR) {
        std::cout << "Error opening MIDI output device. Error Code : " << mm_open_const_result << "\n";
        return 1;
    }

    // Send a MIDI message (e.g., a Note On for middle C on channel 1) //
    // 0x90 is Note On for channel 1, 0x3C is middle C, 0x40 is velocity of 64 //    
    DWORD midiMessage = 0x00403C90;  
    const MMRESULT& mm_out_const_result = midiOutShortMsg(hMidiOut, midiMessage);
    if (mm_out_const_result != MMSYSERR_NOERROR) {
        std::cout << "Error sending MIDI message. Error Code: " << mm_out_const_result << "\n";
        return 1;
    }

    // Let the note play for 1.5 seconds //
    Sleep(1500);  

    // Close the MIDI output device //
    midiOutClose(hMidiOut);

    // Wait for user input //
    std::string ch{};
    std::cin >> ch;

    return 0;
}
