/*
 * DbTest.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: ruanbo
 */


#include "AtsMain/DbDriver/MysqlDb.hpp"

class MysqlDbTest
{
private:
    MysqlDbPtr _pdb;

public:
    MysqlDbTest()
    {

    }

    virtual ~MysqlDbTest()
    {

    }

public:
    bool init()
    {
        _pdb = MysqlDbPtr(new MysqlDb("127.0.0.1:3306", "root", "123456"));
        if(_pdb->init() == false)
        {
            Log("MysqlDb init failed");
            return false;
        }

        int i = 0;
        while(i <= 10)
        {
        	sleep(1);
        	i++;

        	Log("db ok:%d", _pdb->isOk());

        	if(_pdb->isOk() == false)
        	{
        		_pdb->reconnect();
        	}
        }

        return true;
    }
    void db_c_test()
    {
        for(int i=0; i<10; ++i)
        {
            string name = "Robert" + std::to_string(i);
            string passwd = "test";
            time_t now = time(NULL);
            bool ret = _pdb->register_user(name, passwd, now);
            if(ret == false)
            {
                Log("name:%s is not valid", name.data());
            }
        }

    }

    void db_u_test()
    {

    }

    void db_r_test()
    {
        string name = "Robert";
        string passwd = "test";

        for(int i=0; i<10; ++i)
        {
            int uid = _pdb->get_uid(name, passwd);
            Log("uid:%d", uid);
        }
    }


    void db_d_test()
    {

    }

};



void db_tests()
{
    MysqlDbTest db;
    if(db.init() == false)
    {
        return;
    }
//    db.db_c_test();

//    db.db_r_test();

}


