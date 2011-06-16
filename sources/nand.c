#include <ogc/isfs.h>
#include <ogc/gu.h>
#include <stdio.h>
#include <gcutil.h>
#include <string.h>
#include "nand.h"
#include "debug.h"
bool existNandFile(const char *strFileName) {
bool varout=true;
int fd;
    if ((fd=ISFS_Open(strFileName,ISFS_OPEN_READ))<0) {
        varout=false;
    }
    else {
        ISFS_Close(fd);
    }
    return varout;
}
s32 deleteNandFile(const char *strFileName) {
s32 varout=ISFS_OK;
    if (existNandFile(strFileName)) {
        varout=ISFS_Delete(strFileName);
    }
    return varout;
}
s32 createNandFile(const char *strFileName) {
s32 varout=-1;
    if (deleteNandFile(strFileName)==ISFS_OK) {
        varout=ISFS_CreateFile(strFileName,0,3,1,1);
    }
    return varout;
}
s32 writeNandFile(const char *strNandFileName,u8 *chFileContent,u32 intContentSize) {
s32 varout,intWriteBytes;
static u8 chBuffer[1024] ATTRIBUTE_ALIGN(0x20);
int fd;
u32 intReadBytes;
    if ((varout=createNandFile(strNandFileName))==ISFS_OK) {
        if ((fd=ISFS_Open(strNandFileName,ISFS_OPEN_WRITE))<0) {
            varout=fd;
            printDebugMsg(ERROR_DEBUG_MESSAGE,"\nISFS_Open(%s) returned %d",strNandFileName,fd);
        }
        else {
            varout=0;
            while (varout<intContentSize) {
                intReadBytes=MIN(intContentSize-varout,1024);
                memcpy(chBuffer,chFileContent+varout,intReadBytes);
                if ((intWriteBytes=ISFS_Write(fd,chBuffer,intReadBytes))<0) {
                    printDebugMsg(ERROR_DEBUG_MESSAGE,"\nISFS_Write(%d,%p,%d) returned %d at offset %d",fd,chBuffer,intReadBytes,intWriteBytes,varout);
                    varout=intWriteBytes;
                    break;
                }
                else {
                    varout=varout+intWriteBytes;
                }
            }
            ISFS_Close(fd);
        }
    }
    else {
        printDebugMsg(ERROR_DEBUG_MESSAGE,"\ncreateNandFile(%s) returned %d",strNandFileName,varout);
    }
    return varout;
}
s32 createNandDirectory(const char *strFolderName) {
s32 varout;
    if ((varout=ISFS_CreateDir(strFolderName,0,3,1,1))) {
        printDebugMsg(ERROR_DEBUG_MESSAGE,"\nISFS_CreateDir(/tmp/patchmii) returned %d",varout);
    }
    return varout;
}
