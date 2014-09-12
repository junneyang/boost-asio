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

#include <fstream>  
#include <streambuf>  
using boost::asio::ip::tcp;

boost::asio::io_service io_service; 
boost::asio::ip::tcp::resolver resolver(io_service); 
boost::asio::ip::tcp::socket sock(io_service); 

void connect_handler(const boost::system::error_code &ec) 
{ 
  if (!ec) 
  { 
    std::cout << "connect success...." ;
  } else{
	std::cout << "connect error...." ;
  }
} 

void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it) 
{ 
  if (!ec) 
  { 
    sock.async_connect(*it, connect_handler); 
  } 
} 

int main(int argc, char** argv) 
{ 
  boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]); 
  resolver.async_resolve(query, resolve_handler); 
	/*boost::asio::ip::address add;
	add.from_string(argv[1]);
	tcp::endpoint endpoint(add , short(18080) );
	sock.async_connect(endpoint, connect_handler); */
  io_service.run(); 
} 

