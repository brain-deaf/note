/*
  ==============================================================================

    instrumentComponent.h
    Created: 9 May 2014 6:12:43pm
    Author:  patrick

  ==============================================================================
*/

#ifndef INSTRUMENTCOMPONENT_H_INCLUDED
#define INSTRUMENTCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "instrumentTabWindow.h"
#include "instrumentMasterComponent.h"

class instrumentComponent : public Component
{
public:
    instrumentComponent(TabbedComponent* Parent);
    ~instrumentComponent();
    void resized();
    //void paint(Graphics& g);
private:
    ScopedPointer<instrumentTabWindow> tabbed_component;
    ScopedPointer<instrumentMasterComponent> instrument_master_component;
    TabbedComponent* parent;
    //ScopedPointer<TextButton> text_button;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (instrumentComponent)
};



#endif  // INSTRUMENTCOMPONENT_H_INCLUDED
