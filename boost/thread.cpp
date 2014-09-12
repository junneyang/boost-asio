#include <boost/thread/thread.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

void hello_world(string thread_id){
	cout << "Hello world, I'm a thread! thread-" + thread_id << endl;
}

//int argc, char** argv
int main(){
	// 開始一條使用 "hello_world" function 的新執行緒
	
	vector<boost::thread *> th_list;
	for(int i=0; i<100; i++){
		th_list.push_back(new boost::thread(hello_world, boost::lexical_cast<string>(i)));
	}
	// 等待執行緒完成工作
	/*for(vector<boost::thread>::size_type i = 0; i != th_list.size(); ++i){
		th_list[i]->join();
	}*/
	
	for(vector<boost::thread *>::iterator iter = th_list.begin(); iter != th_list.end(); ++iter){
		(*iter)->join();
	}

	return 0;
}

