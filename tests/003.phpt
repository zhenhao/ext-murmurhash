--TEST--
murmurhash64b() Basic test
--SKIPIF--
<?php
if (!extension_loaded('murmurhash')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo dechex(murmurhash64b('zhenhao', 0x1234abcd));
?>
--EXPECT--
58cbb7497cf1c488