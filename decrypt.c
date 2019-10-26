/**
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Albert Zhan  www.5lazy.cn   github.com/2654709623/php-decrypt               |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_decrypt.h"

#include "lib.h"

ZEND_DECLARE_MODULE_GLOBALS(decrypt)

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("decrypt.switch","enable",PHP_INI_SYSTEM, OnUpdateString, global_switch, zend_decrypt_globals, decrypt_globals)
    STD_PHP_INI_ENTRY("decrypt.save_path","''",PHP_INI_SYSTEM, OnUpdateString, global_new, zend_decrypt_globals, decrypt_globals)
PHP_INI_END()

zend_op_array *decrypt_compile_file(zend_file_handle *file_handle, int type)
{
    if(is_dirs(DCG(global_new)))
    {
        php_error_docref(NULL,E_ERROR, "%s folder does not exist", DCG(global_new));
    }

    char   *buf = NULL;
    size_t size = 0;

    if(zend_stream_fixup(file_handle, &buf, &size) == SUCCESS)
    {
        char *dir_name  = strcat(DCG(global_new), DS);
        char *file_name = "decrypt.code";

        FILE *fp = fopen(strcat(dir_name, file_name), "a+");
        if(fp != NULL)
        {
            fwrite(buf, size, 1, fp);
        }
        fclose(fp);
    }

    return old_compile_file(file_handle, type);
}

//if (access(beast_debug_path, F_OK) == 0) {
//
//            char realpath[1024];
//
//            sprintf(realpath, "%s/%s", beast_debug_path, h->filename);
//
//            if (super_mkdir(realpath) == 0) {
//
//                FILE *debug_fp = fopen(realpath, "w+");
//
//                if (debug_fp) {
//                    fwrite(buf, size, 1, debug_fp);
//                    fclose(debug_fp);
//                }
//            }
//        }

int super_mkdir(char *path)
{
    char *head, *last;
    char temp[1024];

    for (head = last = path; *last; last++) {

        if (*last == '/') {

            if (last > head) {

                memset(temp, 0, 1024);
                memcpy(temp, path, last - path);

                if (access(temp, F_OK) == -1) {
                    if (mkdir(temp, 0777) != 0) {
                        return -1;
                    }
                }
            }

            head = last + 1;
        }
    }

    return 0;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(decrypt)
{
    REGISTER_INI_ENTRIES();

    if(!strcmp(DCG(global_switch),"enable"))
    {
        old_compile_file = zend_compile_file;
        zend_compile_file = decrypt_compile_file;
    }

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(decrypt)
{
    if(!strcmp(DCG(global_switch),"enable"))
    {
        zend_compile_file = old_compile_file;
    }
    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(decrypt)
{
#if defined(COMPILE_DL_DECRYPT) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(decrypt)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(decrypt)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "Decrypt Support", "enabled");
    php_info_print_table_row(2, "Version", PHP_DECRYPT_VERSION);
    php_info_print_table_row(2, "Author", "Albert Zhan");
    php_info_print_table_row(2, "Email", "albertzhan666@gmail.com");
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ decrypt_functions[]
 *
 * Every user visible function must have an entry in decrypt_functions[].
 */
const zend_function_entry decrypt_functions[] = {
    PHP_FE_END	/* Must be the last line in decrypt_functions[] */
};
/* }}} */

/* {{{ decrypt_module_entry
 */
zend_module_entry decrypt_module_entry = {
    STANDARD_MODULE_HEADER,
    "decrypt",
    decrypt_functions,
    PHP_MINIT(decrypt),
    PHP_MSHUTDOWN(decrypt),
    NULL,   /* Replace with NULL if there's nothing to do at request start */
    NULL,	/* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(decrypt),
    PHP_DECRYPT_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DECRYPT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(decrypt)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
