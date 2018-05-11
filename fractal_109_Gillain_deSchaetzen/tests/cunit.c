#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libfractal/fractal.h"
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <semaphore.h>
#include <getopt.h>
#include <string.h>
#include "consumer_producer.h"
#include <Cunit/Basic.h>
void test()
{
(const char *name, int width, int height, double a, double b)
frac_t* f=fractal_new("fractal_test", 450, 320, 0.9, -0.3));
CU_ASSERT(strcmp(fractal_get_name(f),"fractal_test")==0);
CU_ASSERT(fractal_get_width(f)==450);
CU_ASSERT(fractal_get_height(f)==320);
CU_ASSERT_NOT_EQUAL(fractal_get_width, 400));
CU_ASSERT_EQUAL(fractal_get_a(f), 0.9);
CU_ASSERT_EQUAL(fractal_get_b(f), -0.3);
}
int main()
{
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
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test of fprintf()", testFPRINTF)) ||
       (NULL == CU_add_test(pSuite, "test of fread()", testFREAD)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
