//
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_SPIRIT_THREADSAFE 

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree; 
using boost::property_tree::read_json; 
using boost::property_tree::write_json;

using boost::asio::ip::tcp;

#include <fstream>  
#include <streambuf>  

int main(){
	boost::asio::io_service io_serv;
	 
	boost::asio::ip::tcp::resolver::query q("10.81.15.47","18080");

	boost::asio::ip::tcp::resolver rslvr(io_serv);

	boost::asio::ip::tcp::resolver::iterator iter = rslvr.resolve(q);

	boost::asio::ip::tcp::resolver::iterator end;

	boost::asio::ip::tcp::endpoint point;

	/*while(iter != end){

	   point = *iter++; 
	  std::cout<<point<<std::endl; 
	 } */
	 point = *iter; 
	 std::cout<<point<<std::endl; 
	
}
