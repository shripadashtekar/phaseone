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
#include<string.h>
#include<iostream>
#include<cstdlib>
#include<sstream>

using namespace std;
ifstream in;
ofstream out;
string line,pid;
int cnt=0,i=0,j=0,TLL,TTL,LLC,TLC,terminate_code;
int SI,PI,TI,page_table_ptr;
bool occupied_pages[30];



class pcb
{
private:
    int returnedIC;

    
public:
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
        LLC=0;TLC=0;terminate_code=0;
    }
    
    
    int allocatepage()
    {
        int page_no=rand()%30;
        while(occupied_pages[page_no]==true)
            page_no=rand()%30;
        occupied_pages[page_no]=true;
        return page_no;
        
    }
    
   
}pcbobj;

class memory
{
private:
    char m[300][4];
    
public:
    void initializepagetable(int row_num,int page_no)
    {
        ostringstream temp;
        temp << page_no;
        string table_entry;
        if(page_no<10)
            table_entry="$10"+temp.str();
        else
            table_entry="$1"+temp.str();
        setmem(table_entry,page_table_ptr+row_num);
    }
    
    
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
        {
            m[position][i]=s[i];
            //cout<<m[position][i];
        }
        
    }
    
    void StoreCardInMemory(string card, int location)
    {
        
        string word="";
        int page_no=pcbobj.allocatepage();
        initializepagetable(location, page_no);
        page_no*=10;

        for(i=0;i<card.length();i+=4)
        {
            for(j=0;j<4;j++)
            {
                word+=card[i+j];
            }
            setmem(word,page_no);
            page_no++;
            word="";
           
        }
       //cout<<endl;
        
    }
    

    
    
    
}mem;




class cpu
{
    
private:
    char IR[4],IC[2],R[4],C,terminate,run_mos,pos;
    int returnedIC,ctemp;
    string returnedIR,temp,operand,opcode,temp1,compare_string;
    
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
   
    void setR(int pos)          // load into register from memory;

    {
        string temp="";
        temp=mem.getmem(pos);

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
        string value_page=mem.getmem(page);
        if(value_page[1]=='$')
        {
            PI=3;
            return -1;
        }
        value_page=value_page.substr(2,2);
        return (s_to_i(value_page)*10+(virtualaddress%10));
    }
    
    int translateaddress(string op)
    {
        if(s_to_i(op)==-1)
        {
            PI=2;
            return -2;
        }
        else
            translateaddress(s_to_i(op));
    }
    
    void  StartExecution()
    {
        setIC(0);
        terminate='F';

        while (terminate!='T')
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
            pos=translateaddress(s_to_i(operand));
            
            if(translateaddress(operand)!=-2 || !(operand.compare("rr")))
            {

                if(opcode.find("GD")!=-1)
                {
                    if(pos!=-1)
                    {
                        TLC++;
                        SI=1;
                    }
                    run_mos='T';
                
                }
            
                if(opcode.find("PD")!=-1)
                {
                    if(pos!=-1)
                    {
                        TLC++;
                        SI=2;
                    }
                    run_mos='T';
                
                }
            
                if(opcode.find("LR")!=-1)
                {
                    if(pos==-1)
                    {
                        run_mos='T';
                    }
                    else
                    {
                        //temp=mem.getmem(ctemp);
                        setR(pos);
                    }
                }
            
                
                if(opcode.find("SR")!=-1)
                {
                    temp=getR();
                    
                    if(pos==-1)
                    {
                        run_mos='T';
                    }
                    else
                    {
                        mem.setmem(temp,pos);
                        TLC++;
                    }
                }
            
                
                if(opcode.find("CR")!=-1)
                {
                 temp=getR();
                 //temp1=mem.getmem(ctemp);
                    
                    if(pos==-1)
                    {
                        run_mos='T';
                    }
                    
                    else
                    {
                        TLC++;
                        compare_string=mem.getmem(pos);
                        if(temp.compare(compare_string)==0)
                            C='T';
                        else
                            C='F';
                    }
                    /*
                    if(temp==temp1)
                    {
                    C='T';
                    }
                    
                    else
                    {
                    C='F';
                    }*/
                    
                    
                
                }
            
                if(opcode.find("BT")!=-1)
                {
                    if(C=='T')
                    setIC(s_to_i(operand));
                
                    TLC++;
                }
            
                if(opcode.find("Hr")!=-1)
                {
                    TLC++;
                    returnedIR='H';
                    SI=3;
                    run_mos='T';
                 
                }
            }
            else
            {
                run_mos='T';
            }
            
            if(TLC>TTL)
            {
                TI=2;
                run_mos='T';
                terminate='T';
                terminate_code=3;
            }
            
            if(run_mos=='T')
            mos();//   As mos is called everytime it calls function from mos with previously set interrupt;
            
            
        }
        
        
        
        
    }
    
    
   void mos ()
    {
        /*switch(SI)
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
         */
        if(TI==0 && SI==1)
        {
            //get the instrution of the file in terms of 4 words
            //store it one by one into memory
            //at start store the program card from memory location with unit place 0
            
            string s;
            pos=(pos/10)*10;
            getline(in,s);
            if(s.find("$END")!=-1)
            {
                terminate='T';
                terminate_code=1;
            }
            else
            {
                if(!s.empty() && s[s.size()-1]=='\r')
                    s.erase(s.size()-1);
                int len=s.length(),start=0,i;
                string s1;
                for(i=pos;start<len;i++)
                {
                    if((len-start)<4)
                        s1=s.substr(start,(len-start));
                    else
                        s1=s.substr(start,4);
                    start+=4;
                    mem.setmem(s1,i);
                }
            }
            SI=0;
        }
        else if(TI==2 && SI==1)
        {
            terminate='T';
            terminate_code=3;
        }
        else if((TI==0 || TI==2) && SI==2)
        {
            LLC++;
            if(LLC>TLL)
            {
                terminate='T';
                terminate_code=2;
            }
            //put the data from memory into the file specified
            else
            {
                int pos=translateaddress(s_to_i(operand)),flag=0;
                pos=(pos/10)*10;
                string ans="",temp="";
                for(int i=pos;i<pos+10;i++)
                {
                    temp=mem.getmem(i);
                    for(int j=0;j<4;j++)
                    {
                        if(temp[j]=='\0' || temp[j]=='0')
                        {
                            break;
                            flag=1;
                        }
                        ans+=temp[j];
                    }
                    if(flag)
                        break;
                }
                out<<ans<<endl;
                if(TI==2)
                {
                    terminate='T';
                    terminate_code=3;
                }
            }
            SI=0;
        }
        else if((TI==0 || TI==2) && SI==3)
        {
            //make terminate true to end the program
            //as we encountered the halt instruction
            
            terminate='T';
            terminate_code=0;
        }
        else if(TI==0 && PI==1)
        {
            terminate_code=4;
            terminate='T';
        }
        else if(TI==0 && PI==2)
        {
            terminate_code=5;
            terminate='T';
        }
        
        else if(TI==0 && PI==3)
        {
            if(!(opcode.compare("GD")) || !(opcode.compare("SR")))
            {
                int page_no=pcbobj.allocatepage();
                mem.initializepagetable((s_to_i(operand))/10,page_no);
                setIC(getIC()-1);
            }
            else
            {
                terminate_code=6;
                terminate='T';
            }
        }
        
        else if(TI==2 && PI==1)
        {
            terminate_code=7;
            terminate='T';
        }
        
        else if(TI==2 && PI==2)
        {
            terminate_code=8;
            terminate='T';
        }
        
        else if(TI==2 && PI==3)
        {
            terminate_code=3;
            terminate='T';
        }
        if(terminate=='T')
        {
            out<<pid<<" ";
            switch(terminate_code)
            {
                case 0:out<<"NO ERROR\n";
                    break;
                case 1:out<<"OUT OF DATA\n";
                    break;
                case 2:out<<"LINE LIMIT EXCEEDED\n";
                    break;
                case 3:out<<"TIME LIMIT EXCEEDED\n";
                    break;
                case 4:out<<"OPERATION CODE ERROR\n";
                    break;
                case 5:out<<"OPERAND ERROR\n";
                    break;
                case 6:out<<"INVALID PAGE FAULT\n";
                    break;
                case 7:out<<"TIME LIMIT EXCEEDED with OPERATION CODE ERROR\n";
                    break;
                case 8:out<<"TIME LIMIT EXCEEDED with OPERAND ERROR\n";
                    break;
            }
            out<<getIC()<<" "<<returnedIR<<" "<<TLC<<" "<<LLC<<endl;
            out<<"-----------------------------------------------------------------------"<<endl<<endl;
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
        
        if(line.empty())
        break;
        
        if(line.find("$AMJ")!=-1)
        {
            mem.resetmem();
            memcnt=0;
            pcbobj.initializepcb(line.substr(4,12));
            pcbobj.allocatepage();
            //pcbobj.initializepagetable();
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
            mem.StoreCardInMemory(line,memcnt++);
            //memcnt=memcnt+10;
        }
        
    }
   
    in.close();
    out.close();
    return 0;
}

