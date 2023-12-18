require 'websocket-client-simple'

url = 'ws://127.0.0.1:8080/ws'  # ws 地址

client = WebSocket::Client::Simple.connect(url)

client.on :message do |msg|
  puts "收到消息: #{msg.data}"
end

client.on :open do
  puts '连接服务器成功'
  client.send('Hello')
  sleep 1
  client.send('Hello' * 100)
  sleep 1
  client.send('Hello' * 1000)
end

client.on :close do |code, reason|
  puts "连接关闭， code #{code} ， reason: #{reason}"
end

# 保持进程不退出
loop do
  sleep 1
end
