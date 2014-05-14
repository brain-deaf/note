/*
  ==============================================================================

    menuBar.h
    Created: 7 May 2014 6:15:08pm
    Author:  patrick

  ==============================================================================
*/

#ifndef MENUBAR_H_INCLUDED
#define MENUBAR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "instrumentBin.h"

class menuBar : public MenuBarModel, public Component
{
public:
    menuBar();
    ~menuBar();
    
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName);
    void menuItemSelected(int menuItemID, int topLevelMenuIndex);
    void resized();
    void set_parent_instrument_bin(instrumentBin* parent_instrument_bin);
    
    enum menuIDs{
        ID_New = 1000,
        ID_Save,  
        ID_Quit,  
        ID_View1, 
        ID_Edit1, 
    };
private:
    MenuBarComponent menu_bar_component;
    PopupMenu menu_file;
    PopupMenu menu_view;
    PopupMenu menu_edit;
    instrumentBin* parent_instrument_bin;
};



#endif  // MENUBAR_H_INCLUDED
