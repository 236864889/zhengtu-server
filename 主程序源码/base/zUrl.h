/**
 * \file
 * \version  $Id: zUrl.h  $
 * \author  
 * \date 
 * \brief 封装一些url操作函数
 */


#ifndef _zUrl_h_
#define _zUrl_h_

#include <string>

namespace Zebra
{
	char *url_encode(const char *s, int len, int *new_length);
	void url_encode(std::string &s);
	int url_decode(char *str, int len);
	void url_decode(std::string &str);
};

#endif


