#pragma once

#if MIDI_INTERFACE_EXPORTS
// Sine.MIDI.Interface.cpp : This file contains the 'main' function. Program execution begins and ends there. //

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

int main() {
    // Enumerate MIDI output devices //
    unsigned int numDevs = midiOutGetNumDevs();

    if (numDevs == 0)
    {
        std::cout << "No MIDI output devices found!" << "\n";
        return 0;
    }

    // MIDI output device information //
    struct midi_object_struct
    {
        // MIDI input device ID //
		std::uint64_t nID;
        // MIDI input device capabilities //
		MIDIINCAPS moc;
	};

    // Print the names of all MIDI output devices //
    std::vector<midi_object_struct> devices(numDevs);

    // Enumerate the MIDI output devices //
    for (unsigned int i = 0; i < numDevs; i++)
    {
        MIDIINCAPS moc;

        if (midiInGetDevCaps(i, &moc, sizeof(MIDIINCAPS)) != MMSYSERR_NOERROR)
            continue;

        std::cout << "Device " << i << ": " << &moc.szPname << "\n";
        midi_object_struct midi_object{};
        midi_object.nID = i;
        midi_object.moc = moc;
        devices.push_back(midi_object);
    }

    // Open the first MIDI output device //
    HMIDIIN h_midi_in = NULL;
    const MMRESULT& mm_open_const_result = midiInOpen(&h_midi_in, devices.at(0).nID, 0, 0, CALLBACK_NULL);
    if (mm_open_const_result != MMSYSERR_NOERROR)
    {
        char error_msg[MAXERRORLENGTH];
        midiInGetErrorTextA(mm_open_const_result, error_msg, static_cast<UINT>( sizeof(error_msg) / sizeof(char) ));
        std::cout << "Error opening MIDI input device. Error Code : " << mm_open_const_result << ", Message: " << error_msg << "\n";
        return 1;
    }

    // Send a MIDI message (e.g., a Note On for middle C on channel 1) //
    // 0x90 is Note On for channel 1, 0x3C is middle C, 0x40 is velocity of 64 //    
    DWORD midiMessage = 0x00403C90;  
    const MMRESULT& mm_out_const_result = midiInStart(h_midi_in);

    if (mm_out_const_result != MMSYSERR_NOERROR)
    {
        std::cout << "Error sending MIDI message. Error Code: " << mm_out_const_result << "\n";
        return 1;
    }

    // Let the note play for 1.5 seconds //
    Sleep(1500);

    // Stop the MIDI output device //
    midiInStop(h_midi_in);

    // Close the MIDI output device //
    midiInClose(h_midi_in);

    // Wait for user input //
    std::string ch{};
    std::cin >> ch;

    return 0;
}
#endif