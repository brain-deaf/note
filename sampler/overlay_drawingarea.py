from gi.repository import Gtk
import select_box

#this was a test, currently not being used for the mapping editor

class MyWindow(Gtk.Window):

	def __init__(self):
		Gtk.Window.__init__(self, title="Hello World")
		self.box = Gtk.Box(spacing=6)
		self.add(self.box)

		self.button1 = Gtk.Button(label="Hello")
		self.button1.connect("clicked", self.on_button1_clicked)
		self.box.pack_start(self.button1, True, True, 0)

		self.button2 = Gtk.Button(label="Hello")
		self.button2.connect("clicked", self.on_button1_clicked)
		self.button2.set_property("expand", True)

		self.overlay = Gtk.Overlay()

		self.select_area = select_box.SelectArea()
		self.select_area.set_property("opacity", .1)
		self.select_area.set_property("expand", True)

		#self.button2.set_valign(Gtk.Align.CENTER)
		#self.button2.set_halign(Gtk.Align.CENTER)
		self.select_area.set_valign(Gtk.Align.CENTER)
		self.select_area.set_halign(Gtk.Align.CENTER)
		self.select_area.set_size_request(300, 300)

		self.box.pack_start(self.overlay, True, True, 0)

		#self.overlay.add(self.select_area)
		#self.overlay.add_overlay(self.button2)
		self.overlay.add(self.button2)
		self.overlay.add_overlay(self.select_area)

		self.overlay.show_all()
	def on_button1_clicked(self, widget):
		print("Hello")
	def on_button2_clicked(self, widget):
		print("Goodbye")

win = MyWindow()
win.connect("delete-event", Gtk.main_quit)
win.show_all()
Gtk.main()
