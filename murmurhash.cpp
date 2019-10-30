/* murmurhash extension for PHP (c) 2019 zhenhaowang */

extern "C" {
    #ifdef HAVE_CONFIG_H
    # include "config.h"
    #endif

    #include "php.h"
    #include "ext/standard/info.h"
}
#include "php_murmurhash.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

PHP_FUNCTION(murmurhash64a)
{
    char *data;
    size_t len;
    long seed;

    #ifdef FAST_ZPP
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sl", &data, &len, &seed) == FAILURE) {
        return;
    }
    #else
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STRING(data, len)
        Z_PARAM_LONG(seed)
    ZEND_PARSE_PARAMETERS_END();
    #endif

    uint64_t h = MurmurHash64A(data, len, seed);
    RETURN_LONG(h)
}

PHP_FUNCTION(murmurhash64b)
{
    char *data;
    size_t len;
    long seed;
    #ifdef FAST_ZPP
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sl", &data, &len, &seed) == FAILURE) {
        return;
    }
    #else
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STRING(data, len)
        Z_PARAM_LONG(seed)
    ZEND_PARSE_PARAMETERS_END();
    #endif

    uint64_t h = MurmurHash64B(data, len, seed);
    RETURN_LONG(h)
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(murmurhash)
{
#if defined(ZTS) && defined(COMPILE_DL_MURMURHASH)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(murmurhash)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "murmurhash support", "enabled");
    php_info_print_table_row(2, "version", PHP_MURMURHASH_VERSION);
    php_info_print_table_row(2, "author", PHP_MURMURHASH_AUTHOR);
    php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_murmurhash64a, 0)
    ZEND_ARG_INFO(0, data)
    ZEND_ARG_INFO(0, seed)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_murmurhash64b, 0)
    ZEND_ARG_INFO(0, data)
    ZEND_ARG_INFO(0, seed)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ murmurhash_functions[]
 */
static const zend_function_entry murmurhash_functions[] = {
	PHP_FE(murmurhash64a,		arginfo_murmurhash64a)
	PHP_FE(murmurhash64b,		arginfo_murmurhash64b)
	PHP_FE_END
};
/* }}} */

/* {{{ murmurhash_module_entry
 */
zend_module_entry murmurhash_module_entry = {
	STANDARD_MODULE_HEADER,
	"murmurhash",					/* Extension name */
	murmurhash_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(murmurhash),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(murmurhash),			/* PHP_MINFO - Module info */
	PHP_MURMURHASH_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MURMURHASH
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(murmurhash)
#endif

