/*
 * MysqlDb.cpp
 *
 *      Author: ruanbo
 */

#include "MysqlDb.hpp"
#include "comm/comm.hpp"
#include "utils/utils.hpp"
#include <fstream>

MysqlDb::MysqlDb(const string& addr, const string& user, const string& passwd) :
		_driver(nullptr)
{
	_buffer = BufferPtr(new Buffer());

	_db_addr = addr;
	_db_user = user;
	_db_passwd = passwd;
	_db_name = "";
}

MysqlDb::~MysqlDb()
{
//    Log("MysqlDb::~MysqlDb");

	unit();
}

bool MysqlDb::init()
{
	try{
		_driver = get_driver_instance();
		if(!_driver)
		{
			LogError("MySql get_driver_instance() error");
			return false;
		}

		sql::Connection* sqlconn = _driver->connect(_db_addr, _db_user, _db_passwd);
		if(!sqlconn)
		{
			Log("mysql connect to db failed");
			return false;
		}

		if(sqlconn->isClosed() == true)
		{
			Log("mysql is closed");
			return false;
		}

		if(sqlconn->isValid() == false)
		{
			LogError("mysqlcppconn is not valid");
			unit();
			return false;
		}
		else
		{
			Log("mysql connect to db success");
		}

		_conn = MysqlConnPtr(sqlconn);
		if(!_conn)
		{
			LogError("mysqlcppconn connect to mysql error");
			unit();
			return false;
		}

		_stmt = MysqlStatementPtr(_conn->createStatement());
		if(!_stmt)
		{
			LogError("MySqlDb createStatement() error");
			unit();
			return false;
		}

		if(init_sql() == false)
		{
			unit();
			return false;
		}

		_conn->setSchema("ctp");
		_conn->setAutoCommit(true);

		return true;
	}
	catch (sql::SQLException &e) {
		LogError("Mysql exception:%s", e.what());
		return false;
	}
}

void MysqlDb::unit()
{
	if(_conn)
	{
		_conn->close();
	}

	if(_driver)
	{
		_driver = nullptr;
	}
}

bool MysqlDb::isOk()
{
	return _conn->isValid();
}

void MysqlDb::reconnect()
{
	bool ret = _conn->reconnect();

	Log("reconnect ret:%d", ret);
}

bool MysqlDb::init_sql()
{
	std::ifstream ifs("AtsMain/DbDriver/mysql.sql", std::ifstream::in);
	if(ifs.is_open() == false)
	{
		LogError("open file mysql.sql error");
		return false;
	}

	string file_sql((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	ifs.close();

	bool ret = false;
	std::vector<string> sqls;
	utils::split(file_sql, ";", sqls);
	for (auto& one_sql : sqls)
	{
		if(one_sql.length() < 5)
		{
			continue;
		}
		utils::trim_left(one_sql, '\n');

		ret = execute_sql(one_sql);
		if(ret == false)
		{
			LogError("init_sql: \n%s error!!!!", one_sql.c_str());
			return false;
		}
		else
		{
//            Log("init mysql:%s", one_sql.data());
		}
	}

	return true;
}

bool MysqlDb::execute_sql(const string& sql)
{
	try
	{
		_stmt->execute(sql);

		return true;
	}
	catch (sql::SQLException& e)
	{
		LogError("execute sql: \n%s \nSQLException:%s\n\n", sql.c_str(), e.what());
		return false;
	}
	catch (std::exception& e)
	{
		LogError("execute sql: \n%s \n std::exception:%s\n\n", sql.c_str(), e.what());
		return false;
	}
}

MysqlResPtr MysqlDb::execute_query(const string& query_sql)
{
	try
	{
//        MysqlPreStatePtr prestate = MysqlPreStatePtr(_conn->prepareStatement(query_sql));
//        return MysqlResPtr(prestate->executeQuery());
		return MysqlResPtr(_stmt->executeQuery(query_sql));
	}
	catch (sql::SQLException& e)
	{
		LogError("SQL error: %s", query_sql.c_str());
		return MysqlResPtr();
	}
	catch (std::exception& e)
	{
		LogError("std error: %s", query_sql.c_str());
		return MysqlResPtr();
	}
}

int MysqlDb::execute_pre_update(const MysqlPreStatePtr& pre_stat)
{
	try
	{
		return pre_stat->executeUpdate();
	}
	catch (sql::SQLException& e)
	{
		LogError("sql::SQLException:mysql prestatement update error:%s", e.what());
		return -1;
	}
	catch (std::exception& e)
	{
		LogError("std::exception:mysql prestatement update error");
		return -1;
	}

}

bool MysqlDb::check_name_registered(const string& name)
{
	static string check_name_pat = "Select uid From register Where name='";
	string check_name_sql = check_name_pat + name + "';";

	MysqlResPtr res = execute_query(check_name_sql);

	if(res)
	{
		return res->rowsCount() >= 1;
	}
	else
	{
		return false;
	}
}

bool MysqlDb::register_user(const string& name, const string& passwd, time_t time)
{
	static string register_pat = "Insert Into test(name,passwd,time) Values(?,?,?)";

	_pstmt = MysqlPreStatePtr(_conn->prepareStatement(register_pat));

	_pstmt->setString(1, name);
	_pstmt->setString(2, passwd);
	_pstmt->setInt(3, time);

//    _pstmt->executeUpdate();
	int update_ret = execute_pre_update(_pstmt);

	return update_ret > 0;
}

int MysqlDb::get_uid(const string& name, const string& passwd)
{
	static string get_sql_format = "Select uid From test Where name='%s' And passwd='%s'";
	string get_sql = string_format(get_sql_format, name.c_str(), passwd.c_str());

	Log("get_uid_sql:%s", get_sql.c_str());

	MysqlResPtr res = execute_query(get_sql);
	if(!res || res->rowsCount() <= 0)
	{
		return -1;
	}

	res->next();
	Log("get_uid_sql res rowsCount:%ld", res->rowsCount());

//    sql::SQLString retstr = res->getString("");
//    string realstr = retstr.asStdString();
	return res->getInt("uid");
}

bool MysqlDb::setttlement_confirm(const string& date, time_t t)
{
	static string register_pat = "Insert Into Settlement(day,time) Values(?,?)";

	_pstmt = MysqlPreStatePtr(_conn->prepareStatement(register_pat));

	_pstmt->setString(1, date);
	_pstmt->setInt64(2, t);

	int update_ret = execute_pre_update(_pstmt);

	return update_ret > 0;
}

bool MysqlDb::is_settlement_confirmed(const string& date)
{
	static string get_sql_format = "Select time From Settlement Where day='%s'";
	string get_sql = string_format(get_sql_format, date.c_str());

	MysqlResPtr res = execute_query(get_sql);
	if(!res || res->rowsCount() <= 0)
	{
		return false;
	}

	res->next();

	return (res->getInt("time") > 0);
}

