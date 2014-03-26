from gi.repository import Gtk, Gdk

#widget used for drawing a box (to show mouse selection). I cannot figure out how to implement this into the mapping editor.
class SelectArea(Gtk.DrawingArea):
	def __init__(self):
		Gtk.DrawingArea.__init__(self)

		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_PRESS_MASK) 
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK) 

		self.dragging = False
		self.context_start = []
		self.clicks = [[-1, -1]]
		self.erase = False

		self.connect('draw', self.on_drawing_area_draw)
		self.connect('button-press-event', self.on_drawing_area_button_press)
		self.connect('button-release-event', self.on_drawing_area_button_release)
		self.connect('motion-notify-event', self.on_drawing_area_motion)

		top_left = []
		top_right = []
		bottom_right = []
		bottom_left = []

	def on_drawing_area_button_press(self, widget, event):
		self.erase = False
		self.dragging = True
		self.context_start = [event.x, event.y]
		self.clicks.append([event.x, event.y])
		return True

	def on_drawing_area_button_release(self, widget, event):
		self.erase = True
		self.queue_draw()
		self.dragging = False
		return True

	def on_drawing_area_motion(self, widget, event):
		if self.dragging:
			self.clicks = [event.x, event.y]
			self.queue_draw()

	def on_drawing_area_draw(self, drawing_area, cairo_context):
		if self.erase:
			cairo_context.fill()
			self.clicks = []
			self.erase = False
		else:
			if self.dragging:
				self.top_left = [self.context_start[0], self.context_start[1]]
				self.top_right = [self.clicks[0], self.context_start[1]]
				self.bottom_right = [self.clicks[0], self.clicks[1]]
				self.bottom_left = [self.context_start[0], self.clicks[1]]

				cairo_context.move_to(self.context_start[0], self.context_start[1])
				cairo_context.rel_line_to(self.top_right[0] - self.top_left[0], 0)
				cairo_context.rel_line_to(0, self.bottom_right[1] - self.top_right[1])
				cairo_context.rel_line_to(self.bottom_left[0] - self.bottom_right[0], 0)
				cairo_context.rel_line_to(0, self.top_left[1] - self.bottom_left[1])
				cairo_context.stroke()
		return False
