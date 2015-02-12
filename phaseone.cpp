#include <stdio.h>
#include<string>
#include<fstream>
using namespace std;
ifstream in;
ofstream out;
string line;
int cnt=0,i=0,j=0;

class memory
{
private:
    char m[100][4];
    
public:
    void resetmem()
    {
     
        memset(m,'0',sizeof(char)*100*4);
    }
    
    string getmem(int position)
    {
        string temp="";
        for(int i=0;i<4;i++)
            temp=temp+m[position][i];
        
        return temp;
    }
    
    void setmem(string s, int position)
    {
        
        for(int i=0;i<4;i++)
            m[position][i]=s[i];
    }
    
    void StoreCardInMemory(string card, int location)
    {
        cnt=0;
        
        for(i=0;i<card.length();i++)
        {
            for(j=0;j<4;j++)
            {
                if(cnt>=card.length())
                break;

                m[location][j]=card[cnt];
                cnt++;
            }
            location++;
           
        }
        i=j=cnt=0;
        
    }
    
    void putdatafrommemory(int location)
    {
        string temp="";
        for(i=0;i<10;i++)
        {
            temp.clear();
            temp=getmem(location);
            
            for(j=0;j<4;j++)
            {
                if(temp[j]=='\0'||temp[j]=='0')
                    break;
                
                else
                    out<<temp[j];
            
            }
            
            location++;
            
        }
        out<<endl;
    }
    
}mem;

class cpu
{
    
private:
    char IR[4],IC[2],R[4],C,terminated;
    int returnedIC,returnedconvIR,returnedconvR,ctemp,SI;
    string returnedIR,returnedR,temp,operand,opcode,temp1;
    
public:
   
    void setIC(int val)             //seperate digits and store in IC by converting it into character;
    
    {
        int y=val % 10;
        IC[1]=y+'0';
        int x=val/10;
        IC[0]=x+'0';
           
    }
    
    int stringtoint(string s)
    {
        int val=((int)s[0]-'0')*10+((int)s[1]-'0');
        return val;
    }
    
    int getIC()                     //extract the characters, convert to a number and return
    {
        int val=((int)IC[0]-'0')*10+((int)IC[1]-'0');
        return val;
        
    }
    
    void setIR(int position)        // get from mem and store in IR
    {
        string temp="";
        temp=mem.getmem(position);
        for(int i=0;i<4;i++)
            IR[i]=temp[i];
        
    }
    
    void incIC()
    {
       int value=getIC();
        value++;
        setIC(value);
    }
    
    int convertIR()
    {
        int val=((int)IR[2]-'0')*10+((int)IC[3]-'0');
        return val;
    }
    
    int convertR()
    {
        int val=((int)R[0]-'0')*10+((int)R[1]-'0');
        return val;
    }
    
    
     string getIR()
    {
        string temp="";
        for(int i=0;i<4;i++)
            temp=temp+IR[i];
        return temp;
        
    }
   
    void setR(string temp)          // load into register from memory;

    {
        R[0]=temp[0];
        R[1]=temp[1];
        R[2]=temp[2];
        R[3]=temp[3];
    }
    
    string getR()
    {
        string temp="";
        for(int i=0;i<4;i++)
            temp=temp+R[i];
        return temp;
      
    }
    
    
    void  StartExecution()
    {
        setIC(0);
        terminated='F';

        while (terminated!='T')
        {
            returnedIC=getIC();
            setIR(returnedIC);
            incIC();
            returnedIR=getIR();
            opcode=returnedIR.substr(0,2);
            operand=returnedIR.substr(2,2);
            ctemp=stringtoint(operand);
            if(opcode.find("GD")!=-1)
            {
                 SI=1;
                 mos();
                
            }
            if(opcode.find("PD")!=-1)
            {
                 SI=2;
                 mos();
                
            }
            if(opcode.find("LR")!=-1)
            {
                temp=mem.getmem(ctemp);
                setR(temp);
            
            }
            
            if(opcode.find("SR")!=-1)
            {
                temp=getR();
                mem.setmem(temp,ctemp);
                
            }
            
            if(opcode.find("CR")!=-1)
            {
                temp=getR();
                temp1=mem.getmem(ctemp);
                if(temp==temp1)
                {
                    C='T';
                    
                }
                else
                {
                    C='F';
                }
                
            }
            
            if(opcode.find("BT")!=-1)
            {
                if(C=='T')
                setIC(ctemp);
                
            }
            
            if(opcode.find("H")!=-1)
            {
                 SI=3;
                 mos();
            }
            
            
           // mos();   As mos is called everytime it calls function from mos with previously set interrupt;
            
            
        }
        
        
        
        
    }
    
    
   void mos ()
    {
        switch(SI)
        {
            case 1: line.clear();
                    getline(in,line);
                    mem.StoreCardInMemory(line,(ctemp/10)*10);
                    break;
    
            case 2:mem.putdatafrommemory((ctemp/10)*10);
                   break;
        
            case 3:terminated='T';
                   out<<"-------------------------------------------------------------"<<endl;
                   break;
        }
        
        
    }

}cpuobj;

int main()
{
    
    int memcnt=0;
    in.open("ip.txt");
    out.open("op.txt");
    
    while(!(in.eof()))
    {
        line.clear();
        getline(in,line);
        if(line.find("$AMJ")!=-1)
        {
            mem.resetmem();
            continue;

        }
        
        else if(line.find("$DTA")!=-1)
        {
            cpuobj.StartExecution();
            continue;
        }
        
        else if(line.find("END")!=-1)
        {
            memcnt=0;
            continue;
        }
        
        else
        {
            mem.StoreCardInMemory(line,memcnt);
            memcnt=memcnt+10;
        }
        
    }
   
    in.close();
    out.close();
    return 0;
}

