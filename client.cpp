#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 7432
#define BUFFER_SIZE 1048576

int main() 
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Создаем сокет
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразуем адрес сервера
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Подключаемся к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to the server. Type your messages below (type 'exit' to quit):" << std::endl;

    while (true) {
        // Считываем сообщение от пользователя
        std::string message;
        std::cout << "You: ";
        std::getline(std::cin, message);

        // Проверяем, не хочет ли пользователь выйти
        if (message == "exit") {
            break; // Выход из цикла
        }

        // Отправляем сообщение на сервер
        send(sock, message.c_str(), message.length(), 0);
        std::cout << "Message sent to server: " << message << std::endl;

        // Чтение ответа от сервера
        memset(buffer, 0, BUFFER_SIZE); // Очищаем буфер перед чтением
        read(sock, buffer, BUFFER_SIZE);
        std::cout << "Received from server:\n" << buffer << std::endl;
    }

    close(sock);
    std::cout << "Connection closed." << std::endl;

    return 0;
}
