/*
  ==============================================================================

    instrumentMasterComponent.h
    Created: 9 May 2014 6:24:36pm
    Author:  patrick

  ==============================================================================
*/

#ifndef INSTRUMENTMASTERCOMPONENT_H_INCLUDED
#define INSTRUMENTMASTERCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "instrumentBin.h"

class instrumentMasterComponent : public Component, public Button::Listener
{
public:

    class instrumentLabel;
    class instrumentButton;

    instrumentMasterComponent(TabbedComponent* parent);
    ~instrumentMasterComponent();
    void paint(Graphics& g);
    void resized();
    void buttonClicked(Button* button);
private:
    TabbedComponent* parent;
    ScopedPointer<Label> instrument_label;
    ScopedPointer<ShapeButton> x_button;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (instrumentMasterComponent)
};

class instrumentMasterComponent::instrumentLabel : public Label
{
public:
    instrumentLabel(const String& componentName, const String& labelText, TabbedComponent* Parent) 
    : Label(componentName, labelText){ parent = Parent;};

    void textWasEdited();
private:
    TabbedComponent* parent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (instrumentLabel)
};

class instrumentMasterComponent::instrumentButton : public ShapeButton
{
public:
    instrumentButton(const String& name, Colour normalColour, Colour overColour, Colour downColour, TabbedComponent* Parent) 
    : ShapeButton(name, normalColour, overColour, downColour){ parent = Parent;};

    void paintButton(Graphics& g, bool isMouseOver, bool isButtonDown);
private:
    TabbedComponent* parent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (instrumentButton)
};



#endif  // INSTRUMENTMASTERCOMPONENT_H_INCLUDED
