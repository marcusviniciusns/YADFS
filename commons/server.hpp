/* 
 * File:   server.hpp
 * Author: marcusviniciusns
 *
 * Created on March 10, 2011, 9:48 PM
 */

#ifndef SERVER_HPP
#define	SERVER_HPP

namespace yadfs
{

class ServerConfig
{
public:
  int m_port;
  unsigned int m_retries;
  char *m_user;
  char *m_pass;
  
  ServerConfig()
  {
    m_port = 10000;
    m_retries = 10;
  }

  ServerConfig(int port, int retries) : m_port(port), m_retries(retries), m_user("root"), m_pass("manager")
  {
  }
};

class Server
{
private:
  bool m_running;
  int m_sockfd;
  static void *Receive(void *data);
protected:
  ServerConfig m_config;
public:
  Server(const ServerConfig& config);
  Server(const Server& orig);
  virtual ~Server();
  int Start();
  int Stop();
  virtual void *Receive(int sockfd);
};

}

#endif	/* SERVER_HPP */