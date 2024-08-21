package ESP.Server;

import javax.swing.*;
import java.awt.*;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

public class ServerJFrame {
     JFrame Jframe;
    Container container;
    ImageIcon imageIcon;
    JLabel jLabel;
    public void Scren_Init() throws IOException {
        Jframe = new JFrame("摄像头显示");
        Jframe.setSize(853,688);
        //创建内容窗格
        container = Jframe.getContentPane();

        Jframe.setAlwaysOnTop(true);
        //显示居中
        Jframe.setLocationRelativeTo(null);
        //设置关闭模式
        Jframe.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        //设置布局方式
        container.setLayout(null);

        FileInputStream fi = new FileInputStream("aaPicture.jpg");
        byte[] bytes = null;
        bytes = fi.readAllBytes();

        imageIcon = new ImageIcon(bytes);
        jLabel =new JLabel(imageIcon);
        jLabel.setBounds(200,200,180,220);
        container.add(jLabel);


        //sc.nextLine();

        Jframe.setVisible(true);


    }
    public void Show_Photo(byte[] bytes) throws IOException {
        //测试用
//        FileInputStream fi2 = new FileInputStream("bbPicture.jpg");
//        bytes = fi2.readAllBytes();
        //while(true) {
        container.remove(jLabel);
        imageIcon = null;
        //jLabel = null;

        imageIcon = new ImageIcon(bytes);

        jLabel.setIcon(imageIcon);
        jLabel.setBounds(20, 20, 800, 600);
        container.add(jLabel);

        container.repaint();

    }
    public static void main(String[] args) throws IOException {
        ServerJFrame serverJFrame = new ServerJFrame();
        serverJFrame.Scren_Init();
        Scanner sc = new Scanner(System.in);
        sc.nextLine();
        byte[]  b = null;
        serverJFrame.Show_Photo(b);

    }
}
