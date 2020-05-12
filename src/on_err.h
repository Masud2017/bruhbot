#pragma once 
/******************************************************/
/* defines */
/******************************************************/
#define ERR_BUF_SIZE 4096

#define ERR_OK 0
#define ERR_FILE_NOT_FOUND -1
#define ERR_INVALID_OAUTH_FILE -2
#define ERR_TIMEOUT -3
#define ERR_CONNECTION -4

/******************************************************/
/* functions */
/******************************************************/
void on_err(int condition, char msg[ERR_BUF_SIZE]);
