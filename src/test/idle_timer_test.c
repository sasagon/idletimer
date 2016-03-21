#ifdef HAVE_CONFIG_H
#include "../config.h"

#ifdef HAVE_CUNIT_CUNIT_H
//#ifdef HAVE_LIBCUNIT

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../idle_timer.h"

#define MAX_IDLE_MINUTES 3

static IdleTimer* idle_timer = NULL;
static unsigned long idle_minutes = 0L;
static unsigned long wakeup_minutes = 0L;

static void idle_listener(unsigned long current_idle_minutes, void* data)
{
    *((unsigned long*)data) = current_idle_minutes;
}

static void wakeup_listener(unsigned long prev_idle_minutes, void* data)
{
    *((unsigned long*)data) = prev_idle_minutes;
    gtk_main_quit();
}

static unsigned long get_repeated_idle_msec()
{
    static int idle_minutes_counter = 0;
    unsigned long n = idle_minutes_counter * 1000 * 60; // milliseconds
    idle_minutes_counter = (idle_minutes_counter + 1) % (MAX_IDLE_MINUTES + 1);
    return n;
}

static int prepare()
{
    idle_timer = create_idle_timer_with_handler(get_repeated_idle_msec);
    idle_minutes = 0L;
    wakeup_minutes = 0L;
    add_idle_listener(idle_timer, idle_listener, &idle_minutes);
    add_wakeup_listener(idle_timer, wakeup_listener, &wakeup_minutes);
    gtk_main();
    return 0;
}

static void test_idle_listener()
{
    CU_ASSERT(idle_minutes == MAX_IDLE_MINUTES);
}

static void test_wakeup_listener()
{
    CU_ASSERT(wakeup_minutes == MAX_IDLE_MINUTES);
}

static int cleanup()
{
    delete_idle_timer(idle_timer);
    idle_timer = NULL;
    return 0;
}


int main(int argc, char* argv[])
{
    CU_TestInfo tests[] = {
        { "test_idle_listener", test_idle_listener },
        { "test_wakeup_listener", test_wakeup_listener },
        CU_TEST_INFO_NULL
    };
    CU_SuiteInfo suites[] = {
        { "IdleTimer", prepare, cleanup, tests },
        CU_SUITE_INFO_NULL
    };

    gtk_init(&argc, &argv);

    CU_initialize_registry();
    CU_register_suites(suites);
    CU_set_error_action(CUEA_ABORT);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    bool failed = (CU_get_failure_list() != NULL);
    CU_cleanup_registry();

    return (failed)? EXIT_FAILURE : EXIT_SUCCESS;
}


#else

#error CUnit not found.

#endif /* HAVE_LIBCUNIT */
#endif /* HAVE_CONFIG_H */

