# ext-murmurhash
extension of murmurhash2 for php7

```bash
cd ext-murmurhash
phpize
./configure
make
make test
```

```php
<?php

echo murmurhash64a('zhenhao', 0x1), "\n";
echo murmurhash64b('zhenhao', 0x1), "\n";
```
