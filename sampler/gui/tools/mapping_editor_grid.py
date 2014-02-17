from gi.repository import Gtk, Gdk
import math

class MappingEditorGrid(Gtk.DrawingArea):
	def __init__(self, parent, grid_width, grid_height):
		Gtk.DrawingArea.__init__(self)
		#self.set_property("expand", True)
		self.set_valign(Gtk.Align.START)
		self.set_halign(Gtk.Align.START)
		self._parent = parent
		#self.set_size_request(760, 600)
		self.width  = (self._parent.mapping_editor.grid_width + 2) * self._parent.cell_width
		self.height = (self._parent.mapping_editor.grid_height + 2) * self._parent.cell_height
		self.set_size_request(self.width, self.height)


		self.grid_width = self._parent.mapping_editor.grid_width + 1
		self.grid_height = self._parent.mapping_editor.grid_height + 1
		self.cell_width = self._parent.cell_width
		self.cell_height = self._parent.cell_height
		
		self.connect('draw', self.on_drawing_area_draw)
		self.queue_draw()

	def on_drawing_area_draw(self, drawing_area, cairo_context):
		cairo_context.set_line_width(2)
		for i in range(1, self.grid_width + 1):
			cairo_context.move_to(round(i * self.cell_width), self.cell_height * 1)
			cairo_context.line_to(round(i * self.cell_width), self.cell_height * self.grid_height)
		cairo_context.move_to(1 * self.cell_width , self.cell_height * 1)
		cairo_context.line_to(round(self.grid_width * self.cell_width), self.cell_height * 1)
		cairo_context.move_to(1 * self.cell_width, self.cell_height * self.grid_height)
		cairo_context.line_to(round(self.grid_width * self.cell_width), self.cell_height * self.grid_height)
		cairo_context.stroke()
		cairo_context.set_line_width(1)
		for i in range(1, self.grid_height-1):
			cairo_context.move_to(1 * self.cell_width, self.cell_height * (i + 1))
			cairo_context.line_to(round(self.grid_width * self.cell_width), self.cell_height * (i + 1))
		cairo_context.stroke()
