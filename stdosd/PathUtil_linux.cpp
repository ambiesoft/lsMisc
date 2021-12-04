//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include <memory>
#include <algorithm>
#include <list>
#include <vector>
#include <locale>
#include <codecvt>
#include <unistd.h>

#include "stdosd.h"

#include "PathUtil.h"

using namespace std;




namespace Ambiesoft { namespace stdosd {

using namespace ::Ambiesoft::stdosd;
u16string stdPathCanonicalize(const char16_t* pPath)
{
    if(pPath==nullptr || pPath[0]==0)
        return u16string();

    if(pPath[0]==u'/' && pPath[1]==0)
        return pPath;

    //        u16string path(pPath);
    //        bool bRoot = false;
    //        if(pPath[0]==u'/')
    //        {
    //            bRoot=true;
    //            path = path.substr(1);
    //        }

    vector<u16string> v = stdSplitString(u16string(pPath), u16string(u"/"));

    list<u16string> retV;
    list<u16string>::iterator prevIt;
    // for(size_t i=0 ; i < v.size(); ++i)
    for(vector<u16string>::iterator it = v.begin() ; it != v.end(); ++it)
    {
        if(*it==u"..")
        {
            if(!retV.empty())
                retV.pop_back();// erase(prevIt);
            continue;
        }
        else if(*it==u".")
        {
            continue;
        }


        retV.push_back(*it);
    }

    // /a => "", "a"
    // /a/ => "", "a", ""

    u16string ret;
    for(list<u16string>::iterator it=retV.begin(); it != retV.end(); ++it)
    {
        u16string& s = *it;

        if(s.empty() && ret.empty())
        {
            ret += u"/";
            continue;
        }



        ret += s;

        list<u16string>::iterator itTmp = it;
        ++itTmp;
        if(itTmp == retV.end())
        {
            break;
        }

        ret += u"/";
    }
    return ret;
}

static u16string tou16string(const string& s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    // std::string narrow = converter.to_bytes(wide_utf16_source_string);
    std::u16string restored = converter.from_bytes(s.c_str());
    //std::u16string wide = converter.from_bytes(s.data());
    return restored;
}

//server/			should be treated as root
//server/folder		should not be treated as root
//server/folder/aaa	should not be treated as root
bool IsPathRoot(const char16_t* pPath)
{
    if (pPath == nullptr || pPath[0] == 0)
        return false;

    u16string path;
    if(pPath[0] != u'/')
    {
        // not fullpath
        unique_ptr<char> cur(get_current_dir_name());
        u16string wcur = tou16string(string(cur.get()));
        if(wcur.empty() || wcur[wcur.length()-1] != u'/')
            wcur += u"/";
        wcur += pPath;
        path = wcur;
    }
    else
    {
        path = pPath;
    }
    u16string tmp = stdPathCanonicalize(path.c_str());
    return tmp.empty() || tmp==u"/";
}

bool IsBothEmptyOrNULL(const char16_t* p1, const char16_t* p2)
{
    if (p1 == nullptr || p1[0] == 0)
        return p2 == nullptr || p2[0] == 0;

    return false;
}

static u16string myRemoveLastSlash(const u16string& s)
{
    if(s.empty())
        return s;
    if(s[s.length()-1]==u'/')
        return s.substr(0,s.length()-1);
    return s;
}
int myPathGetComponentCount(const char16_t* p)
{
    u16string s = p;
    s = myRemoveLastSlash(s);
    auto n = std::count(s.begin(), s.end(), u'/');

    return static_cast<int>(n);
}
bool IsSamePathDepth(const char16_t* p1org, const char16_t* p2org)
{
    return myPathGetComponentCount(p1org) == myPathGetComponentCount(p2org);
}

bool IsPathSame(const char16_t* p1, const char16_t* p2)
{
    return u16string(p1)==u16string(p2);
    // return wcscmp(p1, p2) == 0;
}


static u16string myPathCommonPrefix(const char16_t* p1, const char16_t* p2)
{
    if(!p1 || !p1[0])
        return u16string();
    if(!p2 || !p2[0])
        return u16string();

    u16string u1(p1);
    u16string u2(p2);
    size_t len1 = u1.size();
    size_t len2 = u2.size();
    size_t minlen = std::min(len1,len2);
    size_t i;
    for(i=0 ; i < minlen; ++i)
    {
        if(p1[i] != p2[i])
            break;
    }

    u16string ret(p1);
    return ret.substr(0,i);
}
bool IsPathChildIncluded(const char16_t* p1org, const char16_t* p2org, std::u16string* pDupPath)
{
    // http://d.hatena.ne.jp/s-kita/20101206/1291651401#PathCommonPrefix
    //        u16string lpCommonPrefix;
    //		int nCommonPrefixCharcters;

    u16string p1(p1org);
    u16string p2(p2org);

    if(p1==u"/") {
        if(pDupPath)
            *pDupPath=u"/";
        return true;
    }
    if(!p1.empty() && p1[p1.length()-1]==u'/')
        p1.substr(0, p1.length()-1);
    if(!p2.empty() && p2[p2.length()-1]==u'/')
        p2.substr(0, p2.length()-1);

    u16string w1 = p1;
    u16string w2 = p2;

    u16string szT1,szT2;

    // TODO: expand env values
    //		if (ExpandEnvironmentStrings(w1.c_str(), szT1, _countof(szT1)) > _countof(szT1))
    //			return FALSE;
    //		if (ExpandEnvironmentStrings(w2.c_str(), szT2, _countof(szT2)) > _countof(szT2))
    //			return FALSE;
    //		w1 = szT1;
    //		w2 = szT2;

    // TODO: canonicalize
    szT1 = stdPathCanonicalize(w1.c_str());
    szT2 = stdPathCanonicalize(w2.c_str());

    u16string dupPath;
    dupPath = myPathCommonPrefix(szT1.c_str(), szT2.c_str());


    //		nCommonPrefixCharcters = PathCommonPrefix(szT1, szT2, lpCommonPrefix);
    //		if (nCommonPrefixCharcters < 1)
    //		{
    //			return FALSE;
    //		}

    if (!IsPathSame(szT1.c_str(), dupPath.c_str()) && !IsPathSame(szT2.c_str(), dupPath.c_str()))
    {
        return false;
    }

    if (IsSamePathDepth(szT1.c_str(), szT2.c_str()))
    {
        return false;
    }

    if (pDupPath)
        *pDupPath = dupPath;

    return dupPath != u"/";//true;
}


static bool HasDupPaths(
        const u16string& left,
        const vector<u16string>& saPaths,
        size_t startindex,
        size_t& hitindex,
        u16string& common)
{
    for (size_t i = startindex; i < saPaths.size(); ++i)
    {
        u16string dupParent;
        if (IsPathChildIncluded(left.c_str(), saPaths[i].c_str(), &dupParent))
        {
            common = dupParent;
            hitindex = i;
            return true;
        }
    }
    return false;
}

bool checkDupPaths(const vector<u16string>& saPaths,
                   u16string& left,
                   u16string& right,
                   u16string& common)
{
    for (size_t i = 0; i < saPaths.size(); ++i)
    {
        u16string tmpleft = saPaths[i];
        size_t hitindex = 0;
        if (HasDupPaths(tmpleft, saPaths, i + 1, hitindex, common))
        {
            left = tmpleft;
            right = saPaths[hitindex];
            return false;
        }
    }
    return true;
}
}}
