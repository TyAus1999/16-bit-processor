#include <stdio.h>
#include <cstdlib>
#include <vector>

typedef unsigned short int WORD;

int getSizeOfCharPtr(char*in){
    int length=0;
    while(1){
        if(*(in+length)==NULL)break;
        else
            length++;
    }
    return length;
}
WORD convertCharP(char*in,bool*success){//Hex-h Binary-i Blank for decimal
    *success=true;
    int length=getSizeOfCharPtr(in);
    //printf("%s\n",in);
    WORD out=0;
    bool isHex=false;
    bool isBin=false;
    if(*(in+length-1)=='h' || *(in+length-2)=='h'){
        isHex=true;
        length--;
        //printf("HEX\n");
    }
    else if(*(in+length-1)=='i' || *(in+length-2)=='i'){//BINARY numbers end in i
        isBin=true;
        length--;
        //printf("BIN\n");
    }
    
    for(int i=0;i<length-1;i++){
        char c=*(in+i);
        if(c!=10 && c!=13)
            if(isHex){
                if(c>=48 && c<=57){
                    WORD temp=c-48;
                    //printf("%x\n",temp);
                    out<<=4;
                    out|=temp;
                }
                else if(c>=65 && c<=70){
                    WORD temp=c-65;
                    temp+=10;
                    //printf("%u\n",temp);
                    out<<=4;
                    out|=temp;
                }
                else if(c>=97 && c<=102){
                    WORD temp=c-97;
                    temp+=10;
                    out<<=4;
                    out|=temp;
                }
                else{
                    //printf("Error in converstion ,%c,\n",c);
                    *success=false;
                    break;
                }
            }
            else if(isBin){
                if(c==48){
                    out<<=1;
                    out|=0;
                }
                else if(c==49){
                    out<<=1;
                    out|=1;
                }
                else{
                    //printf("Error in converstion ,%c,\n",c);
                    *success=false;
                    break;
                }
            }
            else{
                if(c>=48 && c<=57){
                    WORD temp=c-48;
                    out*=10;
                    out+=temp;
                }
                else{
                    //printf("Error in converstion ,%c,\n",c);
                    *success=false;
                    break;
                }
            }
    }
    return out;
}
void macroASM(char*in){
    FILE*cFile=fopen(in,"r");
    int lineSize=2048;
    char* retV;
    char buffer[lineSize];
    bool error=false;
    unsigned int lineNumber=1;
    while(!error){
        retV=fgets(buffer,lineSize,(FILE*)cFile);

        for(int i=0;i<lineSize;i++){
            if(*(retV+i)=='i')
                if(*(retV+i+1)=='f'){

                }
        }
    }
    fclose(cFile);
}
bool compareString(char*in,char*compare){
    unsigned int offset=0;
    while(1){
        if(*(in+offset)==0 && *(compare+offset)==0)
            break;
        else if(*(in+offset)!=*(compare+offset))
            return false;
        else if(offset==(unsigned int)-1)
            break;
        offset++;
    }
    return true;
}
struct asmLine{
    char*line;
    unsigned int lineNumber;
};
struct asmLabel{
    char*name;
};
struct instruction{
    char* line;
    WORD opCode;
    bool hasData=false;
    WORD data;
};
struct macro{
    std::vector<instruction> instructions;
    char*name;
};
std::vector<WORD> compile3(FILE*asmFile){//version of compiler includes labels and macros
    std::vector<WORD> out;
    std::vector<instruction> instructions;
    std::vector<macro> macros;
    std::vector<asmLabel> labels;
    std::vector<asmLine>fileContents;
    char* retV;
    int bSize=255;
    char buffer[bSize];
    unsigned int lineNumber=1;
    bool error=false;
    //transfers the file to a vector as to be easy to traverse
    while(1){
        readFile:
        retV=fgets(buffer,bSize,(FILE*)asmFile);
        if(retV==NULL)break;
        else if(*retV==10 || *retV==13){
            goto readFile;
        }
        char*temp=new char[bSize+1];
        for(int i=0;i<bSize;i++){
            temp[i]=*(retV+i);
        }
        temp[bSize]=0;
        asmLine tempLine;
        tempLine.line=temp;
        tempLine.lineNumber=lineNumber;

        fileContents.push_back(tempLine);
        lineNumber++;
    }
    //pre comp Stage 1
    //removes comments from line of code
    //comments are denoted in the c mannor minus the multi line commenting (No clue how to do that just yet)
    for(int i=0;i<fileContents.size();i++){
        for(int ch=0;ch<bSize+1;ch++){
            char* temp=fileContents[i].line;
            if(*(temp+ch)=='/' && *(temp+ch+1)=='/'){//remove line
                for(int tempRemove=ch;tempRemove<bSize;tempRemove++)
                    *(temp+tempRemove)=0;
            }
        }
    }
    //pre comp Stage 2
    //removes empty lines left over by comment purge and frees memory
    std::vector<int> indexsToRemove;
    for(int i=0;i<fileContents.size();i++){
        if(*fileContents[i].line==0){
            indexsToRemove.push_back(i);
            delete[] fileContents[i].line;
        }
    }
    for(int i=0;i<indexsToRemove.size();i++)
        fileContents.erase(fileContents.begin()+indexsToRemove[i]);
    //pre comp Stage 3
    //searches for macro definitions
    bool foundMacro=false;
    indexsToRemove.clear();
    for(int i=0;i<fileContents.size();i++){
        char*l=fileContents[i].line;
        for(int p=0;p<bSize;p++){
            if(*(l+p)=='.' && *(l+p+1)=='m' && *(l+p+2)=='a' && *(l+p+3)=='c' && *(l+p+4)=='r' && *(l+p+5)=='o'){
                foundMacro=true;
                macro m;
                int count=0;
                for(int ch=p+7;ch<bSize;ch++)
                    if(*(l+ch)!=0)count++;
                    else break;
                char* n=new char[count];
                m.name=n;
                macros.push_back(m);
                break;
            }
            else if(*(l+p)=='.' && *(l+p+1)=='e' && *(l+p+2)=='n' && *(l+p+3)=='d' && *(l+p+4)=='m' && *(l+p+5)=='a' && *(l+p+6)=='c' && *(l+p+7)=='r' && *(l+p+8)=='o'){
                foundMacro=false;
                break;
            }
        }
        if(foundMacro){
            instruction ins;
            int count=0;
            for(int p=0;p<bSize;p++)
                if(*(l+p)!=0)count++;
                else break;
            char*t=new char[count];
            for(int p=0;p<count;p++)
                t[p]=*(l+p);
            macros[macros.size()-1].instructions.push_back(ins);
            indexsToRemove.push_back(i);
        }
    }
    for(int i=0;<indexsToRemove.size();i++)
        fileContents.erase(fileContents.begin()+indexsToRemove[i]);
    //pre comp Stage 4
    //search for labels
    for(int i=0;i<fileContents.size();i++){
        char*l=fileContents[i].line;
        for(int ch=0;ch<bSize;ch++){
            if(*(l+ch)==':'){
                
            }
        }
    }
    //pre comp Stage 5
    //set addresses for the labels

    //com stage 1
    //convert instructions to byte code


    
    
    for(int i=0;i<macros.size();i++){
        delete[] macros[i].name;
        for(int p=0;p<macros[i].instructions.size();p++)
            delete[] macros[i].instructions[p].line;
    }
    for(int i=0;i<instructions.size();i++)
        delete[] instructions[i].line;
    for(int i=0;i<fileContents.size();i++)
        delete[] fileContents[i].line;
    return out;
}
std::vector<WORD> compile1(char*in){//remaster of compile0
    FILE*asmFile=fopen(in,"r");
    std::vector<WORD> out;
    std::vector<macro> macros;

    bool error=false,isMacro=false;
    char* retV;
    int bSize=255;
    char buffer[bSize];
    unsigned int lineNumber=1;
    while(!error){
        topLoop:
        retV=fgets(buffer,bSize,(FILE*)asmFile);
        if(retV==NULL)break;
        else if(*retV==10 || *retV==13){
            lineNumber++;
            goto topLoop;
        }
        bool foundInstruction=false;
        int commaPos=-1,periodPos=-1;
        for(int i=0;i<bSize;i++){
            if(*(retV+i)==','){
                commaPos=i;
                break;
            }
            else if(*(retV+i)=='.'){
                periodPos=i;
                break;
            }
        }
        if(commaPos!=-1){//Check for instructions with commas
            //Check for mov instruction
            bool isMovInstruction=false,isAddInstruction=false,isSubInstruction=false,isCmpInstruction=false,
                    isAndInstruction=false,isOrInstruction=false,isXorInstruction=false,isShlInstruction=false,
                    isShrInstruction=false;
            for(int i=0;i<bSize-3;i++){//Checking for instruction
                if(*(retV+i)=='m' && *(retV+i+1)=='o' && *(retV+i+2)=='v'){
                    isMovInstruction=true;
                    break;
                }
                else if(*(retV+i)=='a' && *(retV+i+1)=='d' && *(retV+i+2)=='d'){
                    isAddInstruction=true;
                    break;
                }
                else if(*(retV+i)=='s' && *(retV+i+1)=='u' && *(retV+i+2)=='b'){
                    isSubInstruction=true;
                    break;
                }
                else if(*(retV+i)=='c' && *(retV+i+1)=='m' && *(retV+i+2)=='p'){
                    isCmpInstruction=true;
                    break;
                }
                else if(*(retV+i)=='a' && *(retV+i+1)=='n' && *(retV+i+2)=='d'){
                    isAndInstruction=true;
                    break;
                }
                else if(*(retV+i)=='o' && *(retV+i+1)=='r'){
                    isOrInstruction=true;
                    break;
                }
                else if(*(retV+i)=='x' && *(retV+i+1)=='o' && *(retV+i+2)=='r'){
                    isXorInstruction=true;
                    break;
                }
                else if(*(retV+i)=='s' && *(retV+i+1)=='h'){
                    if(*(retV+i+2)=='l')isShlInstruction=true;
                    else if(*(retV+i+2)=='r')isShrInstruction=true;
                    break;
                }
            }
            //printf("%s\n",retV);
            if(isMovInstruction){
                //check for mov ax,reg/value16/[reg]
                if(*(retV+commaPos-2)=='a'){
                    //check for mov ax,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x1);
                        out.push_back(value);
                    }
                    else{
                        //check for mov ax,bx
                        if(*(retV+commaPos+1)=='b')
                            out.push_back(0x5);
                        //check for mov ax,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x6);
                        //check for mov ax,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x7);
                        //check for mov ax,[ax]
                        else if(*(retV+commaPos+2)=='a')
                            out.push_back(0x11);
                        //check for mov ax,[bx]
                        else if(*(retV+commaPos+2)=='b')
                            out.push_back(0x12);
                        //check for mov ax,[cx]
                        else if(*(retV+commaPos+2)=='c')
                            out.push_back(0x13);
                        //check for mov ax,[dx]
                        else if(*(retV+commaPos+2)=='d')
                            out.push_back(0x14);
                        else{
                            printf("Error on line:%u\n",lineNumber);
                            printf("%s\n",retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for mov bx,reg/value16/[reg]
                else if(*(retV+commaPos-2)=='b'){
                    //check for mov bx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    //printf("%u\n",value);
                    //printf("%s\n",retV+commaPos+1);
                    if(success){
                        out.push_back(0x2);
                        out.push_back(value);
                    }
                    else{
                        //check for mov bx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x8);
                        //check for mov bx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x9);
                        //check for mov bx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0xa);
                        //check for mov bx,[ax]
                        else if(*(retV+commaPos+2)=='a')
                            out.push_back(0x15);
                        //check for mov bx,[bx]
                        else if(*(retV+commaPos+2)=='b')
                            out.push_back(0x16);
                        //check for mov bx,[cx]
                        else if(*(retV+commaPos+2)=='c')
                            out.push_back(0x17);
                        //check for mov bx,[dx]
                        else if(*(retV+commaPos+2)=='d')
                            out.push_back(0x18);
                        else{
                            printf("Error on line:%u\n",lineNumber);
                            printf("%s\n",retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for mov cx,reg/value16/[reg]
                else if(*(retV+commaPos-2)=='c'){
                    //check for mov cx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x3);
                        out.push_back(value);
                    }
                    else{
                        //check for mov cx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0xb);
                        //check for mov cx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0xc);
                        //check for mov cx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0xd);
                        //check for mov cx,[ax]
                        else if(*(retV+commaPos+2)=='a')
                            out.push_back(0x19);
                        //check for mov cx,[bx]
                        else if(*(retV+commaPos+2)=='b')
                            out.push_back(0x1a);
                        //check for mov cx,[cx]
                        else if(*(retV+commaPos+2)=='c')
                            out.push_back(0x1b);
                        //check for mov cx,[dx]
                        else if(*(retV+commaPos+2)=='d')
                            out.push_back(0x1c);
                        else{
                            printf("Error on line:%u\n",lineNumber);
                            printf("%s\n",retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for mov dx,reg/value16/[reg]
                else if(*(retV+commaPos-2)=='d'){
                    //check for mov dx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x4);
                        out.push_back(value);
                    }
                    else{
                        //check for mov dx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0xe);
                        //check for mov dx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0xf);
                        //check for mov dx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x10);
                        //check for mov dx,[ax]
                        else if(*(retV+commaPos+2)=='a')
                            out.push_back(0x1d);
                        //check for mov dx,[bx]
                        else if(*(retV+commaPos+2)=='b')
                            out.push_back(0x1e);
                        //check for mov dx,[cx]
                        else if(*(retV+commaPos+2)=='c')
                            out.push_back(0x1f);
                        //check for mov dx,[dx]
                        else if(*(retV+commaPos+2)=='d')
                            out.push_back(0x20);
                        else{
                            printf("Error on line:%u\n",lineNumber);
                            printf("%s\n",retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for mov [ax],reg
                else if(*(retV+commaPos-3)=='a'){
                    //check for mov [ax],ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x21);
                    //check for mov [ax],bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x22);
                    //check for mov [ax],cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x23);
                    //check for mov [ax],dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x24);
                    else{
                        printf("Error on line:%u\n",lineNumber);
                        printf("%s\n",retV);
                        error=true;
                        break;
                    }
                }
                //check for mov [bx],reg
                else if(*(retV+commaPos-3)=='b'){
                    //check for mov [bx],ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x25);
                    //check for mov [bx],bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x26);
                    //check for mov [bx],cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x27);
                    //check for mov [bx],dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x28);
                    else{
                        printf("Error on line:%u\n",lineNumber);
                        printf("%s\n",retV);
                        error=true;
                        break;
                    }
                }
                //check for mov [cx],reg
                else if(*(retV+commaPos-3)=='c'){
                    //check for mov [cx],ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x29);
                    //check for mov [cx],bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x2a);
                    //check for mov [cx],cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x2b);
                    //check for mov [cx],dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x2c);
                    else{
                        printf("Error on line:%u\n",lineNumber);
                        printf("%s\n",retV);
                        error=true;
                        break;
                    }
                }
                //check for mov [dx],reg
                else if(*(retV+commaPos-3)=='d'){
                    //check for mov [dx],ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x2d);
                    //check for mov [dx],bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x2e);
                    //check for mov [dx],cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x2f);
                    //check for mov [dx],dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x30);
                    else{
                        printf("Error on line:%u\n",lineNumber);
                        printf("%s\n",retV);
                        error=true;
                        break;
                    }
                }
                else{
                    printf("Error on line: %u\n",lineNumber);
                    printf("%s\n",retV);
                    error=true;
                    break;
                }
            }
            else if(isAddInstruction){
                //check for add ax,reg
                if(*(retV+commaPos-2)=='a'){
                    //check for add ax,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x31);
                    //check for add ax,bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x32);
                    //check for add ax,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x33);
                    //check for add ax,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x34);
                    else{
                        printf("Error on line: %u\n",lineNumber);
                        printf("%s\n",retV);
                        error=true;
                        break;
                    }
                }
                //check for add bx,reg
                else if(*(retV+commaPos-2)=='b'){
                    //check for add bx,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x35);
                    //check for add bx,bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x36);
                    //check for add bx,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x37);
                    //check for add bx,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x38);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
                //check for add cx,reg
                else if(*(retV+commaPos-2)=='c'){
                    //check for add cx,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x39);
                    //check for add cx,bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x3a);
                    //check for add cx,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x3b);
                    //check for add cx,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x3c);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
                //check for add dx,reg
                else if(*(retV+commaPos-2)=='d'){
                    //check for add dx,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x3d);
                    //check for add dx,bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x3e);
                    //check for add dx,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x3f);
                    //check for add dx,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x40);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
                else{
                    printf("Error on line: %u\n%s\n",lineNumber,retV);
                    error=true;
                    break;
                }
            }
            else if(isSubInstruction){
                //check for sub ax,reg
                if(*(retV+commaPos-2)=='a'){
                    //check for sub ax,bx
                    if(*(retV+commaPos+1)=='b')
                        out.push_back(0x41);
                    //check for sub ax,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x42);
                    //check for sub ax,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x43);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
                //check for sub bx,reg
                else if(*(retV+commaPos-2)=='b'){
                    //check for sub bx,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x44);
                    //check for sub bx,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x45);
                    //check for sub bx,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x46);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
                //check for sub cx,reg
                else if(*(retV+commaPos-2)=='c'){
                    //check for sub cx,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x47);
                    //check for sub cx,bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x48);
                    //check for sub cx,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x49);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
                //check for sub dx,reg
                else if(*(retV+commaPos-2)=='d'){
                    //check for sub dx,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x4a);
                    //check for sub dx,bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x4b);
                    //check for sub dx,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x4c);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
            }
            else if(isCmpInstruction){
                //check for cmp ax,reg/value16
                if(*(retV+commaPos-2)=='a'){
                    //check for cmp ax,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x59);
                        out.push_back(value);
                    }
                    else{
                        //check for cmp ax,bx
                        if(*(retV+commaPos+1)=='b')
                            out.push_back(0x4d);
                        //check for cmp ax,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x4e);
                        //check for cmp ax,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x4f);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for cmp bx,reg/value16
                else if(*(retV+commaPos-2)=='b'){
                    //check for cmp bx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x5a);
                        out.push_back(value);
                    }
                    else{
                        //check for cmp bx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x50);
                        //check for cmp bx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x51);
                        //check for cmp bx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x52);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for cmp cx,reg/value16
                else if(*(retV+commaPos-2)=='c'){
                    //check for cmp cx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x5b);
                        out.push_back(value);
                    }
                    else{
                        //check for cmp cx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x53);
                        //check for cmp cx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0x54);
                        //check for cmp cx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x55);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for cmp dx,reg/value16
                else if(*(retV+commaPos-2)=='d'){
                    //check for cmp dx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x5c);
                        out.push_back(value);
                    }
                    else{
                        //check for cmp dx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x56);
                        //cehck for cmp dx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0x57);
                        //check for cmp dx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x58);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
            }
            else if(isAndInstruction){
                //check for and ax,reg
                if(*(retV+commaPos-2)=='a'){
                    //check for and ax,ax
                    if(*(retV+commaPos+1)=='a')
                        out.push_back(0x62);
                    //check for and ax,bx
                    else if(*(retV+commaPos+1)=='b')
                        out.push_back(0x63);
                    //check for and ax,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x64);
                    //check for and ax,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x65);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
            }
            else if(isOrInstruction){
                //check for or ax,reg
                if(*(retV+commaPos-2)=='a'){
                    //check for or ax,bx
                    if(*(retV+commaPos+1)=='b')
                        out.push_back(0x66);
                    //check for or ax,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x67);
                    //check for or ax,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x68);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
            }
            else if(isXorInstruction){
                //check for xor ax,reg
                if(*(retV+commaPos-2)=='a'){
                    //check for xor ax,bx
                    if(*(retV+commaPos+1)=='b')
                        out.push_back(0x6d);
                    //check for xor ax,cx
                    else if(*(retV+commaPos+1)=='c')
                        out.push_back(0x6e);
                    //check for xor ax,dx
                    else if(*(retV+commaPos+1)=='d')
                        out.push_back(0x6f);
                    else{
                        printf("Error on line: %u\n%s\n",lineNumber,retV);
                        error=true;
                        break;
                    }
                }
            }
            else if(isShlInstruction){
                //check for shl ax,value16/reg
                if(*(retV+commaPos-2)=='a'){
                    //check for shl ax,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x70);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shl ax,bx
                        if(*(retV+commaPos+1)=='b')
                            out.push_back(0x71);
                        //check for shl ax,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x72);
                        //check for shl ax,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x73);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for shl bx,value16/reg
                else if(*(retV+commaPos-2)=='b'){
                    //check for shl bx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x74);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shl bx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x75);
                        //check for shl bx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x76);
                        //check for shl bx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x77);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for shl cx,value16/reg
                else if(*(retV+commaPos-2)=='c'){
                    //check for shl cx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x78);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shl cx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x79);
                        //check for shl cx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0x7a);
                        //check for shl cx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x7b);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for shl dx,value16/reg
                else if(*(retV+commaPos-2)=='d'){
                    //check for shl dx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x7c);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shl dx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x7d);
                        //check for shl dx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0x7e);
                        //check for shl dx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x7f);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                else{
                    printf("Not an instruction :%s on line %u\n",retV,lineNumber);
                    error=true;
                    break;
                }
            }
            else if(isShrInstruction){
                //check for shr ax,value16/reg
                if(*(retV+commaPos-2)=='a'){
                    //check for shr ax,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x80);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shr ax,bx
                        if(*(retV+commaPos+1)=='b')
                            out.push_back(0x81);
                        //check for shr ax,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x82);
                        //check for shr ax,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x83);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }

                    }
                }
                //check for shr bx,value16/reg
                else if(*(retV+commaPos-2)=='b'){
                    //check for shr bx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x84);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shr bx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x85);
                        //check for shr bx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x86);
                        //check for shr bx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x87);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for shr cx,value16/reg
                else if(*(retV+commaPos-2)=='c'){
                    //check for shr cx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x88);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shr cx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x89);
                        //check for shr cx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0x8a);
                        //check for shr cx,dx
                        else if(*(retV+commaPos+1)=='d')
                            out.push_back(0x8b);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
                //check for shr dx,value16/reg
                else if(*(retV+commaPos-2)=='d'){
                    //check for shr dx,value16
                    bool success;
                    WORD value=convertCharP(retV+commaPos+1,&success);
                    if(success){
                        out.push_back(0x8c);
                        out.push_back(value);
                        break;
                    }
                    else{
                        //check for shr dx,ax
                        if(*(retV+commaPos+1)=='a')
                            out.push_back(0x8d);
                        //check for shr dx,bx
                        else if(*(retV+commaPos+1)=='b')
                            out.push_back(0x8e);
                        //check for shr dx,cx
                        else if(*(retV+commaPos+1)=='c')
                            out.push_back(0x8f);
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
            }
            else{
                printf("Not an instruction :%s on line %u\n",retV,lineNumber);
                error=true;
                break;
            }
        }
        else if(periodPos!=-1){//check for .macros
            bool isWordMacro=false,isMacroMacro=false,isEndMacro=false,isOrgMacro=false;
            for(int i=periodPos+1;i<bSize-3;i++){
                //check for .word macro
                if(*(retV+i)=='w' && *(retV+i+1)=='o' && *(retV+i+2)=='r' && *(retV+i+3)=='d'){
                    isWordMacro=true;
                    break;
                }
                else if(*(retV+i)=='m' && *(retV+i+1)=='a' && *(retV+i+2)=='c' && *(retV+i+3)=='r' && *(retV+i+4)=='o'){
                    isMacroMacro=true;
                    isMacro=true;
                    break;
                }
                else if(*(retV+i)=='e' && *(retV+i+1)=='n' && *(retV+i+2)=='d' && *(retV+i+3)=='m' && *(retV+i+4)=='a' && *(retV+i+5)=='c' && *(retV+i+6)=='r' && *(retV+i+7)=='o'){
                    isMacro=false;
                    isEndMacro=true;
                    break;
                }
                else if(*(retV+i)=='o' && *(retV+i+1)=='r' && *(retV+i+2)=='g'){
                    isOrgMacro=true;
                    break;
                }
            }
            if(isWordMacro){
                bool success;
                WORD value=convertCharP(retV+periodPos+6,&success);
                if(success)out.push_back(value);
                else{
                    printf("Not success\n");
                }
            }
            else if(isMacroMacro){
                char* line=new char[bSize];
                for(int i=0;i<bSize;i++){
                    int toUse=i+periodPos+7;
                    line[i]=*(retV+toUse);
                }

            }
            else if(isEndMacro){

            }
            else if(isOrgMacro){
                bool success;
                WORD value=convertCharP(retV+periodPos+5,&success);
                if(success){
                    for(int i=0;i<value;i++)
                        out.push_back(0);
                }
            }
        }
        else{
            //check for not instruction
            bool isNotInstruction=false,isJmpInstruction=false,isJeInstruction=false,isJlInstruction=false,
                    isJgInstruction=false,isZInstruction=false,isIncInstruction=false,isDecInstruction=false;
            for(int i=0;i<bSize-3;i++){
                if(*(retV+i)=='n' && *(retV+i+1)=='o' && *(retV+i+2)=='t'){
                    isNotInstruction=true;
                    break;
                }
                else if(*(retV+i)=='j' && *(retV+i+1)=='m' && *(retV+i+2)=='p'){
                    isJmpInstruction=true;
                    break;
                }
                else if(*(retV+i)=='j' && *(retV+i+1)=='e'){
                    isJeInstruction=true;
                    break;
                }
                else if(*(retV+i)=='j' && *(retV+i+1)=='l'){
                    isJlInstruction=true;
                    break;
                }
                else if(*(retV+i)=='j' && *(retV+i+1)=='g'){
                    isJgInstruction=true;
                    break;
                }
                else if(*(retV+i)=='z'){
                    isZInstruction=true;
                    break;
                }
                else if(*(retV+i)=='i' && *(retV+i+1)=='n' && *(retV+i+2)=='c'){
                    isIncInstruction=true;
                    break;
                }
                else if(*(retV+i)=='d' && *(retV+i+1)=='e' && *(retV+i+2)=='c'){
                    isDecInstruction=true;
                    break;
                }
            }
            if(isNotInstruction){
                for(int i=3;i<bSize;i++){
                    if(*(retV+i)=='a'){
                        //check for not ax
                        out.push_back(0x69);
                        break;
                    }
                    else if(*(retV+i)=='b'){
                        //check for not bx
                        out.push_back(0x6a);
                        break;
                    }
                    else if(*(retV+i)=='c'){
                        //check for not cx
                        out.push_back(0x6b);
                        break;
                    }
                    else if(*(retV+i)=='d'){
                        //check for not dx
                        out.push_back(0x6c);
                        break;
                    }
                }
            }
            else if(isJmpInstruction){
                for(int i=3;i<bSize;i++){
                    if(*(retV+i)!=32 && *(retV+i)!=9){
                        if(*(retV+i)=='[' && *(retV+i+1)=='a'){
                            out.push_back(0x5e);
                            break;
                        }
                        else{
                            bool success;
                            WORD value=convertCharP(retV+i,&success);
                            if(success){
                                out.push_back(0x5d);
                                out.push_back(value);
                                break;
                            }
                            else{
                                printf("Error on line: %u\n%s\n",lineNumber,retV);
                                error=true;
                                break;
                            }
                        }
                    }
                }
            }
            else if(isJeInstruction){
                for(int i=2;i<bSize;i++){
                    if(*(retV+i)!=32 && *(retV+i)!=9){
                        bool success;
                        WORD value=convertCharP(retV+i,&success);
                        if(success){
                            out.push_back(0x5f);
                            out.push_back(value);
                            break;
                        }
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
            }
            else if(isJlInstruction){
                for(int i=2;i<bSize;i++){
                    if(*(retV+i)!=32 && *(retV+i)!=9){
                        bool success;
                        WORD value=convertCharP(retV+i,&success);
                        if(success){
                            out.push_back(0x60);
                            out.push_back(value);
                            break;
                        }
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
            }
            else if(isJgInstruction){
                for(int i=2;i<bSize;i++){
                    if(*(retV+i)!=32 && *(retV+i)!=9){
                        bool success;
                        WORD value=convertCharP(retV+i,&success);
                        if(success){
                            out.push_back(0x61);
                            out.push_back(value);
                            break;
                        }
                        else{
                            printf("Error on line: %u\n%s\n",lineNumber,retV);
                            error=true;
                            break;
                        }
                    }
                }
            }
            else if(isZInstruction){
                for(int i=0;i<bSize;i++){
                    if(*(retV+i)=='a'){
                        out.push_back(0x90);
                        break;
                    }
                    else if(*(retV+i)=='b'){
                        out.push_back(0x91);
                        break;
                    }
                    else if(*(retV+i)=='c'){
                        out.push_back(0x92);
                        break;
                    }
                    else if(*(retV+i)=='d'){
                        out.push_back(0x93);
                        break;
                    }
                }
            }
            else if(isIncInstruction){
                for(int i=3;i<bSize;i++){
                    if(*(retV+i)!=32 && *(retV+i)!=9){
                        if(*(retV+i)=='a'){
                            out.push_back(0x94);
                            break;
                        }
                        else if(*(retV+i)=='b'){
                            out.push_back(0x95);
                            break;
                        }
                        else if(*(retV+i)=='c'){
                            out.push_back(0x96);
                            break;
                        }
                        else if(*(retV+i)=='d'){
                            out.push_back(0x97);
                            break;
                        }
                    }
                }
            }
            else if(isDecInstruction){
                for(int i=3;i<bSize;i++){
                    if(*(retV+i)!=32 && *(retV+i)!=9){
                        if(*(retV+i)=='a'){
                            out.push_back(0x98);
                            break;
                        }
                        else if(*(retV+i)=='b'){
                            out.push_back(0x99);
                            break;
                        }
                        else if(*(retV+i)=='c'){
                            out.push_back(0x9a);
                            break;
                        }
                        else if(*(retV+i)=='d'){
                            out.push_back(0x9b);
                            break;
                        }
                    }
                }
            }
            else{
                printf("Instruction not currently implimented\n");
                error=true;
                break;
            }
        }
        lineNumber++;
    }

    fclose(asmFile);
    return out;
}
void writeToFile(std::vector<WORD> toFile,char*fileName){
    FILE*out=fopen(fileName,"w");
    fprintf(out,"v2.0 raw\n");
    for(int i=0;i<toFile.size();i++){
        fprintf(out,"%x ",toFile[i]);
    }
    fclose(out);
}
int main(int argc, char **argv){
    //argv[1] is the file name
    //printf("Size of WORD:%i\n",sizeof(WORD));
    std::vector<WORD> toFile=compile1(argv[1]);
    writeToFile(toFile,argv[2]);
    /*
    char*test=(char*)malloc(sizeof(char)*4);
    *test='1';
    *(test+1)='0';
    *(test+2)='1';
    *(test+3)='1';
    //*(test+4)='i';
    bool testB;
    printf("%x\n",convertCharP(test,&testB));
    free(test);
    */
    return 0;
}