/*
  ==============================================================================

    instrumentMasterComponent.cpp
    Created: 9 May 2014 6:25:05pm
    Author:  patrick

  ==============================================================================
*/

#include "instrumentMasterComponent.h"


instrumentMasterComponent::instrumentMasterComponent(TabbedComponent* Parent) 
: Component(), parent(Parent){
    instrument_label = new instrumentMasterComponent::instrumentLabel("component name", "New Instrument", parent);
    instrument_label->setEditable(false, true, true);
    instrument_label->setJustificationType(Justification::centred);
    addAndMakeVisible(instrument_label);

    x_button = new instrumentButton("close", Colour(50, 50, 50), Colour(60, 60, 60), Colour(0, 20, 90), parent);
    x_button->addListener(this);
    addAndMakeVisible(x_button);
}

instrumentMasterComponent::~instrumentMasterComponent(){
    instrument_label = nullptr;
    x_button = nullptr;
}

void instrumentMasterComponent::paint (Graphics& g){
    g.fillAll (Colours::grey);

    //g.setFont (Font (16.0f));
    //g.setColour (Colours::blue);
    //g.drawText ("Instrument Controls", getLocalBounds(), Justification::centred, true);
}

void instrumentMasterComponent::resized(){
    instrument_label->setBounds(0, 0, getWidth(), getHeight());
    x_button->setBounds(getWidth() - 16, 4, 12, 12);
}

void instrumentMasterComponent::buttonClicked(Button* button){
    if (button == x_button){
        parent->removeTab(parent->getCurrentTabIndex());
    }
}

void instrumentMasterComponent::instrumentLabel::textWasEdited(){
    this->parent->setTabName(this->parent->getCurrentTabIndex(), this->getText());
}

void instrumentMasterComponent::instrumentButton::paintButton(Graphics& g, bool isMouseOver, bool isButtonDown){
    Path myPath;
    myPath.startNewSubPath (0.0f, 0.0f);          // move the current position to (10, 10)
    myPath.lineTo (12.0f, 12.0f);                 // draw a line from here to (100, 200)
    myPath.startNewSubPath (12.0f, 0.0f);
    myPath.lineTo (0.0f, 12.0f);

    g.strokePath (myPath, PathStrokeType (2.0f));
}