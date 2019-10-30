/* murmurhash extension for PHP (c) 2019 zhenhao */

#ifndef PHP_MURMURHASH_H
# define PHP_MURMURHASH_H

extern zend_module_entry murmurhash_module_entry;
# define phpext_murmurhash_ptr &murmurhash_module_entry

# define PHP_MURMURHASH_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_MURMURHASH)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#include <iostream>

uint64_t MurmurHash64A ( const void * key, int len, unsigned int seed );
uint64_t MurmurHash64B ( const void * key, int len, unsigned int seed );

#endif	/* PHP_MURMURHASH_H */

