from gi.repository import Gtk
from mapping_editor_grid import MappingEditorGrid, MappingOverlay


class TestApp(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self)
        self.connect('delete-event',Gtk.main_quit)
        self.cell_width  = 14
        self.cell_height = 13
        self.grid_width  = 127
        self.grid_height = 127

        self.grid_drawing = MappingEditorGrid(self,self.grid_width, self.grid_height)
        self.grid_overlay = MappingOverlay(self,self.grid_width,self.grid_height)

        self.grid_overlay.show_all()

        self.show_all()

win = TestApp()

Gtk.main()
