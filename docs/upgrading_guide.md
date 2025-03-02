# Percona Server for MySQL In-Place Upgrading Guide: From 5.7 to 8.0

An in-place upgrade is performed by using existing data on the server and involves the following actions:


* Stopping the MySQL 5.7 server


* Replacing the old binaries with MySQL 8.0 binaries


* Starting the MySQL 8.0 server with the same data files.

While an in-place upgrade may not be suitable for all environments, especially those environments with many variables to consider, the upgrade should work in most cases.

The following list summarizes a number of the changes in the 8.0 series and has useful guides that can help you perform a smooth upgrade. We strongly recommend reading this information:


* [Upgrading MySQL](https://dev.mysql.com/doc/refman/8.0/en/upgrading.html)


* [Before You Begin](https://dev.mysql.com/doc/refman/8.0/en/upgrade-before-you-begin.html)


* [Upgrade Paths](https://dev.mysql.com/doc/refman/8.0/en/upgrade-paths.html)


* [Changes in MySQL 8.0](https://dev.mysql.com/doc/refman/8.0/en/upgrading-from-previous-series.html)


* [Preparing your Installation for Upgrade](https://dev.mysql.com/doc/refman/8.0/en/upgrade-prerequisites.html)


* [MySQL 8 Minor Version Upgrades Are ONE-WAY Only](https://www.percona.com/blog/2020/01/10/mysql-8-minor-version-upgrades-are-one-way-only/)


* [Percona Utilities That Make Major MySQL Version Upgrades Easier](https://www.percona.com/blog/percona-utilities-that-make-major-mysql-version-upgrades-easier/)


* [Percona Server for MySQL 8.0 Release notes](https://docs.percona.com/percona-server/latest/release-notes/release-notes_index.html)


* [Upgrade Troubleshooting](https://dev.mysql.com/doc/refman/8.0/en/upgrade-troubleshooting.html)


* [Rebuilding or Repairing Tables or Indexes](https://dev.mysql.com/doc/refman/8.0/en/rebuilding-tables.html)

Review other [Percona blogs](https://www.percona.com/blog/) that contain upgrade information.

Implemented in release Percona Server for MySQL 8.0.15-5, *Percona Server for MySQL* uses the upstream
implementation of binary log file encryption and relay log file encryption.

The encrypt-binlog variable is
removed, and the related command-line option –encrypt-binlog is not
supported. It is important to remove the encrypt-binlog variable from your
configuration file before you attempt to upgrade either from another release
in the *Percona Server for MySQL* 8.0 series or from *Percona Server for MySQL* 5.7.
Otherwise, a server boot error is generated and reports an unknown
variable.

The implemented binary log file encryption is compatible with the older
format. The encrypted binary log file used in a previous version of MySQL 8.0
series or Percona Server for MySQL series is supported.

Before you start the upgrade process, it is recommended to make a full backup of your database.
Copy the database configuration file, for example, `my.cnf`, to another directory to save it.

!!! warning

    Do not upgrade from 5.7 to 8.0 on a crashed instance. If the server instance has crashed, run the crash recovery before proceeding with the upgrade.

You can select one of the following ways to upgrade *Percona Server for MySQL* from 5.7 to 8.0:


* Upgrading using the Percona repositories


* Upgrading from Systems that Use the MyRocks or TokuDB Storage Engine and Partitioned Tables


* Upgrading using Standalone Packages
