#ifdef HAVE_CONFIG_H
#include "../src/config.h"

#ifdef HAVE_CUNIT_CUNIT_H
//#ifdef HAVE_LIBCUNIT

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "config_parser.h"


static void test_parse_config()
{
    char test_input[] = "# comment\n"
                        "\r\n"
                        "idle:1:test.sh\n"
                        "wakeup:30:wakeup.sh\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    Config* p = parse_config(f, NULL);

    CU_ASSERT_STRING_EQUAL(find_equals(p->idle_commands, 1)[0], "test.sh");
    CU_ASSERT_STRING_EQUAL(find_equals(p->wakeup_commands, 30)[0], "wakeup.sh");

    fclose(f);
    delete_config(p);
}


int main(int argc, char* argv[])
{
    CU_TestInfo tests[] = {
        { "test_parse_config", test_parse_config },
        CU_TEST_INFO_NULL
    };
    CU_SuiteInfo suites[] = {
        { "Config", NULL, NULL, tests },
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
