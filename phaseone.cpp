#include <stdio.h>
#include<string.h>
#include<iostream>
#include<stdlib.h>
#include<fstream>
using namespace std;
ifstream in;
ofstream out;
int cnt=0;
class memory {
    
    
public:
    char m[100][4];
    void resetmem()
    {
        for(int i=0;i<100;i++)
            for(int j=0;j<4;j++)
                m[i][j]=48;
    }

    void StoreCardInMemory(string card, int location)
    {
        cnt=0;
        for(int i=0;i<card.length();i++)
            for(int j=0;j<4;j++)
            {
                if(cnt>=card.length())
                break;

                m[i][j]=card[cnt];
                cnt++;
                out<<m[i][j];
            }
        
    }
    
}mem;

int main()
{
    
    int m=0;
    in.open("ip.c");
    out.open("op.c");
    string line;
    while(!(in.eof()))
    {
        line.clear();
        getline(in,line);
        cout<<line<<endl;
        if(line.find("$AMJ")!=-1)
        {
            mem.resetmem();

        }
        
        else if(line.find("$DTA")!=-1)
        {
            //startexecution();
            continue;
        }
        
        else if(line.find("END")!=-1)
        {
            continue;
        }
        
        else
        {
            
            mem.StoreCardInMemory(line,m);
            
            m=m+10;
        }
        
    }
    in.close();
    out.close();
    return 0;
}

