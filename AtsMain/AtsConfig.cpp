/*
 * AtsConfit.cpp
 *
 *  Created on: 2017年8月7日
 *      Author: ruanbo
 */

#include <AtsMain/AtsConfig.hpp>
#include "ThirdLib/TinyXml2/tinyxml2.h"

using namespace tinyxml2;

AtsConfig::AtsConfig(const string& file)
{
	_config_file_path = file;

	_broker = "";
	_user = "";
	_passwd = "";

	_quote_run = false;
	_quote_front = "";

	_trader_run = false;
	_trader_front = "";

	_mysql_addr = "";
	_mysql_user = "";
	_mysql_passwd = "";
}

AtsConfig::~AtsConfig()
{

}

bool AtsConfig::read_config()
{
	XMLDocument doc;
	int ret = doc.LoadFile(_config_file_path.data());
	if(ret != 0)
	{
		Log("xml load config file(%s) failed(%d)", _config_file_path.data(), ret);
		return false;
	}

	XMLElement* root = doc.RootElement();
	if(!root)
	{
		Log("config file has no root element?");
		return false;
	}

	// --------------- account config -----------------
	{
		XMLElement* accounts = root->FirstChildElement("account");
		if(!accounts)
		{
			Log("config file has no account");
			return false;
		}

		const XMLElement* one_account = accounts->FirstChildElement("item");
		if(one_account)
		{
			if(one_account->FindAttribute("broker"))
			{
				_broker = one_account->Attribute("broker");
			}
			else
			{
				Log("account has no broker");
				return false;
			}
			if(one_account->FindAttribute("user"))
			{
				_user = one_account->Attribute("user");
			}
			else
			{
				Log("account has no user");
				return false;
			}
			if(one_account->FindAttribute("passwd"))
			{
				_passwd = one_account->Attribute("passwd");
			}
			else
			{
				Log("account has no passwd");
				return false;
			}
		}
		else
		{
			Log("no account found");
			return false;
		}

	}

	// ---------- mysql config -----------
	{
		const XMLElement* mysql = root->FirstChildElement("mysql");
		if(!mysql)
		{
			Log("config file has no mysql configed");
			return false;
		}

		if(mysql->FindAttribute("addr"))
		{
			_mysql_addr = mysql->Attribute("addr");
		}
		else
		{
			Log("mysql addr not found");
			return false;
		}
		if(mysql->FindAttribute("user"))
		{
			_mysql_user = mysql->Attribute("user");
		}
		else
		{
			Log("mysql user not found");
			return false;
		}
		if(mysql->FindAttribute("passwd"))
		{
			_mysql_passwd = mysql->Attribute("passwd");
		}
		else
		{
			Log("mysql passwd not found");
			return false;
		}
	}

	// ---------- quote config -----------
	{
		const XMLElement* quote = root->FirstChildElement("quote");
		if(!quote)
		{
			Log("config file has no quote");
			return false;
		}

		if(quote->FindAttribute("run"))
		{
			_quote_run = quote->BoolAttribute("run", false);
		}

		const XMLElement* front = quote->FirstChildElement("front");
		if(!front)
		{
			Log("quote has not front configed");
			return false;
		}

		if(front->FindAttribute("addr"))
		{
			_quote_front = front->Attribute("addr");
		}
		else
		{
			Log("quote front has no addr");
			return false;
		}
	}

	// ---------- trader config -----------
	{
		const XMLElement* trader = root->FirstChildElement("trader");
		if(!trader)
		{
			Log("config file has no trader");
			return false;
		}

		if(trader->FindAttribute("run"))
		{
			_trader_run = trader->BoolAttribute("run", false);
		}

		const XMLElement* front = trader->FirstChildElement("front");
		if(!front)
		{
			Log("trader has not front configed");
			return false;
		}

		if(front->FindAttribute("addr"))
		{
			_trader_front = front->Attribute("addr");
		}
		else
		{
			Log("trader front has no addr");
			return false;
		}
	}

	return true;
}

