/*- 
 * Copyright (c) 2017 Etienne Bagnoud <etienne@artisan-numerique.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <check.h>
#include <ctype.h>
#include "../strid.h"

START_TEST(test_crc8) {
    int i = 0;
    Strid id1, id2;

    strid_init("");

    strid_generate_random(id1);
    ck_assert_int_eq(strid_valid(id1), 1);
    strid_generate_prefix(id2, "test.");
    ck_assert_int_eq(strid_valid(id2), 1);

    for(i = 0; id1[i] != '\0'; i++) {
        id1[i] = (unsigned char) tolower(id1[i]);
    }
    ck_assert_int_eq(strid_valid(id1), 1);
    for(i = 0; id2[i] != '\0'; i++) {
        id2[i] = (unsigned char) tolower(id2[i]);
    }
    ck_assert_int_eq(strid_valid(id2), 1);

    id1[0] = '-';
    ck_assert_int_eq(strid_valid(id1), 0);
    id2[0] = '-';
    ck_assert_int_eq(strid_valid(id2), 0);
}
END_TEST

START_TEST(test_id) {
    int i = 0;
    const char * A[] = { "AK6DKMLU5B", "UIF1S52X0D", "N9J6ZK6W66", "ULIEP2NN60", "CK5ZO32Y86", "BBQNB5MD91", "SS055HP533", "I49LULJX5C", "NT9UQ28P24", "D8A9Y7V738", NULL};
    const char * B[] = { "67QWW73092", "67HPM5AG58", "67RDOOBW83", "67QTKEUU74", "67A27LIQ56", "67DE168CD6", "674VWJXV2F", "67EZ3YIWA3", "67N6L8Q02B", "679KYJK0C3", NULL };
    Strid id;

    strid_init("");
    for(int i = 0; i < 10; i++) {
        strid_generate_random(id);
        ck_assert_str_eq(A[i], id);
        strid_generate_prefix(id, "strid");
        ck_assert_str_eq(B[i], id);
    }
}
END_TEST

START_TEST(test_equal_copy) {
    int i = 0;
    Strid id1, id2;

    strid_init("");
    for(i = 0; i < 10; i++) {
        strid_generate_random(id1);
        strid_copy(id2, id1);
        ck_assert_str_eq(id1, id2);
        ck_assert_int_eq(strid_equal(id1, id2), 1);
    }
}
END_TEST

Suite * strid_suite(void) {
    Suite * s;
    TCase * tc;

    s = suite_create("STRID");

    tc = tcase_create("CRC8");
    tcase_add_test(tc, test_crc8);
    suite_add_tcase(s, tc);

    tc = tcase_create("ID");
    tcase_add_test(tc, test_id);
    suite_add_tcase(s, tc);

    tc = tcase_create("COPY / EQUAL");
    tcase_add_test(tc, test_equal_copy);
    suite_add_tcase(s, tc);

    return s;
}

int main(int argc, char ** argv) {
    int failed = 0;
    Suite * s;
    SRunner  * sr;

    s = strid_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
