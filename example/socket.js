const sendBtn = document.getElementById("send");
const connectBtn = document.getElementById("connect");
const message = document.getElementById("message");
const ws_url = document.getElementById("ws_url");
const send_data = document.getElementById("send_data");
const clearBtn = document.getElementById("clear");

let socket = null;

// 设置按钮禁用
sendBtn.disabled = true;

// 设置文本框禁用
message.disabled = true;

// 默认值
ws_url.value = "ws://172.24.183.82:8080/ws?name=lsm";

const openSocket = function () {
    try {
        // 实例化WebSocket对象
        socket = new WebSocket(ws_url.value);
    } catch (e) {
        debugger
        message.append('连接失败，原因:' + e.message + "\n");
        return;
    }

    // 监听WebSocket连接打开事件
    socket.onopen = function (event) {
        console.log("WebSocket连接成功");
        // 设置按钮可用
        sendBtn.disabled = false;
        message.append("WebSocket连接成功\n");
    };
    // 监听WebSocket连接关闭事件
    socket.onclose = function (event) {
        console.log("WebSocket连接关闭");
        // 设置按钮禁用
        sendBtn.disabled = true;
        message.append("WebSocket连接关闭\n");
    };
    socket.onerror = function (event) {
        console.log("WebSocket意外断开");
        message.append("WebSocket意外断开\n");
    };
    socket.onmessage = function (event) {
        message.append("收到消息:" + event.data + "\n");
    };
};

connectBtn.onclick = function () {
    if (!window.WebSocket) {
        alert("您的浏览器不支持WebSocket");
        return;
    } else if (ws_url.value === "") {
        alert("请输入WebSocket地址");
        return;
    }
    openSocket();
};

sendBtn.onclick = function () {
    if (send_data.value === "") {
        alert("请输入要发送的消息");
        return;
    }
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(send_data.value);
        message.append("发送消息:" + send_data.value + "\n");
        send_data.value = ''
    } else {
        message.append("WebSocket连接没有建立成功\n");
    }
};

clearBtn.onclick = () => {
    message.innerText = ''
};