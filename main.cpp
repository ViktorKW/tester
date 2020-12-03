#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <limits>
#include <stdlib.h>
#include "CSerial.h"
using namespace std;


int num = 0, requests = 0;


void print_main_menu()
{
    cout<<"\nOptions:"<<endl;
    cout<<"1. Kill all autorun processes"<<endl;
    cout<<"2. Execute custom commnad"<<endl;
    cout<<"\nType number to choose an option"<<endl;
}

void working_with_uart(string serial_address)
{
    int serial = serialOpen(serial_address.c_str(), 115200);
    string command;
    cout<<"Type [command]:"<<endl;
    cin>>command;
    if(command.find("GREEN") != std::string::npos||command.find("RED") != std::string::npos)
    {
       serialPrintf(serial, "%s\n\r", command.c_str());
       cout<<"commands GREEN and RED won't return anything\n"<<endl;
    }
    else if(command.find("exit")!=std::string::npos)
    {
        return;
    }
    else
    { 
        int amount = 0;
        while (!amount)
        {
            serialPrintf(serial, "%s\n\r", command.c_str());
            amount = serialDataAvail(serial); 
            while(amount < 5)
            {
                requests++;
                amount = serialDataAvail(serial);
                if(requests > 10)
                    {
                        requests = 0;
                        serialPrintf(serial, "%s\n\r", command.c_str());
                        amount = serialDataAvail(serial);
                    }
                usleep(100000);
            }
            printf("Amount of bytes gained %d\n", amount);
            
            if(amount>0) 
            {
                char answer[amount+2];
                for(int i = 0; i < amount; i++)
                {	 	
                    answer[i] = (char)serialGetchar(serial);
                } 

                printf("%s\n", answer);
            }
            
            usleep(100000);
            requests = 0;   
        }
    }
    serialFlush(serial);
    serialClose(serial);
    working_with_uart(serial_address);
}
int choice;
int main() 
{
    while(true)
    {
        system("clear");
        print_main_menu();
        choice = 0;
        cin>>choice;

        if(choice==1)
        {
            try
            {
                system("clear");
                cout<<"Executing task..."<<endl;
                try
                {
                    system("sudo pkill -o ./main");
                    system("sudo pkill -o a.out");
                    system("sudo pkill -o main");
                    system("sudo pkill -o /main");
                    system("sudo pkill -o ./a.out");
                    system("sudo pkill -o cronetab");
                    system("sudo pkill -o cron");
                    cout<<"Task was executed successfully"<<endl;
                    system("read -p '\n\nPress enter to continue' var");
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if(choice == 2)
        {   
            cout<<"\nChoose uart"<<endl;
            cout<<"1. /dev/ttyS0"<<endl;
            cout<<"2. /dev/ttyAMA1"<<endl;
            int choose_uart = 0;
            cin>>choose_uart;
            string str;
            if(choose_uart == 1 )
            {
                    system("clear");
                     str = "/dev/ttyS0";
                    cout<<"Serial: "<<str<<endl;
                cout<<"Print 'exit' to go back"<<endl;
                
               working_with_uart(str);
            }
            else if(choose_uart==2)
            {
                    system("clear");
                    str = "/dev/ttyAMA1";
                    cout<<"Serial: "<<str<<endl;
            cout<<"Print 'exit' to go back"<<endl;

                working_with_uart(str);
            }
           
        }
        else
        {
            cout<<"\nNo such option"<<endl;
        }
    }
    return 0;
}