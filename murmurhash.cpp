/* murmurhash extension for PHP (c) 2019 zhenhao */
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
	char* data;
	int len;
	int seed;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"sl",&data,&len,&seed) == FAILURE) {
        return;
	}

	uint64_t h = MurmurHash64A(data, len, seed);

	RETURN_LONG(h);
}

PHP_FUNCTION(murmurhash64b)
{
	char* data;
	int len;
	int seed;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"sl",&data,&len,&seed) == FAILURE) {
        return;
	}

	uint64_t h = MurmurHash64B(data, len, seed);

	RETURN_LONG(h);
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
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_murmurhash_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_murmurhash_test2, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_murmurhash64a, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, seed)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_murmurhash64b, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, seed)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ murmurhash_functions[]
 */
static const zend_function_entry murmurhash_functions[] = {
	//PHP_FE(murmurhash_test1,		arginfo_murmurhash_test1)
	//PHP_FE(murmurhash_test2,		arginfo_murmurhash_test2)
	PHP_FE(murmurhash64a, arginfo_murmurhash64a)
	PHP_FE(murmurhash64b, arginfo_murmurhash64b)
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

uint64_t MurmurHash64A ( const void * key, int len, unsigned int seed )
{
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len/8);

	while(data != end)
	{
		uint64_t k = *data++;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h ^= k;
		h *= m; 
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7)
	{
	case 7: h ^= uint64_t(data2[6]) << 48;
	case 6: h ^= uint64_t(data2[5]) << 40;
	case 5: h ^= uint64_t(data2[4]) << 32;
	case 4: h ^= uint64_t(data2[3]) << 24;
	case 3: h ^= uint64_t(data2[2]) << 16;
	case 2: h ^= uint64_t(data2[1]) << 8;
	case 1: h ^= uint64_t(data2[0]);
	        h *= m;
	};
 
	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
} 


// 64-bit hash for 32-bit platforms

uint64_t MurmurHash64B ( const void * key, int len, unsigned int seed )
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	unsigned int h1 = seed ^ len;
	unsigned int h2 = 0;

	const unsigned int * data = (const unsigned int *)key;

	while(len >= 8)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;

		unsigned int k2 = *data++;
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}

	if(len >= 4)
	{
		unsigned int k1 = *data++;
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}

	switch(len)
	{
	case 3: h2 ^= ((unsigned char*)data)[2] << 16;
	case 2: h2 ^= ((unsigned char*)data)[1] << 8;
	case 1: h2 ^= ((unsigned char*)data)[0];
			h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	uint64_t h = h1;

	h = (h << 32) | h2;

	return h;
} 

#ifdef COMPILE_DL_MURMURHASH
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(murmurhash)
#endif

