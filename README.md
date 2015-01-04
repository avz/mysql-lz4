# mysql-lz4

LZ4 compression/decompression UDF for MySQL

## Examples

#### Compression (`LZ4_COMPRESS`)

```sql
SELECT LENGTH(LZ4_COMPRESS(REPEAT('0', 1024 * 1024)))
```
```
+------------------------------------------------+
| LENGTH(LZ4_COMPRESS(REPEAT('0', 1024 * 1024))) |
+------------------------------------------------+
|                                           4122 |
+------------------------------------------------+
```

#### Decompression (`LZ4_DECOMPRESS`)

```sql
SELECT LENGTH(LZ4_DECOMPRESS(LZ4_COMPRESS(REPEAT('0', 1024 * 1024))))
```

```
+----------------------------------------------------------------+
| LENGTH(LZ4_DECOMPRESS(LZ4_COMPRESS(REPEAT('0', 1024 * 1024)))) |
+----------------------------------------------------------------+
|                                                        1048576 |
+----------------------------------------------------------------+
```

## Installation

Build and install plugin
```
% make
# make install
```

Import functions
```sql
CREATE FUNCTION LZ4_COMPRESS RETURNS STRING SONAME 'mysql-lz4.so';
CREATE FUNCTION LZ4_DECOMPRESS RETURNS STRING SONAME 'mysql-lz4.so';
```
