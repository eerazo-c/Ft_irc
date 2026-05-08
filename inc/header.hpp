#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <cerrno>

#include <exception>
#include <unistd.h>
#include <fcntl.h>
#include <cctype>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include "Client.hpp"
#include "Parser.hpp"
#include "Server.hpp"
#include "Command.hpp"

#define MAX_CONECTIONS 10




#define RESET   "\033[0m"       /*0      */

#define BLACK   "\033[30m"      /*1 Black */   

#define RED     "\033[31m"      /*2 Red */     
                                                   
#define GREEN   "\033[32m"      /*3 Green */                          

#define YELLOW  "\033[33m"      /*4 Yellow */      
                                                                                                         
#define BLUE    "\033[34m"      /*5 Blue */                                

#define MAGENTA "\033[35m"      /*6 Magenta */    
                                               
#define CYAN    "\033[36m"      /*7 Cyan */      
                                                               
#define WHITE   "\033[37m"      /*8 White */         
                                                      
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */    
                                                      
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */     
                                                   
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */      
                                                 
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */      
  
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */ 
 
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */  

#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */   

#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */    

//std::cout << RED << "hello world" << RESET << std::endl;                                                      
//Note: If you don't use RESET the color will remain changed until the next time you use a color code.     

//AUXILIAR
void wrong_args();
int check_args(int n, const char *port, const char* password);
void ft_message(std::string s, const char *color , int end_line);
#endif
