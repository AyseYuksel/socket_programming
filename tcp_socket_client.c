#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

#define NSTRS 3 //number of strings
#define ADDRESS "mysocket" //address to connect

//strings we send to the server
char *strs[NSTRS]={
"This is the first string from the client \n",
"This is the second string from the clien \n",
"This is the third string from the client"
};

main(){

char c;
FILE *fp;
register int i,s,len;
struct sockaddr_un saun;

//creating TCP socket with socket() method
if((s=socket(AF_UNIX,SOCK_STREAM,0))<0){
perror("client : socket");
exit(1);
}

//create the address we will be connecting to
saun.sun_family=AF_UNIX;
strcpy(saun.sun_path,ADDRESS);

//try to connect to the address. for this to succeed,the server must already 
//have bound this address, and must have issued a listen() request


//the third argument indicates the length of the structure,not just the length
//of the socket name
len=sizeof(saun.sun_family)+strlen(saun.sun_path);

if(connect(s,&saun,len)<0){
perror("client : connect");
}

//after than connect() method,we'll use stdio for reading and writing the socket

fp=fdopen(s,"r");

//read some strings from the server and print them out.
for(i=0;i<NSTRS;i++){
while((c=fgetc(fp))!=EOF){
putchar(c);
if(c=='\n')
	break;
        }
}

//send some strings to the server
for(i=0;i<NSTRS;i++){
send(s,strs[i],strlen(strs[i]));
}

close(s);
exit(0);
}
