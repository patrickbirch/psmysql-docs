# TokuDB file management

As mentioned in the TokuDB files and file types *Percona FT* is
extremely pedantic about validating its data set. If a file goes missing or
can’t be accessed, or seems to contain some nonsensical data, it will
assert, abort or fail to start. It does this not to annoy you, but to try to
protect you from doing any further damage to your data.

This document contains examples of common file maintenance operations and
instructions on how to safely execute these operations.

Beginning in Percona Server Percona Server for MySQL 5.7.15-9 a new server option was
introduced called tokudb_dir_per_db. This feature addressed two
shortcomings the renaming of data files on table/index rename, and the ability
to group data files together
within a directory that represents a single database. This feature is enabled
by default.

In *Percona Server for MySQL* Percona Server for MySQL 5.7.18-14 new tokudb_dir_cmd variable
has been implemented that can be used to edit the contents of the
TokuDB/PerconaFT directory map.

## Moving TokuDB data files to a location outside of the default MySQL datadir

TokuDB uses the location specified by the tokudb_data_dir
variable for all of its data files. If the tokudb_data_dir variable
is not explicitly set, TokuDB will use the location specified by the servers
source/glossary.rst\`datadir\` for these files.

The TokuDB data files are protected from concurrent process access by the
`__tokudb_lock_dont_delete_me_data` file that is located in the same
directory as the TokuDB data files.

TokuDB data files may be moved to other locations with symlinks left behind
in their place. If those symlinks refer to files on other physical data
volumes, the tokudb_fs_reserve_percent monitor will not traverse
the symlink and monitor the real location for adequate space in the file
system.

To safely move your TokuDB data files:


1. Shut the server down cleanly.


2. Change the tokudb_data_dir in your `my.cnf` configuration
file to the location where you wish to store your TokuDB data files.


3. Create your new target directory.


4. Move your `\*.tokudb` files and your `__tokudb_lock_dont_delete_me_data`
from the current location to the new location.


5. Restart your server.

## Moving TokuDB temporary files to a location outside of the default MySQL datadir

TokuDB will use the location specified by the tokudb_tmp_dir
variable for all of its temporary files. If tokudb_tmp_dir variable
is not explicitly set, TokuDB will use the location specified by the
tokudb_data_dir variable. If the tokudb_data_dir
variable is also not explicitly set, TokuDB will use the location specified
by the servers source/glossary.rst\`datadir\` for these files.

TokuDB temporary files are protected from concurrent process access by the
`__tokudb_lock_dont_delete_me_temp` file that is located in the same
directory as the TokuDB temporary files.

If you locate your TokuDB temporary files on a physical volume that is
different from where your TokuDB data files or recovery log files are
located, the tokudb_fs_reserve_percent monitor will not monitor
their location for adequate space in the file system.

To safely move your TokuDB temporary files:


1. Shut the server down cleanly. A clean shutdown will ensure that there are no
temporary files that need to be relocated.


2. Change the tokudb_tmp_dir variable in your `my.cnf`
configuration file to the location where you wish to store your new TokuDB
temporary files.


3. Create your new target directory.


4. Move your `__tokudb_lock_dont_delete_me_temp` file from the current
location to the new location.


5. Restart your server.

## Moving TokuDB recovery log files to a location outside of the default MySQL datadir

TokuDB will use the location specified by the tokudb_log_dir
variable for all of its recovery log files. If the tokudb_log_dir
variable is not explicitly set, TokuDB will use the location specified by the
servers source/glossary.rst\`datadir\` for these files.

The TokuDB recovery log files are protected from concurrent process access by
the `__tokudb_lock_dont_delete_me_logs` file that is located in the same
directory as the TokuDB recovery log files.

TokuDB recovery log files may be moved to another location with symlinks left
behind in place of the tokudb_log_dir. If that symlink refers to a
directory on another physical data volume, the
tokudb_fs_reserve_percent monitor will not traverse the symlink and
monitor the real location for adequate space in the file system.

To safely move your TokuDB recovery log files:


1. Shut the server down cleanly.


2. Change the tokudb_log_dir in your `my.cnf` configuration
file to the location where you wish to store your TokuDB recovery log
files.


3. Create your new target directory.


4. Move your `log\*.tokulog\*` files and your
`__tokudb_lock_dont_delete_me_logs` file from the current location to the
new location.


5. Restart your server.

## Improved table renaming functionality

When you rename a TokuDB table via SQL, the data files on disk keep their
original names and only the mapping in the *Percona FT* directory file is
changed to map the new dictionary name to the original internal file names.
This makes it difficult to quickly match database/table/index names to their
actual files on disk, requiring you to use the
refTOKUDB_FILE_MAP table to cross reference.

Beginning with *Percona Server for MySQL* Percona Server for MySQL 5.7.15-9 a new server option was
introduced called tokudb_dir_per_db to address this issue.

When tokudb_dir_per_db is enabled (`ON` by default), this is no
longer the case. When you rename a table, the mapping in the *Percona FT*
directory file will be updated and the files will be renamed on disk to reflect
the new table name.

## Improved directory layout functionality

Many users have had issues with managing the huge volume of individual files
that TokuDB and *Percona FT* use.

Beginning with *Percona Server for MySQL* Percona Server for MySQL 5.7.15-9 a new server option was
introduced called tokudb_dir_per_db to address this issue.

When tokudb_dir_per_db variable is enabled (`ON` by default),
all new tables and indices will be placed within their corresponding database
directory within the `tokudb_data_dir` or server source/glossary.rst\`datadir\`.

If you have tokudb_data_dir variable set to something other than
the server source/glossary.rst\`datadir\`, TokuDB will create a directory matching the name
of the database, but upon dropping of the database, this directory will remain
behind.

Existing table files will not be automatically relocated to their corresponding
database directory.

You can easily move a tables data files into the new scheme and proper database
directory with a few steps:

```sql
mysql> SET GLOBAL tokudb_dir_per_db=true;
mysql> RENAME TABLE <table> TO <tmp_table>;
mysql> RENAME TABLE <tmp_table> TO <table>;
```

!!! note

    Two renames are needed because MySQL doesn’t allow you to rename a table to itself. The first rename, renames the table to the temporary name and moves the table files into the owning database directory. The second rename sets the table name back to the original name. Tables can also be renamed/moved across databases and will be placed correctly into the corresponding database directory.

!!! warning

    You must be careful with renaming tables in case you have used any tricks to create symlinks of the database directories on different storage volumes, the move is not a simple directory move on the same volume but a physical copy across volumes. This can take quite some time and prevent access to the table being moved during the copy.

## Editing TokuDB directory map with tokudb_dir_cmd

!!! note

    This feature is currently considered *Experimental*.

In *Percona Server for MySQL* Percona Server for MySQL 5.7.18-14 new tokudb_dir_cmd variable
has been implemented that can be used to edit the TokuDB directory map.

!!! warning

    Use this variable only if you know what you’re doing otherwise it will have data loss.

This method can be used if any kind of system issue causes the loss of specific
`.tokudb` files for a given table, because the TokuDB tablespace file
mapping will then contain invalid (nonexistent) entries, visible in
TokuDB_file_map table.

This variable is used to send commands to edit directory file. The format of
the command line is the following:

```text
command arg1 arg2 .. argn
```

I.e, if we want to execute some command the following statement can be used:

```text
SET tokudb_dir_cmd = "command arg1 ... argn"
```

Currently the following commands are available:


* `attach dictionary_name internal_file_name` - attach internal_file_name to
a dictionary_name, if the dictionary_name exists override the previous value,
add new record otherwise


* `detach dictionary_name` - remove record with corresponding
dictionary_name, the corresponding internal_file_name file stays untouched


* `move old_dictionary_name new_dictionary_name` - rename (only)
dictionary_name from old_dictionary_name to new_dictionary_name

Information about the dictionary_name and internal_file_name can be found in
the TokuDB_file_map table:

```sql
mysql> SELECT dictionary_name, internal_file_name FROM INFORMATION_SCHEMA.TokuDB_file_map;
```

The output should be similar to the following:

```text
+------------------------------+---------------------------------------------------------+
| dictionary_name              | internal_file_name                                      |
+------------------------------+---------------------------------------------------------+
| ./world/City-key-CountryCode | ./_world_sql_340a_39_key_CountryCode_12_1_1d_B_1.tokudb |
| ./world/City-main            | ./_world_sql_340a_39_main_12_1_1d_B_0.tokudb            |
| ./world/City-status          | ./_world_sql_340a_39_status_f_1_1d.tokudb               |
+------------------------------+---------------------------------------------------------+
```

### System Variables

### `tokudb_dir_cmd`

| Option       | Description |
|--------------|-------------|
| Command-line | Yes         |
| Config file  | Yes         |
| Scope        | Global      |
| Dynamic      | Yes         |
| Data type    | String      |

The variable has been implemented in Percona Server for MySQL 5.7.18-14. This variable is used to send commands to edit TokuDB directory map.

!!! warning

    Use this variable only if you know what you’re doing otherwise it **WILL** lead to data loss.

### Status Variables

### `tokudb_dir_cmd_last_error`

| Option | Description |
| --- | --- |
| Scope | Global |
| Data type | Numeric |

This variable contains the error number of the last executed command by using
the tokudb_dir_cmd variable.

### `tokudb_dir_cmd_last_error_string`

| Option | Description |
| --- | --- |
| Scope | Global |
| Data type | Numeric |

This variable contains the error string of the last executed command by using
the tokudb_dir_cmd variable.
