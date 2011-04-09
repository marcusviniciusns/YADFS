/* 
 * File:   yadfs_client.hpp
 * Author: marcusviniciusns
 *
 * Created on March 22, 2011, 7:31 PM
 */
#ifndef YADFS_CLIENT_HPP
#define	YADFS_CLIENT_HPP

#include <fuse.h>

#ifdef	__cplusplus

#include "job.hpp"
#include "worker.hpp"
#include "worker_pool.hpp"
#include "../commons/raid_mode.h"
#include "../commons/data_node.hpp"
#include "../commons/chunk.h"
#include "../commons/client.hpp"

#include <map>
#include <vector>
#include <pthread.h>

using std::map;
using std::pair;
using std::vector;


namespace yadfs
{

typedef map<string, size_t> sizes_map;
typedef sizes_map::iterator sizes_it;
typedef pair<string, size_t> sizes_pair;

typedef map<string, WorkerPool *> workers_map;
typedef workers_map::iterator workers_it;
typedef pair<string, WorkerPool *> workers_pair;

class YADFSClient : public Client
{
private:
  unsigned int m_nodeCount;

  // vector<Worker *> m_workers;
  workers_map m_workers;
  vector<Client *> m_node_clients;
  
  sizes_map m_sizes;
  Mode m_mode;

  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;

  // int m_waiting_count;
  // string m_path_to_release;
  // pthread_mutex_t m_mutex;

  void releaseWrite(void *instance);
public:
  YADFSClient(const ClientConfig& config);
  virtual ~YADFSClient();
  bool init();
  bool enqueueWrite(const char *path, const char *buf, size_t size,
    off_t offset);
  bool releaseWrite(const char *path);

  void incSize(const string& path, size_t size)
  {
    sizes_it it = m_sizes.find(path);
    if (it == m_sizes.end())
    {
      sizes_pair p;
      p.first = path;
      p.second = size;
      m_sizes.insert(p);
      return;
    }
    it->second += size;
  }

  size_t getSize(const string& path)
  {
    sizes_it it = m_sizes.find(path);
    if (it == m_sizes.end())
    {
      return 0;
    }
    return it->second;
  }

  void remSize(const string& path)
  {
    sizes_it it = m_sizes.find(path);
    if (it == m_sizes.end())
    {
      return;
    }
    m_sizes.erase(it);
  }

};

class JobData
{
public:
  unsigned int m_file_id;
  unsigned int m_chunk_id;
  char m_path[256];
  char m_data[CHUNK_SIZE];
  size_t m_size;
  Client *m_node_client;
};

}

extern "C"
{
#endif

int yadfs_client_init(char *host, int port);
int yadfs_getattr_real(const char *path, struct stat *stbuf);
int yadfs_readdir_real(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi);
int yadfs_mknod_real(const char *path, mode_t mode, dev_t rdev);
int yadfs_utimens_real(const char *path, const struct timespec ts[2]);
int yadfs_open_real(const char *path, struct fuse_file_info *fi);
int yadfs_write_real(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi);
int yadfs_read_real(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi);
int yadfs_release_real(const char *path, struct fuse_file_info *fi);

void write_func(void *data);
void read_func(void *data);

#ifdef	__cplusplus
}
#endif

#endif	/* YADFS_CLIENT_HPP */

