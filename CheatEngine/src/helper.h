#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <iostream>

// Declarations
template <class T>
T ask_for(const std::string& message);



// Definitions
template <class T>
T ask_for(const std::string& message) {
	T t;
	
	std::cout << message << ": ";
	cin >> t;

	return t;
}

#endif