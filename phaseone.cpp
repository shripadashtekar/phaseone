#include <stdio.h>
#include<string.h>
#include<string>
#include<iostream>
#include<stdlib.h>
#include<fstream>
using namespace std;
ifstream in;
ofstream out;
string line;
int cnt=0,i=0,j=0;
class memory {
    
    
private:
    char m[100][4];
    
public:
    void resetmem()
    {
     //    cout<<"I am here 5";
        //for( i=0;i<100;i++)
          //  for(j=0;j<4;j++)
            //    m[i][j]=0;
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
        
         //cout<<"I am here 6";
        for(i=0;i<card.length();i++)
        {
            for(j=0;j<4;j++)
            {
                if(cnt>=card.length())
                break;

                m[location][j]=card[cnt];
                
                //cout<<i<<j<<location<<cnt<<" "<<m[i][j]<<endl;
                cnt++;
                //out<<m[location][j];
            }
            location++;
           
        }
        i=j=cnt=0;
        //out<<endl;
        
    }
    
    void putdatafrommemory(int location)
    {
        string temp="",temp1="";
        //out<<"Putting data";
        for(i=0;i<10;i++)
        {
            temp.clear();
            temp1.clear();
          temp=getmem(location);
            for(j=0;j<4;j++)
            {
                if(temp[j]=='\0'||temp[j]=='0')
                    break;
                else
                    temp1=temp1+temp[j];
            
            }
            out<<temp1;
            location++;
            
        }
        out<<endl;
    }
    
}mem;

class cpu
{
private:
    char IR[4],IC[2],R[4],C='F',terminated='F';             //Declaration of variables IC, R, IR, C;
    int returnedIC,returnedconvIR,returnedconvR,ctemp,SI=0;
    string returnedIR,returnedR,temp,operand,opcode,temp1;
    
public:
   
    
    void setIC(int val)     //seperate digits and store in IC by converting it into character;
    
        {
            // cout<<"I am here 7";
            int y=val % 10;
            IC[1]=y+'0';
            int x=val/10;
            IC[0]=x+'0';
            //cout<<mem.m[0][0]<<mem.m[0][1]<<mem.m[0][2]<<mem.m[0][3]<<endl;
           
           
        }
    
    int stringtoint(string s)
    {
        int val=((int)s[0]-'0')*10+((int)s[1]-'0');
        return val;
    }
    
    int getIC()
    {
         //cout<<"I am here 8";
        //extract the characters, convert to a number and return
        int val=((int)IC[0]-'0')*10+((int)IC[1]-'0');
        
       // cout<<(int)IC[0]<<(int)IC[1]<<endl;
        return val;
        
    }
    
    void setIR(int position)
    {
        string temp="";
        temp=mem.getmem(position);
        for(int i=0;i<4;i++)
            IR[i]=temp[i];
        
        
        
         //cout<<"I am here 9";
        // get from mem and store in IR
        
        //cout<<position<<endl;
       /* j=0;
        IR[0]=getmem(position);
        //cout<<IR[0];
            j++;
        IR[1]=mem.m[position][j];
        //cout<<IR[1];
            j++;
        IR[2]=mem.m[position][j];
        //cout<<IR[2];
            j++;
        IR[3]=mem.m[position][j];*/
        //cout<<IR[3];
       // cout<<mem.m[0][0]<<mem.m[0][1]<<mem.m[0][2]<<mem.m[0][3]<<endl<<endl;
        
        //cout<<IR[0]<<IR[1]<<IR[2]<<IR[3]<<endl;
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
   
    void setR(string temp)
    {
        R[0]=temp[0];
        R[1]=temp[1];
        R[2]=temp[2];
        R[3]=temp[3];
        
        // load into register from memory;
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
        // cout<<"I am here 107"<<endl;
        setIC(0);
        while (terminated!='T')
        {
            //fetch instruction from ic memory location
            //store instruction in IR
            returnedIC=getIC();
            //cout<<returnedIC<<endl;
            setIR(returnedIC);
            //store M[IC] into IR;
            incIC();
            returnedIR=getIR();
        opcode=returnedIR.substr(0,2);
           // cout<<opcode<<endl;
        operand=returnedIR.substr(2,2);
        ctemp=stringtoint(operand);
            //cout<<ctemp<<endl;
            if(opcode.find("GD")!=-1)
            {
                SI=1;
                
            }
            if(opcode.find("PD")!=-1)
            {
                SI=2;
                
            }
            if(opcode.find("LR")!=-1)
            {
                //cout<<"here 11";
                temp=mem.getmem(ctemp);
                setR(temp);
                temp=getR();
               // cout<<temp<<endl;
            
            }
            
            if(opcode.find("SR")!=-1)
            {
                //cout<<"here 12";
                temp=getR();
                mem.setmem(temp,ctemp);
                
            }
            
            if(opcode.find("CR")!=-1)
            {
             //   cout<<"here 13";
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
                //cout<<"here 14";
                if(C=='T')
                setIC(ctemp);
                
            }
            
            if(opcode.find("H")!=-1)
            {
               // cout<<"here 15";
                SI=3;
            }
            
            
            mos();
            
            
        }
        
        
        
        
    }
    
    
   void mos ()
    {
        switch(SI)
        {
            case 1:
               // out<<"GD calling"<<endl;
                //cout<<line<<endl;
                line.clear();
                getline(in,line);
               // cout<<line<<(ctemp/10)*10<<endl;
                mem.StoreCardInMemory(line,(ctemp/10)*10);
                //mem.setmem();
                
            
            
            
        case 2:
                //out<<"PD calling"<<endl;
                //cout<<line<<(ctemp/10)*10<<endl;
                mem.putdatafrommemory((ctemp/10)*10);
                    
                
            
        case 3: terminated='T';
                out<<"-------------------------------------------------------------"<<endl<<endl;// print dash lines
        
        }
        
        
    }

}cpuobj;

int main()
{
    
    int memcnt=0;
    in.open("ip.c");
    out.open("op.c");
    
    while(!(in.eof()))
    {
        line.clear();
        getline(in,line);
        //cout<<line<<endl;
        if(line.find("$AMJ")!=-1)
        {
            // cout<<"I am here 1";
            mem.resetmem();
            //i=0;
            //continue;

        }
        
        else if(line.find("$DTA")!=-1)
        {
            //cout<<"I am here 2";
            //cpuobj.setIC(0);
            cpuobj.StartExecution();
           // continue;
        }
        
        else if(line.find("END")!=-1)
        {
             //cout<<"I am here 3";
            continue;
        }
        
        else
        {
             //cout<<"I am here 4";
            mem.StoreCardInMemory(line,memcnt);
            //cout<<mem.m[0][0]<<mem.m[0][1]<<mem.m[0][2]<<mem.m[0][3]<<endl;

            memcnt=memcnt+10;
        }
        //out<<endl;
        
    }
    
    in.close();
    out.close();
    return 0;
}

