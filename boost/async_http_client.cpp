//
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <string>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

using boost::asio::ip::tcp;

std::string get_randomdata(std::vector<std::string> testdata){
	srand((unsigned)time(NULL));
	random_shuffle(testdata.begin(),testdata.end());
	return (*testdata.begin());
}

std::vector<std::string> split_string(const std::string& str, const std::string& sep){
	std::vector<std::string> ret;
	size_t start = 0;
	size_t str_len = str.size();
	size_t found = std::string::npos;
	while (start < str_len && (found = str.find(sep, start)) != std::string::npos){
		if (found > start){
			std::string sub = str.substr(start, found - start);
			if (!sub.empty()){
				ret.push_back(sub);
			}
		}
		start = (found + sep.size());
	}
	if (start < str_len){
		std::string sub = str.substr(start);
		if (!sub.empty()){
			ret.push_back(sub);
		}
	}
	return ret;
}

class client
{
	private:
		boost::asio::io_service& io_service_;
		tcp::endpoint &endpoint_;
		const std::string& server_;
		const std::string& path_;
		std::vector<std::string> testdata_;
		typedef boost::shared_ptr<tcp::socket> sock_pt;

	public:
		client(boost::asio::io_service& io_service, tcp::endpoint &endpoint, const std::string& server, const std::string& path, std::vector<std::string> testdata)
		:io_service_(io_service),
		endpoint_(endpoint),
		server_(server),
		path_(path),
		testdata_(testdata){
			// Form the request. We specify the "Connection: close" header so that the
			// server will close the socket after transmitting the response. This will
			// allow us to treat all data up until the EOF as the content.
			// Start an asynchronous resolve to translate the server and service names
			// into a list of endpoints.
			start();
		}

	private:
		void start(){
			//智能指针，新建socket对象
			sock_pt socket_(new tcp::socket(io_service_));
			//异步连接server
			socket_->async_connect(endpoint_, boost::bind(&client::handle_connect, this, boost::asio::placeholders::error, socket_));
			std::cout << "async_connect: " << "\n";
		}

		void handle_connect(const boost::system::error_code& err, sock_pt socket_){
			std::cout << "handle_connect: " << "\n";
			boost::asio::streambuf *request_ = new boost::asio::streambuf();
			std::ostream request_stream(request_);
			//request_stream << "GET " << "/gettest/?para1=Hello&para2=World" << " HTTP/1.0\r\n";
			//request_stream << "Host: " << "10.81.15.47" << "\r\n";
			//request_stream << "Accept: */*\r\n";
			//request_stream << "Connection: close\r\n\r\n";

			std::string request_str=get_randomdata(testdata_);
			std::cout << "request_str: \n" << request_str <<"\n";
			request_stream << "POST " << "/lbs/da/openservice" << " HTTP/1.0\r\n";
			request_stream << "Host:" << "10.81.15.47" << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "Content-Type: application/json; charset=utf-8\r\n";
			request_stream << "Content-Length: " << request_str.length() << "\r\n";
			request_stream << "Referer: http://10.81.15.47:18080/lbs/da/openservice\r\n";
			request_stream << "Connection: close\r\n\r\n";
			request_stream << request_str;

			if (!err){
				// The connection was successful. Send the request.
				std::cout << "async_write: " << "\n";
				boost::asio::async_write(*socket_, *request_, boost::bind(&client::handle_write_request, this, boost::asio::placeholders::error, socket_, request_));
			}
			else{
				std::cout << "Error: " << err.message() << "\n";
			}
		}

		void handle_write_request(const boost::system::error_code& err, sock_pt socket_, boost::asio::streambuf *request_){
			std::cout << "handle_write_request: " << "\n";
			if (!err){
				delete request_;
				boost::asio::streambuf *response_ = new boost::asio::streambuf();
				///////////////////////////////////////////////////////////////////
				// Read the response status line. The response_ streambuf will
				// automatically grow to accommodate the entire line. The growth may be
				// limited by passing a maximum size to the streambuf constructor.
				
				//std::cout << "async_read_until: " << "\n";
				//boost::asio::async_read_until(*socket_, *response_, "\r\n", boost::bind(&client::handle_read_content, this, boost::asio::placeholders::error, response_, socket_));
				std::cout << "async_read: " << "\n";
				boost::asio::async_read(*socket_, *response_, boost::asio::transfer_at_least(1), boost::bind(&client::handle_read_content, this, boost::asio::placeholders::error, response_, socket_));
			}else{
				std::cout << "handle_write_request Error: " << err.message() << "\n";
			}
		}

		/*void handle_read_status_line(const boost::system::error_code& err){
			if (!err){
				// Check that response is OK.
				std::istream response_stream(&response_);
				std::string http_version;
				response_stream >> http_version;
				unsigned int status_code;
				response_stream >> status_code;
				std::string status_message;
				std::getline(response_stream, status_message);
				if (!response_stream || http_version.substr(0, 5) != "HTTP/"){
					std::cout << "Invalid response\n";
					return;
				}if (status_code != 200){
					std::cout << "Response returned with status code ";
					std::cout << status_code << "\n";
					return;
				}
				// Read the response headers, which are terminated by a blank line.
				boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
				boost::bind(&client::handle_read_headers, this,
				boost::asio::placeholders::error));
			}else{
				std::cout << "Error: " << err << "\n";
			}
		}*/

		/*void handle_read_headers(const boost::system::error_code& err){
			if (!err){
				// Process the response headers.
				std::istream response_stream(&response_);
				std::string header;
				while (std::getline(response_stream, header) && header != "\r")
				std::cout << header << "\n";
				std::cout << "\n";

				// Write whatever content we already have to output.
				if (response_.size() > 0)
				std::cout << &response_;

				// Start reading remaining data until EOF.
				boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&client::handle_read_content, this,
				boost::asio::placeholders::error));
			}else{
				std::cout << "Error: " << err << "\n";
			}
		}*/

		void handle_read_content(const boost::system::error_code& err, boost::asio::streambuf *response_, sock_pt socket_){
			//std::cout << "handle_read_content: " << "\n";
			if (!err){
				// Write all of the data that has been read so far.
				//std::cout << "write data: " << "\n";
				std::cout << response_;
				// Continue reading remaining data until EOF.
				boost::asio::async_read(*socket_, *response_, boost::asio::transfer_at_least(1), boost::bind(&client::handle_read_content, this, boost::asio::placeholders::error, response_, socket_));
			}else if (err != boost::asio::error::eof){
				//std::cout << "handle_read_content Error: " << err << "\n";
				delete response_;
				socket_->close();
			}else if (err == boost::asio::error::eof){
				//std::cout << "read end:" << err << "\n";
				delete response_;
				socket_->close();
				//delete socket_;
				start();
			}else{
				delete response_;
				socket_->close();
			}
		}
};

int main(int argc, char* argv[]){
	try{
		if (argc != 6){
			std::cout << "Usage: async_client <server> <port> <clientnum> <threadnum> <testdata>\n";
			std::cout << "Example:\n";
			std::cout << "./async_http_client 10.81.15.47 18080\n";
			return -1;
		}

		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(argv[1], argv[2]);
		tcp::resolver::iterator iterator = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;
		boost::asio::ip::tcp::endpoint endpoint;
		endpoint = *iterator;

		std::vector<std::string> testdata;
		std::ifstream t(argv[5]);
		std::string testdata_str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
		std::cout << "testdata_str:\n" << testdata_str << "\n";
		testdata=split_string(testdata_str,"*************************splitline*************************");
		
		std::vector<std::string>::iterator it;
		for(it=testdata.begin(); it!=testdata.end(); it++){
			std::cout<<"data_str:\n"<<*it<<std::endl;
		}
		
		//client个数
		for(int i=0; i<atoi(argv[3]); i++){
			new client(io_service, endpoint, argv[1], argv[2], testdata);
		}
		//线程池方式
		// Wait for all threads in the pool to exit.
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (int i = 0; i < atoi(argv[4]); ++i){
			boost::shared_ptr<boost::thread> thread(new boost::thread(
				boost::bind(&boost::asio::io_service::run, &io_service)));
			threads.push_back(thread);
		}
		for (std::size_t i = 0; i < threads.size(); ++i){
			threads[i]->join();
		}
		//新建线程方式
		//boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
		//t.join();
		//单线程方式
		//io_service.run();
	}catch (std::exception& e){
		std::cout << "Exception: " << e.what() << "\n";
	}
	
	return 0;
}

