package server;

import app.App;
import http.ContentType;
import http.HttpStatus;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.Setter;

import java.io.*;
import java.net.*;

@Getter(AccessLevel.PRIVATE)
@Setter(AccessLevel.PRIVATE)
public class Server {
    private ServerSocket serverSocket;
    private Socket clientSocket;
    private App app;
    private int port;

    public Server(App app, int port) {
        setApp(app);
        setPort(port);
    }

    public void start() throws IOException {
        setServerSocket(new ServerSocket(getPort()));
        run();
    }

    private void run() {
        int counter = 0;
        ThreadGroup threads = new ThreadGroup("mtcg");
        while (true) {
            try {
                setClientSocket(getServerSocket().accept());
                RequestHandler task = new RequestHandler(this.clientSocket, this.app);
                Thread thread = new Thread(threads, task);
                thread.setName(String.valueOf(counter));
                System.out.println("Thread: " + thread.getName());
                ++counter;
                thread.start();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }
}