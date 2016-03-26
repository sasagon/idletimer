#ifdef HAVE_CONFIG_H
#include "../config.h"

#ifdef HAVE_CUNIT_CUNIT_H
//#ifdef HAVE_LIBCUNIT

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include "../config_parser.h"

typedef struct {
    ConfigErrorType error_type;
    char* error_filename;
    int error_line_number;
} ErrorInfo;


static bool config_error_handler(
    ConfigErrorType type, const char* filename, int line_number, void* data)
{
    ErrorInfo* p = (ErrorInfo*)data;
    p->error_type = type;
    p->error_filename = strdup(filename);
    p->error_line_number = line_number;
    return true;
}


static void test_parse_config()
{
    char test_input[] = "# comment\n"
                        "\r\n"
                        "idle:1:test.sh\n"
                        "wakeup:30:wakeup.sh\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    ErrorInfo error_info = { NO_ERROR, NULL, 0 };

    Config* p = parse_config(f, "test.conf", config_error_handler, &error_info);

    CU_ASSERT_EQUAL(error_info.error_type, NO_ERROR);
    CU_ASSERT_STRING_EQUAL(find_equals(p->idle_commands, 1)[0], "test.sh");
    CU_ASSERT_STRING_EQUAL(find_equals(p->wakeup_commands, 30)[0], "wakeup.sh");

    fclose(f);
    delete_config(p);
}


static void test_parse_config_too_short_minutes_error()
{
    char test_input[] = "idle:0:too_short_minute\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    ErrorInfo error_info = { NO_ERROR, NULL, 0 };

    Config* p = parse_config(f, "test.conf", config_error_handler, &error_info);

    CU_ASSERT_EQUAL(error_info.error_type, TOO_SHORT_MINUTES);

    fclose(f);
    delete_config(p);
}


static void test_parse_config_too_long_minutes_error()
{
    char test_input[1024];
    snprintf(test_input, sizeof(test_input),
        "idle:%lu:too_long_minutes\n", ULONG_MAX);
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    ErrorInfo error_info = { NO_ERROR, NULL, 0 };

    Config* p = parse_config(f, "test.conf", config_error_handler, &error_info);

    CU_ASSERT_EQUAL(error_info.error_type, TOO_LONG_MINUTES);

    fclose(f);
    delete_config(p);
}


static void test_parse_config_illegal_command_type_error()
{
    char test_input[] = "illegal:1:illegal_command_type\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    ErrorInfo error_info = { NO_ERROR, NULL, 0 };

    Config* p = parse_config(f, "test.conf", config_error_handler, &error_info);

    CU_ASSERT_EQUAL(error_info.error_type, ILLEGAL_COMMAND_TYPE);

    fclose(f);
    delete_config(p);
}


static void test_parse_config_illegal_minutes_error()
{
    char test_input[] = "idle:illegal:illegal_minutes\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    ErrorInfo error_info = { NO_ERROR, NULL, 0 };

    Config* p = parse_config(f, "test.conf", config_error_handler, &error_info);

    CU_ASSERT_EQUAL(error_info.error_type, ILLEGAL_MINUTES);

    fclose(f);
    delete_config(p);
}


static void test_parse_config_illegal_line_format_error()
{
    char test_input[] = "*** ILLEGAL LINE ***\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    ErrorInfo error_info = { NO_ERROR, NULL, 0 };

    Config* p = parse_config(f, "test.conf", config_error_handler, &error_info);

    CU_ASSERT_EQUAL(error_info.error_type, ILLEGAL_LINE_FORMAT);

    fclose(f);
    delete_config(p);
}


static void test_parse_config_too_long_line_error()
{
    char test_input[CONFIG_PARSER_MAX_LINE_LENGTH * 3];
    memset(test_input, 'x', sizeof(test_input) - 1);
    test_input[sizeof(test_input) - 1] = '\0';
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");
    ErrorInfo error_info = { NO_ERROR, NULL, 0 };

    Config* p = parse_config(f, "test.conf", config_error_handler, &error_info);

    CU_ASSERT_EQUAL(error_info.error_type, TOO_LONG_LINE);

    fclose(f);
    delete_config(p);
}


static bool config_error_handler_returns_false(
    ConfigErrorType type, const char* filename, int line_number, void* data)
{
    return false;
}


static void test_parse_config_stop_on_error()
{
    char test_input[] = "idle:1:test.sh\n"
                        "*** ILLEGAL LINE ***\n"
                        "wakeup:30:wakeup.sh\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");

    Config* p = parse_config(
        f, "test.conf", config_error_handler_returns_false, NULL);

    CU_ASSERT_STRING_EQUAL(find_equals(p->idle_commands, 1)[0], "test.sh");
    CU_ASSERT_TRUE(is_command_map_empty(p->wakeup_commands));

    fclose(f);
    delete_config(p);
}


static bool config_error_handler_returns_true(
    ConfigErrorType type, const char* filename, int line_number, void* data)
{
    return true;
}


static void test_parse_config_continue_on_error()
{
    char test_input[] = "idle:1:test.sh\n"
                        "*** ILLEGAL LINE ***\n"
                        "wakeup:30:wakeup.sh\n";
    FILE* f = fmemopen(test_input, sizeof(test_input), "r");

    Config* p = parse_config(
        f, "test.conf", config_error_handler_returns_true, NULL);

    CU_ASSERT_STRING_EQUAL(find_equals(p->idle_commands, 1)[0], "test.sh");
    CU_ASSERT_STRING_EQUAL(find_equals(p->wakeup_commands, 30)[0], "wakeup.sh");

    fclose(f);
    delete_config(p);
}


int main(int argc, char* argv[])
{
    CU_TestInfo tests[] = {
        { "test_parse_config", test_parse_config },
        { "test_parse_config_too_short_minutes_error",
            test_parse_config_too_short_minutes_error },
        { "test_parse_config_too_long_minutes_error",
            test_parse_config_too_long_minutes_error },
        { "test_parse_config_illegal_command_type_error",
            test_parse_config_illegal_command_type_error },
        { "test_parse_config_illegal_minutes_error",
            test_parse_config_illegal_minutes_error },
        { "test_parse_config_illegal_line_format_error",
            test_parse_config_illegal_line_format_error },
        { "test_parse_config_too_long_line_error",
            test_parse_config_too_long_line_error },
        { "test_parse_config_stop_on_error",
            test_parse_config_stop_on_error },
        { "test_parse_config_continue_on_error",
            test_parse_config_continue_on_error },
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
