### Joonyoung Moon 3035666657

## Few things to note

1. The server allows at most three tries in authentication, after which the connection is closed.
2. The server has 10 rooms.
3. The server manages at most 20 connections.

## Execution

python3 server.py port userInfoFilePath
...
python3 client.py hostname(use 127.0.0.1) port
