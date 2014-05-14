/*
  ==============================================================================

    instrumentComponent.cpp
    Created: 9 May 2014 6:12:25pm
    Author:  patrick

  ==============================================================================
*/

#include "instrumentComponent.h"

instrumentComponent::instrumentComponent(TabbedComponent* Parent)
: parent(Parent), Component(){
    
    tabbed_component = new instrumentTabWindow(TabbedButtonBar::TabsAtBottom);
    instrument_master_component = new instrumentMasterComponent(parent);
    
    addAndMakeVisible(tabbed_component);
    addAndMakeVisible(instrument_master_component);
}

instrumentComponent::~instrumentComponent(){
    tabbed_component = nullptr;
    instrument_master_component = nullptr;
}

void instrumentComponent::resized(){
    tabbed_component->setBounds(0, 80, getWidth(), getHeight() - 80);
    instrument_master_component->setBounds(0, 0, getWidth(), 80);
}

/*void instrumentComponent::paint (Graphics& g){
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::red);
    g.drawText ("Hello Worlds!", getLocalBounds(), Justification::centred, true);
}*/
