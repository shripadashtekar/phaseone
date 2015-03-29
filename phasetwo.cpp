/*
TITLE: Implementation of Multiprogramming operating system Phase 1:
NAME: Ashtekar Shripad, Anmol Biyani, Bhupendra Singh Solanki
ROLLNO: 4,11,10
CLASS: TY H                	 BRANCH: IT              	BATCH: 1
DATE OF Submission: 13 February 2015
*/

#include <stdio.h>
#include<string>
#include<fstream>
using namespace std;
ifstream in;
ofstream out;
string line;
int cnt=0,i=0,j=0;
int SI,PI,TI,page_table_ptr;
bool occupied_pages[30];

class memory
{
private:
    char m[300][4];
    
public:
    void resetmem()
    {
     
        memset(m,'0',sizeof(char)*300*4);
        memset(occupied_pages,false,sizeof(bool)*30);
        page_table_ptr=rand()%30;
        occupied_pages[page_table_ptr]=true;
        page_table_ptr*=10;
        SI=PI=TI=0;
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
        
        string word="";
        int page_no=pcbobj.allocate_page();
        initializepagetable(location, page_no);
        page_no*=10;

        for(i=0;i<card.length();i++)
        {
            for(j=0;j<4;j++)
            {
                word+=s[i+j];
            }
            setmem(word,page_no);
            page_no++;
            word="";
           
        }
       
        
    }
    
    void putdatafrommemory(int location)
    {
        string temp="";
        for(i=0;i<300;i++)
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
    char IR[4],IC[2],R[4],C,terminated,run_mos,pos;
    int returnedIC,ctemp;
    string returnedIR,temp,operand,opcode,temp1;
    
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
    
    int s_to_i(string operand)
    {
        if(operand[0]>='0' && operand[0]<='9' && operand[1]>='0' && operand[1]<='9')
            return ((int)operand[0]-48)*10+((int)operand[1]-48);
        return -1;
    }
    
    int translateaddress(int virtualaddress)
    {
        int page=page_table_ptr+(virtualaddress/10);
        string value_page=m_obj.getmem(page);
        if(value_page[1]=='$')
        {
            PI=3;
            return -1;
        }
        value_page=value_page.substr(2,2);
        return (s_to_i(value_page)*10+(virtualaddress%10));
    }
    
    void  StartExecution()
    {
        setIC(0);
        terminated='F';

        while (terminated!='T')
        {
            run_mos='F';
            returnedIC=translateaddress(getIC());
            setIR(returnedIC);
            incIC();
            returnedIR=getIR();

            if((returnedIR.compare("H"))==3)
                returnedIR="Hrrr";

            opcode=returnedIR.substr(0,2);
            operand=returnedIR.substr(2,2);
            ctemp=stringtoint(operand);
            pos=address_tranlation(s_to_i(operand));
            
            if(address_tranlation(operand)!=-2 || !(operand.compare("rr")))
            {

                if(opcode.find("GD")!=-1)
                {
                 SI=1;
                 //mos();
                
                }
            
                if(opcode.find("PD")!=-1)
                {
                 SI=2;
                 //mos();
                
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
            
                if(opcode.find("Hr")!=-1)
                {
                 SI=3;
                 //mos();
                }
            
            
           mos();//   As mos is called everytime it calls function from mos with previously set interrupt;
            
            
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
        SI=0;
        
        
    }

}cpuobj;

class pcb
{
private:
    int em,TLL,TTL,LLC,TLC,returnedIC;
    string pid;

    int s_to_i(string operand)
    {
        if(operand[0]>='0' && operand[0]<='9' && operand[1]>='0' && operand[1]<='9')
            return ((int)operand[0]-48)*10+((int)operand[1]-48);
        return -1;
    }
    
    void initializepcb(string length)
    {
        pid=length.substr(0,4);
        TTL=s_to_i(length.substr(4,2))*100+s_to_i(length.substr(6,2));
        TLL=s_to_i(length.substr(8,2))*100+s_to_i(length.substr(10,2));
        LLC=0;TLC=0;em=0;
    }
    
    
    int allocatepage()
    {
        int page_no=rand()%30;
        while(occupied_pages[page_no]==true)
            page_no=rand()%30;
        occupied_pages[page_no]=true;
        return page_no;
        
    }
    
    void initializepagetable(int row_num,int page_no)
    {
        ostringstream temp;
        temp << page_no;
        string table_entry;
        if(page_no<10)
            table_entry="$10"+temp.str();
        else
            table_entry="$1"+temp.str();
        mem.setmem(table_entry,page_table_ptr+row_num);
    }
    
    
}pcbobj;

int main()
{
    
    int memcnt=0;
    in.open("ip.txt");
    out.open("op.txt");
    
    while(!(in.eof()))
    {
        line.clear();
        getline(in,line);
        
        if(in.empty())
        break;
        
        if(line.find("$AMJ")!=-1)
        {
            mem.resetmem();
            memcnt=0;
            pcbobj.initializepcb(in.substr(4,12));
            pcbobj.allocatepage();
            pcbobj.initializepagetable();
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

