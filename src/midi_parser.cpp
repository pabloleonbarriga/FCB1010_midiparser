#include <iostream>
#include <cstdlib>
#include "..\rtmidi\RtMidi.h"

#include <windows.h>
#define SLEEP(milliseconds) Sleep((DWORD)milliseconds)
#define LENGTH 128
#define WAHWAH 27

RtMidiOut *midiout = 0;
RtMidiIn *midiin = 0;

std::vector<bool> dataarray(LENGTH, false);
bool wah = false;

void mycallback(double deltatime, std::vector<unsigned char> *message, void * /*userData*/)
{
    unsigned int nBytes = message->size();

    // for (unsigned int i = 0; i < nBytes; i++)
    //   std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    // std::cout << std::endl;
    if ((int)message->at(1) == WAHWAH && (int)message->at(2) < 4)
    {

                std::vector<unsigned char> midiMessag2;

        midiMessag2.push_back(176);
        midiMessag2.push_back((int)message->at(1));
        midiMessag2.push_back(0);
        midiout->sendMessage(&midiMessag2);

        std::vector<unsigned char> midiMessage;

        midiMessage.push_back(176);
        midiMessage.push_back(125);
        midiMessage.push_back(0);
        midiout->sendMessage(&midiMessage);

        wah = false;
    }
    else if ((int)message->at(1) == WAHWAH && (int)message->at(2) >= 4)
    {
        if (wah == false)
        {
            std::vector<unsigned char> midiMessage;

            midiMessage.push_back(176);
            midiMessage.push_back(125);
            midiMessage.push_back(127);
            midiout->sendMessage(&midiMessage);
            wah = true;
        }

        std::vector<unsigned char> midiMessag2;

        midiMessag2.push_back(176);
        midiMessag2.push_back((int)message->at(1));
        midiMessag2.push_back((int)message->at(2));
        midiout->sendMessage(&midiMessag2);
    }
    else if ((int)message->at(1) != WAHWAH && (int)message->at(2) == 127)
    {

        std::vector<unsigned char> midiMessage;

        midiMessage.push_back(176);
        midiMessage.push_back((int)message->at(1));
        midiMessage.push_back((dataarray[(int)message->at(1)] == false) ? 127 : 0);
        midiout->sendMessage(&midiMessage);

        dataarray[(int)message->at(1)] = !(dataarray[(int)message->at(1)]);
    }
}

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPortIn(RtMidiIn *rtmidi);
bool chooseMidiPortOut(RtMidiOut *rtmidi);

int main(int argc, char ** /*argv[]*/)
{

    // RtMidiOut constructor
    try
    {
        midiout = new RtMidiOut();
    }
    catch (RtMidiError &error)
    {
        error.printMessage();
    }

    // Call function to select port.
    try
    {
        if (chooseMidiPortOut(midiout) == false)
            goto cleanup;
    }
    catch (RtMidiError &error)
    {
        error.printMessage();
        goto cleanup;
    }

    // Turn off all pedals
    for (int iter = 0; iter < LENGTH; iter++)
    {

        std::vector<unsigned char> message;

        message.push_back(176);
        message.push_back(iter);
        message.push_back(0);
        midiout->sendMessage(&message);
        SLEEP(5);
    }

    try
    {

        // RtMidiIn constructor
        midiin = new RtMidiIn();

        // Call function to select port.
        if (chooseMidiPortIn(midiin) == false)
            goto cleanup;

        // Set our callback function.  This should be done immediately after
        // opening the port to avoid having incoming messages written to the
        // queue instead of sent to the callback function.
        midiin->setCallback(&mycallback);

        // Don't ignore sysex, timing, or active sensing messages.
        midiin->ignoreTypes(false, false, false);

        std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
        char input;
        std::cin.get(input);
    }
    catch (RtMidiError &error)
    {
        error.printMessage();
    }

cleanup:

    delete midiin;

    return 0;
}

bool chooseMidiPortIn(RtMidiIn *rtmidi)
{
    std::string keyHit;

    std::string portName;
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if (nPorts == 0)
    {
        std::cout << "No input ports available!" << std::endl;
        return false;
    }

    if (nPorts == 1)
    {
        std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
    }
    else
    {
        for (i = 0; i < nPorts; i++)
        {
            portName = rtmidi->getPortName(i);
            std::cout << "  Input port #" << i << ": " << portName << '\n';
        }

        do
        {
            std::cout << "\nChoose a port number: ";
            std::cin >> i;
        } while (i >= nPorts);
        std::getline(std::cin, keyHit); // used to clear out stdin
    }

    rtmidi->openPort(i);

    return true;
}

bool chooseMidiPortOut(RtMidiOut *rtmidi)
{

    std::string keyHit;

    std::string portName;
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if (nPorts == 0)
    {
        std::cout << "No output ports available!" << std::endl;
        return false;
    }

    if (nPorts == 1)
    {
        std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
    }
    else
    {
        for (i = 0; i < nPorts; i++)
        {
            portName = rtmidi->getPortName(i);
            std::cout << "  Output port #" << i << ": " << portName << '\n';
        }

        do
        {
            std::cout << "\nChoose a port number: ";
            std::cin >> i;
        } while (i >= nPorts);
    }

    std::cout << "\n";
    rtmidi->openPort(i);

    return true;
}