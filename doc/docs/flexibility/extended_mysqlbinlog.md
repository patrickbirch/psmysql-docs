# Extended `mysqlbinlog`

*Percona Server for MySQL* has implemented protocol compression support for the
**mysqlbinlog** command.

You can request protocol compression when connecting to a remote server to
transfer binary log files. The protocol compression helps reduce the
bandwidth use and improves the transfer speed.

In the [mysqlbinlog utility](https://dev.mysql.com/doc/refman/5.7/en/mysqlbinlog.html) add either the
`--compress` or `-C` flag to the command-line options.

```sql
mysqlbinlog [--compress|-C] --remote-server
```

## Version Specific Information

> 
> * Percona Server for MySQL 5.7.10-1: Feature ported from *Percona Server for MySQL* 5.6
