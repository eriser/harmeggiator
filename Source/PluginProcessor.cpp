/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HarmeggiatorAudioProcessor::HarmeggiatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
      arpSpeed (0.1),
      arpPattern (patternDown),
      isItTrained(false)
#endif
{
}

HarmeggiatorAudioProcessor::~HarmeggiatorAudioProcessor()
{
}

//==============================================================================
const String HarmeggiatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmeggiatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HarmeggiatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double HarmeggiatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmeggiatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HarmeggiatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmeggiatorAudioProcessor::setCurrentProgram (int index)
{
}

const String HarmeggiatorAudioProcessor::getProgramName (int index)
{
    return String();
}

void HarmeggiatorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HarmeggiatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused (samplesPerBlock);
    
    notes.clear();
    currentNote = 0;
    lastNoteValue = -1;
    time = 0.0;
    rate = static_cast<float> (sampleRate);
}

void HarmeggiatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmeggiatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HarmeggiatorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midi)
{
    // the audio buffer in a midi effect will have zero channels!
    jassert (buffer.getNumChannels() == 0);
    
    // however we use the buffer to get timing information
    const int numSamples = buffer.getNumSamples();
    
    // get note duration
    const int noteDuration = static_cast<int> (std::ceil (rate * 0.25f * (0.1f + (1.0f - (arpSpeed)))));
    
    MidiMessage msg;
    int ignore;
    
    for (MidiBuffer::Iterator it (midi); it.getNextEvent (msg, ignore);)
    {
        if      (msg.isNoteOn() && notes.size() < 3)  notes.add (msg.getNoteNumber());
        else if (msg.isNoteOff()) notes.removeValue (msg.getNoteNumber());
    }
    
    intervals.clear();
    for (int i = 0; i < notes.size() - 1; ++i)
    {
        const int interval = notes[i + 1] - notes[i];
        intervals.add (interval);
    }
    
    midi.clear();
    
    if (prevIntervals != intervals) {
        if (isItTrained == true && intervals.size() == 2)
        {
            Array <int> output = classificationObject.process(intervals);
            arpSpeed = output[0] / 1000.0;
            arpPattern = output[1];
            std::cout << "input: " << intervals[0] << intervals[1] << std::endl;
            std::cout << "output: " << output[0] << output[1] << std::endl;
        }
        prevIntervals = intervals;
    }

    if ((time + numSamples) >= noteDuration)
    {
        const int offset = jmax (0, jmin ((int) (noteDuration - time), numSamples - 1));
        
        if (lastNoteValue > 0)
        {
            midi.addEvent (MidiMessage::noteOff (1, lastNoteValue), offset);
            lastNoteValue = -1;
        }
        
        if (notes.size() > 0)
        {
            if (arpPattern == patternUp) currentNote++;
            else if (arpPattern == patternDown) currentNote--;
            else jassertfalse; // oops!
            
            if (currentNote >= notes.size()) currentNote = 0;
            if (currentNote < 0) currentNote = notes.size() - 1;
            
            lastNoteValue = notes[currentNote];
            midi.addEvent (MidiMessage::noteOn  (1, lastNoteValue, (uint8) 127), offset);
        }
        
    }
    
    time = (time + numSamples) % noteDuration;
}

//==============================================================================
bool HarmeggiatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HarmeggiatorAudioProcessor::createEditor()
{
    return new HarmeggiatorAudioProcessorEditor (*this);
}

//==============================================================================
void HarmeggiatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HarmeggiatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmeggiatorAudioProcessor();
}
