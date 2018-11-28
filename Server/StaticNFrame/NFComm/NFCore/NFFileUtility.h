// -------------------------------------------------------------------------
//    @FileName         :    NFFileUtility.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#define _MAX_PATH 260
#endif

#include <list>
#include <string>
#include <cstring>

// File access utility class which provides many help methods
// to make it easy to do operations against file.
class _NFExport NFFileUtility
{
public:
	// Query whether the file name is a valid directory. e.g. ".", ".." are invalid
	// directory name.
	static bool IsValidDirName(const std::string& szName);

	// Query whether strFileName is a absolute path.
	static bool IsAbsolutePath(const std::string& path);

	// Gets absolute path name.
	// @param path If it is already a absolute path, we return it or else we build
	// a absolute path name with current path.
	static std::string GetAbsolutePathName(const std::string& path);

	// This method is copied from Golang project
	// Clean returns the shortest path name equivalent to path
	// by purely lexical processing.  It applies the following rules
	// iteratively until no further processing can be done:
	//
	//	1. Replace multiple Separator elements with a single one.
	//	2. Eliminate each . path name element (the current directory).
	//	3. Eliminate each inner .. path name element (the parent directory)
	//	   along with the non-.. element that precedes it.
	//	4. Eliminate .. elements that begin a rooted path:
	//	   that is, replace "/.." by "/" at the beginning of a path,
	//	   assuming Separator is '/'.
	//
	// The returned path ends in a slash only if it represents a root directory,
	// such as "/" on Unix or `C:\` on Windows.
	//
	// If the result of this process is an empty string, Clean
	// returns the string ".".
	//
	// See also Rob Pike, ``Lexical File Names in Plan 9 or
	// Getting Dot-Dot Right,''
	// http://plan9.bell-labs.com/sys/doc/lexnames.html
	static std::string Clean(const std::string& path);

	// @brief splitting a fully qualified filename into file name and base path dir.
	// @param qualifiedName Path is standardized as in <code>NormalizePath</code>
	// @param file_name[out]
	// @param base_dir[out]
	// @param with_trailing_slash[in] true, End of the string With a SLASH "/".
	static void SplitFileName(const std::wstring& filepath,
		std::wstring& file_name, std::wstring& base_dir, bool with_trailing_slash = false);
	static void SplitFileName(const std::string& filepath,
		std::string& file_name, std::string& base_dir, bool with_trailing_slash = false);

	// Concatenate tow path into one, and return it.
	static std::string Join(const std::string& prefix, const std::string& postfix);
	static std::wstring Join(const std::wstring& prefix, const std::wstring& postfix);

	// @brief Method for standardizing paths - use forward slashes "/" only
	// @param filepath
	// @param with_trailing_slash
	//         true, End of the string With a SLASH "/".
	//         false, without a SLASH "/" at the end of the string
	// @return string
	static std::string NormalizePath(const std::string& filepath, bool with_trailing_slash = true);
	static std::wstring NormalizePath(const std::wstring& filepath, bool with_trailing_slash = true);

	// @brief Gets path's parent directory name. The path can be a normal file or a directory
	// e.g. "D:\test\aab.jpg" ==> "D:/test/" or "D:/test"
	// e.g. "D:/test/abc/" ==> "D:/test/" or "D:/test"
	// @warning The return parent director name is suitable for <code>NormalizePath</code>
	//         That means it uses forward slash "/" as path separator
	// @note It take care of relative path.
	// @param with_trailing_slash
	//         true, End of the string With a SLASH "/".
	//         false, without a SLASH "/" at the end of the string
	// @return string,
	static std::string GetParentDir(const std::string& filepath, bool with_trailing_slash = true);

	// Gets file name without path..
	// e.g. "D:/test/aab.jpg" ==> "aab.jpg"
	static std::string GetFileName(const std::string& filepath);

	// Gets base file name. the file extension will be removed.
	// e.g. "D:/test/aab.jpg" ==> "aab"
	static std::string GetFileNameWithoutExt(const std::string& filepath);

	// Gets file name extension. If can not find the last '.', null string
	// will be returned. e.g. "D:/test/aab.jpg" ==> "jpg"
	static std::string GetFileNameExtension(const std::string& filepath);

	// Gets file path name without end '/'.
	// e.g. "D:/test/aab.jpg" ==> "D:/test"
	static std::string GetFileDirName(const std::string& filepath);

	static std::string GetExcludeFileExt(const std::string &sFullFileName);

	// Quickly remove file or directory from local file system.
	// @param strFileName if end with '/', it is a directory, all the children will be
	// remove, else is a file and the file will be removed.
	// @return bool True if successfully.
	static bool Unlink(const std::string& filepath);

	static bool Remove(const std::string& filepath)
	{
		return Unlink(filepath);
	}

	// @brief Remove the empty directory. If it is not empty or it is not directory, we do nothing and just return true.
	// @remark Empty directory definition: it doesn't has any normal file.
	// e.g. /home/a_dir/b_dir/c_dir
	//     Condition: c_dir has no files in it, and b_dir only has c_dir and a_dir only has b_dir
	//     If we call RemoveDirIfEmpty( "/home/a_dir/b_dir", false ), them b_dir will be deleted and its child c_dir deleted
	//     If we call RemoveDirIfEmpty( "/home/a_dir/b_dir", true ), them a_dir, b_dir, c_dir will all be deleted.
	// @param recursively_delete_empty_parent_dir True, we will delete its parent directory if its parent directory is also empty
	// @return bool True if successfully.
	static bool RemoveDirIfEmpty(const std::string& dir, bool recursively_delete_empty_parent_dir = true);

	// Make directory hierarchy in local file system.
	// Its behavior is similar with the command 'mkdir -p'
	// @param dir Absolute directory file name. e.g. '/home/weizili/test'
	// @return true if successfully
	static bool Mkdir(const std::string& dir);

	static bool Rmdir(const std::string& dir);
	static bool Rmdir(const char* dir);

	// Determines if a file exists and could be opened.
	// @note The file CAN be a directory
	// @param filename is the string identifying the file which should be tested for existence.
	// @return Returns true if file exists, and false if it does not exist or an error occurred.
	static bool IsFileExist(const std::string& filepath);

	static bool IsReadable(const std::string& filepath);
	static bool IsWriteable(const std::string& filepath);
	static bool IsExecutable(const std::string& filepath);

	// @brief copy a file to another place. This function has the same feature like DOS Command 'copy' or Linux Command 'cp'
	// @warning If the destination file is exist, it will be replaced.
	// @warning dest_file MUST BE a file, NOT a directory
	// @param src_file The source file
	// @param src_file The target file
	// @param override True, this will override the old existent file.
	// @return bool True if successfully
	static bool CopyFile(const std::string& src_file, const std::string& dest_file, bool override = true);

	// @brief Query whether the given path is a directory.
	static bool IsDir(const char* filepath);
	static bool IsDir(const std::string& filepath);

	// @brief Walk the directory and get a list of files, excluded directory.
	// @param dir The directory path
	// @param files[out] The list of files are stored here. The file name is with the full path name(include the directory name)
	// @param recursively Whether walk the subdirectories.
	// @param filter Pattern to match against; which can include simple '*' wildcards
	static void GetFiles(const std::string& dir, std::list<std::string>& files, bool recursively = true, const std::string& filter = "*");

	// @brief Walk the directory and get a list of files, not include directory.
	// @param dir The directory path
	// @param files[out] The list of files are stored here. The file name is with the full path name(include the directory name)
	// @param depth The depth to walk the subdirectories. 0 means only walk the top dir strDirName
	// @param filter Pattern to match against; which can include simple '*' wildcards
	static void GetFiles(const std::string& dir, std::list<std::string>& files, int depth, const std::string& filter = "*");

	// Extract strings from the a file line-by-line,
	// every line content as a list element will be inserted to a list.
	static bool ReadFile(const char* filepath, std::list<std::string>& lines);

	static bool ReadFileContent(const std::string& strFileName, std::string& strContent);

	// @brief Write the data into a file.
	//  The file is created if it does not exist, otherwise it is truncated
	// @param const char* filepath - The file path where the data is stored
	// @param const void* content - The file content
	// @param const size_t len - The length of the content
	// @return true if successfully
	static bool WriteFile(const char* filepath, const void* content, const size_t len);
	static bool WriteFile(const std::string& filepath, const void* content, const size_t len);
};

inline bool NFFileUtility::IsValidDirName(const std::string& szName)
{
	size_t len = szName.length();
	return !((len >= 1 && szName[0] == ('.')) ||
		(len >= 2 && szName[1] == '.'));
}

// Query whether strFileName is a absolute path.
inline bool NFFileUtility::IsAbsolutePath(const std::string& path)
{
	if (path.size() == 0)
	{
		return false;
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	if (isalpha(static_cast<unsigned char>(path[0])) && path[1] == ':')
	{
		return true;
	}
	else
	{
		return false;
	}

#else
	return path[0] == '/';
#endif
}

inline std::string NFFileUtility::GetAbsolutePathName(const std::string& strPathName)
{
	std::string strTmp = strPathName;

	if (!IsAbsolutePath(strTmp))
	{
		//
		char szDir[_MAX_PATH ];
#if NF_PLATFORM == NF_PLATFORM_WIN
		if (_getcwd(szDir, _MAX_PATH))
		{
			strTmp = std::string(szDir) + ("/") + strTmp;
		}
#else
		getcwd(szDir, _MAX_PATH);
		strTmp = string(szDir) + ("/") + strTmp;
#endif
	}

	// convert file name here.
	NormalizePath(strTmp);

	return strTmp;
}

inline std::string NFFileUtility::GetFileName(const std::string& strFileName)
{
	// since '/' equal to L'/', we can cast to either one.
	std::string::size_type i = strFileName.find_last_of((std::string::value_type)'/');

	if (i == std::string::npos)
	{
		return strFileName;
	}

	return strFileName.substr(i + 1);
}

inline std::string NFFileUtility::GetFileNameWithoutExt(const std::string& strFileName)
{
	// since '/' equal to L'/', we can cast to either one.
	std::string::size_type iStart = strFileName.find_last_of((std::string::value_type)'/');

	if (iStart == std::string::npos)
	{
		iStart = 0;
	}
	else
	{
		// skip the '/'
		++iStart;
	}

	return strFileName.substr(iStart, strFileName.find_last_of((std::string::value_type)'.') - iStart);
}

inline std::string NFFileUtility::GetFileNameExtension(const std::string& strFileName)
{
	std::string::size_type endPos = strFileName.find_last_of((std::string::value_type)'.');

	if (endPos < strFileName.length() - 1)
	{
		return strFileName.substr(endPos + 1);
	}

	return std::string();
}

inline std::string NFFileUtility::GetFileDirName(const std::string& strFileName)
{
	// since '/' equal to L'/', we can cast to either one.
	std::string::size_type i = strFileName.find_last_of((std::string::value_type)'/');

	if (i == std::string::npos)
	{
		return std::string();
	}

	return strFileName.substr(0, i);
}

inline std::string NFFileUtility::GetExcludeFileExt(const std::string &sFullFileName)
{
	std::string::size_type pos;
	if ((pos = sFullFileName.rfind('.')) == std::string::npos)
	{
		return sFullFileName;
	}

	return sFullFileName.substr(0, pos);
}

inline bool NFFileUtility::IsDir(const char* szFileName)
{
	if (!szFileName)
	{
		return false;
	}

	const std::string strFileName(szFileName);

	return IsDir(strFileName);
}

inline bool NFFileUtility::IsDir(const std::string& strFileName)
{
	//for the 'stat' API, the parameter strFileName SHOULD not contain a trailing backslash
	std::string strStandardisePath = NormalizePath(strFileName, false);

	struct stat st;
	if (0 != stat(strStandardisePath.c_str(), &st))
	{
		return false;
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	if ((st.st_mode & _S_IFDIR) == _S_IFDIR)
#else
	if (S_ISDIR(st.st_mode))
#endif
	{
		return true;
	}

	return false;
}

inline bool NFFileUtility::RemoveDirIfEmpty(const std::string& strDirName, bool bRecursivelyDeleteEmptyParentDir /*= true*/)
{
	if (!IsDir(strDirName))
	{
		return true;
	}

	bool retval = true;

	std::list<std::string> files;
	GetFiles(strDirName, files, true);

	if (files.size() == 0)
	{
		if (NFFileUtility::Rmdir(strDirName.c_str()) != 0)
		{
			retval = false;
		}
	}

	if (bRecursivelyDeleteEmptyParentDir)
	{
		std::string strParentDir = GetParentDir(strDirName);

		if (!RemoveDirIfEmpty(strParentDir, bRecursivelyDeleteEmptyParentDir))
		{
			retval = false;
		}
	}

	return retval;
}

inline std::string NFFileUtility::GetParentDir(const std::string& strFileName, bool with_trailing_slash /*= true*/)
{
	std::string path = NormalizePath(strFileName, false);

	if (!IsAbsolutePath(strFileName))
	{
		path = GetAbsolutePathName(path);
	}

	size_t pos = path.find_last_of('/');

	if (pos == 0 || pos == std::string::npos)
	{
		if (with_trailing_slash)
		{
			path += "/";
		}

		return path;
	}
	else
	{
		if (with_trailing_slash)
		{
			std::string retp = std::string(path.c_str(), pos) + "/";
			return retp;
		}
		else
		{
			return std::string(path.c_str(), pos);
		}
	}
}

inline bool NFFileUtility::Rmdir(const std::string& strDirName)
{
	return Rmdir(strDirName.c_str());
}

inline bool NFFileUtility::Rmdir(const char* strDirName)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	if (::_rmdir(strDirName) == 0)
#else
	if (::rmdir(strDirName) == 0)
#endif
	{
		return true;
	}

	return false;
}

