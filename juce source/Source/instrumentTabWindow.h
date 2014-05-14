/*
  ==============================================================================

    instrumentTabWindow.h
    Created: 9 May 2014 6:26:40pm
    Author:  patrick

  ==============================================================================
*/

#ifndef INSTRUMENTTABWINDOW_H_INCLUDED
#define INSTRUMENTTABWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "instrumentMappingEditor.h"

class instrumentTabWindow : public TabbedComponent
{
public:
    instrumentTabWindow(TabbedButtonBar::Orientation orientation);
    ~instrumentTabWindow();
private:
    ScopedPointer<instrumentMappingEditor> mapping_editor;
    ScopedPointer<TextButton> zoom_button;
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (instrumentTabWindow)
};



#endif  // INSTRUMENTTABWINDOW_H_INCLUDED
