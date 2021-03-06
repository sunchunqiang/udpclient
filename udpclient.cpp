*************cpp : Defines the entry point for the console application.
//



#include "stdafx.h" 
#define  __WINDOWS__
#ifdef __WINDOWS__
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include  <cstring>
#include <string>
#include <numeric>
#include <chrono>
#include <ctime>
#include <future>
#include <thread>
#include <sstream>
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <iostream>

using namespace rapidjson;
using namespace rapidxml;

std::string testcreaterapidxml()
{
	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	xml_node<>* root = doc.allocate_node(node_element, "rootnode");
	doc.append_node(root);

	xml_node<>* child = doc.allocate_node(node_element, "hello");
	child->append_attribute(doc.allocate_attribute("value", "world"));
	child->append_attribute(doc.allocate_attribute("t", "True"));
	child->append_attribute(doc.allocate_attribute("f", "False"));
	child->append_attribute(doc.allocate_attribute("i", "123"));
	child->append_attribute(doc.allocate_attribute("pi", "3.1415"));
	root->append_node(child);
	xml_node<>* child1 = doc.allocate_node(node_element, "a");
	child1->append_attribute(doc.allocate_attribute("value", "0"));
	child->append_node(child1);
	child1 = doc.allocate_node(node_element, "a");
	child1->append_attribute(doc.allocate_attribute("value", "1"));
	child->append_node(child1);
	child1 = doc.allocate_node(node_element, "a");
	child1->append_attribute(doc.allocate_attribute("value", "2"));
	child->append_node(child1);

	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);
	return xml_as_string;

	 

}
void testparserapidxml(std::string str)
{
	xml_document<> doc;
	xml_node<> * root_node;
 
	// Parse the buffer using the xml file parsing library into doc 
	char* pbuf = new char[str.size() + 2];
	std::unique_ptr<char> buff(pbuf);
	strcpy_s(buff.get(), str.size() + 1, str.c_str());
	doc.parse<0>(pbuf);
	// Find our root node
	root_node = doc.first_node("rootnode");
	xml_node<> * hello = root_node->first_node("hello");
	std::string value = hello->first_attribute("value")->value();

	std::string t1 = hello->first_attribute("t")->value(); 
	std::string t2 = hello->first_attribute("f")->value();
	std::string t3 = hello->first_attribute("i")->value();
	std::string t4 = hello->first_attribute("pi")->value();
// Iterate over the brewerys
	std::vector<std::string> aa;
	for (xml_node<> * brewery_node = hello->first_node("a"); brewery_node; brewery_node = brewery_node->next_sibling())
	{
		 
		aa.push_back(brewery_node->first_attribute("value")->value());
	}
}

int testparsejson(  const std::string& json) {

	Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

#if 1
						// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
	if (document.Parse(json.c_str()).HasParseError())
		return 1;
#else
						// In-situ parsing, decode strings directly in the source string. Source must be string.
	char buffer[json.length() ];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu(buffer).HasParseError())
		return 1;
#endif

 
 
	std::string s = document["hello"].GetString(); 
	bool  t = document["t"].GetBool();
	bool f = document["f"].GetBool();
	int i = document["i"].GetInt();
	float pi = document["pi"].IsDouble(); 

  
 
		const Value& a = document["a"]; // Using a reference for consecutive access is handy and faster.
		std::vector<int> aa;
		for (SizeType i = 0; i < a.Size(); i++) // rapidjson uses SizeType instead of size_t.
			aa.push_back( a[i].GetInt());
 
 
	// Iterating object members
	//static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
	//for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	//	printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);

	 


	return 0;
}

std::string testrapidjson()
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();               // Between StartObject()/EndObject(), 
	writer.Key("hello");                // output a key,
	writer.String("world");             // follow by a value.
	writer.Key("t");
	writer.Bool(true);
	writer.Key("f");
	writer.Bool(false);
	writer.Key("n");
	writer.Null();
	writer.Key("i");
	writer.Uint(123);
	writer.Key("pi");
	writer.Double(3.1416);
	writer.Key("a");
	writer.StartArray();                // Between StartArray()/EndArray(),
	for (unsigned i = 0; i < 4; i++)
		writer.Uint(i);                 // all values are elements of the array.
	writer.EndArray();
	writer.EndObject();
	return  s.GetString();
	// {"hello":"world","t":true,"f":false,"n":null,"i":123,"pi":3.1416,"a":[0,1,2,3]}
	//std::cout << s.GetString() << std::endl;

}
int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr)
{
	int result;
	addrinfo* result_list = NULL;
	addrinfo hints = {};
	hints.ai_family = family;
	hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
	result = getaddrinfo(hostname, service, &hints, &result_list);
	if (result == 0)
	{
		//ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
		memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
		freeaddrinfo(result_list);
	}

	return result;
}
const int ncount = 100;
long  starttime[ncount];
#ifdef __WINDOWS__
void receivereponse(SOCKET s_socket, sockaddr* addrDest, std::chrono::time_point<std::chrono::high_resolution_clock>& start, std::promise<std::string> result)
#else
void receivereponse(int s_socket, sockaddr* addrDest, std::chrono::time_point<std::chrono::high_resolution_clock>& start, std::promise<std::string> result)
#endif
{
	socklen_t slen = sizeof(sockaddr);
	char buf[1024];
	int bufflen = 1024;
	int len = recvfrom(s_socket, buf, bufflen, 0, addrDest, &slen);
	if (len == -1)
	{
		std::cout << "recvfrom() error" << std::endl;
		result.set_value("recvfrom() error");
		return;

	}
	else
	{
		buf[len] = 0;
		auto  finish = std::chrono::high_resolution_clock::now();

		long finishedtime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

		Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

		std::stringstream s;

		if (document.Parse(buf).HasParseError())
			s << "failed to parse the response";
		else
		{
			int index = document["index"].GetInt();
			long cost = finishedtime - starttime[index];
			s << document["response"].GetString();
			s << "  index:" << index << " costed time:" << cost;
		}
		result.set_value(s.str());
	}

}
std::string getsendmessage(int index)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();                // Between StartObject()/EndObject(), 
	writer.Key("name");                // output a key,[=l
	writer.String("world");             // follow by a value.
	time_t now = time(0);
	char buf[256];
	ctime_s(buf, 256, &now);
	writer.Key("time");                // output a key,
	writer.String(buf);
	writer.Key("index");
	writer.Uint(index);
	writer.EndObject();
	return  s.GetString();
}
int test()
{
	std::string str;
	{

		auto start = std::chrono::high_resolution_clock::now();
		str = testrapidjson();
		auto  finish = std::chrono::high_resolution_clock::now();
		//		s << start << ":" << finish << std::endl;
		std::cout << str << std::endl;
		std::cout << "create rapid json: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;

	}
	{
		auto start = std::chrono::high_resolution_clock::now();
		testparsejson(str);

		auto  finish = std::chrono::high_resolution_clock::now();


		//		s << start << ":" << finish << std::endl;
		std::cout << "parse rapid json: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
	}
	{

		auto start = std::chrono::high_resolution_clock::now();
		str = testcreaterapidxml();
		auto  finish = std::chrono::high_resolution_clock::now();
		//		s << start << ":" << finish << std::endl;
		std::cout << "create rapid xml: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
		//	std::cout << str << std::endl;

	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		testparserapidxml(str);

		auto  finish = std::chrono::high_resolution_clock::now();


		//		s << start << ":" << finish << std::endl;
		std::cout << "parse rapid xml: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
	}

	return 0;

}
int main() {
 
#ifdef __WINDOWS__
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
#else 
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

#endif

	int result = 0;

	sockaddr_in addrListen = {}; // zero-int, sin_port is 0, which picks a random port for bind.
	addrListen.sin_family = AF_INET;
	result = bind(sock, (sockaddr*)&addrListen, sizeof(addrListen));
	if (result == -1)
	{
		int lasterror = -1;
		std::cout << "error: " << lasterror;
		return 1;
	}


	sockaddr_storage addrDest = {};
	result = resolvehelper("172.25.30.110", AF_INET, "9020", &addrDest);
	if (result != 0)
	{
		int lasterror = -2;
		std::cout << "error: " << lasterror;
		return 1;
	}
	std::vector<std::future<std::string>> vec;
	std::vector<std::thread> threads;
	auto start = std::chrono::high_resolution_clock::now();
	int i = 0;
	__int64 allcosted = 0;
	 for(;;) //int i = 0 ; i < ncount; i++)
	 {

		std::string  str = std::move(getsendmessage(i));
		i++;
		const char* msg = str.c_str();
		size_t msg_length = strlen(msg);
	//	auto  finish = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::high_resolution_clock::now();
	///	starttime[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
	//	std::cout << "send message" << std::endl;
		result = sendto(sock, msg, msg_length, 0, (sockaddr*)&addrDest, sizeof(addrDest));
		//std::promise<std::string> result;
		//std::future<std::string> d = result.get_future();
		
		//vec.push_back(move(d));
		{

			socklen_t slen = sizeof(addrDest);
			char buf[1024];
			int bufflen = 1024;
			int len = recvfrom(sock, buf, bufflen, 0, (sockaddr*)&addrDest, &slen);
			if (len == -1)
			{
				std::cout << "recvfrom() error" << std::endl;
			//	result.set_value("recvfrom() error");
			//	return;

			}
			else
			{
				buf[len] = 0;
				auto  finish = std::chrono::high_resolution_clock::now();

				__int64 finishedtime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish -  start).count();

				Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

				

				if (document.Parse(buf).HasParseError())
				{
				//	s << "failed to parse the response";
				}				
				else
				{
					int index = document["index"].GetInt();
					__int64 cost = finishedtime ;
					allcosted += cost;
					if (i == 100)
					{
						std::cout << " costed time:" << allcosted / i << std::endl;;
						i = 0;
						allcosted = 0; 
					}
				//	s << document["response"].GetString();
					 
				}
			//	std::cout << s.str() << std::endl; 
				//result.set_value(s.str());
			}

		}
	 //	std::thread t(receivereponse, sock, (sockaddr*)&addrDest, ref(start), move(result));
	//	threads.push_back(move(t));

	}
	 //for (auto& result : vec)
	 //{
		// std::cout << result.get() << std::endl;

	 //}
	 //for (auto& t:threads)
	 //{
		// t.join();
	 //}


	return 0;

}
