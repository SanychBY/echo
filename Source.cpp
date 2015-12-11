
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <iostream>
//#define ADDR "87.240.131.118"  //IP- адрес
#include <windows.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;

unsigned long ipaddr;
//преобразование IP-адреса

//данные эхо-запроса
LPVOID ReplyBuffer = NULL;
//данные эхо-ответа
//DWORDReplySize = 0;
//размер данных эхо-ответа

  

int main()
{
	setlocale(LC_ALL,"RU");
	HANDLE hIcmpFile;
	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE)
	{
		printf("\tНевозможно открыть дискриптор.\n");
		printf(" Ошибка IcmpCreatefile: %ld\n",
			WSAGetLastError());
		return -1;
	}
	
	while (true)
	{
		char *ADDR = new char[200];
		int col = 0;
		cout << "Введите ip\n> ";
		cin.getline(ADDR, 200);
		ipaddr = inet_addr(ADDR);
		cout << "Количество запросов\n> ";
		(cin >> col).get();
		int S = 0;
		cout << "Размер пакета\n> ";
		(cin >> S).get();
		char *SendData = (char *)malloc(S);
		//for (char *b = SendData;*b;b++)
		//{
			//*b = '0';
		//}
		cout << _msize(SendData) << endl;
		int ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
		//Выделяем память
		ReplyBuffer = (VOID*)malloc(ReplySize);

		if (ReplyBuffer == NULL)
		{
			printf("\tНевозможно выделить память\n");
			return -1;
		}
		for (int i = 0; i < col; i++)
		{
			DWORD dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
			if (dwRetVal != 0)
			{
				PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
				struct in_addr ReplyAddr;
				ReplyAddr.S_un.S_addr = pEchoReply->Address;
				printf("\tПосылка icmp сообщения на %s\n",
					ADDR);
				if (dwRetVal > 1) {
					printf("\tПолучен %ldicmp ответ\n",
						dwRetVal);
					printf("\tИнформация:\n");
				}
				else {
					printf("\tПолучен %ld icmp ответ\n",
						dwRetVal);
					printf("\tИнформация:\n");
				}
				printf("\tПолучено от %s\n",
					inet_ntoa(ReplyAddr));
				printf("\t  Статус = %ld\n",
					pEchoReply->Status);
				printf("\tВремя отклика  = %ld миллисекунд \n",
					pEchoReply->RoundTripTime);
				cout << "------------------------------------------------------" << endl;
			}
			else {
				printf("\tВызов IcmpSendEcho завершился с ошибкой.\n");
				printf("\tIcmpSendEcho ошибка: %ld\n",
					WSAGetLastError());
				//return -1;
			}
		}
		delete []ADDR;
		free(SendData);
		free(ReplyBuffer);
	}
	DWORD bRetVal = IcmpCloseHandle(hIcmpFile);

	if (bRetVal)
		printf("\tHandle was closed\n");
	else
		printf("IcmpCloseHandle failed with error: %ld\n",
			WSAGetLastError());
	system("pause");
}



