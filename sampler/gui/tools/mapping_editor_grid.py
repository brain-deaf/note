from gi.repository import Gtk
from math import floor

class MappingEditorGrid(Gtk.DrawingArea):
	def __init__(self, parent, grid_width, grid_height):
		Gtk.DrawingArea.__init__(self)
		self.set_valign(Gtk.Align.START)
		self.set_halign(Gtk.Align.START)
		self._parent = parent
		self.width   = (grid_width + 2) * self._parent.cell_width
		self.height  = (grid_height + 2) * self._parent.cell_height
		self.set_size_request(self.width, self.height)

		self.grid_width  = grid_width + 1
		self.grid_height = grid_height + 1
		self.cell_width  = self._parent.cell_width
		self.cell_height = self._parent.cell_height
	
		self.grid_init    = True
		
		self.connect('draw', self.on_drawing_area_draw)
		self.queue_draw()

	def on_drawing_area_draw(self, drawing_area, cc):
		cc.set_line_width(2)
		for i in range(1, self.grid_width + 1):
			cc.move_to(round(i * self.cell_width), self.cell_height * 1)
			cc.line_to(round(i * self.cell_width), self.cell_height * self.grid_height)
		cc.move_to(1 * self.cell_width , self.cell_height * 1)
		cc.line_to(round(self.grid_width * self.cell_width), self.cell_height * 1)
		cc.move_to(1 * self.cell_width, self.cell_height * self.grid_height)
		cc.line_to(round(self.grid_width * self.cell_width), self.cell_height * self.grid_height)
		cc.stroke()
		cc.set_line_width(1)
		for i in range(1, self.grid_height-1):
			cc.move_to(1 * self.cell_width, self.cell_height * (i + 1))
			cc.line_to(round(self.grid_width * self.cell_width), self.cell_height * (i + 1))
		cc.stroke()

class MappingOverlay(Gtk.DrawingArea):
	def __init__(self, parent, grid_width, grid_height):
		Gtk.DrawingArea.__init__(self)
		self._parent = parent
		self.set_valign(Gtk.Align.START)
		self.set_halign(Gtk.Align.START)
		self._parent = parent
		self.width   = (grid_width + 2) * self._parent.cell_width
		self.height  = (grid_height + 2) * self._parent.cell_height
		self.set_size_request(self.width, self.height)

		self.set_property("opacity", 0.5)

		self.grid_width  = grid_width + 1
		self.grid_height = grid_height + 1
		self.cell_width  = self._parent.cell_width
		self.cell_height = self._parent.cell_height

		self.notes = []
		self.connect('draw', self.on_drawing_area_draw)

	def draw_midi_note(self, note_on):
		self.notes.append(note_on)
		self.queue_draw()

	def draw_release_midi_note(self, note_off):
		released_note = note_off[0]
		for note_on in self.notes:
			if note_on[0] == released_note:
				self.notes.remove(note_on)
		self.queue_draw()

	def on_drawing_area_draw(self, drawing_area, cc):
		cc.set_source_rgb(0, 0, 0)
		cc.set_line_width(self.cell_width)
	
		for n in self.notes:
			note     = n[0] + 1
			velocity = n[1]
			cc.move_to(floor(note * self.cell_width + self.cell_width/2), self.cell_height * (128 - velocity))
			cc.line_to(floor(note * self.cell_width + self.cell_width/2), self.cell_height * (128 - (velocity - 1)))
		cc.stroke()


