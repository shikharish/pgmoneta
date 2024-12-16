struct message:
- data :
    - contains _only_ the data of message, excluding 1 byte of `kind` and 4 bytes of `length`
- kind
- length:
    - length of `data`. we read the 4 bytes after 1st byte and subtract it by 4 to get this length