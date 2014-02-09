#ifndef PLAYER_H
#define PLAYER_H 
#include <vector>
#include <functional>
#include <cmath>
#include <future>
#include <boost/utility.hpp>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include "WaveSynth.h"
#include "global.h"

/// @brief master class to handle audio generation and playback
class Player : public boost::noncopyable, public std::enable_shared_from_this<Player> {
    static const char * format_;

public: 
    enum class BackendType {local,vorbis};
    Player(BackendType backend_type, const Offset& sample_rate_init, const double freq_reference_init, const std::string& output_name);
    Beat get_stream_end() const;
    void play();
    void add_instrument(InstrumentHandle instrument);
    std::string wait_until_ready();
    void eos();
    void quit();
    Offset sample_rate() const;
    double freq_reference() const;

protected:
    void need_data(guint hint);
    void enough_data();
    gboolean seek_data(Offset);
    gboolean push_data();

    class Backend {
    protected:
        static void build_gst_element(GstElement * &element, const char * kind, const char * name);

        static void wrap_need_data(GstAppSrc * element, guint length, gpointer instance);
        static void wrap_enough_data(GstAppSrc * element, gpointer instance);
        static gboolean wrap_seek_data(GstAppSrc * element, guint64 destination, gpointer instance);
        static gboolean wrap_push_data(gpointer instance);
        static gboolean wrap_bus_callback (GstBus *bus, GstMessage *message, gpointer instance);

        void need_data(guint hint);
        void enough_data();
        gboolean seek_data(Offset);
        gboolean bus_callback(GstBus * bus, GstMessage * message);

        GstElement * pipeline_;
        GstElement * appsrc_;
        GstElement * conv_;
        GstElement * audiosink_;
        GMainLoop * loop_;
        guint push_id_;
        guint bus_watch_id_;
        guint last_hint_;
        std::future<void> main_loop_done_;
        Player * player_;

    public:
        Backend(Player * player);
        virtual ~Backend();
        bool push_data(Packet& data, Offset current_offset);
        void play(Offset);
        void eos();
        void quit();
        virtual void wait_until_ready() = 0;
        virtual std::string where()=0;
    };

    class LocalBackend : public Backend {
    public:
        LocalBackend(Player * player);
        virtual void wait_until_ready() override;
        virtual std::string where() override;
    };

    class VorbisBackend : public Backend {
        GstElement * vorbisencoder_;
        GstElement * oggmuxer_;
        std::string where_;
    public:
        std::string where();
        VorbisBackend(Player * player, const std::string& output_name);
        virtual void wait_until_ready() override;
    };


    const Offset sample_rate_;
    const double freq_reference_;
    std::vector<InstrumentHandle> instruments_;
    Offset current_offset_;
    Offset end_offset_;
    Offset last_hint_;
    std::unique_ptr<Backend> backend_;
};

typedef std::shared_ptr<Player> PlayerHandle;
#endif /* PLAYER_H */
