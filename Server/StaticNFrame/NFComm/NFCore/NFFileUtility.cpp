// -------------------------------------------------------------------------
//    @FileName         :    NFFileUtility.cpp
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFFileUtility.h"

#include "NFStringUtility.h"
#include "NFDataStream.h"

#include <fstream>

string NFFileUtility::Clean(const string& path)
{
	// Not Tested yet.

	std::vector<string> v;
	NFStringUtility::Split(v, path, "/\\", 0);

	std::ostringstream newpath;

	// in case of root dir.
	if (path[0] == '/')
	{
		newpath << static_cast<char>('/');
	}

	// erase null.
	std::list<string> vv;
	for (auto it = v.rbegin(), ite = v.rend(); it != ite; ++it)
	{
		if (*it == ".")
		{
			continue;
		}
		else if (*it == "..")
		{
			// erase none '.' parent dir.
			while (++it != ite
				&& *it == ".")
			{
				// do nothing.
			}

			// if first of the path is "../" , should reserve it.
			if (it == ite)
			{
				vv.push_front("..");
				break;
			}
		}
		else
		{
			vv.push_front(*it);
		}
	}

	// build string
	for (auto it = vv.begin(), ite = vv.end(); it != ite;)
	{
		newpath << *it;
		if (++it == ite)
		{
			break;
		}
		newpath << static_cast<char>('/');
	}

	return newpath.str();
}

string NFFileUtility::Join(const string& prefix, const string& postfix)
{
	string pre = prefix;
	string post = postfix;
	pre = NormalizePath(pre, false);
	post = NormalizePath(post, false);
	NFStringUtility::Trim(post, "/", true, false);
	if (NFStringUtility::EndsWith(postfix, "/"))
	{
		post += "/";
	}
	return pre + "/" + post;
}

wstring NFFileUtility::Join(const wstring& prefix, const wstring& postfix)
{
	typedef wstring MyString;
	MyString pre = prefix;
	MyString post = postfix;
	pre = NormalizePath(pre, false);
	post = NormalizePath(post, false);
	NFStringUtility::Trim(post, L"/", true, false);
	if (NFStringUtility::EndsWith(postfix, L"/"))
	{
		post += L"/";
	}
	return pre + L"/" + post;
}

string NFFileUtility::NormalizePath(const string& init, bool with_trailing_slash /*= true*/)
{
	if (init.length() < 1)
	{
		return init;
	}

	string path = init;

	std::replace(path.begin(), path.end(), '\\', '/');

	if (path[path.length() - 1] != '/' && with_trailing_slash)
	{
		path += "/";
	}
	else if (path[path.length() - 1] == '/' && !with_trailing_slash)
	{
		NFStringUtility::Trim(path, "/", false, true);
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	NFStringUtility::ToLower(path);
#endif

	return path;
}

wstring NFFileUtility::NormalizePath(const wstring& init, bool with_trailing_slash /*= true*/)
{
	if (init.length() < 1)
	{
		return init;
	}

	wstring path = init;
	std::replace(path.begin(), path.end(), L'\\', L'/');
	if (path[path.length() - 1] != L'/' && with_trailing_slash)
	{
		path += L"/";
	}
	else if (path[path.length() - 1] == L'/' && !with_trailing_slash)
	{
		NFStringUtility::Trim(path, L"/", false, true);
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	NFStringUtility::ToLower(path);
#endif
	return path;
}

void NFFileUtility::SplitFileName(const string& filepath,
	string& base, string& dir_path, bool with_trailing_slash /*= false */)
{
	string path = filepath;
	// Replace \ to /
	std::replace(path.begin(), path.end(), '\\', '/');
	// split based on final /
	size_t i = path.find_last_of('/');
	if (i == string::npos)
	{
		//dir_path = "";
		dir_path.clear();
		base = filepath;
	}
	else
	{
		base = path.substr(i + 1, path.size() - i - 1);
		if (with_trailing_slash)
		{
			dir_path = path.substr(0, i + 1);
		}
		else
		{
			dir_path = path.substr(0, i);
		}
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	NFStringUtility::ToLower(base);
	NFStringUtility::ToLower(dir_path);
#endif
}

void NFFileUtility::SplitFileName(const wstring& filepath,
	wstring& base, wstring& dir_path, bool with_trailing_slash /*= false */)
{
	wstring path = filepath;
	// Replace \ with / first
	std::replace(path.begin(), path.end(), L'\\', L'/');
	// split based on final /
	size_t i = path.find_last_of(L'/');

	if (i == wstring::npos)
	{
		//dir_path = L"";
		dir_path.clear();
		base = filepath;
	}
	else
	{
		base = path.substr(i + 1, path.size() - i - 1);
		if (with_trailing_slash)
		{
			dir_path = path.substr(0, i + 1);
		}
		else
		{
			dir_path = path.substr(0, i);
		}
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	NFStringUtility::ToLower(base);
	NFStringUtility::ToLower(dir_path);
#endif
}

bool NFFileUtility::Unlink(const string& filepath)
{
	bool is_dir = NFFileUtility::IsDir(filepath);

	if (is_dir)
	{
		assert(0 && "not supported");
		return false;
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	return ::DeleteFileA(filepath.c_str()) ? true : false;
#else
	return remove(filepath.c_str()) == 0;
#endif
}

bool NFFileUtility::Mkdir(const string& dirpath)
{
	size_t nCurSplit;
	size_t nNextSplit = 0;

	int iret;

	do
	{
		// get current dir name.
		nCurSplit = dirpath.find_first_of(static_cast<string::value_type>('/'), nNextSplit);
		nNextSplit = dirpath.find_first_of(static_cast<string::value_type>('/'), nCurSplit + 1);

#if NF_PLATFORM == NF_PLATFORM_WIN
		// current dir
		iret = ::_mkdir(dirpath.substr(0, nNextSplit).c_str());
#else
		// current dir
		iret = ::mkdir(dirpath.substr(0, nNextSplit).c_str(), 0755);
#endif

		// next dir
		//nCurSplit = nNextSplit + 1;
	} while (nNextSplit != string::npos);

	return true;
}

#if NF_PLATFORM == NF_PLATFORM_WIN
// 00
// Existence only
//
// 02
// Write-only
//
// 04
// Read-only
//
// 06
// Read and write
#define F_OK  0
#define W_OK  2
#define R_OK  4
#define RW_OK 6
#define X_OK  1
#endif

bool NFFileUtility::IsFileExist(const string& strFileName)
{
	if (access(strFileName.c_str(), F_OK) != 0)
	{
		return false;
	}
	return true;
}

bool NFFileUtility::IsReadable(const string& strFileNmae)
{
	if (access(strFileNmae.c_str(), R_OK) != 0)
	{
		return false;
	}
	return true;
}

bool NFFileUtility::IsWriteable(const string& strFileNmae)
{
	if (access(strFileNmae.c_str(), W_OK) != 0)
	{
		return false;
	}
	return true;
}

bool NFFileUtility::IsExecutable(const string& strFileNmae)
{
	if (access(strFileNmae.c_str(), X_OK) != 0)
	{
		return false;
	}
	return true;
}

bool NFFileUtility::CopyFile(const string& strSrcFile, const string& strDestFile, bool bOverride/* = true */)
{
	if (IsDir(strSrcFile))
	{
		return false;
	}

	string strDestFilePath = strDestFile;

	if (IsDir(strDestFile))
	{
		strDestFilePath = Join(strDestFile, GetFileName(strSrcFile));
	}

	if (IsFileExist(strDestFilePath) && !bOverride)
	{
		return false;
	}

	NFDataStream databuf;
	databuf.ReadFile(strSrcFile);
	databuf.WriteFile(strDestFile);
	return true;
}

bool NFFileUtility::ReadFileContent(const std::string& strFileName, std::string& strContent)
{
	FILE* fp = fopen(strFileName.c_str(), "rb");
	if (!fp)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	const long filelength = ftell(fp);
	if (filelength < 0)
	{
		fclose(fp);
		return false;
	}
	fseek(fp, 0, SEEK_SET);
	strContent.resize(static_cast<size_t>(filelength));
	fread((void*)strContent.data(), static_cast<size_t>(filelength), 1, fp);
	fclose(fp);

	return true;
}

bool NFFileUtility::ReadFile(const char* szFileName, std::list<string>& lines)
{
	if (!szFileName)
	{
		return false;
	}

	std::ifstream ifs(szFileName, std::ios::binary);
	if (!ifs)
	{
		return false;
	}

	string line;

	while (!ifs.eof())
	{
		getline(ifs, line);
		lines.push_back(line);
	}

	return true;
}

bool NFFileUtility::WriteFile(const string& filepath, const void* content, const size_t len)
{
	return WriteFile(filepath.c_str(), content, len);
}

bool NFFileUtility::WriteFile(const char* filepath, const void* content, const size_t len)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	const char* mode = "wb+";
#else
	const char* mode = "w+";
#endif
	FILE* fp = ::fopen(filepath, mode);

	if (fp == nullptr)
	{
		fprintf(stderr, "%s : could not open file \"%s\" for write\n", __func__, filepath);
		return false;
	}

	::fwrite(content, sizeof(char), len, fp);
	::fflush(fp);
	::fclose(fp);

	return true;
}

void NFFileUtility::GetFiles(const string& strDirName, std::list<string>& files, bool recursively/* = true*/, const string& filter/* = "*.*"*/)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	struct _finddata_t finfo; //file info

	intptr_t hFile = _findfirst(NFFileUtility::Join(strDirName, "*").c_str(), &finfo);
	if (-1 == hFile)
	{
		return;
	}

	do
	{
		if ((finfo.attrib & _A_SUBDIR))
		{
			if (recursively && strcmp(finfo.name, ".") != 0 && strcmp(finfo.name, "..") != 0)
			{
				GetFiles(NFFileUtility::Join(strDirName, finfo.name), files, recursively, filter);
			}
		}
		else
		{
			string strFileName = NFFileUtility::Join(strDirName, finfo.name);
			if (NFStringUtility::Match(strFileName, filter))
			{
				files.push_back(strFileName);
			}
		}
	} while (_findnext(hFile, &finfo) == 0);

	_findclose(hFile);

#else
	struct dirent** namelist;
	int n = scandir(strDirName.c_str(), &namelist, 0, alphasort);

	if (n < 0) {
		return;
	}

	for (int i = 0; i < n; ++i) {
		if (namelist[i]->d_type == DT_DIR) {
			if (recursively && strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0) {
				GetFiles(NFFileUtility::Join(strDirName, namelist[i]->d_name), files, recursively, filter);
			}
		}
		else {
			string strFileName = NFFileUtility::Join(strDirName, namelist[i]->d_name);

			if (NFStringUtility::Match(strFileName, filter)) {
				files.push_back(strFileName);
			}
		}
		free(namelist[i]);
	}
	free(namelist);
#endif
}

void NFFileUtility::GetFiles(const string& strDirName, std::list<string>& files, int depth /*= 0*/, const string& filter/* = "*"*/)
{
	if (depth < 0)
	{
		return;
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	struct _finddata_t finfo; //file info

	//if ( ( hFile = _findfirst( p.assign( strDirName ).append( "/*" ).append( "*" ).c_str(), &fileinfo ) ) != -1 )
	intptr_t hFile = _findfirst(NFFileUtility::Join(strDirName, "*").c_str(), &finfo);
	if (-1 == hFile)
	{
		return;
	}

	do
	{
		string strFileName = NFFileUtility::Join(strDirName, finfo.name);
		if ((finfo.attrib & _A_SUBDIR))
		{
			if (strcmp(finfo.name, ".") != 0 && strcmp(finfo.name, "..") != 0)
			{
				if (NFStringUtility::Match(strFileName, filter))
				{
					files.push_back(strFileName);
				}

				if (depth > 0)
				{
					GetFiles(strFileName, files, depth - 1, filter);
				}
			}
		}
		else
		{
			if (NFStringUtility::Match(strFileName, filter))
			{
				files.push_back(strFileName);
			}
		}
	} while (_findnext(hFile, &finfo) == 0);

	_findclose(hFile);

#else
	struct dirent** namelist = NULL;
	int n = scandir(strDirName.c_str(), &namelist, 0, alphasort);

	if (n < 0) {
		std::ostringstream log;
		log << "scandir failed. return value = -1, errno=" << errno;
		//H_LOG_NAME_DEBUG( "FileUtil", log.str() );
		return;
	}

	for (int i = 0; i < n; ++i) {
		string strFileName = NFFileUtility::Join(strDirName, namelist[i]->d_name);
		if (namelist[i]->d_type == DT_DIR) {
			if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0) {
				if (NFStringUtility::Match(strFileName, filter)) {
					files.push_back(strFileName);
				}

				if (depth > 0) {
					GetFiles(strFileName, files, depth - 1, filter);
				}
			}
		}
		else {
			if (NFStringUtility::Match(strFileName, filter)) {
				files.push_back(strFileName);
			}
		}
		free(namelist[i]);
	}
	free(namelist);
#endif
}