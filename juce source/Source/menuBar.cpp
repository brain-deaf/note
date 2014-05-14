/*
  ==============================================================================

    menuBar.cpp
    Created: 7 May 2014 6:14:56pm
    Author:  patrick

  ==============================================================================
*/

#include "menuBar.h"

menuBar::menuBar() : menu_bar_component(this), menu_file(), menu_view(), menu_edit(){
    addAndMakeVisible(&menu_bar_component);
    
    menu_file.addItem(ID_New, "New Instrument");
    menu_file.addItem(ID_Save, "Save Instrument");
    menu_file.addItem(ID_Quit, "Quit");
    menu_view.addItem(ID_View1, "Nothing Here");
    menu_edit.addItem(ID_Edit1, "Nothing Here");
}

menuBar::~menuBar(){}
    
StringArray menuBar::getMenuBarNames(){
    StringArray s;
    s.add("File");
    s.add("View");
    s.add("Edit");
    
    return s;
}

PopupMenu menuBar::getMenuForIndex(int topLevelMenuIndex, const String& menuName){
    if (menuName == "File"){
        return menu_file;
    }
    
    if (menuName == "View"){
        return menu_view;
    }
    
    if (menuName == "Edit"){
        return menu_edit;
    }
}

void menuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex){
    if (menuItemID == ID_Quit){
        JUCEApplication::quit();
    }
    if (menuItemID == ID_New){
        instrumentComponent* i = new instrumentComponent(parent_instrument_bin);
        parent_instrument_bin->addTab("New Instrument", Colour(100, 100, 100), i, false);
        parent_instrument_bin->register_tab(i);
    }
}

void menuBar::resized(){
    menu_bar_component.setBounds(0, 0, getWidth(), 20);
}

void menuBar::set_parent_instrument_bin(instrumentBin* bin){
    parent_instrument_bin = bin;
}