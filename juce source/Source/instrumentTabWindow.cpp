/*
  ==============================================================================

    instrumentTabWindow.cpp
    Created: 9 May 2014 6:26:59pm
    Author:  patrick

  ==============================================================================
*/

#include "instrumentTabWindow.h"

instrumentTabWindow::instrumentTabWindow(TabbedButtonBar::Orientation orientation) : TabbedComponent(orientation){
    addTab("Main", Colour(100, 100, 100), nullptr, false);
    addTab("FX", Colour(100, 100, 100), nullptr, false);
    addTab("Mapping", Colour(100, 100, 100), mapping_editor = new instrumentMappingEditor("", this), false);
    addTab("Wave", Colour(100, 100, 100), nullptr, false);
    addTab("Mod", Colour(100, 100, 100), nullptr, false);
    
    //addAndMakeVisible(zoom_button = new TextButton("zoom"));
    //zoom_button->setBounds(0, -30, 20, 20);
}

instrumentTabWindow::~instrumentTabWindow(){
    mapping_editor = nullptr;
    zoom_button = nullptr;
}