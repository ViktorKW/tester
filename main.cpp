#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <limits>
#include <stdlib.h>
#include <string.h>
using namespace std;


int num = 0, requests = 0;


void print_main_menu()
{
    cout<<"\nOptions:\n";
    cout<<"1. Kill all autorun processes\n";
    cout<<"2. Reboot system\n";
    //cout<<"3. Execute CLR_CARD\\n\\r command\n";
    //cout<<"4. Execute WR_CARD:[card_id]\\n\\r command\n";
    cout<<"3. Execute custom commnad\n";
    //cout<<"6. Green\n";
    cout<<"4. Exit\n";
    /*cout<<"5. Execute LIST\\n\\r command\n";
    cout<<"6. Execute GREEN\n";*/
    cout<<"\nType number to choose an option\n";
}


/*void finish_task(string result)
{
    system("clear");
    cout<<result;
}*/


int main() 
{
    int choice = 0;
    int serial = serialOpen("/dev/ttyS0",115200);
    

    while(true)
    {
        system("clear");
        print_main_menu();
        if (!cin) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        cin>>choice;

        if(choice>0)
        {
            switch(choice)
            {
                case 1:
                {
                    try
                    {
                        system("clear");
                        cout<<"Executing task...\n";
                        try
                        {
                            system("sudo pkill -o ./main");
                            system("sudo pkill -o a.out");
                            system("sudo pkill -o main");
                            system("sudo pkill -o /main");
                            system("sudo pkill -o ./a.out");
                            system("sudo pkill -o cronetab");
                            system("sudo pkill -o cron");
                            cout<<"Task was executed successfully\n";
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
                    
                    break;

                }
                case 2:
                {
                    system("clear");
                    cout<<"Rebooting...";
                    sleep(1);
                    system("sudo reboot");
                    break;
                }
                /*case 3:
                {
                    serialPrintf(serial, "CLR_CARD\n\r");
                    break;
                }
                case 4:
                {
                    system("clear");
                    char buff[]={};
                    cout<<"Enter card id\n";
                    cout<<"WR_CARD:";
                    cin>>buff;
                    serialPrintf(serial, "WR_CARD:%s\n\r", buff);
                    cout<<"command WR_CARD:";
                    cout<<buff;
                    cout<<" was executed successfully\n";
                    system("read -p '\n\nPress enter to continue' var");        
                    break;
                }*/
                case 3:
                {
                    system("clear");
                    cout<<"Type [command]:\n";
                    char command[]={};
                    cin>>command;
                    if(strstr(command, "GREEN")==NULL&&strstr(command, "RED")==NULL)
                    {serialClose(serial);
                    serial = serialOpen("/dev/ttyS0",115200);
                    //serialPrintf(serial, "%s\n\r", command);
                    int amount = 0;
                    while (!amount)
                    {
                        serialFlush(serial);
                        //amount = serialDataAvail(serial); //listens to serial host till it gets any kind of response*
                        /* Send the line details to serial */
                        serialPrintf(serial, "%s\n\r", command);
                        amount = serialDataAvail(serial); //listens to serial host till it gets any kind of response
                        while(amount < 5)
                        {
                            requests++;
                            amount = serialDataAvail(serial);
                            if(requests > 10)
                                {
                                requests = 0;
                                serialPrintf(serial, "%s\n\r", command);
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
                    else{
                        serialPrintf(serial, "%s", command);
                    }
                    system("read -p '\n\nPress enter to continue' var");

                    break;
                }
                /*case 6:
                {
                    serialPrintf(serial, "1:GREEN");
                    break;
                }*/
                case 4:
                {
                    return 0;  
                    break;
                }
                default:
                {
                    cout<<"\nNo such option\n";
                    break;
                }
            }
        }
        else
        {
            cout<<"\nNo such option\n";
            choice = 0;
        }
    }
    //print_main_menu();
    serialClose(serial);
    return 0;
}