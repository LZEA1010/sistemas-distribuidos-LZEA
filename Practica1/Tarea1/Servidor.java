import java.io.*;
import java.net.*;

public class Servidor {

    public static void main(String[] args) {
        int puerto = 5000; // puerto en el que el servidor escuchará conexiones entrantes

        try {
            ServerSocket serverSocket = new ServerSocket(puerto); // crea el socket servidor que espera conexiones
            System.out.println("Servidor esperando conexion en el puerto " + puerto + "...");

            Socket socket = serverSocket.accept(); // bloquea el hilo hasta que un cliente se conecta, retorna el socket del cliente
            System.out.println("Cliente conectado: " + socket.getInetAddress());

            // streams para leer y escribir datos a través del socket
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));   // leer lo que manda el cliente
            PrintWriter salida   = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true); // escribir hacia el cliente (true = autoflush)

            String mensajeCliente = entrada.readLine(); // lee una línea de texto enviada por el cliente (bloqueante hasta recibir '\n')
            System.out.println("Mensaje del cliente: " + mensajeCliente);

            salida.println("Hola, que tal?"); // responde al cliente con un mensaje de texto terminado en '\n'
            System.out.println("Respuesta enviada al cliente.");

            // cierre ordenado de recursos
            entrada.close();
            salida.close();
            socket.close();
            serverSocket.close();

        } catch (IOException e) {
            System.err.println("Error en el servidor: " + e.getMessage()); // muestra el error si algo falla al abrir/leer/escribir el socket
        }
    }
}