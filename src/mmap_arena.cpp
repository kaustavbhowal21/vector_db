#include "mmap_arena.hpp"
 
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
 
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <utility>
using namespace std;


MmapArena::MmapArena(const string& filename,size_t size) : filename_(filename), size_(size), data_(nullptr), fd_(-1)
{
    if (size == 0) {
              throw invalid_argument( "MmapArena size must be greater than 0."  );
    }

   fd_ = open(filename_.c_str(), O_RDWR | O_CREAT, 0644);

if (fd_ == -1) {
    throw runtime_error("Failed to open file " + filename + " : " + strerror(errno));
}

struct stat st;
if (fstat(fd_, &st) == -1) {
    int saved_errno = errno;
    close(fd_);
    fd_ = -1;
    throw runtime_error("Failed to stat file " + filename_ + ": " + strerror(saved_errno));
}

size_t existing = static_cast<size_t>(st.st_size);
if (existing > size_) {
    size_ = existing;
}
if (existing < size_) {
    if (ftruncate(fd_, static_cast<off_t>(size_)) == -1) {
        int saved_errno = errno;
        close(fd_);
        fd_ = -1;
        throw runtime_error("Failed to resize file " + filename_ + ": " + strerror(saved_errno));
    }
}

      try {
          map();
     }
    catch (...) {
        close(fd_);
        fd_ = -1;
        throw;
    }

    }


MmapArena::~MmapArena()
{
    if (data_ != nullptr) {
        msync(data_, size_, MS_SYNC);
        munmap(data_, size_);   
        data_ = nullptr;
    }
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}
   



    void MmapArena::map()
    {
    data_ = mmap(nullptr,size_,PROT_READ | PROT_WRITE,MAP_SHARED,fd_,0);

    if (data_ == MAP_FAILED) {
        data_ = nullptr;

        throw runtime_error("mmap failed for" +filename_ + ": " +strerror(errno));
    }
}


void MmapArena::unmap()
{
    if (data_ == nullptr)  return;
    

    if (munmap(data_, size_) == -1) {
        throw std::runtime_error("munmap failed: " +string(std::strerror(errno)));
    }

    data_ = nullptr;
}

void* MmapArena::data()
{
    return data_;
}



const void* MmapArena::data() const
{
    return data_;
}


size_t MmapArena::size() const
{
    return size_;
}


void MmapArena::flush()
{
    if (data_ == nullptr) {
        return;
    }

    if (msync(data_, size_, MS_SYNC) == -1) {

        throw runtime_error("msync failed: " +string(strerror(errno)));
  
    }


}



void MmapArena::grow(size_t new_size)
{
    
    if (new_size <= size_) {
        return;
    }

   
    void* old_data = data_;
    size_t old_size = size_;

    if (msync(old_data, old_size, MS_SYNC) == -1) {
        throw runtime_error("msync failed while growing arena: " +string(strerror(errno)));
    }

    if (munmap(old_data, old_size) == -1) {
        throw runtime_error("munmap failed while growing arena: " + string(strerror(errno))
        );
    }

    data_ = nullptr;
 if (ftruncate(fd_, static_cast<off_t>(new_size)) == -1) {
    int saved_errno = errno;

    data_ = mmap(nullptr, old_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (data_ == MAP_FAILED) {
        data_ = nullptr;
    }

    throw runtime_error("Failed to grow file " + filename_ + ": " + string(strerror(saved_errno)));
}
  
    size_ = new_size;

    try {
        map();
    }
    catch (...) {
       
        size_ = old_size;
        data_ = mmap(nullptr,old_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd_,0 );

        if (data_ == MAP_FAILED) {
            data_ = nullptr;
        }

        throw;
    }
}



MmapArena::MmapArena(MmapArena&& other) noexcept : filename_(move(other.filename_)) , size_(other.size_),data_(other.data_),fd_(other.fd_)
{
   
    other.size_ = 0;
    other.data_ = nullptr;
    other.fd_ = -1;
}



MmapArena& MmapArena::operator=(MmapArena&& other) noexcept
{

    if (this == &other) {
        return *this;
    }

    if (data_ != nullptr) {
    msync(data_, size_, MS_SYNC);
        munmap(data_, size_);
    }

    if (fd_ != -1) {
        close(fd_);
    }


    filename_ = std::move(other.filename_);
    size_ = other.size_;
    data_ = other.data_;
    fd_ = other.fd_;

    other.size_ = 0;
    other.data_ = nullptr;
    other.fd_ = -1;

    return *this;
}
