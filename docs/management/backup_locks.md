# Backup Locks

*Percona Server for MySQL* offers the `LOCK TABLES FOR BACKUP` statement as a
lightweight alternative to `FLUSH TABLES WITH READ LOCK` for both physical and
logical backups.

!!! note

    As of *Percona Server for MySQL* 8.0.13-4, `LOCK TABLES FOR BACKUP` requires the `BACKUP_ADMIN` privilege.

## LOCK TABLES FOR BACKUP

`LOCK TABLES FOR BACKUP` uses a new MDL lock type to block updates to
non-transactional tables and DDL statements for all tables. If there is an
active `LOCK TABLES FOR BACKUP` lock then all DDL statements and all updates
to MyISAM, CSV, MEMORY, ARCHIVE, *TokuDB*, and *MyRocks* tables will be blocked
in the `Waiting for backup lock` status, visible in `PERFORMANCE_SCHEMA` or
`PROCESSLIST`.

`LOCK TABLES FOR BACKUP` has no effect on `SELECT` queries for all mentioned
storage engines. Against *InnoDB*, *MyRocks*, Blackhole and Federated tables,
the `LOCK TABLES FOR BACKUP` is not applicable to the `INSERT`, `REPLACE`,
`UPDATE`, `DELETE` statements: Blackhole tables obviously have no relevance
to backups, and Federated tables are ignored by both logical and physical backup
tools.

Unlike `FLUSH TABLES WITH READ LOCK`, `LOCK TABLES FOR BACKUP` does not
flush tables, i.e. storage engines are not forced to close tables and tables are
not expelled from the table cache. As a result, `LOCK TABLES FOR BACKUP` only
waits for conflicting statements to complete (i.e. DDL and updates to
non-transactional tables). It never waits for SELECTs, or UPDATEs to *InnoDB* or
*MyRocks* tables to complete, for example.

If an “unsafe” statement is executed in the same connection that is holding a
`LOCK TABLES FOR BACKUP` lock, it fails with the following error:

```text
ERROR 1880 (HY000): Can't execute the query because you have a conflicting backup lock

UNLOCK TABLES releases the lock acquired by LOCK TABLES FOR BACKUP.
```

The intended use case for *Percona XtraBackup* is:

```text
LOCK TABLES FOR BACKUP
... copy .frm, MyISAM, CSV, etc. ...
UNLOCK TABLES
... get binlog coordinates ...
... wait for redo log copying to finish ...
```

## Privileges

The `LOCK TABLES FOR BACKUP` requires the `BACKUP_ADMIN`
privilege.

## Interaction with other global locks

The `LOCK TABLES FOR BACKUP` has no effect if the current connection already
owns a `FLUSH TABLES WITH READ LOCK` lock, as it is a more restrictive
lock. If `FLUSH TABLES WITH READ LOCK` is executed in a connection that has
acquired `LOCK TABLES FOR BACKUP`, `FLUSH TABLES WITH READ LOCK` fails with
an error.

If the server is operating in the read-only mode (i.e. read_only set
to `1`), statements that are unsafe for backups will be either blocked or fail
with an error, depending on whether they are executed in the same connection
that owns `LOCK TABLES FOR BACKUP` lock, or other connections.

## MyISAM index and data buffering

*MyISAM* key buffering is normally write-through, i.e. by the time each update
to a *MyISAM* table is completed, all index updates are written to disk. The
only exception is delayed key writing feature which is disabled by default.

When the global system variable delay_key_write is set to `ALL`,
key buffers for all *MyISAM* tables are not flushed between updates, so a
physical backup of those tables may result in broken *MyISAM* indexes. To
prevent this, `LOCK TABLES FOR BACKUP` will fail with an error if
delay_key_write is set to `ALL`. An attempt to set delay_key_write to `ALL` when there’s an active backup lock will
also fail with an error.

Another option to involve delayed key writing is to create *MyISAM* tables with
the DELAY_KEY_WRITE option and set the delay_key_write variable to
`ON` (which is the default). In this case, `LOCK TABLES FOR BACKUP` will not
be able to prevent stale index files from appearing in the backup. Users are
encouraged to set delay_key_writes to `OFF` in the configuration
file, `my.cnf`, or repair *MyISAM* indexes after restoring from a physical
backup created with backup locks.

*MyISAM* may also cache data for bulk inserts, e.g. when executing multi-row
INSERTs or `LOAD DATA` statements. Those caches, however, are flushed between
statements, so have no effect on physical backups as long as all statements
updating *MyISAM* tables are blocked.

## The mysqldump Command

`mysqldump` has also been extended with a new option,
[lock-for-backup](#backup-locks) (disabled by default). When used together with the
`--single-transaction` option, the option makes `mysqldump` issue
`LOCK TABLES FOR BACKUP` before starting the dump operation to prevent unsafe
statements that would normally result in an inconsistent backup.

When used without the `--single-transaction` option,
[lock-for-backup](#backup-locks) is automatically converted to lock-all-tables.

The option [lock-for-backup](#backup-locks) is mutually exclusive with *lock-all-tables*, i.e. specifying both on the command line will lead to
an error.

If the backup locks feature is not supported by the target server, but
[lock-for-backup](#backup-locks) is specified on the command line, `mysqldump` aborts with an error.

## Version Specific Information

* 8.0.12-1: The feature was ported from *Percona Server for MySQL* 5.7.

## System Variables

### `have_backup_locks`

| Option         | Description        |
| -------------- | ------------------ |
| Command Line:  | Yes                |
| Config file    | No                 |
| Scope:         | Global             |
| Dynamic:       | No                 |
| Data type      | Boolean            |
| Default value   | YES                |

This is a server variable implemented to help other utilities decide what
locking strategy can be implemented for a server. When available, the backup
locks feature is supported by the server and the variable value is always
`YES`.

## Status Variables

### `Com_lock_tables_for_backup`

| Option         | Description        |
| -------------- | ------------------ |
| Scope:         | Global/Session     |
| Data type      | Numeric            |

This status variable indicates the number of times the corresponding statements
have been executed.

## Client Command Line Parameter

### `lock-for-backup`

| Option         | Description        |
| -------------- | ------------------ |
| Command Line:  | Yes                |
| Scope:         | Global             |
| Dynamic:       | No                 |
| Data type      | String             |
| Default value   | Off                |

When used together with the *–-single-transaction* option, the option
makes `mysqldump` issue `LOCK TABLES FOR BACKUP` before starting the dump
operation to prevent unsafe statements that would normally result in an
inconsistent backup.
