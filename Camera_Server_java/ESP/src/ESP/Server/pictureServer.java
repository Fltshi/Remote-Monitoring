package ESP.Server;

import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class pictureServer {
    //端口
    private static final int PORT = 10001;



    public static void main(String[] args) throws IOException {
        //TCP协议，建立服务端
        ServerJFrame sj = new ServerJFrame();
        sj.Scren_Init();
        ServerSocket server = new ServerSocket(PORT);
        //监听客户端连接
        Socket socket = server.accept();
        System.out.println("有人连接上了");
        //从连接通道获取输入流读取数据
        InputStream is = socket.getInputStream();
        DataInputStream dis = new DataInputStream(is);
        //接收数据

        int lenth = 0;
        int off =0;
        int data= dis.readInt();
        byte[] buff = new byte[data];
        //System.out.println("data  = "+data);
        //太耻辱了
        while ((lenth = dis.read(buff,off,data-off)) != -1) {
            off += lenth;
            if(off >= data) {
                //显示照片
                sj.Show_Photo(buff);
                data = dis.readInt();
                off = 0;
                buff = null;
                buff = new byte[data];

            }
        }
        //注意

        socket.close();
        server.close();
    }
}
