### WebSocket握手过程

#### 请求格式
````text
GET ${url} HTTP/1.1\r\n
Host: ${host}:${port}\r\n
Connection: Upgrade\r\n
Pragma: no-cache\r\n
Cache-Control: no-cache\r\n
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n
Upgrade: websocket\r\n
Origin: ${originUrl}\r\n
Sec-WebSocket-Version: 13\r\n
Accept-Encoding: gzip, deflate\r\n
Accept-Language: zh-CN,zh;q=0.9\r\n
Sec-WebSocket-Key: vL60uRfIQ/bZFqfHHA1kmg==\r\n
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n
\r\n
````

|  字段   | 含义  |
|  ----  | ----  |
| Connection  | 固定为‘Upgrade’ |
| Upgrade  | 固定为‘websocket’ |
| Sec-WebSocket-Version  | 固定为‘13’ |
| Sec-WebSocket-Key  | base64编码后的客户端标识 |
| Sec-WebSocket-Extensions  | 规定协议扩展 |

#### 应答格式
````text
HTTP/1.1 101 Switching Protocols\r\n
Upgrade: websocket\r\n
Connection: Upgrade\r\n
Sec-WebSocket-Accept: E4VK19UUgGJPt9S1akzL5lFppY4=\r\n
\r\n
````

|  字段   | 含义  |
|  ----  | ----  |
| Connection  | 固定为‘Upgrade’ |
| Upgrade  | 固定为‘websocket’ |
| Connection  | 固定为‘Upgrade’ |
| Sec-WebSocket-Accept  | 根据请求的Sec-WebSocket-Key计算 |

**Sec-WebSocket-Accept计算方式**
````Java
String mask = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
String accept = base64(sha1(Sec-WebSocket-Key + mask));
````
