#include <kdl/kdl.h>

#include "test_util.h"

#include <stdio.h>
#include <string.h>

static void test_basics(void)
{
    char const* const kdl_text = "node1 key=0x123 \"gar\xc3\xa7on\" ;"
                                 "node2 \\ // COMMENT\n"
                                 "    \"gar\\u{e7}on\"\n"
                                 "node3 { (t)child1; child2\n"
                                 "child3 }";
    char const* const garcon = "gar\xc3\xa7on";

    kdl_str doc = kdl_str_from_cstr(kdl_text);
    kdl_parser* parser = kdl_create_string_parser(doc, 0);

    // test all events
    kdl_event_data* ev;

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node1", 5) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_PROPERTY);
    ASSERT(ev->value.type == KDL_TYPE_NUMBER);
    ASSERT(ev->value.number.type == KDL_NUMBER_TYPE_INTEGER);
    ASSERT(ev->value.number.integer == 0x123);
    ASSERT(ev->value.type_annotation.data == NULL);
    ASSERT(memcmp("key", ev->name.data, 3) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_ARGUMENT);
    ASSERT(ev->value.type == KDL_TYPE_STRING);
    ASSERT(memcmp(ev->value.str.data, garcon, 7) == 0);
    ASSERT(ev->value.type_annotation.data == NULL);
    ASSERT(ev->name.data == NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node2", 5) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_ARGUMENT);
    ASSERT(ev->value.type == KDL_TYPE_STRING);
    ASSERT(memcmp(ev->value.str.data, garcon, 7) == 0);
    ASSERT(ev->value.type_annotation.data == NULL);
    ASSERT(ev->name.data == NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node3", 5) == 0);
    ASSERT(ev->value.type_annotation.data == NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "child1", 6) == 0);
    ASSERT(ev->value.type_annotation.data[0] == 't');
    ASSERT(ev->value.type_annotation.len == 1);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "child2", 6) == 0);
    ASSERT(ev->value.type_annotation.data == NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "child3", 6) == 0);
    ASSERT(ev->value.type_annotation.data == NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_EOF);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    kdl_destroy_parser(parser);
}

static void test_slashdash(void)
{
    char const* const kdl_text = "node1 /-key=0x123\n"
                                 "/- node2 { (t)child1; child2; child3 }\n"
                                 "node3";

    kdl_str doc = kdl_str_from_cstr(kdl_text);
    kdl_parser* parser = kdl_create_string_parser(doc, 0);

    // test all events
    kdl_event_data* ev;

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node1", 5) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node3", 5) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_EOF);
    ASSERT(ev->value.type == KDL_TYPE_NULL);

    kdl_destroy_parser(parser);

    parser = kdl_create_string_parser(doc, KDL_EMIT_COMMENTS);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node1", 5) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_PROPERTY));

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_START_NODE));
    ASSERT(memcmp(ev->name.data, "node2", 5) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_START_NODE));
    ASSERT(memcmp(ev->name.data, "child1", 6) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_END_NODE));

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_START_NODE));
    ASSERT(memcmp(ev->name.data, "child2", 6) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_END_NODE));

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_START_NODE));
    ASSERT(memcmp(ev->name.data, "child3", 6) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_END_NODE));

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == (KDL_EVENT_COMMENT | KDL_EVENT_END_NODE));

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node3", 5) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_EOF);

    kdl_destroy_parser(parser);
}

static void test_slashdash_child_block(void)
{
    kdl_str invalid_docs[] = {
        kdl_str_from_cstr("node /-{child} arg"),               // arg can't follow child block
        kdl_str_from_cstr("node /-{child} /-arg"),             // arg can't follow child block
        kdl_str_from_cstr("node {child} /-arg"),               // arg can't follow child block
        kdl_str_from_cstr("node { one } /-{ two } { three }"), // two child blocks
        kdl_str_from_cstr("node /-{ one } { two } { three }"), // two child blocks
    };
    int n_invalid = sizeof(invalid_docs) / sizeof(invalid_docs[0]);

    kdl_str valid_docs[] = {
        kdl_str_from_cstr("node arg /-{child}"),                  // child blocks can be slashdashed
        kdl_str_from_cstr("node /-arg {child}"),                  // args can be slashdashed
        kdl_str_from_cstr("node /-arg /-{child}"),                // multiple slashdashes
        kdl_str_from_cstr("node /-{one}/-{two}{three}"),          // {} after /-{}
        kdl_str_from_cstr("node { one; } /-{ two } /-{ three }"), // /-{} after {}
    };
    int n_valid = sizeof(valid_docs) / sizeof(valid_docs[0]);

    for (int i = 0; i < n_invalid; ++i) {
        kdl_parser* parser = kdl_create_string_parser(invalid_docs[i], KDL_READ_VERSION_2);

        kdl_event_data* ev;
        do {
            ev = kdl_parser_next_event(parser);
        } while (ev->event != KDL_EVENT_EOF && ev->event != KDL_EVENT_PARSE_ERROR);

        ASSERT(ev->event == KDL_EVENT_PARSE_ERROR);

        kdl_destroy_parser(parser);
    }

    for (int i = 0; i < n_valid; ++i) {
        kdl_parser* parser = kdl_create_string_parser(valid_docs[i], KDL_READ_VERSION_2);

        kdl_event_data* ev;
        do {
            ev = kdl_parser_next_event(parser);
        } while (ev->event != KDL_EVENT_EOF && ev->event != KDL_EVENT_PARSE_ERROR);

        ASSERT(ev->event == KDL_EVENT_EOF);

        kdl_destroy_parser(parser);
    }
}

static void test_unbalanced_brace(void)
{
    char const* const kdl_text = "node1 {";

    kdl_str doc = kdl_str_from_cstr(kdl_text);
    kdl_parser* parser = kdl_create_string_parser(doc, 0);

    // test all events
    kdl_event_data* ev;

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_PARSE_ERROR);

    kdl_destroy_parser(parser);
}

static void test_identifier_arg_v1(void)
{
    char const* const kdl_text = "node1 \"arg1\" arg2";

    kdl_str doc = kdl_str_from_cstr(kdl_text);
    kdl_parser* parser = kdl_create_string_parser(doc, KDL_READ_VERSION_1);

    // test all events
    kdl_event_data* ev;

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_ARGUMENT);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_PARSE_ERROR);

    kdl_destroy_parser(parser);
}

static void test_identifier_arg_v2(void)
{
    char const* const kdl_text = "node1 \"arg1\" arg2";

    kdl_str doc = kdl_str_from_cstr(kdl_text);
    kdl_parser* parser = kdl_create_string_parser(doc, KDL_READ_VERSION_2);

    // test all events
    kdl_event_data* ev;

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_ARGUMENT);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_ARGUMENT);

    kdl_destroy_parser(parser);
}

static void test_number_type(void)
{
    char const* const kdl_text = "node1; (12)node2;";

    kdl_str doc = kdl_str_from_cstr(kdl_text);
    kdl_parser* parser = kdl_create_string_parser(doc, 0);

    // test all events
    kdl_event_data* ev;

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_PARSE_ERROR);

    kdl_destroy_parser(parser);
}

static void test_bom(void)
{
    char const* const kdl_text = "\xEF\xBB\xBFnode";

    kdl_str doc = kdl_str_from_cstr(kdl_text);
    kdl_parser* parser = kdl_create_string_parser(doc, 0);

    // test all events
    kdl_event_data* ev;

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node", 4) == 0);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_END_NODE);

    ev = kdl_parser_next_event(parser);
    ASSERT(ev->event == KDL_EVENT_EOF);

    kdl_destroy_parser(parser);
}

static void test_parser_detects_version(void)
{
    kdl_event_data* ev;

    // KDL 1.0.0 variant

    char const* const kdl_text_v1 = "r#\"node\"#";

    kdl_str doc_v1 = kdl_str_from_cstr(kdl_text_v1);
    kdl_parser* parser_v1 = kdl_create_string_parser(doc_v1, KDL_DEFAULTS);


    ev = kdl_parser_next_event(parser_v1);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node", 4) == 0);

    ev = kdl_parser_next_event(parser_v1);
    ASSERT(ev->event == KDL_EVENT_END_NODE);

    ev = kdl_parser_next_event(parser_v1);
    ASSERT(ev->event == KDL_EVENT_EOF);

    kdl_destroy_parser(parser_v1);

    // KDL 2.0.0 variant

    char const* const kdl_text_v2 = "#\"node\"#";

    kdl_str doc_v2 = kdl_str_from_cstr(kdl_text_v2);
    kdl_parser* parser_v2 = kdl_create_string_parser(doc_v2, KDL_DEFAULTS);

    ev = kdl_parser_next_event(parser_v2);
    ASSERT(ev->event == KDL_EVENT_START_NODE);
    ASSERT(memcmp(ev->name.data, "node", 4) == 0);

    ev = kdl_parser_next_event(parser_v2);
    ASSERT(ev->event == KDL_EVENT_END_NODE);

    ev = kdl_parser_next_event(parser_v2);
    ASSERT(ev->event == KDL_EVENT_EOF);

    kdl_destroy_parser(parser_v2);
}

void TEST_MAIN(void)
{
    run_test("Parser: basics", &test_basics);
    run_test("Parser: slashdash", &test_slashdash);
    run_test("Parser: slashdash with child blocks", &test_slashdash_child_block);
    run_test("Parser: unbalanced {", &test_unbalanced_brace);
    run_test("Parser: arg can't be identifier in v1", &test_identifier_arg_v1);
    run_test("Parser: arg can be bare string in v2", &test_identifier_arg_v2);
    run_test("Parser: type can't be number", &test_number_type);
    run_test("Parser: BOM treated as whitespace", &test_bom);
    run_test("Parser: KDLv1 and KDLv2 both supported", &test_parser_detects_version);
}
