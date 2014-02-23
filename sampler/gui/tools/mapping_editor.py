from gi.repository import Gtk, Gdk
import math
import mapping_editor_grid
import sample_editor_grid
import sample_description

class MyApp(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.connect('delete-event', Gtk.main_quit)
		self.cell_width = 14   #14 is the lowest GTK can handle
		self.cell_height = 13  #13 is the lowest GTK can handle
		self.grid_width = 128
		self.grid_height = 40

		self.pane_view   = Gtk.Paned()
		self.add(self.pane_view)
		self.scroll_view = Gtk.ScrolledWindow()

		self.mapping_editor = sample_editor_grid.SampleEditorGrid(self, self.grid_width, self.grid_height)
		#self.set_size_request((self.mapping_editor.grid_width + 2) * self.cell_width, (self.mapping_editor.grid_height + 2) * self.cell_height)
		self.set_size_request(1000, 700)

		cursor = Gdk.Cursor(Gdk.CursorType.ARROW)
		#self.set_property("resizable", False)

		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK) 
		self.connect('motion-notify-event', self.on_motion)
		self.connect('button-release-event', self.on_button_release)

		css = Gtk.CssProvider()
		css.load_from_path('test.css')
		screen = Gdk.Screen.get_default()
		style_context = Gtk.StyleContext()
		style_context.add_provider_for_screen(screen, css, Gtk.STYLE_PROVIDER_PRIORITY_USER)

		self.grid_drawing = mapping_editor_grid.MappingEditorGrid(self, self.mapping_editor.grid_width, self.mapping_editor.grid_height)
		self.sample_description = sample_description.SampleDescription()

		self.overlay = Gtk.Overlay()
		self.pane_view.pack1(self.sample_description)
		self.pane_view.pack2(self.scroll_view)
		self.pane_view.show_all()
		self.scroll_view.add(self.overlay)
		self.overlay.add(self.grid_drawing)
		self.overlay.add_overlay(self.mapping_editor)
		self.overlay.set_property("opacity", 0.9)
		self.overlay.show_all()

		self.show_all()
	
	def on_motion(self, widget, event):
		if self.mapping_editor.dragging:
			self.motion_trigger_flag = True
			cell_height = self.cell_height
			cell_width = self.cell_width
			drag_widget = self.mapping_editor.drag_widget
			mouse_x = self.get_pointer()[0]
			mouse_y = self.get_pointer()[1]

			if (self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_bottom_resize):
				new_height = round(mouse_y / cell_height) - drag_widget.y
				if drag_widget.y + new_height > self.mapping_editor.grid_height:
					new_height = self.mapping_editor.grid_height - drag_widget.height
				if (new_height >= 1 and drag_widget.height != new_height):
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, drag_widget.x, drag_widget.y, drag_widget.width, new_height)
					drag_widget.height = new_height

			if (self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_top_resize):
				new_cell   = round(mouse_y / cell_height) - 1
				if new_cell < 0:
					new_cell = 0
				new_height = round(drag_widget.height - (new_cell - drag_widget.y))
				if (new_height >= 1 and new_height != drag_widget.height):
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, drag_widget.x, new_cell, drag_widget.width, new_height)
					drag_widget.height = new_height
					drag_widget.y = new_cell

			if (self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_draft):
				height_displacement = round((mouse_y - self.mapping_editor.drag_start_y) / cell_height)
				width_displacement = round(float(mouse_x - self.mapping_editor.drag_start_x) / float(cell_width))
				if (drag_widget.x + width_displacement < 0):
					x = 0
				else:
					x = drag_widget.x + width_displacement 
					if (x + drag_widget.width > self.mapping_editor.grid_width):
						x = self.mapping_editor.grid_width - drag_widget.width
				if (drag_widget.y + height_displacement < 0):
					y = 0
				else:
					y = drag_widget.y + height_displacement
					if (y + drag_widget.height > self.mapping_editor.grid_height):
						y = self.mapping_editor.grid_height - drag_widget.height
				if ((drag_widget.y_temp != y or drag_widget.x_temp != x)
				and (y + drag_widget.height <= self.mapping_editor.grid_height
				or  x + drag_widget.width <= self.mapping_editor.grid_width)):
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, x, y, drag_widget.width, drag_widget.height)
					drag_widget.y_temp = y
					drag_widget.x_temp = x

			if (self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_right_resize):
				width_displacement = round(float(mouse_x - self.mapping_editor.drag_start_x) / float(cell_width))
				if (width_displacement + drag_widget.x + drag_widget.width <= self.mapping_editor.grid_width
				and drag_widget.width + width_displacement != drag_widget.width_temp): 
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, drag_widget.x, drag_widget.y, drag_widget.width + width_displacement, drag_widget.height)
					drag_widget.width_temp = drag_widget.width + width_displacement

			if (self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_left_resize):
				new_cell = drag_widget.x + round(float(mouse_x - self.mapping_editor.drag_start_x) / float(cell_width))
				if new_cell < 0:
					new_cell = 0
				new_width = round(drag_widget.width + (drag_widget.x - new_cell))
				if (new_width >= 1 and new_width != drag_widget.width):
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, new_cell, drag_widget.y, new_width, drag_widget.height)
					drag_widget.width_temp = new_width
					drag_widget.x_temp = new_cell
					

	def on_button_release(self, widget, event):
		print("button release window level")
		if self.mapping_editor.dragging:
			self.mapping_editor.dragging = False
			if self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_draft:
				self.mapping_editor.drag_widget.y = self.mapping_editor.drag_widget.y_temp
				self.mapping_editor.drag_widget.x = self.mapping_editor.drag_widget.x_temp
			if self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_right_resize:
				self.mapping_editor.drag_widget.width = self.mapping_editor.drag_widget.width_temp
			if self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_left_resize:
				self.mapping_editor.drag_widget.width = self.mapping_editor.drag_widget.width_temp
				self.mapping_editor.drag_widget.x = self.mapping_editor.drag_widget.x_temp

app = MyApp()
Gtk.main()

