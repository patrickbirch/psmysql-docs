# Extended mysqlbinlog

*Percona Server for MySQL* has implemented compression support for
**mysqlbinlog**. This is similar to support that both `mysql` and
`mysqldump` programs include (the `-C`, `--compress` options “Use
compression in server/client protocol”). Using the compressed protocol helps
reduce the bandwidth use and speed up transfers.

*Percona Server for MySQL* has also implemented support for `SSL`.
**mysqlbinlog** now accepts the `SSL` connection options as all the
other client programs. This feature can be useful with
`--read-from-remote-server` option. Following `SSL` options are now
available:

* `--ssl` - Enable SSL for connection (automatically enabled with other flags).

* `--ssl-ca=name` - CA file in PEM format (check OpenSSL docs, implies –ssl).

* `--ssl-capath=name` - CA directory (check OpenSSL docs, implies –ssl).

* `--ssl-cert=name` - X509 cert in PEM format (implies –ssl).

* `--ssl-cipher=name` - SSL cipher to use (implies –ssl).

* `--ssl-key=name` - X509 key in PEM format (implies –ssl).

* `--ssl-verify-server-cert` - Verify server’s “Common Name” in its cert against hostname used when connecting. This option is disabled by default.

## Version Specific Information

    * 8.0.12-1: The feature was ported from *Percona Server for MySQL* 5.7
