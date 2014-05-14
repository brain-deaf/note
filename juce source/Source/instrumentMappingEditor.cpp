/*
  ==============================================================================

    instrumentMappingEditor.cpp
    Created: 11 May 2014 11:31:53am
    Author:  patrick

  ==============================================================================
*/

#include "instrumentMappingEditor.h"

instrumentMappingEditor::instrumentMappingEditor(const String& componentName, Component* Parent)
: parent(Parent), Viewport(componentName){
    setViewedComponent(graph = new mappingEditorGraph());
    
    graph->width = 1800.0f;
    graph->height = 450.0f;
    graph->num_columns = 128;
    graph->setBounds(0, 0, graph->width, graph->height);
}

instrumentMappingEditor::~instrumentMappingEditor(){
    graph = nullptr;
}



instrumentMappingEditor::mappingEditorGraph::mappingEditorGraph()
: Component(), dragged_zone(nullptr){
    
    //lasso_source.set->addChangeListener(lasso_source);
    lasso = new LassoComponent<Zone*>;
    lasso_source = new Lasso_Source<Zone*>;
    lasso_source->set = new SelectedItemSet<Zone*>;
}

instrumentMappingEditor::mappingEditorGraph::~mappingEditorGraph(){
    for (int i=0; i<zones.size(); i++){
        delete zones[i];
    }
    lasso_source = nullptr;
}

void instrumentMappingEditor::mappingEditorGraph::paint(Graphics& g){
    g.setImageResamplingQuality(Graphics::highResamplingQuality);
    
    g.fillAll (Colours::antiquewhite);
    
    float grid_outline = 1.0f;
    float grid_width = width / num_columns;
    
    Path myPath;
    for (int i=0; i<this->num_columns; i++){
        myPath.startNewSubPath (i*grid_width, 0.0f);       
        myPath.lineTo (i*grid_width, height);                
    }

    g.strokePath (myPath, PathStrokeType (grid_outline));
}



void instrumentMappingEditor::mappingEditorGraph::filesDropped(const StringArray& files, int x, int y){
    float grid_outline = 1.0f;
    float grid_width = width / num_columns;
    
    for (int i=0; i<files.size(); i++){
        Zone* new_zone;
        zones.add(new_zone = new Zone(files[i]));
        
        new_zone->removeListener(this);
        new_zone->addListener(this);
        new_zone->removeMouseListener(new_zone);
        new_zone->addMouseListener(this, true);

        new_zone->x = ((int)(x / grid_width)) * grid_width + grid_outline + grid_width*i;
        new_zone->y = 0;
        new_zone->height = height;
        new_zone->register_parent(this);
        
        new_zone->setBounds(new_zone->x, new_zone->y, grid_width - grid_outline, new_zone->height);
        addAndMakeVisible(new_zone);
        
        lasso_source->zones_.add(new_zone);
    }
}

void instrumentMappingEditor::mappingEditorGraph::buttonClicked(Button* button){
    //dragged_zone = (Zone*)button;
    //std::cout<<"dragged zone: "<<dragged_zone<<std::endl;
}

void instrumentMappingEditor::mappingEditorGraph::mouseDown(const MouseEvent& e){
    addAndMakeVisible(lasso);
    lasso->beginLasso(e, lasso_source);
}

void instrumentMappingEditor::mappingEditorGraph::mouseDrag(const MouseEvent& e){
    if (dragged_zone != nullptr){
        lasso_source->dragging = true;
        float grid_outline = 1.0f;
        float grid_width = width / num_columns;
        
        int grid_x_offset = (int)(e.x / grid_width);
        
        if (lasso_source->set->getItemArray().contains(dragged_zone)){
            for (Zone** i = lasso_source->set->begin(); i<lasso_source->set->end(); i++){
                int grid_x = (int)((*i)->x / grid_width);
                int new_grid_x = grid_x_offset + grid_x;
    
                if (new_grid_x >= 0 and new_grid_x < num_columns){
                    (*i)->x = new_grid_x * grid_width + grid_outline; 
                    (*i)->setBounds((*i)->x, (*i)->y, grid_width - grid_outline, (*i)->height);
                }
            }
        }else{   
            int grid_x = (int)(dragged_zone->x / grid_width);
            int new_grid_x = grid_x_offset + grid_x;
    
            if (new_grid_x >= 0 and new_grid_x < num_columns){
                dragged_zone->x = new_grid_x * grid_width + grid_outline; 
                dragged_zone->setBounds(dragged_zone->x, dragged_zone->y, grid_width - grid_outline, dragged_zone->height);
            }
        }
    }else{
        lasso->dragLasso(e);
    }
}

void instrumentMappingEditor::mappingEditorGraph::mouseUp(const MouseEvent& e){
    dragged_zone = nullptr;
    lasso->endLasso();
    //lasso = nullptr;
    
    if (!lasso_source->dragging){
        for (Zone** i = lasso_source->set->begin(); i<lasso_source->set->end(); i++){
            (*i)->setToggleState(false, 1);
        }
        lasso_source->set->deselectAll();
    }
    lasso_source->dragging = false;
}

void instrumentMappingEditor::mappingEditorGraph::Zone::register_parent(instrumentMappingEditor::mappingEditorGraph* c){parent=c;}

void instrumentMappingEditor::mappingEditorGraph::Zone::mouseDown(const MouseEvent& e){
    parent->dragged_zone = this;
    this->clicked();
}

