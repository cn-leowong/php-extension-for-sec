#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
 
 
/*包含ZEND提供的API、宏和基本的PHP内置函数,例如php_trim*/
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_test.h"
 
/* 开启模块中的全局变量 */
ZEND_DECLARE_MODULE_GLOBALS(test)
 
/* True global resources - no need for thread safety here */
static int le_test;
 
/* 
* 声明函数数组，提供给PHP使用
*/
const zend_function_entry test_functions[] = {
     PHP_FE(my_function,     NULL)          /* For testing, remove later. */
     PHP_FE_END     /* Must be the last line in test_functions[] */
};
/* }}} */
 
/* 模块结构，声明了startup\shutdown、模块名及phpinfo打印时的函数
*/
zend_module_entry test_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
     STANDARD_MODULE_HEADER,
#endif
     "test",
     test_functions,
     PHP_MINIT(test),
     PHP_MSHUTDOWN(test),
     PHP_RINIT(test),          /* Replace with NULL if there's nothing to do at request start */
     PHP_RSHUTDOWN(test),     /* Replace with NULL if there's nothing to do at request end */
     PHP_MINFO(test),
#if ZEND_MODULE_API_NO >= 20010901
     "0.1", /* Replace with version number for your extension */
#endif
     STANDARD_MODULE_PROPERTIES
};
 
#ifdef COMPILE_DL_TEST
ZEND_GET_MODULE(test)
#endif
 
/* 从php.ini配置文件中读取配置信息 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("test.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_test_globals, test_globals)
    STD_PHP_INI_ENTRY("test.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_test_globals, test_globals)
PHP_INI_END()
 
 
/* 初始化全局变量默认值 */
static void php_test_init_globals(zend_test_globals *test_globals)
{
     test_globals->global_value = 0;
     test_globals->global_string = NULL;
}
 
 
/* 模块第一次加载时被调用
*/
PHP_MINIT_FUNCTION(test)
{
     /* 注册全局变量 */
     REGISTER_INI_ENTRIES();
     
     return SUCCESS;
}
 
/* 模块关闭时调用
*/
PHP_MSHUTDOWN_FUNCTION(test)
{
     /* 释放全局变量 */
     UNREGISTER_INI_ENTRIES();
     
     return SUCCESS;
}
 
/* 每次请求前调用
*/
PHP_RINIT_FUNCTION(test)
{
     return SUCCESS;
}
 
/* 
/* 每次请求结束时调用
*/
PHP_RSHUTDOWN_FUNCTION(test)
{
     return SUCCESS;
}
 
/* phpinfo()输出扩展信息
*/
PHP_MINFO_FUNCTION(test)
{
     php_info_print_table_start();
     php_info_print_table_header(2, "test support", "enabled");
     php_info_print_table_end();
 
     /* 是否输出php.ini中的配置信息
     DISPLAY_INI_ENTRIES();
     */
}
 
 
/* 定义函数my_function提供给PHP中使用 */
PHP_FUNCTION(my_function)
{
     char *arg = NULL;
     int arg_len, len;
     char *strg;
 
     if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
          return;
     }
 
     len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "test", arg);
     RETURN_STRINGL(strg, len, 0);
}
