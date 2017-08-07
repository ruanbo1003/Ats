/*
 * AtsConfit.hpp
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: ruanbo
 */

#ifndef ATSMAIN_ATSCONFIG_HPP_
#define ATSMAIN_ATSCONFIG_HPP_

#include "comm/comm.hpp"

class AtsConfig
{
public:
	string _config_file_path;

	string _broker;
	string _user;
	string _passwd;

	bool _quote_run;
	string _quote_front;

	bool _trader_run;
	string _trader_front;

public:
	AtsConfig(const string& file);
	virtual ~AtsConfig();

public:
	bool read_config();
};

using AtsConfigPtr = tr1::shared_ptr<AtsConfig>;


#endif /* ATSMAIN_ATSCONFIG_HPP_ */








