# pgmoneta configuration

The configuration is loaded from either the path specified by the `-c` flag or `/etc/pgmoneta/pgmoneta.conf`.

The configuration of `pgmoneta` is split into sections using the `[` and `]` characters.

The main section, called `[pgmoneta]`, is where you configure the overall properties
of the connection pool.

Other sections doesn't have any requirements to their naming so you can give them
meaningful names like `[primary]` for the primary [PostgreSQL](https://www.postgresql.org)
instance.

All properties are in the format `key = value`.

The characters `#` and `;` can be used for comments; must be the first character on the line.
The `Bool` data type supports the following values: `on`, `1`, `true`, `off`, `0` and `false`.

See a [sample](./etc/pgmoneta/pgmoneta.conf) configuration for running `pgmoneta` on `localhost`.

## [pgmoneta]

| Property | Default | Unit | Required | Description |
|----------|---------|------|----------|-------------|
| host | | String | Yes | The bind address for pgmoneta |
| unix_socket_dir | | String | Yes | The Unix Domain Socket location |
| base_dir | | String | Yes | The base directory for the backup |
| pgsql_dir | | String | Yes | The directory for the PostgreSQL binaries |
| metrics | 0 | Int | No | The metrics port (disable = 0) |
| management | 0 | Int | No | The remote management port (disable = 0) |
| compression | gzip | String | No | The compression type (none, gzip) |
| compression_level | 9 | int | No | The compression level (1..9) |
| retention | 7 | Int | No | The retention time in days |
| log_type | console | String | No | The logging type (console, file, syslog) |
| log_level | info | String | No | The logging level (fatal, error, warn, info, debug1, ..., debug5) |
| log_path | pgmoneta.log | String | No | The log file location |
| blocking_timeout | 30 | Int | No | The number of seconds the process will be blocking for a connection (disable = 0) |
| tls | `off` | Bool | No | Enable Transport Layer Security (TLS) |
| tls_cert_file | | String | No | Certificate file for TLS. This file must be owned by either the user running pgmoneta or root. |
| tls_key_file | | String | No | Private key file for TLS. This file must be owned by either the user running pgmoneta or root. Additionally permissions must be at least `0640` when owned by root or `0600` otherwise. |
| tls_ca_file | | String | No | Certificate Authority (CA) file for TLS. This file must be owned by either the user running pgmoneta or root.  |
| libev | `auto` | String | No | Select the [libev](http://software.schmorp.de/pkg/libev.html) backend to use. Valid options: `auto`, `select`, `poll`, `epoll`, `iouring`, `devpoll` and `port` |
| buffer_size | 65535 | Int | No | The network buffer size (`SO_RCVBUF` and `SO_SNDBUF`) |
| keep_alive | on | Bool | No | Have `SO_KEEPALIVE` on sockets |
| nodelay | on | Bool | No | Have `TCP_NODELAY` on sockets |
| non_blocking | on | Bool | No | Have `O_NONBLOCK` on sockets |
| backlog | `max_connections` / 4 | Int | No | The backlog for `listen()`. Minimum `16` |
| hugepage | `try` | String | No | Huge page support (`off`, `try`, `on`) |
| pidfile | | String | No | Path to the PID file |

## Server section

| Property | Default | Unit | Required | Description |
|----------|---------|------|----------|-------------|
| host | | String | Yes | The address of the PostgreSQL instance |
| port | | Int | Yes | The port of the PostgreSQL instance |
| user | | String | Yes | The replication user name |
| backup_slot | | String | No | The replication slot for the backup |
| wal_slot | | String | No | The replication slot for WAL |


# pgmoneta_users configuration

The `pgmoneta_users` configuration defines the users known to the system. This file is created and managed through
the `pgmoneta-admin` tool.

The configuration is loaded from either the path specified by the `-u` flag or `/etc/pgmoneta/pgmoneta_users.conf`.

# pgmoneta_admins configuration

The `pgmoneta_admins` configuration defines the administrators known to the system. This file is created and managed through
the `pgmoneta-admin` tool.

The configuration is loaded from either the path specified by the `-A` flag or `/etc/pgmoneta/pgmoneta_admins.conf`.

If pgmoneta has both Transport Layer Security (TLS) and `management` enabled then `pgmoneta-cli` can
connect with TLS using the files `~/.pgmoneta/pgmoneta.key` (must be 0600 permission),
`~/.pgmoneta/pgmoneta.crt` and `~/.pgmoneta/root.crt`.