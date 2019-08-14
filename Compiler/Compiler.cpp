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
    //printf("%s\n",in);
    int length=0;
    while(1){
        if(*(in+length)!=NULL)length++;
        else break;
    }
    WORD out=0;
    bool isHex=false;
    bool isBin=false;
    if(*(in+length-1)=='h'){
        isHex=true;
        length--;
    }
    else if(*(in+length-1)=='i'){//BINARY numbers end in i
        isBin=true;
        length--;
    }
    for(int i=0;i<length;i++){
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
std::vector<WORD> compile1(char*in){//remaster of compile0
    FILE*asmFile=fopen(in,"r");
    std::vector<WORD> out;

    bool error=false;
    char* retV;
    int bSize=255;
    char buffer[bSize];
    while(!error){
        retV=fgets(buffer,bSize,(FILE*)asmFile);
        if(retV==NULL)break;
        bool foundInstruction=false;
        int commaPos=-1;
        for(int i=0;i<bSize;i++){
            if(*(retV+i)==','){
                commaPos=i;
                break;
            }
        }
        if(commaPos!=-1){
            //Check for mov instruction
            bool isMovInstruction=false;
            for(int i=;i<bSize-3;i++){
                if(*(retV+i)=='m' && *(retV+i+1)=='o' && *(retV+i+2)=='v'){
                    isMovInstruction=true;
                    break;
                }
            }
            if(isMovInstruction){
                
            }
        }
        else{

        }
    }

    fclose(asmFile);
    return out;
}
std::vector<WORD> compile0(char*in){
    //Jumps not taken into consideration
    //.word value16     is a macro to place data
    //No labels just yet
    //printf("%s\n",in);
    FILE*asmFile=fopen(in,"r");
    char buff[255];
    char* retV;
    std::vector<WORD> toFile;
    unsigned int lineNumber=1;
    bool error=false;
    while(!error){
        retV=fgets(buff,255,(FILE*)asmFile);
        if(retV==NULL)break;
        bool foundInstruction=false;
        int commaPos=-1;
        for(int i=0;i<255;i++){
            if(buff[i]==','){
                commaPos=i;
                //foundInstruction=true;
                break;
            }
        }
        if(commaPos!=-1){
            //Check for mov instructions
            if(*retV=='m' && *(retV+1)=='o' && *(retV+2)=='v'){
                //Is a mov instruction

                //Check for mov reg,reg
                if(*(retV+commaPos+2)=='x' && *(retV+commaPos-1)=='x'){
                    //Means mov reg,reg
                    if(*(retV+commaPos-2)=='a'){
                        //mov ax,reg
                        if(*(retV+commaPos+1)=='b'){
                            //mov ax,bx
                            toFile.push_back(0x5);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='c'){
                            //mov ax,cx
                            toFile.push_back(0x6);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='d'){
                            //mov ax,dx
                            toFile.push_back(0x7);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    }
                    else if(*(retV+commaPos-2)=='b'){
                        //mov bx,reg
                        if(*(retV+commaPos+1)=='a'){
                            //mov bx,ax
                            toFile.push_back(0x8);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='c'){
                            //mov bx,cx
                            toFile.push_back(0x9);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='d'){
                            //mov bx,dx
                            toFile.push_back(0xa);
                            foundInstruction=true;
                        }
                    }
                    else if(*(retV+commaPos-2)=='c'){
                        //mov cx,reg
                        if(*(retV+commaPos+1)=='a'){
                            //mov cx,ax
                            toFile.push_back(0xb);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='b'){
                            //mov cx,bx
                            toFile.push_back(0xc);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='d'){
                            //mov cx,dx
                            toFile.push_back(0xd);
                            foundInstruction=true;
                        }
                    }
                    else if(*(retV+commaPos-2)=='d'){
                        //mov dx,reg
                        if(*(retV+commaPos+1)=='a'){
                            //mov dx,ax
                            toFile.push_back(0xe);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='b'){
                            //mov dx,bx
                            toFile.push_back(0xf);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='c'){
                            //mov dx,cx
                            toFile.push_back(0x10);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    }
                }
                else if(*(retV+commaPos-1)=='x' && *(retV+commaPos+3)=='x'){//Check for mov reg,[reg]
                    //checks for mov reg,[reg]
                    if(*(retV+commaPos-2)=='a')
                    //checks for mov ax,[reg]
                            if(*(retV+commaPos+2)=='a'){
                                //mov ax,[ax]
                                toFile.push_back(0x11);
                                foundInstruction=true;
                            }
                            else if(*(retV+commaPos+2)=='b'){
                                //mov ax,[bx]
                                toFile.push_back(0x12);
                                foundInstruction=true;
                            }
                            else if(*(retV+commaPos+2)=='c'){
                                //mov ax,[cx]
                                toFile.push_back(0x13);
                                foundInstruction=true;
                            }
                            else if(*(retV+commaPos+2)=='d'){
                                toFile.push_back(0x14);
                                foundInstruction=true;
                            }
                            else{
                                printf("Error on line %u\n",lineNumber);
                                error=true;
                            }
                    else if(*(retV+commaPos-2)=='b')
                        //checks for mov bx,[reg]
                        if(*(retV+commaPos+2)=='a'){
                            //mov bx,[ax]
                            toFile.push_back(0x15);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='b'){
                            //mov bx,[bx]
                            toFile.push_back(0x16);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='c'){
                            //mov bx,[cx]
                            toFile.push_back(0x17);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='d'){
                            //mov bx,[dx]
                            toFile.push_back(0x18);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    else if(*(retV+commaPos-2)=='c')
                        //checks for mov cx,[reg]
                        if(*(retV+commaPos+2)=='a'){
                            //mov cx,[ax]
                            toFile.push_back(0x19);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='b'){
                            //mov cx,[bx]
                            toFile.push_back(0x1a);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='c'){
                            //mov cx,[cx]
                            toFile.push_back(0x1b);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='d'){
                            //mov cx,[dx]
                            toFile.push_back(0x1c);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    else if(*(retV+commaPos-2)=='d')
                        //checks for mov dx,[reg]
                        if(*(retV+commaPos+2)=='a'){
                            //mov dx,[ax]
                            toFile.push_back(0x1d);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='b'){
                            //mov dx,[bx]
                            toFile.push_back(0x1e);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='c'){
                            //mov dx,[cx]
                            toFile.push_back(0x1f);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+2)=='d'){
                            //mov dx,[dx]
                            toFile.push_back(0x20);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                }
                else if(*(retV+commaPos-2)=='x' && *(retV+commaPos+2)=='x'){//Check for mov [reg],reg
                    if(*(retV+commaPos-3)=='a')
                        //check for mov [ax],reg
                        if(*(retV+commaPos+1)=='a'){
                            //check for mov [ax],ax
                            toFile.push_back(0x21);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='b'){
                            //check for mov [ax],bx
                            toFile.push_back(0x22);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='c'){
                            //check for mov [ax],cx
                            toFile.push_back(0x23);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='d'){
                            //check for mov [ax],dx
                            toFile.push_back(0x24);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    else if(*(retV+commaPos-3)=='b')
                        //check for mov [bx],reg
                        if(*(retV+commaPos+1)=='a'){
                            //check for mov [bx],ax
                            toFile.push_back(0x25);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='b'){
                            //check for mov [bx],bx
                            toFile.push_back(0x26);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='c'){
                            //check for mov [bx],cx
                            toFile.push_back(0x27);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='d'){
                            //check for mov [bx],dx
                            toFile.push_back(0x28);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    else if(*(retV+commaPos-3)=='c')
                        //check for mov [cx],reg
                        if(*(retV+commaPos+1)=='a'){
                            //check for mov [cx],ax
                            toFile.push_back(0x29);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='b'){
                            //check for mov [cx],bx
                            toFile.push_back(0x2a);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='c'){
                            //check for mov [cx],cx
                            toFile.push_back(0x2b);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='d'){
                            //check for mov [cx],dx
                            toFile.push_back(0x2c);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    else if(*(retV+commaPos-3)=='d')
                        //check for mov [dx],reg
                        if(*(retV+commaPos+1)=='a'){
                            //check for mov [dx],ax
                            toFile.push_back(0x2d);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='b'){
                            //check for mov [dx],bx
                            toFile.push_back(0x2e);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='c'){
                            //check for mov [dx],cx
                            toFile.push_back(0x2f);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos+1)=='d'){
                            //check for mov [dx],dx
                            toFile.push_back(0x30);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                }
                //check for mov reg,value16
                else if(*(retV+commaPos-1)=='x'){

                }
            }

            //check for add Instruction
            else if(*retV=='a' && *(retV+1)=='d' && *(retV+2)=='d'){
                //check for add ax,reg
                if(*(retV+commaPos-2)=='a')
                    //check for add ax,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x31);
                        foundInstruction=true;
                    }
                    //check for add ax,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x32);
                        foundInstruction=true;
                    }
                    //check for add ax,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x33);
                        foundInstruction=true;
                    }
                    //check for add ax,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x34);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
                //check for add bx,reg
                else if(*(retV+commaPos-2)=='b')
                    //check for add bx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x35);
                        foundInstruction=true;
                    }
                    //check for add bx,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x36);
                        foundInstruction=true;
                    }
                    //check for add bx,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x37);
                        foundInstruction=true;
                    }
                    //check for add bx,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x38);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
                //check for add cx,reg
                else if(*(retV+commaPos-2)=='c')
                    //check for add cx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x39);
                        foundInstruction=true;
                    }
                    //check for add cx,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x3a);
                        foundInstruction=true;
                    }
                    //check for add cx,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x3b);
                        foundInstruction=true;
                    }
                    //check for add cx,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x3c);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
                //check for add dx,reg
                else if(*(retV+commaPos-2)=='d')
                    //check for add dx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x3d);
                        foundInstruction=true;
                    }
                    //check for add dx,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x3e);
                        foundInstruction=true;
                    }
                    //check for add dx,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x3f);
                        foundInstruction=true;
                    }
                    //cehck for add dx,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x40);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
            }

            //check for sub instructions
            else if(*retV=='s' && *(retV+1)=='u' && *(retV+2)=='b'){
                //cehck for sub ax,reg
                if(*(retV+commaPos-2)=='a')
                    //cehck for sub ax,bx
                    if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x41);
                        foundInstruction=true;
                    }
                    //check for sub ax,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x42);
                        foundInstruction=true;
                    }
                    //check for sub ax,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x43);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
                //check for sub bx,reg
                else if(*(retV+commaPos-2)=='b')
                    //cehck for sub bx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x44);
                        foundInstruction=true;
                    }
                    //check for sub bx,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x45);
                        foundInstruction=true;
                    }
                    //check for sub bx,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x46);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
                //check for sub cx,reg
                else if(*(retV+commaPos-2)=='c')
                    //check for sub cx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x47);
                        foundInstruction=true;
                    }
                    //check for sub cx,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x48);
                        foundInstruction=true;
                    }
                    //check for sub cx,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x49);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
                //check for sub dx,reg
                else if(*(retV+commaPos-2)=='d')
                    //check for sub dx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x4a);
                        foundInstruction=true;
                    }
                    //check for sub dx,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x4b);
                        foundInstruction=true;
                    }
                    //check for sub dx,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x4c);
                        foundInstruction=true;
                    }
                    else{
                        printf("Error on line %u\n",lineNumber);
                        error=true;
                    }
            }

            //check for cmp instructions
            else if(*retV=='c' && *(retV+1)=='m' && *(retV+2)=='p'){
                //check for cmp ax,reg
                if(*(retV+commaPos-2)=='a')
                    //check for cmp ax,bx
                    if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x4d);
                        foundInstruction=true;
                    }
                    //check for cmp ax,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x4e);
                        foundInstruction=true;
                    }
                    //check for cmp ax,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x4f);
                        foundInstruction=true;
                    }
                    else{
                        //check for cmp ax,value16
                        bool success;
                        WORD value=convertCharP(retV+commaPos+1,&success);
                        if(success){
                            toFile.push_back(0x59);
                            toFile.push_back(value);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    }
                //check for cmp bx,reg
                else if(*(retV+commaPos-2)=='b')
                    //check for cmp bx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x50);
                        foundInstruction=true;
                    }
                    //check for cmp bx,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x51);
                        foundInstruction=true;
                    }
                    //check for cmp bx,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x52);
                        foundInstruction=true;
                    }
                    else{
                        //check for cmp bx,value16
                        bool success;
                        WORD value=convertCharP(retV+commaPos+1,&success);
                        if(success){
                            toFile.push_back(0x5a);
                            toFile.push_back(value);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    }
                //check for cmp cx,reg
                else if(*(retV+commaPos-2)=='c')
                    //check for cmp cx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x53);
                        foundInstruction=true;
                    }
                    //check for cmp cx,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x54);
                        foundInstruction=true;
                    }
                    //check for cmp cx,dx
                    else if(*(retV+commaPos+1)=='d'){
                        toFile.push_back(0x55);
                        foundInstruction=true;
                    }
                    else{
                        //check for cmp cx,value16
                        bool success;
                        WORD value=convertCharP(retV+commaPos+1,&success);
                        if(success){
                            toFile.push_back(0x5b);
                            toFile.push_back(value);
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    }
                //check for cmp dx,reg
                else if(*(retV+commaPos-2)=='d')
                    //check for cmp dx,ax
                    if(*(retV+commaPos+1)=='a'){
                        toFile.push_back(0x56);
                        foundInstruction=true;
                    }
                    //check for cmp dx,bx
                    else if(*(retV+commaPos+1)=='b'){
                        toFile.push_back(0x57);
                        foundInstruction=true;
                    }
                    //check for cmp dx,cx
                    else if(*(retV+commaPos+1)=='c'){
                        toFile.push_back(0x58);
                        foundInstruction=true;
                    }
                    else{
                        //check for cmp dx,value16
                        bool success;
                        WORD value=convertCharP(retV+commaPos+1,&success);
                        if(success){
                            toFile.push_back(0x5c);
                            toFile.push_back(value);
                            foundInstruction=true;
                        }
                        else{
                            printf("Error on line %u\n",lineNumber);
                            error=true;
                        }
                    }
            }
        }
        if(!foundInstruction){
            //check for jmp address16
            if(*retV=='j' && *(retV+1)=='m' && *(retV+2)=='p'){
                int addr=-1,regJump=-1;
                for(int i=0;i<252;i++){
                    if(*(retV+3+i)!=32 && *(retV+3+i)!=9){
                        addr=i+3;
                        break;
                    }
                    else if(*(retV+3+i)=='['){
                        regJump=i+3;
                        break;
                    }
                }
                if(addr!=-1){
                    bool success;
                    WORD value=convertCharP(retV+addr,&success);
                    if(success){
                        toFile.push_back(0x5d);
                        toFile.push_back(value);
                        foundInstruction=true;
                    }
                }
                else if(regJump!=-1){
                    toFile.push_back(0x5e);
                    foundInstruction=true;
                }
            }
            //check for je
            else if(*retV=='j' && *(retV+1)=='e'){
                
            }
            //check for jl
            else if(*retV=='j' && *(retV+1)=='l'){
                
            }
            //check for jg
            else if(*retV=='j' && *(retV+1)=='g'){
                
            }
            else if(*retV=='.'){
                if(*(retV+1)=='w' &&
                *(retV+2)=='o' &&
                *(retV+3)=='r' &&
                *(retV+4)=='d'){
                    bool success;
                    WORD value=convertCharP(retV+6,&success);
                    if(success){
                        toFile.push_back(value);
                    }
                }
            }
            else{
                printf("Error on line %u\n",lineNumber);
                error=true;
            }
            
        }
        lineNumber++;
    }
    fclose(asmFile);
    return toFile;
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
    std::vector<WORD> toFile=compile0(argv[1]);
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