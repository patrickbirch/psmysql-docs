# Installing Percona Server for MySQL on Red Hat Enterprise Linux and CentOS

<!-- package name: percona-server-server-8.0.13-3.1.el7.x86_64.rpm -->
Ready-to-use packages are available from the *Percona Server for MySQL* software
repositories and the [download page](http://www.percona.com/downloads/Percona-Server-8.0/). The
*Percona* **yum** repository supports popular *RPM*-based
operating systems. The easiest way to install the *Percona Yum* repository is to install an *RPM*
that configures **yum** and installs the [Percona GPG key](https://www.percona.com/downloads/RPM-GPG-KEY-percona).

Specific information on the supported platforms, products, and versions are described in [Percona Software and Platform Lifecycle](https://www.percona.com/services/policies/percona-software-platform-lifecycle#mysql).

*Percona Server for MySQL* is certified for Red Hat Enterprise Linux 8. This certification is based on common and secure best practices and successful interoperability with the operating system. Percona Server is listed in the [Red Hat Ecosystem Catalog](https://catalog.redhat.com/software/applications/detail/5869161).

!!! note

    The RPM packages for Red Hat Enterprise Linux 7 and the compatible derivatives do not support TLSv1.3, as it requires OpenSSL 1.1.1, which is currently not available on this platform.

## What’s in each RPM package?

Each of the *Percona Server for MySQL* RPM packages has a particular purpose.

|Package                 | Contains                                                                                                                                                                                                                                                   |
|------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| percona-server-server        | Server itself (the mysqld binary)                                                                                                                                                                                                                          |
| percona-server-debuginfo     | Debug symbols for the server                                                                                                                                                                                                                               |
| percona-server-client        | Command line client                                                                                                                                                                                                                                        |
| percona-server-devel         | Header files needed to compile software using the client library.                                                                                                                                                                                          |
| percona-server-shared        | Client shared library.                                                                                                                                                                                                                                     |
| percona-server-shared-compat | Shared libraries for software compiled against old versions of the client library. The following libraries are included in this package: libmysqlclient.so.12, libmysqlclient.so.14, libmysqlclient.so.15, libmysqlclient.so.16, and libmysqlclient.so.18. |
| percona-server-test          | Includes the test suite for Percona Server for MySQL.                                                                                                                                                                                                      |

## Installing *Percona Server for MySQL* from Percona `yum` repository

You can install Percona yum repository by running the following commands as a `root` user or with sudo.


1. Install the Percona repository

	```shell
	$ sudo yum install https://repo.percona.com/yum/percona-release-latest.noarch.rpm
	```
	
	You should see an output that the files are being downloaded, like the following:
	
	```text
	percona-release-latest.noarch-rpm               36 kB/s | 19 kb 00:00
	=====================================================================
	  Package         Architecture      Version    Repository    Size
	=====================================================================
	Installing:
	   percona release noarch         1.0-25     @commandline  19k
	...
	```


2. Enable the repository:

	```shell
	$ sudo percona-release setup ps80
	```
	
	The output could be like the following:
	
	```text
	On RedHat 8 systems it is needed to disable dnf mysql module to install Percona-Server
	Do you want to disable it? [y/N] y
	...
	```


3. Install the packages

	```shell
	$ sudo yum install percona-server-server
	```
	
	!!! note
	
	    *Percona Server for MySQL* 8.0 also provides the TokuDB storage engine and MyRocks storage engines which can be installed as plugins.

Starting with Percona Server for MySQL 8.0.28-19 (2022-05-12), the TokuDB storage engine is no longer supported. We have removed the storage engine from the installation packages and disabled the storage engine in our binary builds. For more information, see TokuDB Introduction.

For more information on how to install and enable the *TokuDB* storage review the TokuDB Installation document.
For information on how to install and enable *MyRocks* review the
section Percona MyRocks Installation Guide.

### Percona yum Testing repository

Percona offers pre-release builds from our testing repository. To
subscribe to the testing repository, you enable the testing
repository in `/etc/yum.repos.d/percona-release.repo`. To do so,
set both `percona-testing-$basearch` and `percona-testing-noarch`
to `enabled = 1` (Note that there are three sections in this file:
release, testing, and experimental - in this case, it is the second section that requires updating).

!!! note

    You must install the Percona repository first if the installation has not been done already.

## Installing *Percona Server for MySQL* using downloaded rpm packages


1. Download the packages of the desired series for your architecture from the
[download page](http://www.percona.com/downloads/Percona-Server-8.0/). The
easiest way is to download the bundle which contains all the packages. The following example downloads *Percona Server for MySQL* 8.0.21-12 release packages for *RHEL* 8.

	```shell
	$ wget https://downloads.percona.com/downloads/Percona-Server-LATEST/Percona-Server-8.0.29-21/binary/redhat/8/x86_64/Percona-Server-8.0.29-21-rc59f87d2854-el8-x86_64-bundle.tar
	```


2. Unpack the bundle to get the packages: `tar xvf Percona-Server-8.0.29-21-rc59f87d2854-el8-x86_64-bundle.tar`


3. To view a list of packages, run the following command:

	```shell
	$ ls *.rpm
	```
	The output should look like the following:
	
	```text
	percona-icu-data-files-8.0.29-21.1.el8.x86_64.rpm
	percona-mysql-router-8.0.29-21.1.el8.x86_64.rpm
	percona-mysql-router-debuginfo-8.0.29-21.1.el8.x86_64.rpm
	percona-server-client-8.0.29-21.1.el8.x86_64.rpm
	percona-server-client-debuginfo-8.0.29-21.1.el8.x86_64.rpm
	percona-server-debuginfo-8.0.29-21.1.el8.x86_64.rpm
	percona-server-debugsource-8.0.29-21.1.el8.x86_64.rpm
	percona-server-devel-8.0.29-21.1.el8.x86_64.rpm
	percona-server-rocksdb-8.0.29-21.1.el8.x86_64.rpm
	percona-server-rocksdb-debuginfo-8.0.29-21.1.el8.x86_64.rpm
	percona-server-server-8.0.29-21.1.el8.x86_64.rpm
	percona-server-server-debuginfo-8.0.29-21.1.el8.x86_64.rpm
	percona-server-shared-8.0.29-21.1.el8.x86_64.rpm
	percona-server-shared-compat-8.0.29-21.1.el8.x86_64.rpm
	percona-server-shared-debuginfo-8.0.29-21.1.el8.x86_64.rpm
	percona-server-test-8.0.29-21.1.el8.x86_64.rpm
	percona-server-test-debuginfo-8.0.29-21.1.el8.x86_64.rpm
	```
	

4. Install `jemalloc` with the following command, if needed:
	
	```shell
	wget https://repo.percona.com/yum/release/8/RPMS/x86_64/jemalloc-3.6.0-1.el8.x86_64.rpm
	```


5. For an EL8-based *RHEL* distribution or derivatives package installation, the *Percona Server for MySQL* requires the mysql module to be disabled before installing the packages:

	```shell
	sudo yum module disable mysql
	```


6. Install all the packages (for debugging, testing, etc.) with the following command:

	```shell
	$ sudo rpm -ivh *.rpm
	```
	
	!!! note
	
	    When installing packages manually, you must make sure to resolve all dependencies and install any missing packages yourself.

## Running *Percona Server for MySQL*

*Percona Server for MySQL* stores the data files in `/var/lib/mysql/` by
default. The configuration file used to manage *Percona Server for MySQL* is the `/etc/my.cnf`.

The following commands start, provide the server status, stop the server, and restart the server.

!!! note

    The *RHEL* distributions and derivatives come with [systemd](http:/ freedesktop.org/wiki/Software/systemd/) as the default system and service manager so you can invoke all of the commands with `sytemctl` instead of `service`. Currently, both options are supported.


* *Percona Server for MySQL* is not started automatically on the *RHEL* distributions and derivatives after installation. Start the server with the following command:
	
	```shell
	$ sudo service mysql start
	```


* Review the service status with the following command:

	```shell
	$ sudo service mysql status
	```


* Stop the service with the following command:
	
	```shell
	$ sudo service mysql stop
	```


* Restart the service with the following command:

	```shell
	$ sudo service mysql restart
	```

## SELinux and security considerations

For information on working with SELinux, see [Working with SELinux](../security/selinux.md).

The *RHEL* 8 distributions and derivatives have added [system-wide cryptographic policies component](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/security_hardening/using-the-system-wide-cryptographic-policies_security-hardening). This component allows the configuration of cryptographic subsystems.

## Uninstalling *Percona Server for MySQL*

To completely uninstall *Percona Server for MySQL*, remove all the installed packages and data files.


1. Stop the *Percona Server for MySQL* service:

	```shell
	$ sudo service mysql stop
	```


2. Remove the packages:

	```shell
	$ sudo yum remove percona-server*
	```


3. Remove the data and configuration files:

	!!! warning
	
	   This step removes all the packages and deletes all the data files (databases, tables, logs, etc.). Take a backup before doing this in case you need the data.
	
	```shell
	$ rm -rf /var/lib/mysql
	$ rm -f /etc/my.cnf
	```
