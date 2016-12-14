/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HarmeggiatorAudioProcessorEditor::HarmeggiatorAudioProcessorEditor (HarmeggiatorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    addAndMakeVisible(buttonTraining = new TextButton("Learn"));
    buttonTraining->addListener(this);
    
    addAndMakeVisible(arpUp = new TextButton("Up"));
    arpUp->addListener(this);
    
    addAndMakeVisible(arpDown = new TextButton("Down"));
    arpDown->addListener(this);
    
    addAndMakeVisible(arpSpeedSlider = new Slider("Speed"));
    arpSpeedSlider->setRange(0.0,1.0);
    arpSpeedSlider->addListener(this);
    
    addAndMakeVisible(labelInformation = new Label("", ""));
    addAndMakeVisible(labelTrainingSet = new Label("", ""));
    setLabels();
    
    setWantsKeyboardFocus(true);
    setExplicitFocusOrder(1);
    
    

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
}

HarmeggiatorAudioProcessorEditor::~HarmeggiatorAudioProcessorEditor()
{
}

//==============================================================================
void HarmeggiatorAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void HarmeggiatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    Rectangle<int> r (getLocalBounds());
    
    buttonTraining->setBounds (r.removeFromLeft(r.getWidth() / 2).removeFromTop(r.getHeight()/2).reduced (10));
    
    Rectangle<int> radioArea (r.removeFromTop(r.getHeight() / 2).reduced (10));
    
    const int numRadioButtons (2);
    const int radioButtonWidth (radioArea.getWidth() / numRadioButtons);
    
    arpUp->setBounds (radioArea.removeFromLeft (radioButtonWidth).reduced(5));
    arpDown->setBounds (radioArea.removeFromLeft (radioButtonWidth).reduced(5));

    r = getLocalBounds().removeFromBottom(r.getHeight()/2);
    
    arpSpeedSlider->setBounds (r.removeFromTop(r.getHeight()/2).reduced (10));
    
    labelTrainingSet->setBounds (r.removeFromTop(r.getHeight()/2));
    labelInformation->setBounds (r);
}

void HarmeggiatorAudioProcessorEditor::setLabels()
{
    String strTrainingSet;
    strTrainingSet = String(processor.trainingSet.size()) + " sample" + (processor.trainingSet.size()>1 ? "s" : "") + " in the training set";
    labelTrainingSet->setText(strTrainingSet, dontSendNotification);
    
    String strInformation;
    strInformation = "To train the classifier, select your arp params, play a chord shape, "
    "and push the space bar.";
    labelInformation->setText(strInformation, dontSendNotification);
    
    strInformation = "When you have three or more samples in your training set, click on the Learn button.";
}

void HarmeggiatorAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == arpUp)
    {
        processor.arpPattern = processor.patternUp;
    }
    else if (button == arpDown)
    {
        processor.arpPattern = processor.patternDown;
    }
    else if (button == buttonTraining)
    {
        if (processor.trainingSet.size() > 2)
        {
            processor.classificationObject.train(processor.trainingSet);
            setLabels();
            processor.isItTrained = true;
        }
        else
        {
            
        }
    }
}

void HarmeggiatorAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == arpSpeedSlider)
    {
        processor.arpSpeed = arpSpeedSlider->getValue();
    }
}

bool HarmeggiatorAudioProcessorEditor::keyPressed(const KeyPress &key)
{
    if (key.getKeyCode() == KeyPress::spaceKey && canWaitForOtherKeypresses)
    {
        if (processor.intervals.size() == 2)
        {
            Classification::DataSample example;
        
            std::cout << "training example: " << processor.intervals[0] << processor.intervals[1] << std::endl;
            example.inputs.addArray(processor.intervals);
            example.outputs.addArray({ static_cast<int> (1000 * processor.arpSpeed), processor.arpPattern });
            
            processor.trainingSet.add(example);
            setLabels();
            
            canWaitForOtherKeypresses = false;
        }
    }
    
    return true;
}

bool HarmeggiatorAudioProcessorEditor::keyStateChanged(bool isKeyDown)
{
    if (isKeyDown == false && KeyPress(KeyPress::spaceKey).isCurrentlyDown() == false)
        canWaitForOtherKeypresses = true;
    
    return true;
}
