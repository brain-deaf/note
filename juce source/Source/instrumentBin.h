/*
  ==============================================================================

    instrumentBin.h
    Created: 8 May 2014 6:58:34pm
    Author:  patrick

  ==============================================================================
*/

#ifndef INSTRUMENTBIN_H_INCLUDED
#define INSTRUMENTBIN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "instrumentComponent.h"
//#include "instrumentTabWindow.h"

class instrumentBin : public TabbedComponent
{
public:
    instrumentBin(TabbedButtonBar::Orientation orientation);
    ~instrumentBin();
    void register_tab(instrumentComponent* c);
    
    //void resized();
private:
    instrumentComponent* instrument_component;
    Array<instrumentComponent*> instruments;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (instrumentBin)
};



#endif  // INSTRUMENTBIN_H_INCLUDED
