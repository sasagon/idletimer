#ifdef HAVE_CONFIG_H
#include "../src/config.h"

#ifdef HAVE_CUNIT_CUNIT_H
//#ifdef HAVE_LIBCUNIT

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "command_map.h"


static void test_is_command_map_empty()
{
    CommandMap* p = create_command_map();

    CU_ASSERT_TRUE(is_command_map_empty(p));

    add_command(p, 1, "A");

    CU_ASSERT_FALSE(is_command_map_empty(p));

    delete_command_map(p);
}


static void test_find_equals()
{
    CommandMap* p = create_command_map();
    add_command(p, 1, "A");
    add_command(p, 2, "B");
    add_command(p, 2, "C");

    const char*const* commands1 = find_equals(p, 1);
    CU_ASSERT_STRING_EQUAL(commands1[0], "A");
    CU_ASSERT_PTR_NULL(commands1[1]);

    const char*const* commands2 = find_equals(p, 2);
    CU_ASSERT_STRING_EQUAL(commands2[0], "B");
    CU_ASSERT_STRING_EQUAL(commands2[1], "C");
    CU_ASSERT_PTR_NULL(commands2[2]);

    const char*const* commands3 = find_equals(p, 3);
    CU_ASSERT_PTR_NULL(commands3[0]);

    delete_command_map(p);
}


static void test_find_less_equals()
{
    CommandMap* p = create_command_map();
    add_command(p, 5, "A");
    add_command(p, 6, "B");
    add_command(p, 6, "C");
    add_command(p, 7, "D");

    const char** commands1 = find_less_equals(p, 1);
    CU_ASSERT_PTR_NULL(commands1[0]);
    delete_found_commands(commands1);

    const char** commands5 = find_less_equals(p, 5);
    CU_ASSERT_STRING_EQUAL(commands5[0], "A");
    CU_ASSERT_PTR_NULL(commands5[2]);
    delete_found_commands(commands5);

    const char** commands6 = find_less_equals(p, 6);
    CU_ASSERT_STRING_EQUAL(commands6[0], "A");
    CU_ASSERT_STRING_EQUAL(commands6[1], "B");
    CU_ASSERT_STRING_EQUAL(commands6[2], "C");
    CU_ASSERT_PTR_NULL(commands6[3]);
    delete_found_commands(commands6);

    delete_command_map(p);
}


int main(int argc, char* argv[])
{
    CU_TestInfo tests[] = {
        { "test_find_equals", test_find_equals },
        { "test_find_less_equals", test_find_less_equals },
        CU_TEST_INFO_NULL
    };
    CU_SuiteInfo suites[] = {
        { "CommandMap", NULL, NULL, tests },
        CU_SUITE_INFO_NULL
    };

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
