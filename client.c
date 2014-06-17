/*
SpeeDL client
Chris Dorman, 2014 - GPLv3
Todo:
    Add file list retrieving
    refreshing download speeds.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "client_config.h"

char* merge(char *server, char *port, char *filename)
{
	char *port_separator = ":"; 
	char *http_string = "http://"; 
	char *forward_slash = "/";
	
    char *result = malloc(strlen(http_string)+strlen(server)+strlen(port_separator)+strlen(port)+strlen(forward_slash)+strlen(filename)+1);
    strcpy(result, http_string);
    strcat(result, server);
    strcat(result, port_separator);
    strcat(result, port);
    strcat(result, forward_slash);
    strcat(result, filename);
    file = result;
    server_dest = server;
}

void file_get()
{
	char* file_name = file;
	file_name = strrchr(file, '/' ) + 1;
	
	// notify user that file is starting
	printf("%s is downloading from '%s'...\n", file_name, server_dest);
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, file);
    FILE* file = fopen(file_name, "w");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    
    CURLcode get_method = curl_easy_perform(curl);
    
    if(CURLE_HTTP_RETURNED_ERROR == get_method) {
    	fprintf(stderr, "Error '%s' not found on %s.\n", file_name, server_dest);
    	remove(file_name);
    } else if(CURLE_OK == get_method) {
        double val;
 
        /* check for bytes downloaded */ 
        get_method = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &val);
        if((CURLE_OK == get_method) && (val>0))
            printf("SpeeDL: [%0.0f KB] '%s' downloaded ",val / 1024, file_name);

        get_method = curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &val);
        if((CURLE_OK == get_method) && (val>0))
            printf("at %0.1f Kbyte(s)/sec.\n", val / 1024);
 
    } else {
        fprintf(stderr, "Error while fetching '%s': %s\n",
        file_name, curl_easy_strerror(get_method));
    }
	
    curl_easy_cleanup(curl);
    fclose(file);
}

int main(int argc, char **argv)
{

	char* address;
	char* port_value = "2750";
	char* filename;
	if(argc < 3 || argc > 4) {
		printf("Required arguments: < argument >\n"
		"Optional arguments: [ argument ]\n"
		"SpeeDL: bad command, Usage: speedl <server> [port] <file name>\n");
        return 1;
	}

	address = argv[1];
	if (argc == 3) {
		filename = argv[2];
	}else{
		port_value = argv[2];
		filename = argv[3];
	}

	merge(address,port_value,filename);
	file_get();
	return 0;

}
