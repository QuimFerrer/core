/*
 * $Id$
 */

#include <filesys.h>
#include <string.h>

#if defined(__CYGNUS__)
  #include <mingw32/share.h>
#endif

#if defined(__GNUC__)
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <errno.h>

  #if !defined(HAVE_POSIX_IO)
  #define HAVE_POSIX_IO
  #endif

  #define PATH_SEPARATOR '/'
#endif

#if defined(__WATCOMC__)
  #include <sys/stat.h>
  #include <share.h>
  #include <fcntl.h>
  #include <io.h>
  #include <direct.h>
  #include <errno.h>

  #if !defined(HAVE_POSIX_IO)
  #define HAVE_POSIX_IO
  #endif

  #define PATH_SEPARATOR '\\'
#endif

#if defined(__BORLANDC__) || defined(__IBMCPP__)
  #include <sys\stat.h>
  #include <io.h>
  #include <fcntl.h>
  #include <share.h>
  #if defined(__IBMCPP__)
    #define SH_COMPAT SH_DENYRW
    #include <direct.h>
  #else
    #include <dir.h>
  #endif

  #if !defined(HAVE_POSIX_IO)
  #define HAVE_POSIX_IO
    #ifndef S_IEXEC
      #define S_IEXEC  0x0040 /* owner may execute <directory search> */
    #endif
    #ifndef S_IRWXU
      #define S_IRWXU  0x01c0 /* RWE permissions mask for owner */
    #endif
    #ifndef S_IRUSR
      #define S_IRUSR  0x0100 /* owner may read */
    #endif
    #ifndef S_IWUSR
      #define S_IWUSR  0x0080 /* owner may write */
    #endif
    #ifndef S_IXUSR
      #define S_IXUSR  0x0040 /* owner may execute <directory search> */
    #endif
  #endif

  #define PATH_SEPARATOR '\\'
#endif

#define IT_NUMBER       (IT_INTEGER|IT_LONG|IT_DOUBLE)

static USHORT last_error = 0;

#if !defined(PATH_MAX)
/* if PATH_MAX isn't defined, 256 bytes is a good number :) */
#define PATH_MAX 256
#endif

#define MKLONG(_1,_2,_3,_4) (((long)_4)<<24)|(((long)_3)<<16)|(((long)_2)<<8)|_1
#define MKINT(_1,_2)        (((long)_2)<<8)|_1

extern int rename( const char *, const char * );

/* Convert HARBOUR flags to IO subsystem flags */

#if defined(HAVE_POSIX_IO)

static int convert_open_flags( int flags )
{
        /* by default FO_READ+FO_COMPAT is set */
        int result_flags = 0;

        result_flags |= O_BINARY;

        if( flags == 0 )
                result_flags |= O_RDONLY|SH_COMPAT;

        /* read & write flags */
        if( flags & FO_WRITE )
                result_flags |= O_WRONLY;

        if( flags & FO_READWRITE )
                result_flags |= O_RDWR;

        /* shared flags */
        if( flags & FO_EXCLUSIVE )
                result_flags |= SH_DENYRW;

        if( flags & FO_DENYWRITE )
                result_flags |= SH_DENYWR;

        if( flags & FO_DENYREAD )
                result_flags |= SH_DENYRD;

        if( flags & FO_DENYNONE )
                result_flags |= SH_DENYNO;

        if( flags & FO_SHARED )
                result_flags |= SH_DENYNO;

        return result_flags;
}

static int convert_seek_flags( int flags )
{
        /* by default FS_SET is set */
        int result_flags=0;

        result_flags = SEEK_SET;

        if( flags & FS_RELATIVE )
                result_flags = SEEK_CUR;

        if( flags & FS_END )
                result_flags = SEEK_END;

        return result_flags;
}

static int convert_create_flags( int flags )
{
        /* by default FC_NORMAL is set */
        int result_flags=S_IWUSR;

        result_flags |= O_BINARY | O_CREAT | O_TRUNC | O_RDWR;

        if( flags & FC_READONLY )
                result_flags = result_flags & ~(S_IWUSR);

        if( flags & FC_HIDDEN )
                result_flags |= 0;

        if( flags & FC_SYSTEM )
                result_flags |= 0;

        return result_flags;
}

#endif


/*
 * FILESYS.API FUNCTIONS --
 */

FHANDLE hb_fsOpen   ( BYTEP name, USHORT flags )
{
        FHANDLE handle;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        handle = open(name,convert_open_flags(flags));
        last_error = errno;
#else
        handle = FS_ERROR;
        last_error = FS_ERROR;
#endif
        return handle;
}

FHANDLE hb_fsCreate ( BYTEP name, USHORT flags )
{
        FHANDLE handle;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        handle = open(name,convert_create_flags(flags));
        last_error = errno;
#else
        handle = FS_ERROR;
        last_error = FS_ERROR;
#endif
        return handle;
}

void    hb_fsClose  ( FHANDLE handle )
{
#if defined(HAVE_POSIX_IO)
    close(handle);
    return;
#endif
}

USHORT  hb_fsRead   ( FHANDLE handle, BYTEP buff, USHORT count )
{
        USHORT bytes;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        bytes = read(handle,buff,count);
        last_error = errno;
#else
        bytes = 0;
        last_error = FS_ERROR;
#endif
        return bytes;
}

USHORT  hb_fsWrite  ( FHANDLE handle, BYTEP buff, USHORT count )
{
        USHORT bytes;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        bytes = write(handle,buff,count);
        last_error = errno;
#else
        bytes = 0;
        last_error = FS_ERROR;
#endif
        return bytes;
}

ULONG   hb_fsSeek   ( FHANDLE handle, LONG offset, USHORT flags )
{
        ULONG position;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        position = lseek(handle,offset,convert_seek_flags(flags));
        last_error = errno;
#else
        position = 0;
        last_error = FS_ERROR;
#endif
        return position;
}

USHORT  hb_fsError  ( void )
{
        return last_error;
}

void    hb_fsDelete ( BYTEP name )
{
#if defined(HAVE_POSIX_IO)
        errno = 0;
        unlink(name);
        last_error = errno;
        return;
#endif
}

void    hb_fsRename ( BYTEP older, BYTEP newer )
{
#if defined(HAVE_POSIX_IO)
        errno = 0;
        rename(older,newer);
        last_error = errno;
        return;
#endif
}

BOOL    hb_fsLock   ( FHANDLE handle, ULONG start,
                      ULONG length, USHORT mode )
{
        int result=0;

#if defined(HAVE_POSIX_IO) && !defined(__GNUC__) && !defined(__IBMCPP__)
        errno = 0;
        switch( mode )
        {
           case FL_LOCK:
              result = lock(handle, start, length);
              break;

           case FL_UNLOCK:
              result = unlock(handle, start, length);
        }
        last_error = errno;
#else
        result = 1;
        last_error = FS_ERROR;
#endif

        return (result ? FALSE : TRUE );
}

void    hb_fsCommit ( FHANDLE handle )
{
#if defined(HAVE_POSIX_IO)

        int dup_handle;
        errno = 0;
        dup_handle = dup(handle);
        last_error = errno;
        if (dup_handle != -1)
        {
           close(dup_handle);
           last_error = errno;
        }

#endif
        return;
}

BOOL    hb_fsMkDir  ( BYTEP name )
{
        int result;
#if defined(HAVE_POSIX_IO)
        errno = 0;
  #if !defined(__WATCOMC__) && !defined(__BORLANDC__) && !defined(__IBMCPP__)
        result = mkdir(name,S_IWUSR|S_IRUSR);
  #else
        result = mkdir( name );
  #endif
        last_error = errno;
#else
        result = 1;
        last_error = FS_ERROR;
#endif
        return (result ? FALSE : TRUE );
}

BOOL    hb_fsChDir  ( BYTEP name )
{
        int result;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        result = chdir(name);
        last_error = errno;
#else
        result = 1;
        last_error = FS_ERROR;
#endif
        return (result ? FALSE : TRUE );
}

BOOL    hb_fsRmDir  ( BYTEP name )
{
        int result;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        result = rmdir(name);
        last_error = errno;
#else
        result = 1;
        last_error = FS_ERROR;
#endif
        return (result ? FALSE : TRUE );
}

BYTEP   hb_fsCurDir ( USHORT uiDrive )
{
        static char cwd_buff[PATH_MAX+1];
#if defined(HAVE_POSIX_IO)
        errno = 0;
        getcwd(cwd_buff,PATH_MAX);
        last_error = errno;
#else
        cwd_buff[0] = 0;
        last_error = FS_ERROR;
#endif
        return cwd_buff;
}

USHORT  hb_fsChDrv  ( BYTEP nDrive )
{
        USHORT result;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        result = 0;
        last_error = errno;
        last_error = FS_ERROR; /* TODO: Remove when function implemented */
#else
        result = 0;
        last_error = FS_ERROR;
#endif
        return result;
}

BYTE    hb_fsCurDrv ( void )
{
        USHORT result;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        result = 0;
        last_error = errno;
        last_error = FS_ERROR; /* TODO: Remove when function implemented */
#else
        result = 0;
        last_error = FS_ERROR;
#endif
        return result;
}

USHORT  hb_fsIsDrv  ( BYTE nDrive )
{
        USHORT result;
#if defined(HAVE_POSIX_IO)
        errno = 0;
        result = 0;
        last_error = errno;
        last_error = FS_ERROR; /* TODO: Remove when function implemented */
#else
        result = 0;
        last_error = FS_ERROR;
#endif
        return result;
}

/* TODO: Implement hb_fsExtOpen */
FHANDLE hb_fsExtOpen( BYTEP fpFilename, BYTEP fpDefExt,
                      USHORT uiFlags, BYTEP fpPaths, ERRORP pError )
{
   return FS_ERROR;
}

/*
 * -- HARBOUR FUNCTIONS --
 */

#ifdef FOPEN
#define HB_FOPEN FOPEN
#undef FOPEN
#endif

HARBOUR FOPEN( void )

#ifdef HB_FOPEN
#define FOPEN HB_FOPEN
#undef HB_FOPEN
#endif

{
        PHB_ITEM arg1_it = _param(1,IT_STRING);
        PHB_ITEM arg2_it = _param(2,IT_NUMBER);

        int open_flags;
        int file_handle = -1;

        if( arg1_it )
        {
            if( arg2_it )
                open_flags = _parni(2);
            else
                open_flags = 0;

            file_handle = hb_fsOpen(_parc(1),open_flags);
        }

        _retni(file_handle);
        return;
}

HARBOUR FCREATE( void )
{
        PHB_ITEM arg1_it = _param(1,IT_STRING);
        PHB_ITEM arg2_it = _param(2,IT_NUMBER);

        int create_flags;
        int file_handle = -1;

        if( arg1_it )
        {
            if( arg2_it )
                create_flags = _parni(2);
            else
                create_flags = 0;

            file_handle = hb_fsCreate(_parc(1),create_flags);
        }

        _retni(file_handle);
        return;
}

#ifdef FREAD
#define HB_FREAD FREAD
#undef FREAD
#endif

HARBOUR FREAD( void )

#ifdef HB_FREAD
#define FREAD HB_FREAD
#undef HB_FREAD
#endif

{
        PHB_ITEM arg1_it = _param(1,IT_NUMBER);
        PHB_ITEM arg2_it = _param(2,IT_STRING+IT_BYREF);
        PHB_ITEM arg3_it = _param(3,IT_NUMBER);

        long   bytes=0;

        if( arg1_it && arg2_it && arg3_it )
        {
            bytes = hb_fsRead(_parni(1),_parc(2),_parnl(3));
        }

        _retnl(bytes);
        return;
}

#ifdef FWRITE
#define HB_FWRITE FWRITE
#undef FWRITE
#endif

HARBOUR FWRITE( void )

#ifdef HB_FWRITE
#define FWRITE HB_FWRITE
#undef HB_FWRITE
#endif

{
        PHB_ITEM arg1_it = _param(1,IT_NUMBER);
        PHB_ITEM arg2_it = _param(2,IT_STRING);
        PHB_ITEM arg3_it = _param(3,IT_NUMBER);

        long   bytes=0;

        if( arg1_it && arg2_it )
        {
            bytes = (arg3_it ? _parnl(3) : arg2_it->wLength );
            bytes = hb_fsWrite(_parni(1),_parc(2),bytes);
        }

        _retnl(bytes);
        return;
}

HARBOUR FERROR( void )
{
        _retni(hb_fsError());
        return;
}

HARBOUR FCLOSE( void )
{
        PHB_ITEM arg1_it = _param(1,IT_NUMBER);

        last_error = 0;
        if( arg1_it )
        {
            hb_fsClose(_parni(1));
        }
        _retl( last_error == 0 );
        return;
}

HARBOUR FERASE( void )
{
        PHB_ITEM arg1_it = _param(1,IT_STRING);

        if( arg1_it )
        {
           hb_fsDelete(_parc(1));
        }

        _retni(last_error=0);
        return;
}

HARBOUR FRENAME( void )
{
        PHB_ITEM arg1_it = _param(1,IT_STRING);
        PHB_ITEM arg2_it = _param(2,IT_STRING);

        if( arg1_it && arg2_it )
        {
            hb_fsRename(_parc(1),_parc(2));
        }

        _retni(last_error);
        return;
}

HARBOUR FSEEK( void )
{
        PHB_ITEM arg1_it = _param(1,IT_NUMBER);
        PHB_ITEM arg2_it = _param(2,IT_NUMBER);
        PHB_ITEM arg3_it = _param(3,IT_NUMBER);

        long bytes=0;
        int  pos;

        if( arg1_it && arg2_it )
        {
            pos = (arg3_it ? _parni(3) : FS_SET);
            bytes = hb_fsSeek(_parni(1),_parnl(2),pos);
        }

        _retnl(bytes);
        return;
}

HARBOUR HB_FILE( void )
{
        PHB_ITEM arg1_it = _param( 1, IT_STRING );

        if( arg1_it )
        {
           _retl( access(_parc(1), 0) == 0 );
        }
        else _retl(0);
        return;
}

HARBOUR FREADSTR( void )
{
        PHB_ITEM arg1_it = _param( 1, IT_NUMBER );
        PHB_ITEM arg2_it = _param( 2, IT_NUMBER );

        int    handle;
        long   bytes;
        long   nRead;
        long   readed;
        char * buffer;
        char   ch[1];

        if( arg1_it )
        {
           handle = _parni(1);
           bytes  = (arg2_it ? _parnl(2) : 0);
           buffer = ( char * ) _xgrab(bytes);

           readed=0; ch[0]=1;
           while( readed < bytes )
           {
                 nRead = read(handle,ch,1);
                 if( nRead < 1 )
                        break;
                 buffer[readed]=ch[0];
                 readed++;
           }

           buffer[readed]=0;
           _retc(buffer);
           _xfree(buffer);
        }
        else
           _retc("");

        return;
}

HARBOUR BIN2I( void )
{
        PHB_ITEM arg1_it = _param( 1, IT_STRING );
        char * s;
        int    result=0;

        if( arg1_it )
        {
           s = _parc(1);
           if( _parclen(1) >= 2 )
                result = MKINT(s[0],s[1]);
           else
              result = 0;
        }

        _retni(result);
        return;
}

HARBOUR BIN2L( void )
{
        PHB_ITEM arg1_it = _param( 1, IT_STRING );
        char * s;
        long   result=0;

        if( arg1_it )
        {
           s = _parc(1);
           if( _parclen(1) >= 4 )
              result = MKLONG(s[0],s[1],s[2],s[3]);
           else
              result = 0;
        }

        _retni(result);
        return;
}

HARBOUR BIN2W( void )
{
        BIN2I();
}

HARBOUR I2BIN( void )
{
        PHB_ITEM arg1_it = _param( 1, IT_INTEGER );
        int n;
        char s[3];

        if( arg1_it )
        {
           n = _parni(1);
           s[0] = n & 0xFF;
           s[1] = (n & 0xFF00)>>8;
           s[2] = 0;
           _retclen(s,3);
        }
        else
           _retclen("\0\0",2);

        return;
}

HARBOUR L2BIN( void )
{
        PHB_ITEM arg1_it = _param( 1, IT_LONG );
        long  n;
        char  s[5];

        if( arg1_it )
        {
           n = _parnl(1);
           s[0] =  n & 0x000000FF;
           s[1] = (n & 0x0000FF00)>>8;
           s[2] = (n & 0x00FF0000)>>16;
           s[3] = (n & 0xFF000000)>>24;
           s[4] = 0;
           _retclen(s,5);
        }
        else
           _retclen("\0\0\0\0",4);

        return;
}

HARBOUR W2BIN( void )
{
        I2BIN();
}

