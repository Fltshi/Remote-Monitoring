package ESP.Server;

import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Main {
    public static void main(String[] args) throws IOException {
        //TCP协议，建立服务端
        ServerSocket server = new ServerSocket(10001);
        //监听客户端连接
        Socket socket = server.accept();
        System.out.println("有人连接上了");
        //从连接通道获取输入流读取数据
        InputStream is = socket.getInputStream();
        int b;
        while((b = is.read() )!= -1){
            System.out.print((char)b);
        }
        socket.close();
        server.close();

    }
}
