// user_app.c - Aplicación en espacio de usuario para controlar LED vía /dev/led_driver

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     // open()
#include <unistd.h>    // write(), close()
#include <string.h>

#define DEVICE_PATH "/dev/led_driver"

void show_menu() {
    printf("\n=== Control de LED ===\n");
    printf("1. Encender LED\n");
    printf("2. Apagar LED\n");
    printf("0. Salir\n");
    printf("Selecciona una opción: ");
}

int main() {
    int choice;
    int fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Error al abrir el dispositivo");
        return EXIT_FAILURE;
    }

    while (1) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Entrada inválida\n");
            while (getchar() != '\n');  // Limpiar buffer
            continue;
        }

        switch (choice) {
            case 1:
                write(fd, "on", strlen("on"));
                break;
            case 2:
                write(fd, "off", strlen("off"));
                break;
            case 0:
                printf("Saliendo...\n");
                close(fd);
                return 0;
            default:
                printf("Opción no válida\n");
                break;
        }
    }

    close(fd);
    return 0;
}
