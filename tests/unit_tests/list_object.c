
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "api.h"

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 * */
int init_suite1(void) {
    return 0;
}

/* The suite cleanup function.
 *  * Closes the temporary file used by the tests.
 *   * Returns zero on success, non-zero otherwise.
 *    */
int clean_suite1(void) {
    return 0;
}

const char* host = "kss.ksyun.com";

void TEST_LIST_OBJECT(void) {
    int error;
    buffer* resp = NULL;

    const char* access_key = "S1guCl0KF/oA285zzEDK";
    const char* secret_key = "DGSTgVMQ08EepL3CanUoatVV9en7mB856ljbNEaK";
    const char* bucket = "c-bucket1";
    resp = list_bucket_objects(host, bucket, access_key, secret_key, NULL, &error);
    CU_ASSERT(0 == error);
    CU_ASSERT(200 == resp->status_code);
    buffer_free(resp);
}

void TEST_LIST_OBJECT_BUCKET_NOT_EXIST(void) {
    int error;
    buffer* resp = NULL;

    const char* access_key = "S1guCl0KF/oA285zzEDK";
    const char* secret_key = "DGSTgVMQ08EepL3CanUoatVV9en7mB856ljbNEaK";
    const char* bucket = "bucket-not-exist1";
    resp = list_bucket_objects(host, bucket, access_key, secret_key, NULL, &error);
    CU_ASSERT(0 == error);
    CU_ASSERT(404 == resp->status_code);
    printf("status msg=%s\n", resp->status_msg);
    printf("error msg=%s\n", resp->body);
    buffer_free(resp);
}

void TEST_LIST_OBJECT_AK_INVALID() {
    int error;
    buffer* resp = NULL;

    const char* access_key = "S1guCl0KF/oA2xxxxxxxxxx";
    const char* secret_key = "DGSTgVMQ08EepL3CanUoatVV9en7mB856ljbNEaK";
    const char* bucket = "c-bucket1";
    resp = list_bucket_objects(host, bucket, access_key, secret_key, NULL, &error);
    CU_ASSERT(0 == error);
    CU_ASSERT(403 == resp->status_code);
    printf("status msg=%s\n", resp->status_msg);
    printf("error msg=%s\n", resp->body);
    buffer_free(resp);
}

void TEST_LIST_OBJECT_SK_INVALID() {
    int error;
    buffer* resp = NULL;

    const char* access_key = "S1guCl0KF/oA285zzEDK";
    const char* secret_key = "DGSTgVMQ08EepL3CanUoaxxxxxxxxxxxxxxxxxxxxxxx";
    const char* bucket = "c-bucket1";
    resp = list_bucket_objects(host, bucket, access_key, secret_key, NULL, &error);
    CU_ASSERT(0 == error);
    CU_ASSERT(403 == resp->status_code);
    printf("status msg=%s\n", resp->status_msg);
    printf("error msg=%s\n", resp->body);
    buffer_free(resp);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 * */
int main() {
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (CU_add_test(pSuite, "test list object\n",
                TEST_LIST_OBJECT) == NULL
          || CU_add_test(pSuite, "test list object with invalid ak\n",
              TEST_LIST_OBJECT_AK_INVALID) == NULL
          || CU_add_test(pSuite, "test list object with invalid sk\n",
              TEST_LIST_OBJECT_SK_INVALID) == NULL
          || CU_add_test(pSuite, "test list object with bucket not exist\n",
              TEST_LIST_OBJECT_BUCKET_NOT_EXIST) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}