#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

#define NSTRS 3 //number of strings that the server will send
#define ADDRESS "mysocket" //address to connect  

//string that the server will send to the client
char *strs[NSTRS]={
"This is the first string from the server \n",
"This is the second string from the server \n",
"This is the second string from the server \n","This is the third string from the server \n"
};

main(){

char c;
FILE *fp;
int fromlen;
register int i,s,ns,len;
struct sockaddr_un saun,fsaun;


//socket creation with socket()
//socket() parameters => domain,type(UDP or TCP),socket protocol
//SOCK_STREAM allows diagrams to be transferred via tcp
//SOCK_DGRAM allows diagrams to be transferred via udp 
if((s=socket(AF_UNIX,SOCK_STREAM,0))<0){
perror("server : socket");
exit(1);
}

//create the address we will be binding to
saun.sun_family=AF_UNIX;
strcpy(saun.sun_path,ADDRESS);

//try to bind the address to the socket.We unlink the name first so that the
//bind won't fail.

unlink(ADDRESS);

//the third argument indicates the "length" of the structure, not just the
//length of the socket name

len=sizeof(saun.sun_family)+strlen(saun.sun_path);

//bind parameters = > socket,address,length
if(bind(s,&saun,len)<0){
perror("server : bind");
exit(1);
}

//listen on the socket  
//this part isn't available in udp socket (SOCK_DGRAM)
//the purpose is to control connection requests to the server. 

if(listen(s,5)<0){
//server can  receive 5 connection requests 
perror("servor : listen");
exit(1);
}

//accepts connections.when we accept one, ns will be connected to the client.
//fsaun will contain the address of the client.
//accept the connections use with accept() method. 
//accept() parameters=>socket,client address,length 
if((ns=accept(s,&fsaun,&fromlen))<0){
perror("server : accept");
exit(1);
}

//at this stage,the socket is created.
//socket will used for read and write operation now.

//stdio use for reading the socket
//fdopen parameters=>accepted connections variable,read(r) or write(w) 
fp=fdopen(ns,"r");

//first we send some strings to the client

for(i=0;i<NTRS;i++){
while((c=fgetc(fp))!=EOF){
//with the send() method, sending(write) is done in the socket
//send() parameters=>socket connection variable,message,message size	
send(ns,strs[i],strlen(strs[i]),0);
}

//read some strings from the client and print them out
for(i=0;i<NSTRS;i++){
while((c=fgetc(fp))!=EOF){
putchar(c);
if(c=='\n'){
break;
           }
      }
}

//we can simply use close() to terminate the connection,since we're done with 
//both sides.

close(s);

exit(0);

}

}
