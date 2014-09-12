#!/bin/sh

BOOST_ROOT="/home/users/yangjun03/boost/boost_1_53_0"
BOOST_EXT_INCLUDE=${BOOST_ROOT}"/builddir/include"
BOOST_EXT_LIB=${BOOST_ROOT}"/builddir/lib"
CXXFLAGS="-pipe -W -Wall -fPIC -D_GNU_SOURCE -D__STDC_LIMIT_MACROS"
LDFLAGS="-lpthread -lrt -lz -lboost_thread -lboost_system"

export LD_LIBRARY_PATH=${BOOST_EXT_LIB}:${LD_LIBRARY_PATH}

:<<EOF
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o helloworld helloworld.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o thread thread.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o sync_timer sync_timer.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o async_timer async_timer.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o async_timer_bind async_timer_bind.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o async_timer_class async_timer_class.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o async_timer_single_ios async_timer_single_ios.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o async_timer_multi_ios async_timer_multi_ios.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o async_connect async_connect.cpp
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o resolve resolve.cpp
EOF
g++ -I. -I$BOOST_ROOT -I$BOOST_EXT_INCLUDE -L$BOOST_EXT_LIB  $CXXFLAGS $LDFLAGS -o async_http_client async_http_client.cpp

