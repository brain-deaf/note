from gi.repository import Gtk, Gdk

#widget used to display sample information. When you click on a button in the mapping editor, the sample's information
#will be displayed with this widget.
class SampleDescription(Gtk.VBox):
	def __init__(self):
		Gtk.VBox.__init__(self)
		self.sample_name = Gtk.Label("my_sample.ogg")
		self.sample_path = Gtk.Label("/home/example/my_sample.ogg")
		self.sample_name.set_property("halign", Gtk.Align.START)
		self.sample_path.set_property("halign", Gtk.Align.START)

		self.pack_start(self.sample_name, True, True, 10)
		self.pack_start(self.sample_path, True, True, 10)
		self.set_size_request(350, 90)
		self.set_border_width(10)
		self.show_all()

	def refresh_info(self, widget):
		self.sample_name.set_text(widget.file_name)
		self.sample_path.set_text(widget.path)
		
