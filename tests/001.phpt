--TEST--
Check if murmurhash is loaded
--SKIPIF--
<?php
if (!extension_loaded('murmurhash')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "murmurhash" is available';
?>
--EXPECT--
The extension "murmurhash" is available
