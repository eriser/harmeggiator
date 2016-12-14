/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class HarmeggiatorAudioProcessorEditor  : public AudioProcessorEditor, private Button::Listener, private Slider::Listener
{
public:
    HarmeggiatorAudioProcessorEditor (HarmeggiatorAudioProcessor&);
    ~HarmeggiatorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;
    void setLabels();
    bool keyPressed(const KeyPress &key) override;
    bool keyStateChanged(bool isKeyDown) override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HarmeggiatorAudioProcessor& processor;

    //==============================================================================
    ScopedPointer<TextButton> buttonTraining, arpUp, arpDown;//, buttonClearTraining, buttonRun, buttonStop;
    ScopedPointer<Slider> arpSpeedSlider;
    ScopedPointer<Label> labelInformation, labelTrainingSet;
    

    
    bool canWaitForOtherKeypresses;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmeggiatorAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
