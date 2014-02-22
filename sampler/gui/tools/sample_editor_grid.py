from gi.repository import Gtk, Gdk
import math

class _transparent_button(Gtk.Button):
	def __init__(self, parent):
		Gtk.Button.__init__(self)
		self._parent = parent
		self.set_property("hexpand", True)
		self.set_property("opacity", 0)
		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK) 
		self.connect('motion-notify-event', self.on_motion)
		self.connect('button-release-event', self.on_button_release)
	
	def on_motion(self, widget, event):
		if not self._parent.dragging:
			self._parent.get_property("window").set_cursor(self._parent.cursor_arrow)
	def on_button_release(self, widget, event):
		if self._parent.dragging:
			self._parent.get_property("window").set_cursor(self._parent.cursor_arrow)
			self._parent.dragging = False
			self._parent.drag_widget.y = self._parent.drag_widget.y_temp
			self._parent.drag_widget.x = self._parent.drag_widget.x_temp
			self._parent.drag_widget.width = self._parent.drag_widget.width_temp

class _button(Gtk.Button):
	def __init__(self, parent, x, y, width=1, height=1, file_name="", path=""):
		Gtk.Button.__init__(self)
		self._parent = parent
		self.x = x
		self.y = y
		self.y_temp = x
		self.x_temp = y
		self.width = width
		self.width_temp = width
		self.height = height
		self.file_name = file_name
		self.path = path

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
		if self._parent.dragging and event.button != 3:
			self._parent.dragging = False
			if self._parent.get_property("window").get_cursor() == self._parent.cursor_draft:
				self._parent.drag_widget.y = self._parent.drag_widget.y_temp
				self._parent.drag_widget.x = self._parent.drag_widget.x_temp
			if self.get_property("window").get_cursor() == self._parent.cursor_right_resize:
				self._parent.drag_widget.width = self._parent.drag_widget.width_temp
			if self.get_property("window").get_cursor() == self._parent.cursor_left_resize:
				self._parent.drag_widget.width = self._parent.drag_widget.width_temp
				self._parent.drag_widget.x = self._parent.drag_widget.x_temp



class SampleEditorGrid(Gtk.Grid):
	def __init__(self, parent, width, height):
		Gtk.Grid.__init__(self)

		self._parent = parent
		self.add_events(Gdk.EventMask.BUTTON_PRESS_MASK) 
		self.connect('button-press-event', self.on_button_press)

		self.connect("drag-motion", self.motion_cb)
		self.connect("drag-drop", self.drop_cb)
		self.connect("drag-data-received", self.got_data_cb)
		self.drag_dest_set(0, [], 0)

		self.dragging = False
		self.drag_start_y = 0
		self.grid_height = height
		self.grid_width = width
		self.drag_widget = None

		self.set_property("row-homogeneous", True)
		self.set_property("column-homogeneous", True)

		self.cursor_left_resize = Gdk.Cursor(Gdk.CursorType.LEFT_SIDE)
		self.cursor_right_resize = Gdk.Cursor(Gdk.CursorType.RIGHT_SIDE)
		self.cursor_top_resize = Gdk.Cursor(Gdk.CursorType.TOP_SIDE)
		self.cursor_bottom_resize = Gdk.Cursor(Gdk.CursorType.BOTTOM_SIDE)
		self.cursor_draft = Gdk.Cursor(Gdk.CursorType.DRAFT_LARGE)
		self.cursor_arrow = Gdk.Cursor(Gdk.CursorType.ARROW)

		for i in range(0, self.grid_width):
			self.attach(_transparent_button(self), i, -1, 1, 1) 
			self.attach(_transparent_button(self), i, self.grid_height, 1, 1) 
			self.attach(_transparent_button(self), -1, i, 1, 1) 
			self.attach(_transparent_button(self), self.grid_width, i, 1, 1) 

		self.show_all()

	def on_button_press(self, widget, event):
		if (event.button == 3):
			self.remove(widget)
		if (self.get_property("window").get_cursor() != self.cursor_arrow and event.button != 3):
			self._parent.sample_description.refresh_info(widget)
			self.dragging = True
			self.drag_widget = widget
			self.drag_start_x = self._parent.get_pointer()[0]
			self.drag_start_y = self._parent.get_pointer()[1]
			widget.y_temp = widget.y
			widget.x_temp = widget.x	

	def motion_cb(self, wid, context, x, y, time):
		Gdk.drag_status(context,Gdk.DragAction.COPY, time)
		return True

	def drop_cb(self, wid, context, x, y, time):
		wid.drag_get_data(context, context.list_targets()[-1], time)

	def got_data_cb(self, wid, context, x, y, data, info, time):
		files=data.get_text().rstrip('\n').split('\n')
		for i,v in enumerate(files):
			button = _button(self, i, 0, 1, self.grid_height, v, v)
			self.attach(button, button.x, button.y, button.width, self.grid_height) 
		self.show_all()
		context.finish(True, False, time)
