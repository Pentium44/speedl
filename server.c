// CTHTTPD - Simple Web Server - GPLv2
// Chris Dorman, 2012-2013 (cddo@riseup.net)
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "server_config.h"

void write_error(int num)
{	
	int fd;
	char logbuffer[BUFSIZE*2];
 
	(void)sprintf(logbuffer,"HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><head><title>Fail</title></head><body><h2>Fail</h2></body></html>\r\n"); 
	(void)write(num,logbuffer,strlen(logbuffer));
	
	exit(3);
}

void write_log(int type, char *s1, char *s2, int num)
{
	// logs the local time of the event 
	time_t now = time(NULL);
	struct tm* tm_info;
	char timebuf[30];
	memset(timebuf, 0, strlen(timebuf));
	tm_info = localtime(&now);
	strftime(timebuf, sizeof(timebuf), "%Y/%m/%d, %H:%M:%S", tm_info);
	
	int fd ;
	char logbuffer[BUFSIZE*2];
	
	if((fd = open("server.log", O_CREAT| O_WRONLY | O_APPEND,0644)) >= 0) {
		switch(type) {
			case INFO:
				(void)sprintf(logbuffer,"%s (%s) [%s]: %s %s",server_version, sys_label, timebuf, s1, s2);
				(void)write(fd,logbuffer,strlen(logbuffer)); 
				(void)write(fd,"\n",1);      
				break;
			case ERROR: 
				(void)sprintf(logbuffer,"%s (%s) [%s]: %s %s",server_version, sys_label, timebuf, s1, s2);
				(void)write(fd,logbuffer,strlen(logbuffer)); 
				(void)write(fd,"\n",1);     
				break;
			case SORRY: 
				(void)sprintf(logbuffer,"HTTP/1.0 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n<html><head><title>Fail</title></head><body><h2>Fail</h2></body></html>\r\n");
				(void)write(num,logbuffer,strlen(logbuffer));
				(void)sprintf(logbuffer,"%s (%s) [%s]: %s %s",server_version, sys_label, timebuf, s1, s2);
				(void)write(fd,logbuffer,strlen(logbuffer)); 
				(void)write(fd,"\n",1);     
				break;
			case SEND_ERROR: (void)sprintf(logbuffer,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<html><head><title>CHTTPD: Found error</title></head><body><h2>Index error</h2>%s</body></html>\r\n", s1);
				(void)write(num,logbuffer,strlen(logbuffer));
				break;
		}
		(void)close(fd);
	}
	
	if(type == ERROR || type == SORRY || type == SEND_ERROR) exit(3);
}

void web(int fd, int hit)
{
	int j, file_fd, buflen, len, contentfs;
	long i, filesize;
	char * fstr;
	static char buffer[BUFSIZE+1];

	// Check to see if file is corrupted
	filesize =read(fd,buffer,BUFSIZE); 
	if(filesize == 0 || filesize == -1) {
		write_log(SORRY,"failed to read browser request","",fd);
	}
	
	if(filesize > 0 && filesize < BUFSIZE)	
		buffer[filesize]=0;	
	else buffer[0]=0;

	for(i=0;i<filesize;i++)	
		if(buffer[i] == '\r' || buffer[i] == '\n')
			buffer[i]='*';
	write_log(INFO,"request",buffer,hit);

	if( strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4) )
		write_log(SORRY,"Only simple GET operation supported",buffer,fd);

	for(i=4;i<BUFSIZE;i++) { 
		if(buffer[i] == ' ') { 
			buffer[i] = 0;
			break;
		}
	}

	for(j=0;j<i-1;j++) 	
		if(buffer[j] == '.' && buffer[j+1] == '.')
			write_log(SORRY,"Parent directory (..) path names not supported",buffer,fd);
			
	if( !strncmp(&buffer[0],"GET /\0",6) || !strncmp(&buffer[0],"get /\0",6) ) {
		write_log(SORRY,"No file to get","",fd);
	}
	
	buflen=strlen(buffer);
	fstr = (char *)0;
	for(i=0;extensions[i].ext != 0;i++) {
		len = strlen(extensions[i].ext);
		if( !strncmp(&buffer[buflen-len], extensions[i].ext, len)) {
			fstr =extensions[i].filetype;
			break;
		}
	}
	
	//if(fstr == 0) write_log(SORRY,"file extension type not supported",buffer,fd);

	if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) 
		write_log(SORRY, "failed to open file",&buffer[5],fd);

	write_log(INFO,"Sending file",&buffer[5],fd);

	struct stat filesz;
	stat(&buffer[5], &filesz);
	contentfs = filesz.st_size;

	(void)sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", fstr, contentfs);
	(void)write(fd,buffer,strlen(buffer));

	while (	(filesize = read(file_fd, buffer, BUFSIZE)) > 0 ) {
		(void)write(fd,buffer,filesize);
	}
#ifdef LINUX
	sleep(1);
#endif
	exit(1);
}


int main(int argc, char **argv)
{
	int i, port, pid, listenfd, socketfd, hit;
	socklen_t length;
	static struct sockaddr_in cli_addr; 
	static struct sockaddr_in serv_addr;

	if( argc < 3  || argc > 3 || !strcmp(argv[1], "-?") ) {
		(void)printf("Usage: speedl-server <port> <server directory>\n"
    "Example: speedl-server 2750 files/\n");
		exit(0);
	}
	if(chdir(argv[2]) == -1){ 
		(void)printf("ERROR: Can't Change to directory %s\n",argv[2]);
		exit(4);
	}

	if(fork() != 0)
		return 0; 
	(void)signal(SIGCLD, SIG_IGN); 
	(void)signal(SIGHUP, SIG_IGN); 
	for(i=0;i<32;i++)
		(void)close(i);	
	(void)setpgrp();	

	write_log(INFO,"CHTTPD server starting",argv[1],getpid());

	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0)
		write_log(ERROR, "system call","socket",0);
	port = atoi(argv[1]);
	if(port < 0 || port >60000)
		write_log(ERROR,"Invalid port number try [1,60000], tried starting on ",argv[1],0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0)
		write_log(ERROR,"Failed to ","bind",0);
	if( listen(listenfd,64) <0)
		write_log(ERROR,"Failed to","listen",0);

	for(hit=1; ;hit++) {
		length = sizeof(cli_addr);
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0)
			write_log(ERROR,"Failed to","accept",0);

		if((pid = fork()) < 0) {
			write_log(ERROR,"Failed to","fork",0);
		}
		else {
			if(pid == 0) {
				(void)close(listenfd);
				web(socketfd,hit);
			} else {
				(void)close(socketfd);
			}
		}
	}
}
