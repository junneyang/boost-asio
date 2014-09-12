#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <vector>

void handler(const boost::system::error_code& /*e*/){
	std::cout << "Hello, world!" << std::endl;
}

int main() { 
	boost::asio::io_service io;
	
	boost::asio::deadline_timer timer1(io, boost::posix_time::seconds(5)); 
	timer1.async_wait(handler); 
	boost::asio::deadline_timer timer2(io, boost::posix_time::seconds(5)); 
	timer2.async_wait(handler); 
	//boost::thread thread1(run); 
	//boost::thread thread2(run); 
	boost::thread thread1(boost::bind(&boost::asio::io_service::run, &io));
	boost::thread thread2(boost::bind(&boost::asio::io_service::run, &io));
	thread1.join(); 
	thread2.join(); 
} 

