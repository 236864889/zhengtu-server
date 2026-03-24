/**
 * \file
 * \version  $Id: zBase64.h  $
 * \author 
 * \date 
 * \brief base64±àÂë½âÂëº¯Êı
 *
 * 
 */


#ifndef _zBase64_h_
#define _zBase64_h_

#include <crypt.h>
#include <string>

namespace Zebra
{
	extern void base64_encrypt(const std::string &input, std::string &output);
	extern void base64_decrypt(const std::string &input, std::string &output);
};

#endif

