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
}

void HarmeggiatorAudioProcessorEditor::setLabels()
{
    String strTrainingSet;
    strTrainingSet = String(trainingSet.size()) + " sample" + (trainingSet.size()>1 ? "s" : "") + " in the training set";
    labelTrainingSet->setText(strTrainingSet, dontSendNotification);
    
    String strInformation;
    strInformation = "To train the classification algorithm, select a pattern LED, put your cursor in the mouse zone, "
    "and push the space bar.";
    labelInformation->setText(strInformation, dontSendNotification);
    
    strInformation = "When you have three or more samples in your training set, click on the Training button and then on the Run button.";
}

void HarmeggiatorAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == buttonRun)
    {
        // insert logic here
    }
    else if (button == buttonStop)
    {
        // insert logic here
    }
    else if (button == buttonTraining)
    {
        if (trainingSet.size() > 2)
        {
            classificationObject.train(trainingSet);
            setLabels();
        }
        else
        {
            
        }
    }
}

bool HarmeggiatorAudioProcessorEditor::keyPressed(const KeyPress &key)
{
    if (key.getKeyCode() == KeyPress::spaceKey && canWaitForOtherKeypresses)
    {
//        Array <double> input = 
        
        Classification::DataSample example;
//        example.inputs.addArray({ input[0], input[1] });
//        example.outputs.addArray({ output[0], output[1] });
        
        trainingSet.add(example);
        setLabels();
        
        canWaitForOtherKeypresses = false;
    }
    
    return true;
}

bool HarmeggiatorAudioProcessorEditor::keyStateChanged(bool isKeyDown)
{
    if (isKeyDown == false && KeyPress(KeyPress::spaceKey).isCurrentlyDown() == false)
        canWaitForOtherKeypresses = true;
    
    return true;
}
