#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <vector>

class printer{
	public:
		printer(boost::asio::io_service &io, int interval, int total_count, int thread_num);
		~printer();
		void print(int th);
	private:
		int interval;
		int cur_count;
		int total_count;
		int thread_num;
		boost::asio::strand strand;
		std::vector<boost::asio::deadline_timer *> timer;
};

printer::printer(boost::asio::io_service &io, int interval, int total_count, int thread_num)
	:interval(interval),cur_count(0),total_count(total_count),thread_num(thread_num),strand(io){
	
	for(int i=0; i<this->thread_num; i++){
		this->timer.push_back(new boost::asio::deadline_timer(io, boost::posix_time::seconds(this->interval)));
		this->timer[i]->async_wait(this->strand.wrap(boost::bind(&printer::print, this, i)));
	}
}

void printer::print(int th){
	if(this->cur_count < this->total_count){
		std::cout << this->cur_count << "\n";
		this->cur_count++;
		this->timer[th]->expires_at(this->timer[th]->expires_at() + boost::posix_time::seconds(this->interval));
		this->timer[th]->async_wait(this->strand.wrap(boost::bind(&printer::print, this, th)));
	}
}

printer::~printer(){
	std::cout << "Final count is " << this->cur_count << "\n";
}

int main(){
	boost::asio::io_service io;

	int total_count = 100;
	int interval = 1;
	int thread_num = 10;
	printer p(io, interval, total_count, thread_num);
	boost::thread th(boost::bind(&boost::asio::io_service::run, &io));

	io.run();
	th.join();

	return 0;
}

