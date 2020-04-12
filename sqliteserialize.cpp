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

#pragma warning(disable :4786)
#include "stdafx.h"
#include <cassert>
#include <string>
#include <memory>
#include <stlsoft/smartptr/scoped_handle.hpp>
#include "sqlite3.h"
#include "IsFileExists.h"
#include "UTF16toUTF8.h"

#include "stdosd/stdosd.h"

#include "sqliteserialize.h"

#include "DebugNew.h"

#if (_MSC_VER <= 1400)
typedef WCHAR* RPC_WSTR;
#endif
#pragma comment(lib,"Rpcrt4.lib")

using namespace std;
using namespace Ambiesoft::stdosd;

// #define IFDISABLED_RETURN do { if(smDisabled_){ return; } } while(false)
#define SAFE_SQLERROR_FREE(s) do { if(s){sqlite3_free(s);     (s)=NULL;} }while(false)
#define SAFE_SQLFINALIZE(s)   do { if(s){sqlite3_finalize(s); (s)=NULL;} }while(false)

#define SQLVERIFY(s) do { if(s != SQLITE_OK) { assert(false);}} while(false);

namespace Ambiesoft {

	using std::wstring;
	using std::vector;

	static int callbackt(void *pString, int argc, char **argv, char **azColName)
	{
		std::string* pResult = (std::string*)pString;
		for (int i = 0; i < argc; i++)
		{
			std::string strt;
			strt = argv[i] ? argv[i] : "NULL";
			(*pResult) += strt;
			if ((i + 1) < argc)
				(*pResult) += "\t";
		}
		(*pResult) += "\r\n";
		return 0;
	}

	int infinite_sqlite3_prepare16_v2(
		sqlite3* db,              /* Database handle. */
		LPCWSTR zSql,         /* UTF-16 encoded SQL statement. */
		int nBytes,               /* Length of zSql in bytes. */
		sqlite3_stmt** ppStmt,    /* OUT: A pointer to the prepared statement */
		const void** pzTail       /* OUT: End of parsed string */
	)
	{
		int ret;
		for (;;)
		{
			ret = sqlite3_prepare16_v2(
				db,
				zSql,
				nBytes,
				ppStmt,
				pzTail
			);
			if (ret != SQLITE_BUSY)
				break;

			Sleep(100);
		}
		return ret;
	}
	int inifinite_sqlite3_step(sqlite3_stmt* pStmt) {
		int ret;
		for (;;) {
			ret = sqlite3_step(pStmt);
			if (ret != SQLITE_BUSY)
				break;

			Sleep(100);
		}
		return ret;
	}

	class CSqlError
	{
		char* pError_ = nullptr;
	public:
		CSqlError() {}
		~CSqlError() {
			if (pError_)
				sqlite3_free(pError_);
		}
		char** address() {
			return &pError_;
		}
		bool isError() {
			return pError_ != nullptr;
		}
	};


	class CSqlFile
	{
		sqlite3* pFile_ = nullptr;
		std::map<LPCWSTR, sqlite3_stmt*> stmtInserts_;
		std::map<LPCWSTR, sqlite3_stmt*> stmtSelectors_;
		std::map<LPCWSTR, sqlite3_stmt*> stmtDeleters_;

		CSqlFile(const CSqlFile&) = delete;
		CSqlFile(CSqlFile&&) = delete;
	public:
		CSqlFile(LPCWSTR pFileName, LPCWSTR pTableNameW, bool bCreate) {
			const std::string tableName = toStdUtf8String(pTableNameW);
			if (!IsFileExistsW(pFileName))
			{
				if (!bCreate)
					return;

				if (SQLITE_OK != sqlite3_open16(pFileName, &pFile_))
					return;
				
				std::string sql = stdFormat(
					"CREATE TABLE [%s] ( "
					"[c1] TEXT, "
					"[c2] TEXT "
					");",
					// "PRIMARY KEY([c1]));",
					tableName.c_str());
				CSqlError error;
				std::string dummy;
				if (SQLITE_OK != sqlite3_exec(get(),
					sql.c_str(),
					callbackt,
					&dummy,
					error.address()) || error.isError())
				{
					assert(false);
				}
			}

			if (!pFile_)
				if (SQLITE_OK != sqlite3_open16(pFileName, &pFile_))
					return;
			
			CSqlError error;
			std::string sql = stdFormat(
				"ALTER TABLE [%s] ADD COLUMN dorder INT;",
				tableName.c_str());

			std::string dummy;
			sqlite3_exec(get(),
				sql.c_str(),
				callbackt,
				&dummy,
				error.address());
			
			// TODO check table
		}
		~CSqlFile() {
			for (auto&& stmt : stmtInserts_)
				sqlite3_finalize(stmt.second);
			for (auto&& stmt : stmtSelectors_)
				sqlite3_finalize(stmt.second);
			for (auto&& stmt : stmtDeleters_)
				sqlite3_finalize(stmt.second);

			if (pFile_)
			{
				SQLVERIFY(sqlite3_close(pFile_));
			}
		}
		bool ok() const {
			return pFile_ != nullptr;
		}
		sqlite3* get() {
			return pFile_;
		}
		sqlite3_stmt* selector(LPCWSTR pAppName) {
			if (stmtSelectors_.find(pAppName) != stmtSelectors_.end())
				return stmtSelectors_[pAppName];

			sqlite3_stmt* pStmtSelect = nullptr;

			//LPCWSTR pKata = L"SELECT c2 FROM [%s] WHERE c1 = ?;";
			//LPWSTR p = (LPWSTR)malloc(wcslen(pKata) * 2 + wcslen(lpAppName) * 2 + 2);
			//stlsoft::scoped_handle<void*> mahh((void*)p, free);
			//wsprintf(p, pKata, lpAppName);
			std::wstring sql = stdFormat(
				L"SELECT c2 FROM [%s] WHERE c1 = ?;",
				pAppName);
			
			int ret = infinite_sqlite3_prepare16_v2(get(),
				sql.c_str(),
				-1,
				&pStmtSelect,
				0);

			if (ret != SQLITE_OK || !pStmtSelect)
			{
				assert(false);
				return nullptr;
			}

			stmtSelectors_[pAppName] = pStmtSelect;
			return pStmtSelect;
		}

		sqlite3_stmt* inserter(LPCWSTR pAppName) {
			if (stmtInserts_.find(pAppName) != stmtInserts_.end())
				return stmtInserts_[pAppName];

			sqlite3_stmt* stmt = nullptr;
			std::wstring sql = stdFormat(
				L"INSERT INTO [%s] VALUES (?, ?, 0);",
				pAppName);
			int ret = infinite_sqlite3_prepare16_v2(get(),
				sql.c_str(),
				-1,
				&stmt,
				0);
			if (SQLITE_OK != ret || !stmt) {
				assert(false);
				return false;
			}

			stmtInserts_[pAppName] = stmt;
			return stmt;
		}
		sqlite3_stmt* deleter(LPCWSTR pAppName) {
			if (stmtDeleters_.find(pAppName) != stmtDeleters_.end())
				return stmtDeleters_[pAppName];

			sqlite3_stmt* stmt = nullptr;
			std::wstring sql = stdFormat(
				L"DELETE FROM [%s] WHERE c1 = ?;",
				pAppName);
			int ret = infinite_sqlite3_prepare16_v2(get(),
				sql.c_str(),
				-1,
				&stmt,
				0);
			if (SQLITE_OK != ret || !stmt) {
				assert(false);
				return false;
			}

			stmtDeleters_[pAppName] = stmt;
			return stmt;
		}
	};

	static void cfree(char*p)
	{
		free((void*)p);
	}


	
	//static BOOL createDB(LPCWSTR pDBPath, LPCWSTR pTableNameW)
	//{
	//	const std::string tableName = toStdUtf8String(pTableNameW);
	//	std::string strResult;
	//	CSqlFile db(pDBPath, pTableNameW);
	//	
	//	if (!db.ok())
	//		return FALSE;

	//	std::string sql = stdFormat(
	//		"CREATE TABLE [%s] ( "
	//		"[c1] TEXT, "
	//		"[c2] TEXT "
	//		");",
	//		// "PRIMARY KEY([c1]));",
	//		tableName.c_str());
	//	CSqlError error;
	//	if (SQLITE_OK != sqlite3_exec(db.get(),
	//		sql.c_str(),
	//		callbackt,
	//		&strResult,
	//		error.address()) || error.isError())
	//	{
	//		return FALSE;
	//	}
	//	return TRUE;
	//}


	
	struct stTransactionScope
	{
	private:
		struct CstmtData
		{
			sqlite3_stmt* stmtBegin_ = nullptr;
			sqlite3_stmt* stmtCommit_ = nullptr;
			int cc_;
			CstmtData(sqlite3_stmt* stmtBegin, sqlite3_stmt* stmtCommit):
				stmtBegin_(stmtBegin),
				stmtCommit_(stmtCommit),
				cc_(0){}
			CstmtData(const CstmtData& that) {
				stmtBegin_ = that.stmtBegin_;
				stmtCommit_ = that.stmtCommit_;
				cc_ = that.cc_;
			}
			~CstmtData() {

			}
			void Increment() {
				++cc_;
			}
			void Decrement() {
				--cc_;
			}
			bool IsFirst() const {
				assert(cc_ >= 0);
				return cc_ == 1;
			}
			bool IsComplete() const {
				assert(cc_ >= 0);
				return cc_ <= 0;
			}
			void Finalize()
			{
				if (SQLITE_DONE != inifinite_sqlite3_step(stmtCommit_))
				{
					assert(false);
				}

				SAFE_SQLFINALIZE(stmtBegin_);
				SAFE_SQLFINALIZE(stmtCommit_);
			}
		};
		static std::map<sqlite3*, std::shared_ptr<CstmtData>> dbs_;
		sqlite3* pDB_ = nullptr;
	public:
		stTransactionScope(sqlite3* pDB) : pDB_(pDB)
		{
			assert(pDB);
			if (dbs_.find(pDB) == dbs_.end())
			{
				sqlite3_stmt* stmtBegin = nullptr;
				{
					if (SQLITE_OK != sqlite3_prepare16_v2(pDB,
						L"BEGIN TRANSACTION",
						-1,
						&stmtBegin,
						0) || !stmtBegin)
					{
						assert(false);
						return;
					}
				}
				sqlite3_stmt* stmtCommit = nullptr;
				{
					if (!stmtCommit)
					{
						if (SQLITE_OK != sqlite3_prepare16_v2(pDB,
							L"COMMIT;",
							-1,
							&stmtCommit,
							0) || !stmtCommit)
						{
							assert(false);
							return;
						}
					}
				}
				dbs_[pDB] = make_shared<CstmtData>(stmtBegin, stmtCommit);
				assert(dbs_.find(pDB) != dbs_.end());
			}

			dbs_[pDB].get()->Increment();
			if (dbs_[pDB].get()->IsFirst())
			{
				if (SQLITE_DONE != inifinite_sqlite3_step(dbs_[pDB].get()->stmtBegin_))
				{
					assert(false);
				}
			}
		}
		~stTransactionScope()
		{
			if (dbs_.find(pDB_) == dbs_.end())
				return;

			dbs_[pDB_].get()->Decrement();
			
			if (dbs_[pDB_].get()->IsComplete())
			{
				dbs_[pDB_].get()->Finalize();
				dbs_.erase(pDB_);
			}
		}

	};
	std::map<sqlite3*, std::shared_ptr<stTransactionScope::CstmtData>> stTransactionScope::dbs_;


	BOOL sqlWritePrivateProfileStringInternal(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		LPCWSTR lpString,
		CSqlFile& db)
	{
		if (!db.ok())
			return FALSE;

		stTransactionScope stts(db.get());

		int sqRet = 0;
		if (SQLITE_OK != sqlite3_reset(db.inserter(lpAppName)))
			return FALSE;

		int index = 0;
		if (SQLITE_OK != sqlite3_bind_text16(db.inserter(lpAppName), ++index, lpKeyName, -1, SQLITE_STATIC))
			return FALSE;
		if (SQLITE_OK != sqlite3_bind_text16(db.inserter(lpAppName), ++index, lpString, -1, SQLITE_STATIC))
			return FALSE;

		sqRet = inifinite_sqlite3_step(db.inserter(lpAppName));

		bool bRet = (sqRet == SQLITE_DONE || sqRet == SQLITE_ROW);
		assert(bRet);
		return bRet;
	}
	BOOL sqlWriteDeleteProfileStringInternal(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		CSqlFile& db)
	{
		stTransactionScope stts(db.get());

		if (SQLITE_OK != sqlite3_reset(db.deleter(lpAppName)))
			return FALSE;

		int index = 0;
		if (SQLITE_OK != sqlite3_bind_text16(db.deleter(lpAppName), ++index, lpKeyName, -1, SQLITE_STATIC))
			return FALSE;

		return SQLITE_DONE == inifinite_sqlite3_step(db.deleter(lpAppName));
	}
	
	BOOL sqlWritePrivateProfileString(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		LPCWSTR lpString,
		LPCWSTR lpFileName)
	{
		//if (!IsFileExistsW(lpFileName))
		//{
		//	if (!createDB(lpFileName, lpAppName))
		//	{
		//		return FALSE;
		//	}
		//}

		CSqlFile db(lpFileName,lpAppName,true);
		if (!db.ok())
			return FALSE;

		stTransactionScope stts(db.get());

		if (!sqlWriteDeleteProfileStringInternal(
			lpAppName,
			lpKeyName,
			db))
		{
			return FALSE;
		}
		return sqlWritePrivateProfileStringInternal(
			lpAppName,
			lpKeyName,
			lpString,
			db);
	}



	BOOL sqlGetPrivateProfileStringInternal(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		LPCWSTR lpDefault,
		LPWSTR lpReturnedString,
		DWORD nSize,
		CSqlFile& db)
	{

		if (!db.ok())
			return FALSE;

		stTransactionScope stts(db.get());

		if (SQLITE_OK != sqlite3_reset(db.selector(lpAppName)))
			return FALSE;

		int index = 0;
		if (SQLITE_OK != sqlite3_bind_text16(db.selector(lpAppName), ++index, lpKeyName, -1, SQLITE_STATIC))
			return FALSE;

		int sqRet = inifinite_sqlite3_step(db.selector(lpAppName));
		if (sqRet != SQLITE_ROW && sqRet != SQLITE_DONE)
		{
			return FALSE;
		}

		LPWSTR pRet = (LPWSTR)sqlite3_column_text16(db.selector(lpAppName), 0);
		if (!pRet)
			return FALSE;

		lstrcpyn(lpReturnedString, pRet, nSize);

		return TRUE;
	}
	BOOL sqlGetPrivateProfileString(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		LPCWSTR lpDefault,
		LPWSTR lpReturnedString,
		DWORD nSize,
		LPCWSTR lpFileName)
	{
		if (!lpReturnedString)
			return FALSE;
		if (lpDefault)
			lstrcpyn(lpReturnedString, lpDefault, nSize);
		else
			lpReturnedString[0] = 0;

		if (!IsFileExistsW(lpFileName))
		{
			return FALSE;
		}

		CSqlFile db(lpFileName,lpAppName,false);

		return  sqlGetPrivateProfileStringInternal(
			lpAppName,
			lpKeyName,
			lpDefault,
			lpReturnedString,
			nSize,
			db);
	}


	BOOL sqlWritePrivateProfileInt(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		int nData,
		LPCWSTR lpFileName)
	{
		TCHAR szT[32];
		wsprintf(szT, L"%d", nData);
		return sqlWritePrivateProfileString(
			lpAppName,
			lpKeyName,
			szT,
			lpFileName);
	}

	UINT sqlGetPrivateProfileInt(
		LPCWSTR lpAppName,
		LPCWSTR lpKeyName,
		INT nDefault,
		LPCWSTR lpFileName
		)
	{
		TCHAR szT[32];
		if (!sqlGetPrivateProfileString(
			lpAppName,
			lpKeyName,
			L"",
			szT,
			sizeof(szT) / sizeof(szT[0]),
			lpFileName))
		{
			return nDefault;
		}

		return _wtoi(szT);
	}














	static wstring GetUuidString()
	{
		UUID uuid;
		UuidCreate(&uuid);
		WCHAR* pS;
		UuidToStringW(&uuid, (RPC_WSTR*)&pS);
		wstring ret((WCHAR*)pS);
		RpcStringFreeW((RPC_WSTR*)&pS);
		return ret;
	}


	BOOL sqlGetPrivateProfileStringArray(
		LPCWSTR pApp,
		LPCWSTR pKey, 
		vector<wstring>& ss, 
		LPCWSTR pIni)
	{
		if (pApp == NULL || pApp[0] == 0)
			return FALSE;

		if (pKey == NULL || pKey[0] == 0)
			return FALSE;

		CSqlFile db(pIni,pApp,false);
		if (!db.ok())
			return FALSE;


		stTransactionScope stts(db.get());

		if (SQLITE_OK != sqlite3_reset(db.selector(pApp)))
			return FALSE;

		int index = 0;
		if (SQLITE_OK != sqlite3_bind_text16(db.selector(pApp), ++index, pKey, -1, SQLITE_STATIC))
			return FALSE;

		do
		{
			int sqRet = inifinite_sqlite3_step(db.selector(pApp));

			if (sqRet == SQLITE_ROW)
			{
				LPWSTR pRet = (LPWSTR)sqlite3_column_text16(db.selector(pApp), 0);
				if (!pRet)
					ss.push_back(wstring());
				else
					ss.push_back(pRet);
			}
			else if (sqRet == SQLITE_DONE)
			{
				break;
			}
			else {
				return FALSE;
			}
		} while (true);
		return TRUE;
	}
	BOOL sqlWritePrivateProfileStringArray(
		LPCWSTR pApp,
		LPCWSTR pKey, 
		vector<wstring>& ss,
		LPCWSTR pIni)
	{
		CSqlFile db(pIni,pApp,true);
		if (!db.ok())
			return FALSE;

		stTransactionScope stts(db.get());

		if (!sqlWriteDeleteProfileStringInternal(
			pApp,
			pKey,
			db))
		{
			return FALSE;
		}

		BOOL bFailed = FALSE;
		const size_t count = ss.size();
		for (size_t i = 0; i < count; ++i)
		{
			bFailed |= !sqlWritePrivateProfileStringInternal(
				pApp,
				pKey,
				ss[i].c_str(),
				db);
		}
		return !bFailed;
	}


} // namespace