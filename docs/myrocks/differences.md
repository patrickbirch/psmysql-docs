# Differences between Percona MyRocks and Facebook MyRocks

The original MyRocks was developed by Facebook
and works with their implementation of MySQL.
*Percona MyRocks* is a branch of MyRocks for *Percona Server for MySQL*
and includes the following differences from the original implementation:


* The behavior of the `START TRANSACTION WITH CONSISTENT SNAPSHOT` statement
depends on the [transaction isolation level](https://dev.mysql.com/doc/refman/5.7/en/innodb-transaction-isolation-levels.html).

| Storage Engine| Transaction isolation level |                              |
| ------------- | --------------------------- | ---------------------------- |
|               | `READ COMMITTED`            | `REPEATABLE READ` |
| InnoDB        | Success                     | Success            |
| Facebook MyRocks | Fail | Success (MyRocks engine only; read-only, as all MyRocks engine snapshots)|
| Percona MyRocks | Fail with any DML which would violate the read-only snapshot constraint | Success (read-only snapshots independent of the engines in use) |

* Percona MyRocks includes the `lz4` and `zstd` statically linked libraries.
