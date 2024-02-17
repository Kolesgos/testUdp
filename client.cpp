#include <iostream>
#include <chrono>
#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")

class iTime {
public:
    int getMS(std::chrono::duration <double> timedelta);
    auto getCurrentTime();
};

class Time : public iTime{
public:
    long long int getMS(std::chrono::duration <double> timedelta) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(timedelta).count();
    }
    auto getCurrentTime() {
        return std::chrono::system_clock::now();
    }
};

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 8000	//The port on which to listen for incoming data

int main(void)
{
	struct sockaddr_in si_other;
	int s, slen=sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//create socket
	if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	std::string data("Hello, ant-lab!");
	int i = 0;
	Time time;
	auto last = time.getCurrentTime();
	while (true) {
	    if (time.getMS(time.getCurrentTime() - last) >= 100) {
            if (i >= data.size()) { break; }
            last = time.getCurrentTime();
            message[0] = data[i];
            if (sendto(s, message, 1, 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
            {
                printf("sendto() failed with error code : %d" , WSAGetLastError());
                exit(EXIT_FAILURE);
            }
            i++;
	    }
	}
	message[0] = 0;
    if (sendto(s, message, 1, 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR) {
        printf("sendto() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

	closesocket(s);
	WSACleanup();

	return 0;
}
