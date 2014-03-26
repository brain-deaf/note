from gi.repository import Gtk, Gdk
import cairo
import math

class MappingEditorGrid(Gtk.DrawingArea):
	def __init__(self, parent, grid_width, grid_height):
		Gtk.DrawingArea.__init__(self)
		self.set_valign(Gtk.Align.START)
		self.set_halign(Gtk.Align.START)
		self._parent = parent
		
		#this widget draws the boundaries of the cells, so let's say you want to draw a vertical line
		#at the right boundary of the grid, you'd have have to position the pencil at 
		#cell_width * (grid_width + 1), so that's why there are random +1s, because we have to draw BOUNDARIES 
		self.width   = (self._parent.mapping_editor.grid_width + 2) * self._parent.cell_width
		self.height  = (self._parent.mapping_editor.grid_height + 2) * self._parent.cell_height
		self.set_size_request(self.width, self.height)

		self.grid_width  = self._parent.mapping_editor.grid_width + 1
		self.grid_height = self._parent.mapping_editor.grid_height + 1
		self.cell_width  = self._parent.cell_width
		self.cell_height = self._parent.cell_height
	
		#i don't think we need this anymore?
		self.grid_init    = True
		
		#connets the Draw signal to the function on_drawing_area_draw()
		self.connect('draw', self.on_drawing_area_draw)
		#manually triggers the on_drawing_area_draw() callback
		self.queue_draw()

	def on_drawing_area_draw(self, drawing_area, cc):
		#cc stands for 'cairo context'
		#sets pixel width of lines
		cc.set_line_width(2)
		#draws vertical lines of the grid starting from the top and moving down
		for i in range(1, self.grid_width + 1):
			cc.move_to(round(i * self.cell_width), self.cell_height)
			cc.line_to(round(i * self.cell_width), self.cell_height * self.grid_height)
			
		#draws the top edge of the grid
		cc.move_to(1 * self.cell_width , self.cell_height)
		cc.line_to(round(self.grid_width * self.cell_width), self.cell_height)
		#draws the bottom edget of the grid
		cc.move_to(1 * self.cell_width, self.cell_height * self.grid_height)
		cc.line_to(round(self.grid_width * self.cell_width), self.cell_height * self.grid_height)
		#paints the drawing onto the widget, this is the function that actually shows the drawing you've done.
		cc.stroke()
		cc.set_line_width(1)
		#draws horizontal lines of grid and paints them
		for i in range(1, self.grid_height-1):
			cc.move_to(1 * self.cell_width, self.cell_height * (i + 1))
			cc.line_to(round(self.grid_width * self.cell_width), self.cell_height * (i + 1))
		cc.stroke()

#this widget handles cairo drawing triggered by MIDI input
class MappingOverlay(Gtk.DrawingArea):
	def __init__(self, parent, grid_width, grid_height):
		Gtk.DrawingArea.__init__(self)
		self._parent = parent
		self.set_valign(Gtk.Align.START)
		self.set_halign(Gtk.Align.START)
		self._parent = parent
		self.width   = (self._parent.mapping_editor.grid_width + 2) * self._parent.cell_width
		self.height  = (self._parent.mapping_editor.grid_height + 2) * self._parent.cell_height
		self.set_size_request(self.width, self.height)

		self.set_property("opacity", 0.5)

		self.grid_width  = self._parent.mapping_editor.grid_width + 1
		self.grid_height = self._parent.mapping_editor.grid_height + 1
		self.cell_width  = self._parent.cell_width
		self.cell_height = self._parent.cell_height

		#list of notes that should be drawn
		self.notes = []
		self.connect('draw', self.on_drawing_area_draw)

	def draw_midi_note(self, note_on):
		#adds note to the list when receiving note_on
		self.notes.append(note_on)
		#manually triggers the on_drawing_area_draw() callback
		self.queue_draw()

	def draw_release_midi_note(self, note_off):
		released_note = note_off[0]
		for note_on in self.notes:
			if note_on[0] == released_note:
				self.notes.remove(note_on)
				#removes note from list if it is released
		self.queue_draw()

	def on_drawing_area_draw(self, drawing_area, cc):
		#sets the color of drawing to black
		cc.set_source_rgb(0, 0, 0)
		#sets width of line
		cc.set_line_width(self.cell_width)
	
		#fills in a specific cell as black for every note in the list
		for n in self.notes:
			note     = n[0] + 1
			velocity = n[1]
			cc.move_to(math.floor(note * self.cell_width + self.cell_width/2), self.cell_height * (128 - velocity))
			cc.line_to(math.floor(note * self.cell_width + self.cell_width/2), self.cell_height * (128 - (velocity - 1)))
		cc.stroke()


