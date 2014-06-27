#define BUFSIZE 4068
#define LIST_BUFSIZE 4068
#define INFO 1
#define ERROR 42
#define SORRY 43
#define SEND_ERROR 45

const char *sys_label = "Linux";
const char *server_version = "SpeeDL Server 1.1a";

// Mimetypes
struct {
	char *ext;
	char *filetype;
} extensions [] = {
	{"gif", "image/gif"	},  
	{"jpg", "image/jpeg"}, 
	{"jpeg","image/jpeg"},
	{"png", "image/png"	},  
	{"zip", "image/zip"	},  
	{"gz",  "image/gz" 	},  
	{"tar", "image/tar"	},  
	{"htm", "text/html"	},  
	{"html","text/html"	},  
	{"php", "image/php"	},  
	{"cgi", "text/cgi" 	},  
	{"asp","text/asp"  	},  
	{"jsp", "image/jsp"	},  
	{"xml", "text/xml" 	},  
	{"js", "text/js"   	},
	{"css", "text/css" 	}, 
	{"c", "text/plain" 	},
	{"h", "text/plain" 	},
	{"txt", "text/plain"},
	{"sh", "text/plain"	},
	{"ogg", "audio/ogg"	},
	{"mp3", "audio/mpeg"},
	{"flv", "video/x-flv"	},
	{"mp4", "video/mp4"	},
	{"webm", "video/webm"	},
	{0,0} 
};

int file_exists(char *fname) {
	struct stat   buffer;   
	return (stat (fname, &buffer) == 0);
}

int ends_with_slash(char *str) {
	return (str && *str && str[strlen(str) - 1] == '/') ? 0 : 1;
}

int is_dir(char* p)
{
	char * stripslash;
	struct stat st;
	stripslash = p + 1; // strip the first forward 'slash' from the string
	if (stat(stripslash, &st) == 0 && (st.st_mode & S_IFDIR)) {
		return 1;
	}
	else if (stat(stripslash, &st) == 0 && (st.st_mode & S_IFREG)) {
		return 2;
	}
	return 0;
}
