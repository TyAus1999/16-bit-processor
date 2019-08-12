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
                    }
                }
                else if(*(retV+commaPos-1)=='x' && *(retV+commaPos+3)=='x'){
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
                }
                //check for mov reg,value16
                if(!foundInstruction){
                    bool success=true;
                    WORD value;
                    value=convertCharP(retV+commaPos+1,&success);
                    if(success)
                        if(*(retV+commaPos-2)=='a'){
                            //mov ax,value16
                            toFile.push_back(0x1);
                            toFile.push_back(value);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos-2)=='b'){
                            //mov bx,value16
                            toFile.push_back(0x2);
                            toFile.push_back(value);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos-2)=='c'){
                            //mov cx,value16
                            toFile.push_back(0x3);
                            toFile.push_back(value);
                            foundInstruction=true;
                        }
                        else if(*(retV+commaPos-2)=='d'){
                            //mov dx,value16
                            toFile.push_back(0x4);
                            toFile.push_back(value);
                            foundInstruction=true;
                        }
                    
                }
            }
        }
        if(!foundInstruction){
            if(*retV=='.'){
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