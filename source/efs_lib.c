/*

  Embedded File System (EFS)
  --------------------------

  file        : efs_lib.c 
  author      : Alekmaul & Noda
  description : File system functions

  history : 

    12/05/2007 - v1.0
      = original release
      
    13/05/2007 - v1.1
      = corrected problems with nds files with loader
      = corrected problems with nds files made with standard libnds makefile
      + optimised variables, now use 505 bytes less in RAM
      + added EFS_Flush() function, to ensure data is written
      + included ASM code for reserved space

    14/05/2007 - v1.1a
      = corrected bug with EFS_fopen() when filename does not begin with "/"
      + added defines for c++ compatibility
      
    28/09/2007 - v1.2
      = corrected a major bug with EFS_fread and EFS_fwrite
      = moved some functions tweaks to fix real fat mode
      + added autoflush on file close by default
      + added extension checking when searching the NDS file (improve speed)
      
    12/01/2008 - v1.3
      = corrected EFS_GetFileSize() function when using real fat mode
      
    18/05/2008 - v2.0
      + complete rewrite of the lib (breaks compatibility with old versions!)
      + added devoptab integration, so it now use standard iolib functions
      + added automatic GBA rom detection (so it works in emu without any 
        modifications), based on Eris' NitroFS driver idea

    TODO:
      + maybe get file size info in DirNext?
      + 2 file handles for reading and searching?
      
*/

#include <nds.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/dir.h>
#include <sys/iosupport.h>
#include <fat.h>

#include "efs_lib.h"

#ifdef __cplusplus
extern "C" {
#endif 

// defines
#define EFS_READBUFFERSIZE      4096
#define EFS_SEARCHFILE          0
#define EFS_SEARCHDIR           1
#define EFS_LISTDIR             2
#define EFS_GBALOADERSTR        "PASSDF"
#define EFS_STDLOADERSTR        "PASS01"
#define EFS_LOADERSTROFFSET     172
#define EFS_LOADEROFFSET        512
#define EFS_FNTOFFSET           64
#define EFS_FATOFFSET           72
#define EFS_NITROROOT           0xF000
#define EFS_ISFILE              0
#define EFS_ISDIR               1
#define EFS_PREFIX              "efs:"

// internal file/dir struct
typedef struct _EFS_FileStruct {
    u32 start;
    u32 pos;
    u32 end;
} EFS_FileStruct;

typedef struct _EFS_DirStruct {
    u32 dir_id;
    u32 pos;
} EFS_DirStruct;

// mix up the magic string to avoid doubling it in the NDS
const char efsMagicStringP2[] = " EFSstr";
const char efsMagicStringP1[] = "\xCE\x05\xA9\xBF";

// export variables
extern int efs_id;
extern u32 efs_filesize;

// globals
FILE *nds_file;
u32 fnt_offset;
u32 fat_offset;
bool hasLoader;
bool useDLDI;

char fileInNDS[EFS_MAXPATHLEN];
bool filematch;
u8 searchmode;
u32 file_idpos;
u32 file_idsize;

devoptab_t EFSdevoptab = {
    "efs",                      // const char *name;
    sizeof(EFS_FileStruct),     // int structSize;
    &EFS_Open,                  // int (*open_r)(struct _reent *r, void *fileStruct, const char *path, int flags, int mode);
    &EFS_Close,                 // int (*close_r)(struct _reent *r, int fd);
    &EFS_Write,                 // int (*write_r)(struct _reent *r, int fd, const char *ptr, int len);
    &EFS_Read,                  // int (*read_r)(struct _reent *r, int fd,char *ptr, int len);
    &EFS_Seek,                  // int (*seek_r)(struct _reent *r, int fd, int pos, int dir);
    &EFS_Fstat,                 // int (*fstat_r)(struct _reent *r, int fd, struct stat *st);
    NULL,                       // int (*stat_r)(struct _reent *r, const char *file, struct stat *st);
    NULL,                       // int (*link_r)(struct _reent *r, const char *existing, const char *newLink);
    NULL,                       // int (*unlink_r)(struct _reent *r, const char *name);
    NULL,                       // int (*chdir_r)(struct _reent *r, const char *name);    
    NULL,                       // int (*rename_r)(struct _reent *r, const char *oldName, const char *newName);
    NULL,                       // int (*mkdir_r)(struct _reent *r, const char *path, int mode);
    
    sizeof(EFS_DirStruct),      // int dirStateSize;
    &EFS_DirOpen,               // DIR_ITER* (*diropen_r)(struct _reent *r, DIR_ITER *dirState, const char *path);
    &EFS_DirReset,              // int (*dirreset_r)(struct _reent *r, DIR_ITER *dirState);
    &EFS_DirNext,               // int (*dirnext_r)(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat);
    &EFS_DirClose               // int (*dirclose_r)(struct _reent *r, DIR_ITER *dirState);    
};


// search into NitroFS
void ExtractDirectory(char *prefix, u32 dir_id) {
    char strbuf[EFS_MAXPATHLEN];
    u32 entry_start;    // reference location of entry name
    u16 top_file_id;    // file ID of top entry
    u32 file_id;
//    u32 save_filepos = (useDLDI ? ftell(nds_file) : seek);
    u32 seek_pos = 0;

    if(useDLDI) {
        fseek(nds_file, fnt_offset + 8*(dir_id & 0xFFF), SEEK_SET);
        fread(&entry_start, sizeof(entry_start), 1, nds_file);
        fread(&top_file_id, sizeof(top_file_id), 1, nds_file);

        if((searchmode == EFS_LISTDIR) && file_idpos)
            fseek(nds_file, file_idpos, SEEK_SET);
        else
            fseek(nds_file, fnt_offset + entry_start, SEEK_SET);        
            
    } else {
        seek_pos = fnt_offset + 8*(dir_id & 0xFFF);
        memcpy(&entry_start, seek_pos + (void*)GBAROM, sizeof(entry_start));
        memcpy(&top_file_id, seek_pos + sizeof(entry_start) + (void*)GBAROM, sizeof(top_file_id));

        if((searchmode == EFS_LISTDIR) && file_idpos)
            seek_pos = file_idpos;
        else
            seek_pos = fnt_offset + entry_start;
    }
    
    for(file_id=top_file_id; !filematch; file_id++) {
        u8 entry_type_name_length;
        u32 name_length;
        bool entry_type_directory;
        char entry_name[EFS_MAXNAMELEN];
        
        if(useDLDI) {
            fread(&entry_type_name_length, sizeof(entry_type_name_length), 1, nds_file);
        } else {
            memcpy(&entry_type_name_length, seek_pos + (void*)GBAROM, sizeof(entry_type_name_length));
            seek_pos += sizeof(entry_type_name_length);
        }
        
        name_length = entry_type_name_length & 127;
        entry_type_directory = (entry_type_name_length & 128) ? true : false;
        if(name_length == 0)
            break;

        memset(entry_name, 0, EFS_MAXNAMELEN);

        if(useDLDI) {
            fread(entry_name, 1, entry_type_name_length & 127, nds_file);
        } else {
            memcpy(entry_name, seek_pos + (void*)GBAROM, entry_type_name_length & 127);
            seek_pos += entry_type_name_length & 127;
        }        

        if(searchmode == EFS_LISTDIR) {
            strcpy(fileInNDS, entry_name);
            
            if(useDLDI) {
                file_idpos = ftell(nds_file);
            } else {
                file_idpos = seek_pos;
            }
            
            if(entry_type_directory) {
                file_idpos += 2;
                file_idsize = 1;
            } else {
                file_idsize = 0;
            }
            filematch = true;
            break;
        }

        if(entry_type_directory) {
            u16 dir_id;
            
            if(useDLDI) {
                fread(&dir_id, sizeof(dir_id), 1, nds_file);
            } else {
                memcpy(&dir_id, seek_pos + (void*)GBAROM, sizeof(dir_id));
                seek_pos += sizeof(dir_id);
            }
            
            strcpy(strbuf, prefix);
            strcat(strbuf, entry_name);
            strcat(strbuf, "/");
            
            if((searchmode == EFS_SEARCHDIR) && !strcmp(strbuf, fileInNDS)) {
                file_idpos = dir_id;
                filematch = true;
                break;
            }
            
            ExtractDirectory(strbuf, dir_id);
            
        } else if(searchmode == EFS_SEARCHFILE) {
            strcpy(strbuf, prefix);
            strcat(strbuf, entry_name);
            
            if (!strcmp(strbuf, fileInNDS)) {
                u32 top;
                u32 bottom;
                file_idpos = fat_offset + 8*file_id;
                
                if(useDLDI) {
                    fseek(nds_file, file_idpos, SEEK_SET);
                    fread(&top, 1, sizeof(top), nds_file);
                    fread(&bottom, 1, sizeof(bottom), nds_file);
                } else {
                    seek_pos = file_idpos;
                    memcpy(&top, seek_pos + (void*)GBAROM, sizeof(top));
                    memcpy(&bottom, seek_pos + sizeof(top) + (void*)GBAROM, sizeof(top));
                    seek_pos += sizeof(top) + sizeof(bottom);
                }                

                file_idsize = bottom - top;
                if(hasLoader)
                    top += 512;
                file_idpos = top;
                filematch = true;
                break;
            }
        }
    }

//    fseek(nds_file, save_filepos, SEEK_SET);
}

// check if the file is the good one, and save the path if desired
bool CheckFile(char *path, bool save) {
    bool ok = false;
    char ext[7], *ext2 = ext + 2;
    int i;
    u32 size;
    FILE *f;
    
    // check file extension
    strcpy(ext, path + strlen(path) - 6);
    for(i=0; i<7; ++i)
        ext[i] = tolower(ext[i]);
    
    if(!strcmp(ext2, ".nds") || !strcmp(ext2, ".bin") || !strcmp(ext, "ds.gba")) {
        if((f = fopen(path, "rb+"))) {
            // check file size
            fseek(f, 0, SEEK_END);
            size = ftell(f);
        
            if(size == efs_filesize) {
                bool found = false;
                char dataChunk[EFS_READBUFFERSIZE];
                char magicString[12] = "";
                int dataChunk_size, efs_offset = 0;
                
                // rebuild magic string
                strcat(magicString, efsMagicStringP1);
                strcat(magicString, efsMagicStringP2);
                
                // search for magic string
                i = 0;
                fseek(f, 0, SEEK_SET);
                while(!feof(f) && !found) {
                    dataChunk_size = fread(dataChunk, sizeof(char), EFS_READBUFFERSIZE, f);
                
                    for(i=0; i < dataChunk_size; i++) {
                        if(dataChunk[i] == magicString[0]) {
                            if(dataChunk_size-i < 12) {
                                break;
                            }
                            if(memcmp(&dataChunk[i], magicString, 12) == 0) {
                                found = true;
                                efs_offset += i;
                                break;
                            }
                        }
                    }
                    if(!found) {
                        efs_offset += dataChunk_size;
                    }
                }
    
                // check file id
                if(found == true && (*(int*)(dataChunk+i+12) == efs_id)) {
                    strcpy(efs_path, path);
                    // store file path in NDS
                    if(save) {
                        fseek(f, efs_offset+20, SEEK_SET);
                        fwrite(path, 1, 256, f);
                    }
                    ok = true;
                }
            }
        }
        fclose(f);
    }

    return ok;
}

// search in directory for the NDS file
bool SearchDirectory() {
    DIR_ITER *dir;
    bool found = false;
    char path[EFS_MAXPATHLEN];
    char filename[EFS_MAXPATHLEN];
    struct stat st; 
 
    dir = diropen(".");
    while((dirnext(dir, filename, &st) == 0) && (!found)) {
        if(st.st_mode & S_IFDIR) {
            if(((strlen(filename) == 1) && (filename[0]!= '.')) ||
                ((strlen(filename) == 2) && (strcasecmp(filename, "..")))  ||
                (strlen(filename) > 2))
            {
                chdir(filename);
                found = SearchDirectory();
                chdir("..");
            }
        } else {
            getcwd(path, EFS_MAXPATHLEN-1);
            strcat(path, filename);
        
            if(CheckFile(path, true)) {
                found = true;
                break;
            }
        }
    }
    dirclose(dir);
    
    return found;
} 

// initialize the file system
int EFS_Init(int options) {

    bool found = false;

    // first try to init NitroFS from GBA mem
    REG_EXMEMCNT &= ~ARM7_OWNS_CARD; // take gba slot ownership
    
    if(strncmp(((const char*)GBAROM)+EFS_LOADERSTROFFSET, EFS_GBALOADERSTR, strlen(EFS_GBALOADERSTR)) == 0) {

        // there's a GBA loader here
        memcpy(&fnt_offset, EFS_FNTOFFSET+EFS_LOADEROFFSET + (void*)GBAROM, sizeof(fnt_offset));
        memcpy(&fat_offset, EFS_FATOFFSET+EFS_LOADEROFFSET + (void*)GBAROM, sizeof(fat_offset));
        fnt_offset += EFS_LOADEROFFSET;
        fat_offset += EFS_LOADEROFFSET;
        hasLoader = true;
        useDLDI = false;
        AddDevice(&EFSdevoptab);
        found = true;
        strcpy(efs_path, "GBA ROM");
        
    } else if(strncmp(((const char*)GBAROM)+EFS_LOADERSTROFFSET, EFS_STDLOADERSTR, strlen(EFS_STDLOADERSTR)) == 0) {

        // there's a standard nds loader here
        memcpy(&fnt_offset, EFS_FNTOFFSET + (void*)GBAROM, sizeof(fnt_offset));
        memcpy(&fat_offset, EFS_FATOFFSET + (void*)GBAROM, sizeof(fat_offset));
        hasLoader = false;
        useDLDI = false;
        AddDevice(&EFSdevoptab);
        found = true;        
        strcpy(efs_path, "GBA ROM");
        
    } else {

        // if init from GBA mem failed go for DLDI I/O    
        useDLDI = true;
        
        // init libfat if requested
        if(options & EFS_AND_FAT) {
            if(!fatInitDefault())
                return false;
        }

        // check if there's already a path stored
        if(efs_path[0]) {
            if(CheckFile(efs_path, false)) {
                found = true;
            } else {
                efs_path[0] = '\0';
            }
        }

        // if no path is defined, search the whole FAT space
        if(!efs_path[0]) {
            chdir("/");
            if(SearchDirectory())
                found = true;
        }

        // if nds file is found, open it and read the header
        if(found) {
            char buffer[5];
        
            nds_file = fopen(efs_path, "rb+");

            // check for if a loader is present
            fseek(nds_file, EFS_LOADERSTROFFSET, SEEK_SET);
            fread(buffer, 6, 1, nds_file);
            buffer[7] = '\0';
            
            if(strcmp(buffer, EFS_GBALOADERSTR) == 0) {
                // loader present
                fseek(nds_file, EFS_LOADEROFFSET+EFS_FNTOFFSET, SEEK_SET);
                fread(&fnt_offset, sizeof(u32), 1, nds_file);
                fseek(nds_file, 4, SEEK_CUR);
                fread(&fat_offset, sizeof(u32), 1, nds_file);
                fnt_offset += EFS_LOADEROFFSET;
                fat_offset += EFS_LOADEROFFSET;
                hasLoader = true;
            } else {
                fseek(nds_file, EFS_FNTOFFSET, SEEK_SET);
                fread(&fnt_offset, sizeof(u32), 1, nds_file);
                fseek(nds_file, 4, SEEK_CUR);
                fread(&fat_offset, sizeof(u32), 1, nds_file);
                hasLoader = false;
            }  
            
            AddDevice(&EFSdevoptab);
        }
    }

    return (found && (!useDLDI || nds_file));
}

// open a file
int EFS_Open(struct _reent *r, void *fileStruct, const char *path, int flags, int mode) {
    EFS_FileStruct *file = (EFS_FileStruct*)fileStruct;

    if(useDLDI && !nds_file)
        return -1;
        
    // search for the file in NitroFS
    filematch = false;
    searchmode = EFS_SEARCHFILE;
    file_idpos = 0;
    file_idsize = 0;
    
    if(strncmp(path, EFS_PREFIX, strlen(EFS_PREFIX)) == 0)
        path += 4;
    
    if(path[0] == '/') {
        strcpy(fileInNDS, path);
    } else {
        strcpy(fileInNDS, "/");
        strcat(fileInNDS, path);
    }

    ExtractDirectory("/", EFS_NITROROOT);
    
    if(file_idpos) {
        file->start = file_idpos;
        file->pos = file_idpos;
        file->end = file_idpos + file_idsize;        
        return 0;
    }
  
    return -1;
}

// set the current position in the file
int EFS_Seek(struct _reent *r, int fd, int pos, int dir) {
    EFS_FileStruct *file = (EFS_FileStruct*)fd;    
    switch(dir) {
        case SEEK_SET:
            file->pos = file->start + pos;
            break;
        case SEEK_CUR:
            file->pos += pos;
            break;
        case SEEK_END:
            file->pos = file->end + pos;
            break;
    }
    return file->pos;
}

// read data from file
int EFS_Read(struct _reent *r, int fd, char *ptr, int len) {
    EFS_FileStruct *file = (EFS_FileStruct*)fd; 

    if(file->pos+len > file->end) 
        len = file->end - file->pos;    // don't read past the end of the file
    if(file->pos > file->end)
        return 0;   // EOF
    
    if (useDLDI) {    
        // seek to right position and read data
        fseek(nds_file, file->pos, SEEK_SET);
        len = fread(ptr, len, 1, nds_file);
    } else {    
        memcpy(ptr, file->pos + (void*)GBAROM, len);
    }
    if (len > 0)
        file->pos += len;        
        
    return len;
}

// write data to file (only works using DLDI)
int EFS_Write(struct _reent *r, int fd, const char *ptr, int len) {
    EFS_FileStruct *file = (EFS_FileStruct*)fd; 

    if(file->pos+len > file->end) 
        len = file->end - file->pos;    // don't write past the end of the file
    if(file->pos > file->end)
        return 0;   // EOF
    
    if (useDLDI) {    
        // seek to right position and write data
        fseek(nds_file, file->pos, SEEK_SET);
        len = fwrite(ptr, len, 1, nds_file);
        if (len > 0)
            file->pos += len;        
    } else {    
        return 0;
    }
        
    return len;
}

// close current file
int EFS_Close(struct _reent *r, int fd) {
    // flush writes in the file system
    if(useDLDI) {
        if(nds_file)
            fclose(nds_file);
        nds_file = fopen(efs_path, "rb+");
    }
    return 0;
}

// open a directory
DIR_ITER* EFS_DirOpen(struct _reent *r, DIR_ITER *dirState, const char *path) {
    EFS_DirStruct *dir = (EFS_DirStruct*)dirState->dirStruct;

    if(useDLDI && !nds_file)
        return NULL;
        
    // search for the directory in NitroFS
    filematch = false;
    searchmode = EFS_SEARCHDIR;
    file_idpos = 0;
    file_idsize = 0;
    
    if(strncmp(path, EFS_PREFIX, strlen(EFS_PREFIX)) == 0)
        path += 4;
    
    if(path[0] == '/') {
        strcpy(fileInNDS, path);
    } else {
        strcpy(fileInNDS, "/");
        strcat(fileInNDS, path);
    }
    
    if(fileInNDS[strlen(fileInNDS)-1] != '/')
        strcat(fileInNDS, "/");

    ExtractDirectory("/", 0xF000);
    
    if(file_idpos) {
        dir->dir_id = file_idpos;
        dir->pos = 0;
        return dirState;
    }
  
    return NULL;
}

// go back to the beginning of the directory
int EFS_DirReset(struct _reent *r, DIR_ITER *dirState) {
    ((EFS_DirStruct*)dirState->dirStruct)->pos = 0;
    return 0;
}

// read next entry of the directory
int EFS_DirNext(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *st) {
    EFS_DirStruct *dir = (EFS_DirStruct*)dirState->dirStruct;

    if(useDLDI && !nds_file)
        return -1;
        
    // search for the file in NitroFS
    filematch = false;
    searchmode = EFS_LISTDIR;
    file_idpos = dir->pos;
    file_idsize = 2;
    ExtractDirectory("", dir->dir_id);
    
    if(file_idsize != 2) {
        strcpy(filename, fileInNDS);
        dir->pos = file_idpos;
        
        if(file_idsize == 0) {
            st->st_mode = 0;
            // maybe get file size info too?                    
        } else {
            st->st_mode = S_IFDIR;
        }        

        return 0;
    }  

    return -1;
}

// get some info on a file
int EFS_Fstat(struct _reent *r, int fd, struct stat *st) {
    EFS_FileStruct *file = (EFS_FileStruct*)fd;
    st->st_size = file->end - file->start;
    // maybe add some other info?
    return 0;    
}

// close a directory
int EFS_DirClose(struct _reent *r, DIR_ITER *dirState) {
    return 0;
}


// reserved space for post-compilation adding of EFS tags
asm (
"@--------------------------------------------------------------------------------------\n"
"   .balign    32                                                                          \n"
"   .arm                                                                                \n"
"@--------------------------------------------------------------------------------------\n"
"   .word   0xBFA905CE      @ Magic number to identify this region                      \n"
"   .asciz  \" EFSstr\"     @ Identifying Magic string (8 bytes with null terminator)   \n"
"   .global efs_id                                                                      \n"
"efs_id:                                                                                \n"
"   .word   0x00            @ ID of the rom                                             \n"
"   .global efs_filesize                                                                \n"
"efs_filesize:                                                                          \n"
"   .word   0x00            @ Size of the rom                                           \n"
"   .global efs_path                                                                    \n"
"efs_path:                                                                              \n"
"   .skip   256             @ Path of the rom                                           \n"
"@--------------------------------------------------------------------------------------\n"
);

#ifdef __cplusplus
}
#endif
