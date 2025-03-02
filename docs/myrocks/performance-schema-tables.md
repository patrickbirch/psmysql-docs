# Performance Schema MyRocks changes

RocksDB WAL file information can be seen in the
[performance_schema.log_status](https://dev.mysql.com/doc/refman/8.0/en/performance-schema-log-status-table.html)
table in the `STORAGE ENGINE` column.

This feature has been implemented in [Percona Server for MySQL 8.0.15-6](../release-notes/Percona-Server-8.0.15-6.md#id1).

## Example

```sql
mysql> select * from performance_schema.log_status\G
```

The example of the output:

```text
*************************** 1. row ***************************

SERVER_UUID: f593b4f8-6fde-11e9-ad90-080027c2be11
    LOCAL: {"gtid_executed": "", "binary_log_file": "binlog.000004", "binary_log_position": 1698222}
REPLICATION: {"channels": []}
STORAGE_ENGINES: {"InnoDB": {"LSN": 36810235, "LSN_checkpoint": 36810235}, "RocksDB": {"wal_files": [{"path_name": "/000026.log", "log_number": 26, "size_file_bytes": 371869}]}}
1 row in set (0.00 sec)
```
