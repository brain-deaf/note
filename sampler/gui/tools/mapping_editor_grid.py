from gi.repository import Gtk, Gdk
import math

class MappingEditorGrid(Gtk.DrawingArea):
	def __init__(self, grid_width, grid_height):
		Gtk.DrawingArea.__init__(self)
		#self.set_property("expand", True)
		self.set_valign(Gtk.Align.START)
		self.set_halign(Gtk.Align.START)
		self.set_size_request(760, 600)
		self.width  = 760
		self.height = 600

		self.grid_width = grid_width
		self.grid_height = grid_height + 2
		self.cell_width = self.width / grid_width
		print(self.cell_width)
		self.cell_height = math.ceil(self.height / grid_height)
		
		self.connect('draw', self.on_drawing_area_draw)
		self.queue_draw()

	def on_drawing_area_draw(self, drawing_area, cairo_context):
		for i in range(0, self.grid_width):
			cairo_context.move_to(round(i * self.cell_width), self.cell_height * 1)
			cairo_context.line_to(round(i * self.cell_width), self.cell_height * (self.grid_height - 3))
		cairo_context.stroke()
