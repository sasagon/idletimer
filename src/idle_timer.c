#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "idle_timer.h"
                                                
#define TIMER_INTERVAL  1000

#ifdef HAVE_CONFIG_H
#include "config.h"

#ifdef HAVE_GTK
#include <gtk/gtk.h>
#include <glib.h>
#else
#error libgtk2 or libgtk3 not found.
#endif /* HAVE_GTK */

#ifdef HAVE_LIBXSS

#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>
#include <gdk/gdkx.h>

static unsigned long get_system_idle_time()
{
    XScreenSaverInfo info;
    Display* display = XOpenDisplay(NULL);
    GdkWindow* gdk_window = gdk_get_default_root_window();
    Window window = GDK_WINDOW_XID(gdk_window);
    XScreenSaverQueryInfo(display, window, &info);
    XCloseDisplay(display);
    return info.idle; /* milliseconds */
}

#else

#error libxss not found.

#endif /* HAVE_LIBXSS */
#endif /* HAVE_CONFIG_H */


typedef struct {
    timer_listener_t func;
    void* data;
} Listener;

typedef struct {
    int num_of_listeners;
    Listener* listeners;
} ListenerArray;


static ListenerArray* create_listener_array()
{
    ListenerArray* p = malloc(sizeof(ListenerArray));
    p->num_of_listeners = 0;
    p->listeners = NULL;
    return p;
}


static void delete_listener_array(ListenerArray* p)
{
    if (p) {
        free(p->listeners);
        free(p);
    }
}


static void add_listener(ListenerArray* p, timer_listener_t func, void* data)
{
    assert(p);
    assert(func);

    int i = p->num_of_listeners++;
    p->listeners = realloc(p->listeners, sizeof(Listener[p->num_of_listeners]));
    p->listeners[i].func = func;
    p->listeners[i].data = data;
}


static void exec_listeners(const ListenerArray* p, unsigned long idle_minutes)
{
    assert(p);

    int i;

    for (i = 0; i < p->num_of_listeners; i++) {
        assert(p->listeners);
        (*p->listeners[i].func)(idle_minutes, p->listeners[i].data);
    }
}


struct _IdleTimer {
    unsigned long (*get_idle_time)();
    gint interval_timer;
    ListenerArray* idle_listeners;
    ListenerArray* wakeup_listeners;
    unsigned long prev_idle_time; /* milliseconds */
};


static unsigned long to_minutes(unsigned long milliseconds)
{
    return milliseconds / 1000 / 60;
}


static void check_idle_time(IdleTimer* p)
{
    assert(p);

    unsigned long current_idle_time = p->get_idle_time();
    unsigned long prev_idle_minutes = to_minutes(p->prev_idle_time);

    if (p->prev_idle_time > current_idle_time) {
        if (prev_idle_minutes > 0) {
            exec_listeners(p->wakeup_listeners, prev_idle_minutes);
        }
    } else {
        unsigned long current_idle_minutes = to_minutes(current_idle_time);
        if (current_idle_minutes > prev_idle_minutes) {
            exec_listeners(p->idle_listeners, current_idle_minutes);
        }
    }
    p->prev_idle_time = current_idle_time;
}


static gint interval_callback(gpointer data)
{
    check_idle_time((IdleTimer*)data);
    return 1;
}


IdleTimer* create_idle_timer_with_handler(unsigned long (*get_idle_time)())
{
    assert(get_idle_time);

    IdleTimer* p = malloc(sizeof(IdleTimer));
    p->get_idle_time = get_idle_time;
    p->interval_timer = g_timeout_add(TIMER_INTERVAL, interval_callback, p);
    p->idle_listeners = create_listener_array();
    p->wakeup_listeners = create_listener_array();
    p->prev_idle_time = 0L;
    return p;
}


IdleTimer* create_idle_timer()
{
    return create_idle_timer_with_handler(get_system_idle_time);
}


void delete_idle_timer(IdleTimer* p)
{
    if (p) {
        delete_listener_array(p->idle_listeners);
        delete_listener_array(p->wakeup_listeners);
        g_source_remove(p->interval_timer);
        free(p);
    }
}


void add_idle_listener(IdleTimer* p, timer_listener_t func, void* data)
{
    assert(p);
    add_listener(p->idle_listeners, func, data);
}


void add_wakeup_listener(IdleTimer* p, timer_listener_t func, void* data)
{
    assert(p);
    add_listener(p->wakeup_listeners, func, data);
}
