from gi.repository import Gtk, Gdk
import math

class _transparent_button(Gtk.Button):
	def __init__(self, parent):
		Gtk.Button.__init__(self)
		self._parent = parent
		self.set_property("hexpand", True)
		self.set_property("opacity", 0)
		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.connect('motion-notify-event', self.on_motion)
	
	def on_motion(self, widget, event):
		if not self._parent.dragging:
			self._parent.get_property("window").set_cursor(self._parent.cursor_arrow)

class _button(Gtk.Button):
	def __init__(self, parent, x, y, width=1, height=1):
		Gtk.Button.__init__(self)
		self._parent = parent
		self.x = x
		self.y = y
		self.y_temp = 0
		self.x_temp = 0
		self.width = width
		self.height = height

		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_PRESS_MASK) 
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK) 
		self.add_events(Gdk.EventMask.LEAVE_NOTIFY_MASK) 

		self.set_property("hexpand", True)
		self.set_property("opacity", 0.5)
		self.zone_grid_height = self._parent.grid_height

		
		self.connect('button-press-event', parent.on_button_press)
		self.connect('motion-notify-event', self.on_motion)
		self.connect('leave-notify-event', self.on_leave)
		self.connect('button-release-event', self.on_button_release)

		self.resize_margin = 5 #px

	def on_motion(self, widget, event):
		width = widget.get_allocated_width()
		height = widget.get_allocated_height()
		grid_height = self._parent.get_allocated_height()
		flag = True
		
		if (not self._parent.dragging):
			if (width - event.x <= self.resize_margin):
				self._parent.get_property("window").set_cursor(self._parent.cursor_right_resize)
				flag = False

			if (width - event.x >= width - self.resize_margin):
				self._parent.get_property("window").set_cursor(self._parent.cursor_left_resize)
				flag = False

			if (height - event.y <= self.resize_margin):
				self._parent.get_property("window").set_cursor(self._parent.cursor_bottom_resize)
				flag = False

			if (height - event.y >= height - self.resize_margin):
				self._parent.get_property("window").set_cursor(self._parent.cursor_top_resize)
				flag = False

			if (self._parent.get_property("window").get_cursor() != self._parent.cursor_draft):
				if flag:
					self._parent.get_property("window").set_cursor(self._parent.cursor_draft)

	def on_leave(self, widget, event):
			if (self._parent.get_property("window").get_cursor() != self._parent.cursor_arrow and not self._parent.dragging):
				self._parent.get_property("window").set_cursor(self._parent.cursor_arrow)

	def on_button_release(self, widget, event):
		if self._parent.dragging:
			self._parent.dragging = False
			if self._parent.get_property("window").get_cursor() == self._parent.cursor_draft:
				self._parent.drag_widget.y = self._parent.drag_widget.y_temp
				self._parent.drag_widget.x = self._parent.drag_widget.x_temp

class SampleEditorGrid(Gtk.Grid):
	def __init__(self, parent):
		Gtk.Grid.__init__(self)

		self._parent = parent

		self.add_events(Gdk.EventMask.BUTTON_PRESS_MASK) 


		self.dragging = False
		self.drag_start_y = 0
		self.grid_height = 40
		self.grid_width =20
		self.drag_widget = None

		self.set_property("row-homogeneous", True)
		self.set_property("column-homogeneous", False)

		self.connect('button-press-event', self.on_button_press)

		self.cursor_left_resize = Gdk.Cursor(Gdk.CursorType.LEFT_SIDE)
		self.cursor_right_resize = Gdk.Cursor(Gdk.CursorType.RIGHT_SIDE)
		self.cursor_top_resize = Gdk.Cursor(Gdk.CursorType.TOP_SIDE)
		self.cursor_bottom_resize = Gdk.Cursor(Gdk.CursorType.BOTTOM_SIDE)
		self.cursor_draft = Gdk.Cursor(Gdk.CursorType.DRAFT_LARGE)
		self.cursor_arrow = Gdk.Cursor(Gdk.CursorType.ARROW)
		self.show_all()

		for i in range(0, self.grid_width):
			button = _button(self, i, 0, 1, 40)
			self.attach(button, i, 0, button.width, button.height)
			self.attach(_transparent_button(self), i, -1, 1, 1) 
			self.attach(_transparent_button(self), i, self.grid_height, 1, 1) 

	def on_button_press(self, widget, event):
		if (self.get_property("window").get_cursor() != self.cursor_arrow):
			self.dragging = True
			self.drag_widget = widget
			self.drag_start_x = self._parent.get_pointer()[0]
			self.drag_start_y = self._parent.get_pointer()[1]

class MyApp(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.set_size_request(750, 400)
		self.connect('delete-event', Gtk.main_quit)
		self.mapping_editor = SampleEditorGrid(self)
		self.add(self.mapping_editor)

		cursor = Gdk.Cursor(Gdk.CursorType.ARROW)
		#self.get_property("window").set_cursor(cursor)

		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK) 
		self.connect('motion-notify-event', self.on_motion)
		self.connect('button-release-event', self.on_button_release)

		css = Gtk.CssProvider()
		css.load_from_path('test.css')
		screen = Gdk.Screen.get_default()
		style_context = Gtk.StyleContext()
		style_context.add_provider_for_screen(screen, css, Gtk.STYLE_PROVIDER_PRIORITY_USER)

		self.show_all()
	
	def on_motion(self, widget, event):
		if self.mapping_editor.dragging:
			grid_height = self.mapping_editor.get_allocated_height()
			grid_width = self.mapping_editor.get_allocated_width()
			cell_height = grid_height / self.mapping_editor.grid_height
			cell_width = grid_width / self.mapping_editor.grid_width
			drag_widget = self.mapping_editor.drag_widget
			mouse_x = self.get_pointer()[0]
			mouse_y = self.get_pointer()[1]
			if (self.get_property("window").get_cursor() == self.mapping_editor.cursor_bottom_resize):
				new_height = round(mouse_y / cell_height) - drag_widget.y
				if drag_widget.y + new_height > self.mapping_editor.grid_height:
					new_height = self.mapping_editor.grid_height - drag_widget.height
				if (new_height >= 1 and drag_widget.height != new_height):
					print("bottom resize")
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, drag_widget.x, drag_widget.y, drag_widget.width, new_height)
					drag_widget.height = new_height
			if (self.get_property("window").get_cursor() == self.mapping_editor.cursor_top_resize):
				new_cell   = round(mouse_y / cell_height) - 1
				if new_cell < 0:
					new_cell = 0
				new_height = round(drag_widget.height - (new_cell - drag_widget.y))
				if (new_height >= 1 and new_height != drag_widget.height):
					print("top resize")
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, drag_widget.x, new_cell, drag_widget.width, new_height)
					drag_widget.height = new_height
					drag_widget.y = new_cell
			if (self.get_property("window").get_cursor() == self.mapping_editor.cursor_draft):
				height_displacement = round((mouse_y - self.mapping_editor.drag_start_y) / cell_height)
				width_displacement = round(float(mouse_x - self.mapping_editor.drag_start_x) / float(cell_width))
				if (drag_widget.x + width_displacement < 0):
					x = 0
				else:
					x = drag_widget.x + width_displacement 
				if (drag_widget.y + height_displacement < 0):
					y = 0
				else:
					y = drag_widget.y + height_displacement
					if (y + drag_widget.height > self.mapping_editor.grid_height):
						y = self.mapping_editor.grid_height - drag_widget.height
				if ((drag_widget.y_temp != y or drag_widget.x_temp != x)
				and (y + drag_widget.height <= self.mapping_editor.grid_height
				or  x + drag_widget.width <= self.mapping_editor.grid_width)):
					print("widget dragged: ", y + drag_widget.height)
					self.mapping_editor.remove(drag_widget)
					self.mapping_editor.attach(drag_widget, x, y, drag_widget.width, drag_widget.height)
					drag_widget.y_temp = drag_widget.y + height_displacement
					drag_widget.x_temp = drag_widget.x + width_displacement

	def on_button_release(self, widget, event):
		if self.mapping_editor.dragging:
			self.mapping_editor.dragging = False
			if self.get_property("window").get_cursor() == self.mapping_editor.cursor_draft:
				self.mapping_editor.drag_widget.y = self.mapping_editor.drag_widget.y_temp
				self.mapping_editor.drag_widget.x = self.mapping_editor.drag_widget.x_temp

app = MyApp()
Gtk.main()

