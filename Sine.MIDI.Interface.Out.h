#pragma once

#define MIDI_INTERFACE_IMPORTS true

#if MIDI_INTERFACE_IMPORTS
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
        // MIDI output device ID //
		std::uint64_t nID;
        // MIDI output device capabilities //
		MIDIOUTCAPS moc;
	};

    // Print the names of all MIDI output devices //
    std::vector<midi_object_struct> devices(numDevs);

    // Enumerate the MIDI output devices //
    for (unsigned int i = 0; i < numDevs; i++)
    {
        MIDIOUTCAPS moc;

        if (midiOutGetDevCaps(i, &moc, sizeof(MIDIOUTCAPS)) != MMSYSERR_NOERROR)
            continue;

        std::cout << "Device " << i << ": " << &moc.szPname << "\n";
        midi_object_struct midi_object;
        midi_object.nID = i;
        midi_object.moc = moc;
        devices.push_back(midi_object);
    }

    // Open the first MIDI output device //
    HMIDIOUT hMidiOut{};

    // Open the MIDI output device //
    MMRESULT mm_open_const_result{};
    do {
        mm_open_const_result = midiOutOpen(&hMidiOut, devices.at(0).nID, 0, 0, CALLBACK_NULL);
        if (mm_open_const_result == MMSYSERR_NOERROR)
        {
            //char error_msg[MAXERRORLENGTH];
            //midiOutGetErrorTextA(mm_open_const_result, error_msg, static_cast<UINT>( sizeof(error_msg) / sizeof(char) ));
            //std::cout << "Error opening MIDI output device. Error Code : " << mm_open_const_result << ", Message: " << error_msg << "\n";
            std::cout << "SUCCESS opening MIDI output device : " << devices.at(0).nID << "\n";
            break;
        }
    } while (mm_open_const_result != MMSYSERR_NOERROR);

    // Send a MIDI message (e.g., a Note On for middle C on channel 1) //
    // 0x90 is Note On for channel 1, 0x3C is middle C, 0x40 is velocity of 64 //
    MMRESULT mm_out_const_result{};
    DWORD midiMessage = 0x00403C90;
    do {
        mm_out_const_result = midiOutShortMsg(hMidiOut, midiMessage);
        if (mm_out_const_result == MMSYSERR_NOERROR)
        {
            //std::cout << "Error sending MIDI message. Error Code: " << mm_out_const_result << "\n";
            std::cout << "SUCCESS sending MIDI message : " << midiMessage << "\n";

            // Let the note play for 1.5 seconds //
            Sleep(1500);

            break;
        }
    } while (mm_out_const_result != MMSYSERR_NOERROR);

    // Close the MIDI output device //
    midiOutClose(hMidiOut);

    // Wait for user input //
    std::string ch{};
    std::cin >> ch;

    return 0;
}
#endif