// -------------------------------------------------------------------------
//    @FileName         :    NFMapEx.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//
// -------------------------------------------------------------------------

#pragma once

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include "NFPlatform.h"

template <typename T , typename TD>
class NFMapEx
{
public:
    typedef std::map<T, NF_SHARE_PTR<TD> > NFMapOBJECT;

    NFMapEx()
    {
    };
    virtual ~NFMapEx()
    {
    };

    virtual bool ExistElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectMap.find(name);
        if (itr != mObjectMap.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual bool AddElement(const T& name, const NF_SHARE_PTR<TD> data)
    {
        if (data == nullptr)
        {
            return false;
        }

        typename NFMapOBJECT::iterator itr = mObjectMap.find(name);
        if (itr == mObjectMap.end())
        {
            mObjectMap.insert(typename NFMapOBJECT::value_type(name, data));

            return true;
        }

        return false;
    }

    virtual bool RemoveElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectMap.find(name);
        if (itr != mObjectMap.end())
        {
            mObjectMap.erase(itr);

            return true;
        }

        return false;
    }

    virtual TD* GetElementNude(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectMap.find(name);
        if (itr != mObjectMap.end())
        {
            return itr->second.get();
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> GetElement(const T& name)
    {
        typename NFMapOBJECT::iterator itr = mObjectMap.find(name);
        if (itr != mObjectMap.end())
        {
            return itr->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual TD* FirstNude(T& name)
    {
        if (mObjectMap.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectMap.begin();
        if (mObjectCurIter != mObjectMap.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return nullptr;
        }
    }

    virtual TD* NextNude(T& name)
    {
        if (mObjectCurIter == mObjectMap.end())
        {
            return nullptr;
        }

	    ++mObjectCurIter;
        if (mObjectCurIter != mObjectMap.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second.get();
        }
        else
        {
            return nullptr;
        }
    }
    virtual TD* FirstNude()
    {
        if (mObjectMap.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectMap.begin();
        if (mObjectCurIter != mObjectMap.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return nullptr;
        }
    }
    virtual TD* NextNude()
    {
        if (mObjectCurIter == mObjectMap.end())
        {
            return nullptr;
        }

	    ++mObjectCurIter;
        if (mObjectCurIter != mObjectMap.end())
        {
            return mObjectCurIter->second.get();
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> First()
    {
        if (mObjectMap.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectMap.begin();
        if (mObjectCurIter != mObjectMap.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> Next()
    {
        if (mObjectCurIter == mObjectMap.end())
        {
            return nullptr;
        }

        ++mObjectCurIter;
        if (mObjectCurIter != mObjectMap.end())
        {
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> First(T& name)
    {
        if (mObjectMap.size() <= 0)
        {
            return nullptr;
        }

        mObjectCurIter = mObjectMap.begin();
        if (mObjectCurIter != mObjectMap.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    virtual NF_SHARE_PTR<TD> Next(T& name)
    {
        if (mObjectCurIter == mObjectMap.end())
        {
            return nullptr;
        }

	    ++mObjectCurIter;
        if (mObjectCurIter != mObjectMap.end())
        {
            name = mObjectCurIter->first;
            return mObjectCurIter->second;
        }
        else
        {
            return nullptr;
        }
    }

    int Count()
    {
        return (int)mObjectMap.size();
    }

    bool Empty()
    {
        return mObjectMap.empty();
    }

	virtual bool ClearAll()
    {
        mObjectMap.clear();
        return true;
    }

    typename NFMapOBJECT::iterator Begin()
    {
        return mObjectMap.begin();
    }

    typename NFMapOBJECT::iterator End()
    {
        return mObjectMap.end();
    }

    typename NFMapOBJECT::const_iterator Begin() const
    {
        return mObjectMap.begin();
    }

    typename NFMapOBJECT::const_iterator End() const
    {
        return mObjectMap.end();
    }
protected:
    NFMapOBJECT     mObjectMap;
    typename NFMapOBJECT::iterator mObjectCurIter;
};