--TEST--
murmurhash64a() Basic test
--SKIPIF--
<?php
if (!extension_loaded('murmurhash')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo dechex(murmurhash64a('zhenhao', 0x1234abcd));
?>
--EXPECT--
4e83d6f89e5bdcd8
