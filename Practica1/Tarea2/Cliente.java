import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Cliente {

    public static void main(String[] args) {
        String ip    = "127.0.0.1"; // dirección del servidor (localhost = misma máquina)
        int    puerto = 5000;        // debe coincidir con el puerto del servidor C

        try {
            Socket socket = new Socket(ip, puerto); // establece la conexión TCP con el servidor C
            System.out.println("Conectado al servidor C.");

            // streams para enviar y recibir datos por el socket
            PrintWriter salida  = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true); // envía texto al servidor (autoflush activado)
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));   // recibe texto del servidor

            Scanner teclado = new Scanner(System.in); // lee los enteros que el usuario escribe por consola

            while (true) {
                System.out.print("Ingresa un número entero (0 para salir): ");
                int numero = teclado.nextInt(); // captura el entero ingresado por el usuario

                salida.println(numero); // envía el número al servidor C como texto terminado en '\n'

                String respuesta = entrada.readLine(); // espera y lee la respuesta del servidor C (bloqueante)
                int numeroRespuesta = Integer.parseInt(respuesta.trim()); // convierte la respuesta de String a int

                System.out.println("Respuesta del servidor: " + numeroRespuesta);

                if (numero == 0) { // si el usuario ingresó 0, termina el programa
                    System.out.println("Enviaste 0. Cerrando conexión.");
                    break;
                }
            }

            // cierre ordenado de recursos
            teclado.close();
            entrada.close();
            salida.close();
            socket.close();

        } catch (IOException e) {
            System.err.println("Error en el cliente: " + e.getMessage()); // muestra el error si falla la conexión o la transmisión
        }
    }
}