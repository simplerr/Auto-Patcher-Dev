////////////////////////////////////////////////////////////////////////////////
//
// Code taken from D. J. Bernstein (C ==> C++)
//
// ftpparse.c, ftpparse.h: library for parsing FTP LIST responses
// 20001223
// D. J. Bernstein, djb@cr.yp.to
// http://cr.yp.to/ftpparse.html
//
// Commercial use is fine, if you let me know what programs you're using this in.
//
// Currently covered formats:
// EPLF.
// UNIX ls, with or without gid.
// Microsoft FTP Service.
// Windows NT FTP Server.
// VMS.
// WFTPD.
// NetPresenz (Mac).
// NetWare.
// MSDOS.
//
// Definitely not covered: 
// Long VMS filenames, with information split across two lines.
// NCSA Telnet FTP server. Has LIST = NLST (and bad NLST for directories).
//
////////////////////////////////////////	////////////////////////////////////////

#include "stdafx.h"
#include "FTPListParse.h"
#include "FTPFileState.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef INSERT_TIME
#ifdef _DEBUG
#define INSERT_TIME() if( ftpFileStatus.m_mtime>0 ) ftpFileStatus.m_strMTime = CCnv::ConvertToTString(asctime(gmtime(&ftpFileStatus.m_mtime)));
#else
#define INSERT_TIME()
#endif
#endif

using namespace nsFTP;
using namespace nsHelper;

TCHAR* CFTPListParse::m_Months[12] = {
   _T("jan"),_T("feb"),_T("mar"),_T("apr"),_T("may"),_T("jun"),
   _T("jul"),_T("aug"),_T("sep"),_T("oct"),_T("nov"),_T("dec")
};

CFTPListParse::CFTPListParse() :
   m_lCurrentYear(-1)
{
   m_tmBase = 0;
   tm* ptm = gmtime(&m_tmBase);
   m_tmBase = -(ToTAI(ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday) + 
                ptm->tm_hour * 3600 + ptm->tm_min * 60 + ptm->tm_sec);
   // assumes the right time_t, counting seconds.
   // base may be slightly off if time_t counts non-leap seconds.
}

CFTPListParse::~CFTPListParse()
{
}

bool CFTPListParse::CheckMonth(LPCTSTR pszBuffer, LPCTSTR pszMonthName) const
{
  if ( tolower(pszBuffer[0]) != pszMonthName[0] ||
       tolower(pszBuffer[1]) != pszMonthName[1] ||
       tolower(pszBuffer[2]) != pszMonthName[2] )
     return false;

  return true;
}

int CFTPListParse::GetMonth(LPCTSTR pszBuffer,int iLength) const
{
  if (iLength == 3)
  {
    for (int i = 0; i < 12; ++i)
    {
      if (CheckMonth(pszBuffer, m_Months[i]))
         return i;
    }
  }
  return -1;
}

bool CFTPListParse::GetLong(LPCTSTR pszLong, int iLength, long& lResult) const
{
   tstring strLong(pszLong, iLength);
   
   LPTSTR pszEndPtr = NULL;
      lResult = tcstoul(strLong.c_str(), &pszEndPtr, 10);

   if( pszEndPtr!=NULL && pszEndPtr[0]!=_T('\0') )
      return false;

   return true;
}

long CFTPListParse::ToTAI(long lYear, long lMonth, long lMDay) const
{
   if (lMonth >= 2)
   {
      lMonth -= 2;
   }
   else
   {
      lMonth += 10;
      --lYear;
   }

   long lResult = (lMDay - 1) * 10 + 5 + 306 * lMonth;
   lResult /= 10;

   if (lResult == 365)
   {
      lYear -= 3;
      lResult = 1460;
   }
   else 
      lResult += 365 * (lYear % 4);

   lYear /= 4;

   lResult += 1461 * (lYear % 25);

   lYear /= 25;

   if (lResult == 36524)
   {
      lYear -= 3;
      lResult = 146096;
   }
   else
   {
      lResult += 36524 * (lYear % 4);
   }

   lYear /= 4;
   lResult += 146097 * (lYear - 5);
   lResult += 11017;

   return lResult * 86400;
}

long CFTPListParse::GetYear(time_t time) const
{
   long lDay = static_cast<long>(time / 86400L);

   if ((time % 86400L) < 0)
      --lDay;
   
   lDay -= 11017;
   
   long lYear = 5 + lDay / 146097;
   
   lDay = lDay % 146097;
   if (lDay < 0)
   {
      lDay += 146097;
      --lYear;
   }
   lYear *= 4;
   
   if (lDay == 146096) 
   {
      lYear += 3;
      lDay = 36524;
   }
   else
   {
      lYear += lDay / 36524;
      lDay %= 36524;
   }

   lYear *= 25;
   lYear += lDay / 1461;
   lDay %= 1461;
   lYear *= 4;

   if (lDay == 1460) 
   {
      lYear += 3;
      lDay = 365;
   }
   else
   {
      lYear += lDay / 365;
      lDay %= 365;
   }

   lDay *= 10;
   if ((lDay + 5) / 306 >= 10)
      ++lYear;

   return lYear;
}

/// UNIX ls does not show the year for dates in the last six months.
/// So we have to guess the year.
/// Apparently NetWare uses ``twelve months'' instead of ``six months''; ugh.
/// Some versions of ls also fail to show the year for future dates.
long CFTPListParse::GuessTAI(long lMonth, long lMDay)
{
  ///////////////////////////*/*/*/*/
  time_t now = time((time_t *) 0) - m_tmBase;

  if (m_lCurrentYear==-1)
  {
    m_lCurrentYear = GetYear(now);
  }

  long lTAI = 0;
  for (long lYear = m_lCurrentYear - 1; lYear < m_lCurrentYear + 100; ++lYear)
  {
    lTAI = ToTAI(lYear, lMonth, lMDay);
    if (now - lTAI < 350 * 86400)
      return lTAI;
  }
  return lTAI;
}

/// Easily Parsed LIST Format (EPLF)
/// see http://pobox.com/~djb/proto/eplf.txt
/// "+i8388621.29609,m824255902,/,\tdev"
/// "+i8388621.44468,m839956783,r,s10376,\tRFCEPLF"
bool CFTPListParse::IsEPLS(LPCTSTR pszLine)
{
   return pszLine && *pszLine == _T('+');
}

bool CFTPListParse::ParseEPLF(CFTPFileStatus& ftpFileStatus, LPCTSTR pszLine, int iLength)
{
   if( !IsEPLS(pszLine) )
      return false;

   long lTemp=0;
   int i = 1;
   for (int j=1; j<iLength; ++j)
   {
      if (pszLine[j] == _T('\t'))
      {
         ftpFileStatus.m_strName = pszLine+j+1;
         return true;
      }

      if (pszLine[j] == _T(','))
      {
         switch(pszLine[i])
         {
         case _T('/'):
            ftpFileStatus.m_fTryCwd    = true;
            break;
         case _T('r'):
            ftpFileStatus.m_fTryRetr   = true;
            break;
         case _T('s'):
            ftpFileStatus.m_enSizeType = CFTPFileStatus::stBinary;
            if( !GetLong(pszLine+i+1, j-i-1, ftpFileStatus.m_lSize) )
               ftpFileStatus.m_lSize = -1;
            break;
         case _T('m'):
            ftpFileStatus.m_enModificationTimeType = CFTPFileStatus::mttLocal;
            GetLong(pszLine+i+1, j-i-1, lTemp);
            ftpFileStatus.m_mtime = m_tmBase + lTemp;
            INSERT_TIME();
            break;
         case _T('i'):
            ftpFileStatus.m_enIDType = CFTPFileStatus::idFull;
            ftpFileStatus.m_strID    = pszLine+i+1;
            ftpFileStatus.m_strID    = ftpFileStatus.m_strID.substr(0, j-i-1);
         }
         i = j + 1;
      }
   }
   return false;
}

/// UNIX-style listing, without inum and without blocks
/// "-rw-r--r--   1 root     other        531 Jan 29 03:26 README"
/// "dr-xr-xr-x   2 root     other        512 Apr  8  1994 etc"
/// "dr-xr-xr-x   2 root     512 Apr  8  1994 etc"
/// "lrwxrwxrwx   1 root     other          7 Jan 25 00:17 bin -> usr/bin"
/// 
/// Also produced by Microsoft's FTP servers for Windows:
/// "----------   1 owner    group         1803128 Jul 10 10:18 ls-lR.Z"
/// "d---------   1 owner    group               0 May  9 19:45 Softlib"
/// 
/// Also WFTPD for MSDOS:
/// "-rwxrwxrwx   1 noone    nogroup      322 Aug 19  1996 message.ftp"
/// 
/// Also NetWare:
/// "d [R----F--] supervisor            512       Jan 16 18:53    login"
/// "- [R----F--] rhesus             214059       Oct 20 15:27    cx.exe"
//
/// Also NetPresenz for the Mac:
/// "-------r--         326  1391972  1392298 Nov 22  1995 MegaPhone.sit"
/// "drwxrwxr-x               folder        2 May 10  1996 network"
bool CFTPListParse::IsUNIXStyleListing(LPCTSTR pszLine)
{
   if( pszLine==NULL )
      return false;

   switch(*pszLine)
   {
   case _T('b'):
   case _T('c'):
   case _T('d'):
   case _T('l'):
   case _T('p'):
   case _T('s'):
   case _T('-'):
      return true;
   }
   return false;
}

bool CFTPListParse::ParseUNIXStyleListing(CFTPFileStatus& ftpFileStatus, LPCTSTR pszLine, int iLength)
{
   if( !IsUNIXStyleListing(pszLine) )
      return false;

   switch( *pszLine )
   {
   case _T('d'): ftpFileStatus.m_fTryCwd  = true; break;
   case _T('-'): ftpFileStatus.m_fTryRetr = true; break;
   case _T('l'): ftpFileStatus.m_fTryCwd  = true;
                 ftpFileStatus.m_fTryRetr = true;
   }

   int  iState  = 1;
   int  i       = 0;
   long lSize   = 0;
   long lYear   = 0;
   long lMonth  = 0;
   long lMDay   = 0;
   long lHour   = 0;
   long lMinute = 0;

   for( int j=1; j<iLength; ++j )
   {
      if( pszLine[j]==_T(' ') && pszLine[j-1]!=_T(' ') )
      {
         switch(iState)
         {
         case 1: // skipping perm
            ftpFileStatus.m_strAttributes.assign(pszLine+i, j-i);
            iState = 2;
            break;
         case 2: // skipping nlink
            iState = 3;
            ftpFileStatus.m_strLink.assign(pszLine+i, j-i);
            if( j-i==6 && pszLine[i]==_T('f') ) // for NetPresenz
               iState = 4;
            break;
         case 3: // skipping uid
            iState = 4;
            ftpFileStatus.m_strUID.assign(pszLine+i, j-i);
            break;
         case 4: // getting tentative size
            if( !GetLong(pszLine+i, j-i, lSize) )
            {
               lSize = -1;
               ftpFileStatus.m_strGID.assign(pszLine+i, j-i);
            }
            iState = 5;
            break;
         case 5: // searching for month, otherwise getting tentative size
            lMonth = GetMonth(pszLine + i,j - i);
            if( lMonth >= 0 )
               iState = 6;
            else
            {
               if( !GetLong(pszLine+i, j-i, lSize) )
                  lSize = -1;
            }
            break;
         case 6: // have size and month
            GetLong(pszLine+i, j-i, lMDay);
            iState = 7;
            break;
         case 7: // have size, month, mday
            if( j-i==4 && pszLine[i+1]==_T(':') )
            {
               GetLong(pszLine+i, 1, lHour);
               GetLong(pszLine+i+2, 2, lMinute);
               ftpFileStatus.m_enModificationTimeType = CFTPFileStatus::mttRemoteMinute;
               ftpFileStatus.m_mtime = m_tmBase + GuessTAI(lMonth,lMDay) + lHour * 3600 + lMinute * 60;
               INSERT_TIME();
            }
            else if( j-i==5 && pszLine[i+2]==_T(':') )
            {
               GetLong(pszLine+i, 2, lHour);
               GetLong(pszLine+i+3, 2, lMinute);
               ftpFileStatus.m_enModificationTimeType = CFTPFileStatus::mttRemoteMinute;
               ftpFileStatus.m_mtime = m_tmBase + GuessTAI(lMonth,lMDay) + lHour * 3600 + lMinute * 60;
               INSERT_TIME();
            }
            else if( j-i >= 4 )
            {
               GetLong(pszLine+i, j-i, lYear);
               ftpFileStatus.m_enModificationTimeType = CFTPFileStatus::mttRemoteDay;
               ftpFileStatus.m_mtime = m_tmBase + CFTPListParse::ToTAI(lYear,lMonth,lMDay);
               INSERT_TIME();
            }
            else
               return false;

            ftpFileStatus.m_strName = pszLine + j + 1;
            iState = 8;
            break;
         case 8: // twiddling thumbs
            break;
         }

         i = j + 1;
         while( i<iLength && pszLine[i]==_T(' ') )
            ++i;
      }
   }

   if( iState != 8 )
      return false;

   ftpFileStatus.m_lSize = lSize;
   ftpFileStatus.m_enSizeType = CFTPFileStatus::stBinary;

   // handle links
   if( pszLine[0] == _T('l') )
   {
      tstring::size_type pos = ftpFileStatus.m_strName.find(_T(" -> "));
      if( pos != tstring::npos )
         ftpFileStatus.m_strName = ftpFileStatus.m_strName.substr(0, pos);
   }

   // eliminate extra NetWare spaces
   if( pszLine[1]==_T(' ') || pszLine[1]==_T('[') )
   {
      if( ftpFileStatus.m_strName.length()>3 && ftpFileStatus.m_strName.substr(0, 3)==_T("   ") )
         ftpFileStatus.m_strName = ftpFileStatus.m_strName.substr(3);
   }

   return true;
}

/// MultiNet (some spaces removed from examples)
/// "00README.TXT;1      2 30-DEC-1996 17:44 [SYSTEM] (RWED,RWED,RE,RE)"
/// "CORE.DIR;1          1  8-SEP-1996 16:09 [SYSTEM] (RWE,RWE,RE,RE)"
/// and non-MutliNet VMS:
/// "CII-MANUAL.TEX;1  213/216  29-JAN-1996 03:33:12  [ANONYMOU,ANONYMOUS]   (RWED,RWED,,)"
bool CFTPListParse::IsMultiNetListing(LPCTSTR pszLine)
{
   return pszLine && tcschr(pszLine, _T(';')) != NULL;
}

bool CFTPListParse::ParseMultiNetListing(CFTPFileStatus& ftpFileStatus, LPCTSTR pszLine, int iLength)
{
   if( !IsMultiNetListing(pszLine) )
      return false;

   // name lookup of `i' changed for new ISO `for' scoping 
   int i=0;
   for( ; i<iLength; ++i )
      if( pszLine[i]==_T(';') )
         break;

   if( i < iLength)
   {
      ftpFileStatus.m_strName = pszLine;
      ftpFileStatus.m_strName = ftpFileStatus.m_strName.substr(0, i);
      if( i > 4 && tcsncmp(pszLine+i-4, _T(".DIR"), 4)==0 )
      {
         ftpFileStatus.m_strName = ftpFileStatus.m_strName.substr(0, ftpFileStatus.m_strName.length()-4);
         ftpFileStatus.m_fTryCwd = true;
      }
      else
         ftpFileStatus.m_fTryRetr = true;

      while (pszLine[i] != _T(' ')) if (++i == iLength) return false;
      while (pszLine[i] == _T(' ')) if (++i == iLength) return false;
      while (pszLine[i] != _T(' ')) if (++i == iLength) return false;
      while (pszLine[i] == _T(' ')) if (++i == iLength) return false;
      
      int j = i;
      while (pszLine[j] != _T('-')) if (++j == iLength) return false;
      long lMDay = 0;
      GetLong(pszLine+i, j-i, lMDay);

      while (pszLine[j] == _T('-')) if (++j == iLength) return false;
      i = j;
      while (pszLine[j] != _T('-')) if (++j == iLength) return false;
      long lMonth = GetMonth(pszLine+i, j-i);
      if (lMonth < 0) return false;

      while (pszLine[j] == _T('-')) if (++j == iLength) return false;
      i = j;
      while (pszLine[j] != _T(' ')) if (++j == iLength) return false;
      long lYear = 0;
      GetLong(pszLine+i, j-i, lYear);

      while (pszLine[j] == _T(' ')) if (++j == iLength) return false;
      i = j;
      while (pszLine[j] != _T(':')) if (++j == iLength) return false;
      long lHour = 0;
      GetLong(pszLine+i, j-i, lHour);

      while (pszLine[j] == _T(':')) if (++j == iLength) return false;
      i = j;
      while (pszLine[j] != _T(':') && pszLine[j] != _T(' ')) if (++j == iLength) return false;
      long lMinute = 0;
      GetLong(pszLine+i, j-i, lMinute);

      ftpFileStatus.m_enModificationTimeType = CFTPFileStatus::mttRemoteMinute;
      ftpFileStatus.m_mtime = m_tmBase + CFTPListParse::ToTAI(lYear, lMonth, lMDay) + lHour * 3600 + lMinute * 60;
      INSERT_TIME();
   }

   return true;
}

/// MSDOS format
/// 04-27-00  09:09PM       <DIR>          licensed
/// 07-18-00  10:16AM       <DIR>          pub
/// 04-14-00  03:47PM                  589 readme.htm
bool CFTPListParse::IsMSDOSListing(LPCTSTR pszLine)
{
   return pszLine && tisdigit(pszLine[0]);
}

bool CFTPListParse::ParseMSDOSListing(CFTPFileStatus& ftpFileStatus, LPCTSTR pszLine, int iLength)
{
   if( !IsMSDOSListing(pszLine) )
      return false;

   int i = 0;
   int j = 0;
   while (pszLine[j] != _T('-')) if (++j == iLength) return false;
   long lMonth = 0;
   GetLong(pszLine+i, j-i, lMonth); //+# -1 

   while (pszLine[j] == _T('-')) if (++j == iLength) return false;
   i = j;
   while (pszLine[j] != _T('-')) if (++j == iLength) return false;
   long lMDay = 0;
   GetLong(pszLine+i, j-i, lMDay);

   while (pszLine[j] == _T('-')) if (++j == iLength) return false;
   i = j;
   while (pszLine[j] != _T(' ')) if (++j == iLength) return false;
   long lYear = 0;
   GetLong(pszLine+i, j-i, lYear);

   if (lYear < 50)
      lYear += 2000;

   if (lYear < 1000)
      lYear += 1900;

   while (pszLine[j] == _T(' ')) if (++j == iLength) return false;
   i = j;
   while (pszLine[j] != _T(':')) if (++j == iLength) return false;
   long lHour = 0;
   GetLong(pszLine+i, j-i, lHour);

   while (pszLine[j] == _T(':')) if (++j == iLength) return false;
   i = j;
   while ((pszLine[j] != _T('A')) && (pszLine[j] != _T('P'))) if (++j == iLength) return false;
   long lMinute = 0;
   GetLong(pszLine+i, j-i, lMinute);

   if (lHour == 12)
      lHour = 0;

   if (pszLine[j] == _T('A')) if (++j == iLength) return false;
   if (pszLine[j] == _T('P')) { lHour += 12; if (++j == iLength) return false; }
   if (pszLine[j] == _T('M')) if (++j == iLength) return false;

   while (pszLine[j] == _T(' ')) if (++j == iLength) return false;
   if (pszLine[j] == _T('<'))
   {
      ftpFileStatus.m_fTryCwd = true;
      while (pszLine[j] != _T(' ')) if (++j == iLength) return false;
   }
   else
   {
      i = j;
      while (pszLine[j] != _T(' ')) if (++j == iLength) return false;
      if( !GetLong(pszLine+i, j-i, ftpFileStatus.m_lSize ) )
         ftpFileStatus.m_lSize = -1;
      ftpFileStatus.m_enSizeType = CFTPFileStatus::stBinary;
      ftpFileStatus.m_fTryRetr = true;
   }
   while (pszLine[j] == _T(' ')) if (++j == iLength) return false;

   ftpFileStatus.m_strName = pszLine + j;

   ftpFileStatus.m_enModificationTimeType = CFTPFileStatus::mttRemoteMinute;
   ftpFileStatus.m_mtime = m_tmBase + CFTPListParse::ToTAI(lYear,lMonth,lMDay) + lHour * 3600 + lMinute * 60;
   INSERT_TIME();

   return true;
}

bool CFTPListParse::Parse(CFTPFileStatus& ftpFileStatus, const tstring& strLineToParse)
{
   ftpFileStatus.Reset();
   LPCTSTR   pszLine = strLineToParse.c_str();
   const int iLength = static_cast<int>(strLineToParse.length());

   if( iLength < 2 ) // an empty name in EPLF, with no info, could be 2 chars
      return false;

   if( IsEPLS(pszLine) )
   {
      if( !ParseEPLF(ftpFileStatus, pszLine, iLength) )
         return false;
   }
   else if( IsUNIXStyleListing(pszLine) )
   {
      if( !ParseUNIXStyleListing(ftpFileStatus, pszLine, iLength) )
         return false;
   }
   else if( IsMultiNetListing(pszLine) )
   {
      if( !ParseMultiNetListing(ftpFileStatus, pszLine, iLength) )
         return false;
   }
   else if( IsMSDOSListing(pszLine) )
   {
      if( !ParseMSDOSListing(ftpFileStatus, pszLine, iLength) )
         return false;
   }
   else
   {
      // Some useless lines, safely ignored:
      // "Total of 11 Files, 10966 Blocks." (VMS)
      // "total 14786" (UNIX)
      // "DISK$ANONFTP:[ANONYMOUS]" (VMS)
      // "Directory DISK$PCSA:[ANONYM]" (VMS)
      return false;
   }

   return true;
}
