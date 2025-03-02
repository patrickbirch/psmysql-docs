# Multiple page asynchronous I/O requests

I/O unit size in *InnoDB* is only one page, even if doing read ahead. 16KB
I/O unit size is too small for sequential reads, and much less efficient than
larger I/O unit size.

*InnoDB* uses Linux asynchronous I/O (`aio`) by default. By submitting multiple
consecutive 16KB read requests at once, Linux internally can merge requests and
reads can be done more efficiently.

[On a HDD RAID 1+0 environment](http://yoshinorimatsunobu.blogspot.hr/2013/10/making-full-table-scan-10x-faster-in.html),
more than 1000MB/s disk reads can be achieved by submitting 64 consecutive pages
requests at once, while only
160MB/s disk reads is shown by submitting single page request.

With this feature *InnoDB* submits multiple page I/O requests.

## Version Specific Information

* 8.0.12-1 - The feature was ported from *Percona Server for MySQL* 5.7.

## Status Variables

### `Innodb_buffered_aio_submitted`

| Option         | Description        |
| -------------- | ------------------ |
| Scope:         | Global             |
| Data type:     | Numeric            |

This variable shows the number of submitted buffered asynchronous I/O requests.

## Other Reading

* [Making full table scan 10x faster in InnoDB](https://yoshinorimatsunobu.blogspot.hr/2013/10/making-full-table-scan-10x-faster-in.html)

* [Bug #68659  InnoDB Linux native aio should submit more i/o requests at once](https://bugs.mysql.com/bug.php?id=68659)
