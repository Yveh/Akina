# Compile CPP

用 server.cpp 替换后端 main.cpp 运行。

# Modify CPP

在 shell() 里的东西和 main 很相似的部分直接修改即可。

# Use PY

```python
import client
import json

def init_client():
    client.client_start()

def shell_command(str):
    ret = client.post_and_get(str)
    # parsing json
    value = json.loads(ret)
    return value["error"], value["result"]

def close_client():
    client.client_end()

```
