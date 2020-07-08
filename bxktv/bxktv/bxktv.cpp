// bxktv.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include <Http/Http.h>
#include <StringFormat/StringFormat.h>
#include <Shlwapi.h>
#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "shlwapi.lib")
#ifdef _DEBUG
#pragma comment(lib, "http_d.lib")
#else
#pragma comment(lib, "http.lib")
#endif


BOOL ToDownloadMp3(LPCTSTR lpUrl)
{
    BOOL downsuccess = FALSE;
    DWORD dwBufLen = 0;
    LPTSTR szUrl = (LPTSTR)AllocMemory(4096);
    wsprintf(szUrl, TEXT("http://www.bxktv.com/MP3/PlayList.ashx?id=%s"), lpUrl);
    CHttp WebRequest(szUrl);

    // 获取请求数据长度 
    const char* szlen = WebRequest.GetDataLenthA();
    if (szlen)
    {
        dwBufLen = strtoul(szlen, NULL, 10);
    }
    else
    {
        dwBufLen = 6*1024;
    }
    LPBYTE lpbuf = (LPBYTE)AllocMemory(dwBufLen);

    // 获取请求数据并解析 
    if (WebRequest.GetData(lpbuf, dwBufLen, TRUE) > 0)
    {
        char* szStartMp3Url = StrStrIA((const char*)lpbuf, "src='");
        if (szStartMp3Url != NULL)
        {
            szStartMp3Url += 5;
            char* szEndMp3Url = StrStrIA(szStartMp3Url, "'");
            if (szEndMp3Url != NULL)
            {
                *szEndMp3Url = '\0';
            }
            char* filename = strrchr(szStartMp3Url, '/')+1;
            dwBufLen = 0;

            char* lpSaveFile = (char*)szUrl;
            GetModuleFileNameA(NULL, lpSaveFile, MAX_PATH);
            PathRemoveFileSpecA(lpSaveFile);
            strcat(lpSaveFile, "\\");
            strcat(lpSaveFile, filename);
            printf("URL:%s -> %s\n", szStartMp3Url, lpSaveFile);
            if ( S_OK == URLDownloadToFileA(NULL, szStartMp3Url, lpSaveFile, NULL, NULL) )
            {
                 downsuccess = TRUE;
            }
        }
    }

    FreeMemory(lpbuf);
    FreeMemory(szUrl);
    return downsuccess;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // http://www.bxktv.com/MP3/232.html
//     ToDownloadMp3(TEXT("232"));
    for (int i=1; i<argc; i++)
    {
        ToDownloadMp3(argv[i]);
    }
    return 0;
}

