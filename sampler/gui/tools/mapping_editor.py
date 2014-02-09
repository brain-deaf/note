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
		self._parent.get_property("window").set_cursor(self._parent.cursor_arrow)

class _button(Gtk.Button):
	def __init__(self, parent, grid_x):
		Gtk.Button.__init__(self)
		self._parent = parent

		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_PRESS_MASK) 
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK) 
		self.add_events(Gdk.EventMask.LEAVE_NOTIFY_MASK) 

		self.set_property("hexpand", True)
		self.zone_grid_height = self._parent.grid_height

		self.connect('button-press-event', parent.on_button_press)
		self.connect('motion-notify-event', self.on_motion)
		self.connect('leave-notify-event', self.on_leave)
		self.connect('button-release-event', self.on_button_release)

		self.resize_margin = 5 #px
		self.grid_x = grid_x

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
				print("bottom resize", abs(height - event.y))
				self._parent.get_property("window").set_cursor(self._parent.cursor_bottom_resize)
				flag = False

			if (height - event.y >= height - self.resize_margin):
				self._parent.get_property("window").set_cursor(self._parent.cursor_top_resize)
				flag = False

			if (self._parent.get_property("window").get_cursor() != self._parent.cursor_arrow):
				if flag:
					self._parent.get_property("window").set_cursor(self._parent.cursor_arrow)

	def on_leave(self, widget, event):
			if (self._parent.get_property("window").get_cursor() != self._parent.cursor_arrow and not self._parent.dragging):
				self._parent.get_property("window").set_cursor(self._parent.cursor_arrow)

	def on_button_release(self, widget, event):
		self._parent.dragging = False

class SampleEditorGrid(Gtk.Grid):
	def __init__(self, parent):
		Gtk.Grid.__init__(self)

		self._parent = parent
		print(self._parent.get_size_request()[0])

		self.add_events(Gdk.EventMask.BUTTON_PRESS_MASK) 

		self.dragging = False
		self.drag_start_y = 0
		self.grid_height = 40
		self.drag_widget = None

		self.set_property("row-homogeneous", True)
		self.set_property("column-homogeneous", False)

		self.connect('button-press-event', self.on_button_press)

		self.cursor_left_resize = Gdk.Cursor(Gdk.CursorType.LEFT_SIDE)
		self.cursor_right_resize = Gdk.Cursor(Gdk.CursorType.RIGHT_SIDE)
		self.cursor_top_resize = Gdk.Cursor(Gdk.CursorType.TOP_SIDE)
		self.cursor_bottom_resize = Gdk.Cursor(Gdk.CursorType.BOTTOM_SIDE)
		self.cursor_arrow = Gdk.Cursor(Gdk.CursorType.ARROW)
		self.show_all()

		total_height = self._parent.get_size_request()[0]
		segment_height = total_height / self.grid_height
		for i in range(0, 20):
			button = _button(self, i)
			button.zone_grid_height = self.grid_height - (i*2)
			self.attach(button, i, 0, 1, self.grid_height - (i*2))
		self.attach(_transparent_button(self), 20, 0, 1, self.grid_height) 

	def on_button_press(self, widget, event):
		if (self.get_property("window").get_cursor() != self.cursor_arrow):
			self.dragging = True
			self.drag_widget = widget
			self.drag_start_y = event.y

class MyApp(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.set_size_request(750, 400)
		self.connect('delete-event', Gtk.main_quit)
		self.mapping_editor = SampleEditorGrid(self)
		self.add(self.mapping_editor)
		self.show_all()
		cursor = Gdk.Cursor(Gdk.CursorType.ARROW)
		self.get_property("window").set_cursor(cursor)

		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK) 
		self.connect('motion-notify-event', self.on_motion)
		self.connect('button-release-event', self.on_button_release)
	
	def on_motion(self, widget, event):
		grid_height = self.mapping_editor.get_allocated_height()
		cell_height = grid_height / self.mapping_editor.grid_height
		if self.mapping_editor.dragging:
			new_height = round(event.y / cell_height)
			if (new_height >= 1):
				self.mapping_editor.remove(self.mapping_editor.drag_widget)
				self.mapping_editor.attach(self.mapping_editor.drag_widget, self.mapping_editor.drag_widget.grid_x, 0, 1, new_height)

	def on_button_release(self, widget, event):
		self.mapping_editor.dragging = False
		print(event.x, event.y)

app = MyApp()
Gtk.main()
