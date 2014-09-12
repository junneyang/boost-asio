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
	boost::asio::io_service io1;
	boost::asio::io_service io2;
	
	boost::asio::deadline_timer timer1(io1, boost::posix_time::seconds(5)); 
	timer1.async_wait(handler); 
	boost::asio::deadline_timer timer2(io2, boost::posix_time::seconds(5)); 
	timer2.async_wait(handler); 
	//boost::thread thread1(run); 
	//boost::thread thread2(run); 
	boost::thread thread1(boost::bind(&boost::asio::io_service::run, &io1));
	boost::thread thread2(boost::bind(&boost::asio::io_service::run, &io2));
	thread1.join(); 
	thread2.join(); 
} 

