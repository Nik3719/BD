#include <iostream>
#include<stdio.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include"BD.h"
#include "DL.h"
#include "Tools.h"
#include <arpa/inet.h>

#define DEFAULT_PORT 7432

using namespace std;

// INSERT INTO таблица1 VALUES("1","234")
//INSERT INTO таблица1 VALUES ('12313','13eefdf')

void handleClient(int client_fd, BD& bd_instance) 
{
    int bufSize = 1048576;
    char buffer[bufSize];

    // Обработка клиента
    while (true) 
    {
        clear(buffer, 0, bufSize);
        ssize_t bytes_received = recv(client_fd, buffer, bufSize, 0);
        if (bytes_received <= 0) 
        {
            cerr << "Client disconnected" << endl;
            break;
        }

        string command(buffer);
        DL<string> split = splitString(command);
        string response = bd_instance.separateCommand(split);
        
        cout << "Received: " << buffer << endl;
        cout << '\n' << response << "\n";
        send(client_fd, response.c_str(), response.size(), 0);
    }

    close(client_fd); // Закрываем соединение с клиентом
}

int main() 
{
    int server_fd, client_fd;
    sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);

    // Создаем сокет
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        cerr << "Error creating socket" << endl;
        return -1;
    }

    // Настраиваем адрес сервера
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // Принимаем соединения на всех интерфейсах
    server_address.sin_port = htons(DEFAULT_PORT);

    // Привязываем сокет к адресу
    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) 
    {
        cerr << "Error binding socket" << endl;
        close(server_fd);
        return -1;
    }

    // Начинаем слушать входящие соединения
    if (listen(server_fd, 5) < 0) 
    {
        cerr << "Error listening on socket" << endl;
        close(server_fd);
        return -1;
    }

    cout << "Server is listening on port " << DEFAULT_PORT << endl;

    string shema = "/home/nik/Desktop/прога/BD/src/shema.json";
    BD bd_instance(shema);

    // Основной цикл сервера
    while (true) 
    {
        // Принимаем входящее соединение
        client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_length);
        if (client_fd < 0) 
        {
            cerr << "Error accepting connection" << endl;
            continue; // Продолжаем слушать другие соединения
        }

        cout << "Client connected" << endl;
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));
        int client_port = ntohs(client_address.sin_port);

        cout << "Client connected: IP = " << client_ip << ", Port = " << client_port << endl;

        // Создаем новый процесс для обработки клиента
        pid_t pid = fork();
        if (pid < 0) 
        {
            cerr << "Error forking process" << endl;
            close(client_fd);
            continue; // Продолжаем слушать другие соединения
        }

        if (pid == 0) 
        {
            // Это дочерний процесс
            close(server_fd); // Дочерний процесс не нуждается в серверном сокете
            handleClient(client_fd, bd_instance);
            exit(0); // Завершаем дочерний процесс
        } 
        else 
        {
            // Это родительский процесс
            close(client_fd); // Родительский процесс закрывает сокет клиента
        }
    }

    close(server_fd); // Закрываем серверный сокет
    return 0;
}
