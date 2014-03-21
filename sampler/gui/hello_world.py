#!/usr/bin/python
from gi.repository import Gtk
import player
import threading

class MyWindow(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self, title="Hello World")
        self.set_default_size(300, 150)

        self.grid = Gtk.Grid()
        self.add(self.grid)
        self.grid.set_properties(expand=True)

        self.button1 = Gtk.Button(label="clip 1")
        self.button1.set_properties(expand=True)
        self.button1.connect("clicked", self.on_button1_clicked)
        self.grid.attach(self.button1, 0, 0, 1, 2)

        self.button2 = Gtk.Button(label="clip 2")
        self.button2.connect("clicked", self.on_button2_clicked)
        self.button2.set_properties(expand=True)
        self.grid.attach(self.button2, 4, 0, 1, 2)

        self.master_slider = Gtk.Scale()
        self.master_slider.set_properties(expand=True)
        self.master_slider.set_range(0.0, 2.0)
        self.master_slider.set_properties(orientation=0, inverted=False)
        self.master_slider.connect("value-changed", self.on_slider_changed)
        self.grid.attach_next_to(self.master_slider, self.button1, Gtk.PositionType.TOP, 7, 1)
        self.player = player.PyPlayer()
        
        self.track_1 = Gtk.Scale()
        self.track_1.set_properties(expand=True)
        self.track_1.set_range(0.0, 2.0)
        self.track_1.set_properties(orientation=1, inverted=True)
        self.track_1.connect("value-changed", self.on_slider_changed)
        self.grid.attach_next_to(self.track_1, self.button1, Gtk.PositionType.RIGHT, 1, 2)

        self.track_2 = Gtk.Scale()
        self.track_2.set_properties(expand=True)
        self.track_2.set_range(0.0, 2.0)
        self.track_2.set_properties(orientation=1, inverted=True)
        self.track_2.connect("value-changed", self.on_slider_changed)
        self.grid.attach_next_to(self.track_2, self.button2, Gtk.PositionType.RIGHT, 1, 2)
        
    def run_main(self):
        self.player.run()
    def on_button1_clicked(self, widget):
        self.player.play_sample(bytes("test.ogg","ascii"))
    def on_button2_clicked(self, widget):
        self.player.play_sample(bytes("test2.ogg","ascii"))
    def close_app(self, _,__):
        self.player.destruct()
        Gtk.main_quit()
    def on_slider_changed(self, slider):
        value = slider.get_value()
        if (slider == self.master_slider):
            self.player.set_volume(value)
        if (slider == self.track_1):
            self.player.set_volume_track1(value)
        if (slider == self.track_2):
            self.player.set_volume_track2(value)

def midi_received(data):
	note = data[1]
	print(note)

win = MyWindow()
thread = threading.Thread(target=win.player.get_midi_in, args=(midi_received,))
thread.daemon = True
thread.start()

win.connect("delete-event", win.close_app)
win.show_all()
Gtk.main()
#use this to see available properties on a widget
#print dir(win.master_slider.props) 
