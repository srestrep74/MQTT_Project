#include "../../include/server/handlers.h"

bool client_handler(int client_socket, Packet client_packet)
{

    if (client_packet.fixed_header == 0) {
        fprintf(stderr, "Error al decodificar el mensaje del cliente\n");
        close(client_socket);
        return false;
    }

    if (get_type(&(client_packet.fixed_header)) != CONNECT) {
        printf("Mensaje recibido no es del tipo CONNECT\n");

        Packet connack_error = create_connack_message(CONNACK_REFUSED_NOT_AUTHORIZED);
        send_packet(client_socket, connack_error);

        close(client_socket);
        return false;
    }

    Packet connack_success = create_connack_message(CONNACK_CONNECTION_ACCEPTED);
    send_packet(client_socket, connack_success);
    return true;
}