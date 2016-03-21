#ifndef __IDLE_TIMER_H__
#define __IDLE_TIMER_H__

typedef struct _IdleTimer IdleTimer;

IdleTimer* create_idle_timer();
void delete_idle_timer(IdleTimer*);

typedef void (*timer_listener_t)(unsigned long idle_minutes, void* data);

void add_idle_listener(IdleTimer*, timer_listener_t, void* data);
void add_wakeup_listener(IdleTimer*, timer_listener_t, void* data);

/* for testing */
IdleTimer* create_idle_timer_with_handler(unsigned long (*get_idle_time_func)());

#endif
