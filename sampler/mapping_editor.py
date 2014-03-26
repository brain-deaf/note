from gi.repository import Gtk, Gdk
import math
import mapping_editor_grid
import sample_editor_grid
import sample_description
import pyplayer
import threading

class MyApp(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.connect('delete-event', Gtk.main_quit)
		
		#sets the height and width of individual cells and applies to all cells in the grid
		self.cell_width  = 14   #14 is the lowest GTK can handle
		self.cell_height = 13  #13 is the lowest GTK can handle
		
		#sets the amount of cells along the x and y axis of the grid, starting from 0
		self.grid_width  = 127
		self.grid_height = 127

		self.pane_view   = Gtk.Paned()
		self.add(self.pane_view)
		self.scroll_view = Gtk.ScrolledWindow()

		#stores a SampleEditorGrid object (grid of Gtk.Buttons) as a member
		self.mapping_editor = sample_editor_grid.SampleEditorGrid(self, self.grid_width, self.grid_height)
		self.set_size_request(1000, 700)

		#stores the default arrow cursor type as a member
		cursor = Gdk.Cursor(Gdk.CursorType.ARROW)

		#stores player object as a member
		self.player = pyplayer.PyPlayer()

		#registers the Gdk window to "listen" for mouse motion and button release events
		self.add_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.add_events(Gdk.EventMask.BUTTON_RELEASE_MASK)
		
		#connects callbacks to motion and button-release events
		self.connect('motion-notify-event', self.on_motion)
		self.connect('button-release-event', self.on_button_release)

		#sets styling of app via an external CSS file
		css = Gtk.CssProvider()
		css.load_from_path('test.css')
		
		#lol wtf i forgot what this shit is
		screen = Gdk.Screen.get_default()
		style_context = Gtk.StyleContext()
		style_context.add_provider_for_screen(screen, css, Gtk.STYLE_PROVIDER_PRIORITY_USER)

		#stores cairo grid_drawing widget as a member
		self.grid_drawing = mapping_editor_grid.MappingEditorGrid(self, self.mapping_editor.grid_width, self.mapping_editor.grid_height)
		#stores overlay widget (used to overlay the button grid on top of the grid drawing) as a member
		self.grid_overlay = mapping_editor_grid.MappingOverlay(self, self.mapping_editor.grid_width, self.mapping_editor.grid_height)

		#stores SampleDescription widget as a member
		self.sample_description = sample_description.SampleDescription()

		#widget used for dragging on the middle divider between mapping editor and sample description widgets
		self.pane_view.pack1(self.sample_description)
		self.pane_view.pack2(self.scroll_view)
		self.pane_view.show_all()

		#overlay all the widgets on top of each other
		self.overlay = Gtk.Overlay()
		self.overlay2 = Gtk.Overlay()
		self.overlay2.add(self.grid_drawing)
		self.overlay2.add_overlay(self.grid_overlay)
		self.overlay.add(self.overlay2)
		self.overlay.add_overlay(self.mapping_editor)

		self.scroll_view.add(self.overlay)

		#self.overlay.add(self.grid_drawing)

		#local variables used for overlay setup
		width  = self.grid_drawing.width
		height = self.grid_drawing.height

		#sets properties on overlay widgets. If opacity is not less than 1.0, you cannot see through each overlay
		self.overlay.set_property("opacity", 0.9)
		self.overlay.set_property("halign", Gtk.Align.START)
		self.overlay.set_property("valign", Gtk.Align.START)
		self.overlay.set_size_request(width, height)

		self.overlay2.set_property("opacity", 0.9)
		self.overlay2.set_property("halign", Gtk.Align.START)
		self.overlay2.set_property("valign", Gtk.Align.START)
		self.overlay2.set_size_request(width, height)

		#show everything (everything is hidden by default)
		self.overlay.show_all()
		self.show_all()
	
	def on_motion(self, widget, event):
		#if a sample button in the SampleEditorGrid is being dragged:
		if self.mapping_editor.dragging:
			self.motion_trigger_flag = True
			
			#local variables used for ease of typing/reading algorithms in this function
			cell_height = self.cell_height
			cell_width = self.cell_width
			
			#the button widget that is currently being dragged
			drag_widget = self.mapping_editor.drag_widget
			
			#the x and y coordinates of the mouse pointer
			mouse_x = self.get_pointer()[0]
			mouse_y = self.get_pointer()[1]

			#if a button from the SampleEditorGrid is being resized from the bottom:
			if (self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_bottom_resize):
				#new height of the dragged widget = round(y position / cell height) - y position of the dragged widget
				new_height = round(mouse_y / cell_height) - drag_widget.y
				
				#if you try to drag below the bottom boundary of the SampleEditorGrid:
				if drag_widget.y + new_height > self.mapping_editor.grid_height:
					new_height = self.mapping_editor.grid_height - drag_widget.height
					
				#if the dragged widget should be resized (i.e. if new_height != to its current height):	
				if (new_height >= 1 and drag_widget.height != new_height):
					#remove the dragged widget from the grid and re-attach it with new dimensions
					#I think you have to remove it in order to resize it, I couldn't figure out another way.
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
		if self.mapping_editor.dragging:
			self.mapping_editor.dragging = False
			if self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_draft:
				self.mapping_editor.drag_widget.y = self.mapping_editor.drag_widget.y_temp
				self.mapping_editor.drag_widget.x = self.mapping_editor.drag_widget.x_temp
			if  self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_right_resize:
				self.mapping_editor.drag_widget.width = self.mapping_editor.drag_widget.width_temp
			if  self.mapping_editor.get_property("window").get_cursor() == self.mapping_editor.cursor_left_resize:
				self.mapping_editor.drag_widget.width = self.mapping_editor.drag_widget.width_temp
				self.mapping_editor.drag_widget.x = self.mapping_editor.drag_widget.x_temp

RELEASE = 128
NOTE_ON = 144
def midi_received(data):
	global win
	note     = data[1]
	velocity = data[2]
	if   data[0] == NOTE_ON:
		win.grid_overlay.draw_midi_note((note, velocity))
		print(data)
	elif data[0] == RELEASE:
		win.grid_overlay.draw_release_midi_note((note, velocity))

win = MyApp()
thread = threading.Thread(target=win.player.get_midi_in, args=(midi_received,))
thread.daemon = True
thread.start()

Gtk.main()
