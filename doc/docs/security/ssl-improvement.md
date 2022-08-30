# SSL Improvements

By default, *Percona Server for MySQL* passes elliptic-curve crypto-based ciphers to OpenSSL, such as ECDHE-RSA-AES128-GCM-SHA256.

!!! note

    Although documented as supported, elliptic-curve crypto-based ciphers do not work with MySQL.
