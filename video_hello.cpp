#include <gst/gst.h>

static GMainLoop *loop;

static void cb_need_data (GstElement *appsrc,
    guint       unused_size,
    gpointer    user_data)
{
    static gboolean white = false;
    static GstClockTime timestamp = 0;
    GstBuffer *buffer;
    guint size;
    GstFlowReturn ret;
    
    guint w = 640;
    guint h = 480;

    size = w * h * 2; // 16 bit, 2 bytes per

    buffer = gst_buffer_new_allocate (nullptr, size, nullptr);

    /* this makes the image black/white */
    for(int i = 0; i < h; ++i) {
        gst_buffer_memset (buffer, 640*i*2, white ? 0xff : 0x00, 320*i*2);
        gst_buffer_memset (buffer, 640*i*2+320*2, white ? 0x00 : 0xff, 320*i*2);
    }

    white = !white;

    GST_BUFFER_PTS (buffer) = timestamp;
    GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 2);

    timestamp += GST_BUFFER_DURATION (buffer);

    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);

    if (ret != GST_FLOW_OK) {
        /* something wrong, stop pushing */
        g_main_loop_quit (loop);
  }
}

gint
main (gint   argc,
      gchar *argv[])
{
  GstElement *pipeline, *appsrc, *conv, *videosink;

  /* init GStreamer */
  gst_init (&argc, &argv);
  loop = g_main_loop_new (NULL, FALSE);

  /* setup pipeline */
  pipeline = gst_pipeline_new ("pipeline");
  appsrc = gst_element_factory_make ("appsrc", "source");
  conv = gst_element_factory_make ("videoconvert", "conv");
  videosink = gst_element_factory_make ("xvimagesink", "videosink");

  /* setup */
  g_object_set (G_OBJECT (appsrc), "caps",
  		gst_caps_new_simple ("video/x-raw",
				     "format", G_TYPE_STRING, "RGB16",
				     "width", G_TYPE_INT, 640,
				     "height", G_TYPE_INT, 480,
				     "framerate", GST_TYPE_FRACTION, 0, 1,
				     nullptr), nullptr);
  gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, videosink, nullptr);
  gst_element_link_many (appsrc, conv, videosink, nullptr);

  /* setup appsrc */
  g_object_set (G_OBJECT (appsrc),
		"stream-type", 0,
		"format", GST_FORMAT_TIME, nullptr);
  g_signal_connect (appsrc, "need-data", G_CALLBACK (cb_need_data), nullptr);

  /* play */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  g_main_loop_run (loop);

  /* clean up */
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline));
  g_main_loop_unref (loop);

  return 0;
  }

