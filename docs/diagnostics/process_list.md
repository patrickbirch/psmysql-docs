# Process List

This page describes Percona changes to both the standard *MySQL* `SHOW PROCESSLIST` command and the standard *MySQL* `INFORMATION_SCHEMA` table `PROCESSLIST`.

## Version Specific Information

> 
> * 8.0.12-1: The feature was ported from *Percona Server for MySQL* 5.7.

## INFORMATION_SCHEMA Tables

`INFORMATION_SCHEMA.PROCESSLIST`

This table implements modifications to the standard MySQL `INFORMATION_SCHEMA` table `PROCESSLIST`.

| Column Name     | Description                                                                                                                                                                                                                                                                    |
|-----------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ‘ID’            | ‘The connection identifier.’                                                                                                                                                                                                                                                   |
| ‘USER’          | ‘The MySQL user who issued the statement.’                                                                                                                                                                                                                                     |
| ‘HOST’          | ‘The host name of the client issuing the statement.’                                                                                                                                                                                                                           |
| ‘DB’            | ‘The default database, if one is selected, otherwise NULL.’                                                                                                                                                                                                                    |
| ‘COMMAND’       | ‘The type of command the thread is executing.’                                                                                                                                                                                                                                 |
| ‘TIME’          | ‘The time in seconds that the thread has been in its current state.’                                                                                                                                                                                                           |
| ‘STATE’         | ‘An action, event, or state that indicates what the thread is doing.’                                                                                                                                                                                                          |
| ‘INFO’          | ‘The statement that the thread is executing, or NULL if it is not executing any statement.’                                                                                                                                                                                    |
| ‘TIME_MS’       | ‘The time in milliseconds that the thread has been in its current state.’                                                                                                                                                                                                      |
| ‘ROWS_EXAMINED’ | ‘The number of rows examined by the statement being executed (NOTE: This column is not updated for each examined row so it does not necessarily show an up-to-date value while the statement is executing. It only shows a correct value after the statement has completed.).’ |
| ‘ROWS_SENT’     | ‘The number of rows sent by the statement being executed.’                                                                                                                                                                                                                     |
| ‘TID’           | ‘The Linux Thread ID. For Linux, this corresponds to light-weight process ID (LWP ID) and can be seen in the ps -L output. In case when Thread Pool is enabled, “TID” is not null for only currently executing statements and statements received via “extra” connection.’     |

## Example Output

Table [PROCESSLIST](https://docs.percona.com/percona-server/8.0/diagnostics/process_list.html#processlist):

```sql
mysql> SELECT * FROM INFORMATION_SCHEMA.PROCESSLIST;
```

The output could be:

```text
+----+------+-----------+--------------------+---------+------+-----------+---------------------------+---------+-----------+---------------+
| ID | USER | HOST      | DB                 | COMMAND | TIME | STATE     | INFO                      | TIME_MS | ROWS_SENT | ROWS_EXAMINED |
+----+------+-----------+--------------------+---------+------+-----------+---------------------------+---------+-----------+---------------+
| 12 | root | localhost | information_schema | Query   |    0 | executing | select * from processlist |       0 |         0 |             0 |
+----+------+-----------+--------------------+---------+------+-----------+---------------------------+---------+-----------+---------------+
```
