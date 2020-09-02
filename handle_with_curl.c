

#include "gfserver.h"
#include "proxy-student.h"

#define BUFSIZE (1219)

/*
 * Replace with an implementation of handle_with_curl and any other
 * functions you may need.
 */

struct DataStruct
{
    char* data;
    size_t size;
};

size_t write_callback(char* contents, size_t size, size_t nmemb, void *userdata)
{
//    printf("entering callback with chunk of size %zu\n",(size*nmemb));
//    printf("entering the write_callback with ptr %s, size %zu, nmemb %zu\n",contents,size,nmemb);
    
    size_t totalSize = size * nmemb;
    struct DataStruct * collection = (struct DataStruct*) userdata;
    char* ptr = realloc(collection->data, collection->size+totalSize+1); // expanding the previous data collection.
    if (ptr == NULL)
    {
        printf("ran out of memory\n");
        return 0;
    }
    collection->data = ptr;
    memcpy(&(collection->data[collection->size]), contents, totalSize);
    collection->size += totalSize;
    collection->data[collection->size] = 0; //null character?
    
    return totalSize;
}

ssize_t handle_with_curl(gfcontext_t *ctx, const char *path, void* arg){
	(void) ctx;
	(void) path;
	(void) arg;
    
    struct DataStruct dataElement;
    
    dataElement.data = malloc(1);
    dataElement.size = 0;

	/* not implemented */
	errno = ENOSYS;
//    path = "/unknown.file";
    int N = strlen(path)+strlen(arg);
    char url[N];
    strcpy(url, arg);
    strcat(url,path);
    
	printf("handle with curl on %s\n",path);
    
    printf("received arg was %s\n",(char*)arg);
    printf("total URL is: %s \n",url);
    curl_global_init(CURL_GLOBAL_ALL);
    size_t fileSize = -1;
    CURL * curlHandle;
    CURLcode webResult;
    curlHandle = curl_easy_init();
    curl_easy_setopt(curlHandle, CURLOPT_URL,url);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION,write_callback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA,(void*) &dataElement);
    webResult = curl_easy_perform(curlHandle);
    
    if (webResult != CURLE_OK)
    {
        printf("couldn't perform the curl operation\n");
    }
    else{
//        have the data
//        http response code
        long httpCode;
        curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE,&httpCode);
        printf("received the http code %ld \n",httpCode);
        
        printf("received data of size %zu\n",dataElement.size);
//        printf("received data is %s\n\n",dataElement.data);
        if(httpCode == 403 || httpCode == 404)
        {
            gfs_sendheader(ctx, GF_FILE_NOT_FOUND, 0);
        }
        else{
        
        fileSize = dataElement.size;
        size_t sentTill = 0;
        gfs_sendheader(ctx, GF_OK, fileSize);
        
            printf("file size is %zu\n",fileSize);
        
        while(sentTill < fileSize)
        {
            size_t sentNow = gfs_send(ctx, &(dataElement.data[sentTill]), fileSize-sentTill);
            sentTill += sentNow;
        }
            printf("total sent %zu\n",sentTill);
        }
    }
    
    curl_easy_cleanup(curlHandle);
    
    free(dataElement.data);
    curl_global_cleanup();
    
	return fileSize;
}


/*
 * We provide a dummy version of handle_with_file that invokes handle_with_curl
 * as a convenience for linking.  We recommend you simply modify the proxy to
 * call handle_with_curl directly.
 */
ssize_t handle_with_file(gfcontext_t *ctx, const char *path, void* arg){

	printf("handle with file inside curl file on path %s\n",path);
	return handle_with_curl(ctx, path, arg);
}	
