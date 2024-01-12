//! @author Thomas Padioleau
//! @brief Class representation of MPI intra-communicator.
//! @file MpiComm.hpp
//! @date 1 Sept 2019
#pragma once

#include <mpi.h>
#include <string>

namespace distributed_memory_session
{

//! @brief Non-owning object representation of a MPI intra-communicator.
//!
//!  MPI communicator stored is not freed at the object destruction, i.e. MPI_Comm_free is not
//!  called.
class MpiComm
{
public:
    //! @return MpiComm object representing MPI_COMM_SELF.
    static MpiComm self() noexcept;

    //! @return MpiComm object representing MPI_COMM_WORLD.
    static MpiComm world() noexcept;

    //! @return MPI data type from MpiComm.
    template <class T>
    static int dataType() noexcept;

    //! @name Data types
    //! @{
    //! Integer data type.
    static constexpr int INT{1};
    //! Float data type.
    static constexpr int FLOAT{2};
    //! Double data type.
    static constexpr int DOUBLE{3};
    //! Long double data type.
    static constexpr int LONG_DOUBLE{9};
    //! Character data type.
    static constexpr int CHAR{4};
    //! @}

    //! @name Operations
    //! @{
    //! Summation operation.
    static constexpr int SUM{5};
    //! Minimize operation.
    static constexpr int MIN{6};
    //! Maximize operation.
    static constexpr int MAX{7};
    //! Dot-product (Multiplication) operation.
    static constexpr int PROD{8};
    //! @}

    //! @brief Checks the return value of an MPI call.
    //! For now it only prints the mpierr and the name of the calling function to stderr.
    static void errCheck(int mpierr, const std::string& methodName);

    //! @brief Converts a PMachine data type code to a MPI_Datatype.
    static MPI_Datatype getDataType(int type);

    //! @brief Converts a PMachine operator code to a MPI_Op operator code.
    static MPI_Op getOp(int op);

public:
    //! @brief Constructs a communicator based on MPI_COMM_WORLD.
    MpiComm() noexcept;

    //! @brief Constructs a communicator based on given comm.
    explicit MpiComm(MPI_Comm comm) noexcept;

    MpiComm(const MpiComm& comm) noexcept;

    MpiComm& operator=(const MpiComm& comm) noexcept;

    //! Is virtual to be able to inherit from this class.
    virtual ~MpiComm() = default;

    //! Returned value is meaningfull until MPI has not been finalized.
    //! @return Process rank.
    int rank() const noexcept;

    //! Returned value is meaningfull until MPI has not been finalized.
    //! @return Size of MPI communicator.
    int size() const noexcept;

    //! @brief Synchronizes all the processors in the communicator.
    void synchronize() const;

    //! @name Collective communications
    //@{
    //! @brief All-to-all gather-scatter
    void allToAll(void* sendBuf, int sendCount, int sendType,
                  void* recvBuf, int recvCount, int recvType) const;

    //! @brief Variable-length gather-scatter
    void allToAllv(void* sendBuf, int* sendCount, int* sendDisplacements,
                   int sendType,
                   void* recvBuf, int* recvCount,
                   int* recvDisplacements,
                   int recvType) const;

    //! @brief Does a collective operation, scattering the results to all processors
    void allReduce(void* input, void* result, int inputCount, int type,
                   int op) const;

    //! @brief Gathers to root
    void gather(void* sendBuf, int sendCount, int sendType,
                void* recvBuf, int recvCount, int recvType,
                int root) const;

    //! @brief Gathers variable-sized arrays to root
    void gatherv(void* sendBuf, int sendCount, int sendType,
                 void* recvBuf, int* recvCount, int* displacements,
                 int recvType, int root) const;

    //! @brief Gathers to all processors
    void allGather(void* sendBuf, int sendCount, int sendType,
                   void* recvBuf, int recvCount, int recvType) const;

    //! @brief Variable-length gather to all processors
    void allGatherv(void* sendBuf, int sendCount, int sendType,
                    void* recvBuf, int* recvCount, int* recvDisplacements,
                    int recvType) const;

    //! @brief Broadcast
    void bcast(void* msg, int length, int type, int src) const;
    //@}

    //! @name Point-to-point communications
    //@{
    //! @brief Blocking send to dest.
    void send(void *sendBuf, int sendCount, int sendType,
              int dest, int tag) const;

    //! @brief Blocking receive from source.
    void recv(void *recvBuf, int recvCount, int recvtype,
              int source, int tag) const;

    //! @brief Blocking send to dest and receive from src.
    void sendrecv(void *sendBuf, int sendCount, int sendType, int dest, int sendtag,
                  void *recvBuf, int recvCount, int recvType, int src, int recvtag) const;

    //! @brief Non-blocking send to dest.
    MPI_Request Isend(void *sendBuf, int sendCount, int sendType,
                      int dest, int tag) const;

    //! @brief Non-blocking buffered send to dest.
    MPI_Request Ibsend(void *sendBuf, int sendCount, int sendType,
                       int dest, int tag) const;

    //! @brief
    MPI_Request Issend(void *sendBuf, int sendCount, int sendType,
                       int dest, int tag) const;

    //! @brief
    MPI_Request Irsend(void *sendBuf, int sendCount, int sendType,
                       int dest, int tag) const;

    //! @brief
    MPI_Request Irecv(void *recvBuf, int recvCount, int recvType,
                      int source, int tag) const;
    //@}

    //! @return Stored communicator handle.
    MPI_Comm getComm() const;

protected:
    MPI_Comm m_comm = MPI_COMM_WORLD;

    int m_rank = 0;
    int m_size = 1;

    //! Initializes internal variables. It is called by all ctors.
    void init();

    //! Indicates whether MPI is currently running
    int mpiIsRunning() const ;
};

}
