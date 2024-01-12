#include "MpiComm.hpp"

#include <iostream>
#include <mpi.h>

namespace distributed_memory_session
{

MpiComm::MpiComm() noexcept
{
    init();
}

MpiComm::MpiComm(MPI_Comm comm) noexcept
    : m_comm{comm}
{
    init();
}

MpiComm::MpiComm(const MpiComm& comm) noexcept
    : m_comm{comm.m_comm}
{
    init();
}

MpiComm& MpiComm::operator=(const MpiComm& comm) noexcept
{
    m_comm = comm.m_comm;
    init();
    return *this;
}

MpiComm MpiComm::self() noexcept
{
    return MpiComm(MPI_COMM_SELF);
}

MpiComm MpiComm::world() noexcept
{
    return MpiComm(MPI_COMM_WORLD);
}

template <>
int MpiComm::dataType<int>() noexcept
{
    return MpiComm::INT;
}

template <>
int MpiComm::dataType<float>() noexcept
{
    return MpiComm::FLOAT;
}

template <>
int MpiComm::dataType<double>() noexcept
{
    return MpiComm::DOUBLE;
}

template <>
int MpiComm::dataType<long double>() noexcept
{
    return MpiComm::LONG_DOUBLE;
}

template <>
int MpiComm::dataType<char>() noexcept
{
    return MpiComm::CHAR;
}

int MpiComm::rank() const noexcept
{
    return m_rank;
}

int MpiComm::size() const noexcept
{
    return m_size;
}

void MpiComm::synchronize() const
{
    if (mpiIsRunning())
    {
        errCheck(::MPI_Barrier(m_comm), "MPI_Barrier");
    }
}

void MpiComm::allToAll(void* sendBuf, int sendCount, int sendType,
                       void* recvBuf, int recvCount, int recvType) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Alltoall(sendBuf, sendCount, mpiSendType,
                                recvBuf, recvCount, mpiRecvType,
                                m_comm), "MPI_Alltoall");
    }
}

void MpiComm::allToAllv(void* sendBuf, int* sendCount,
                        int* sendDisplacements, int sendType,
                        void* recvBuf, int* recvCount,
                        int* recvDisplacements, int recvType) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Alltoallv(sendBuf, sendCount, sendDisplacements, mpiSendType,
                                 recvBuf, recvCount, recvDisplacements, mpiRecvType,
                                 m_comm), "MPI_Alltoallv");
    }
}

void MpiComm::allReduce(void* input, void* result, int inputCount,
                        int type, int op) const
{
    MPI_Op mpiOp = getOp(op);
    MPI_Datatype mpiType = getDataType(type);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Allreduce(input, result, inputCount, mpiType,
                                 mpiOp, m_comm),
                 "MPI_Allreduce");
    }
}

void MpiComm::gather(void* sendBuf, int sendCount, int sendType,
                     void* recvBuf, int recvCount, int recvType,
                     int root) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);
    MPI_Datatype mpiRecvType = getDataType(recvType);


    if (mpiIsRunning())
    {
        errCheck(::MPI_Gather(sendBuf, sendCount, mpiSendType,
                              recvBuf, recvCount, mpiRecvType,
                              root, m_comm), "MPI_Gather");
    }
}

void MpiComm::gatherv(void* sendBuf, int sendCount, int sendType,
                      void* recvBuf, int* recvCount, int* displacements, int recvType,
                      int root) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Gatherv(sendBuf, sendCount, mpiSendType,
                               recvBuf, recvCount, displacements, mpiRecvType,
                               root, m_comm), "MPI_Gatherv");
    }
}

void MpiComm::allGather(void* sendBuf, int sendCount, int sendType,
                        void* recvBuf, int recvCount,
                        int recvType) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Allgather(sendBuf, sendCount, mpiSendType,
                                 recvBuf, recvCount,
                                 mpiRecvType, m_comm),
                 "MPI_AllGather");
    }
}


void MpiComm::allGatherv(void* sendBuf, int sendCount, int sendType,
                         void* recvBuf, int* recvCount,
                         int* recvDisplacements,
                         int recvType) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Allgatherv(sendBuf, sendCount, mpiSendType,
                                  recvBuf, recvCount, recvDisplacements,
                                  mpiRecvType,
                                  m_comm),
                 "AllGatherv");
    }
}

void MpiComm::bcast(void* msg, int length, int type, int src) const
{
    if (mpiIsRunning())
    {
        MPI_Datatype mpiType = getDataType(type);
        errCheck(::MPI_Bcast(msg, length, mpiType, src,
                             m_comm), "Bcast");
    }
}

void MpiComm::send(void *sendBuf, int sendCount, int sendType,
                   int dest, int tag) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Send(sendBuf, sendCount, mpiSendType, dest, tag, m_comm), "MPI_Send");
    }
}

void MpiComm::recv(void *recvBuf, int recvCount, int recvType,
                   int source, int tag) const
{
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Recv(recvBuf, recvCount, mpiRecvType, source, tag, m_comm, MPI_STATUS_IGNORE), "MPI_Send");
    }
}

void MpiComm::sendrecv(void *sendBuf, int sendCount, int sendType,
                       int dest, int sendtag,
                       void *recvBuf, int recvCount, int recvType,
                       int src, int recvtag) const
{
    MPI_Datatype mpiSendType = getDataType(sendType);
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Sendrecv(sendBuf, sendCount, mpiSendType, dest, sendtag,
                                recvBuf, recvCount, mpiRecvType, src, recvtag,
                                m_comm, MPI_STATUS_IGNORE), "MPI_Sendrecv");
    }
}

MPI_Request MpiComm::Isend(void *sendBuf, int sendCount, int sendType,
                           int dest, int tag) const
{
    MPI_Request request;
    MPI_Datatype mpiSendType = getDataType(sendType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Isend(sendBuf, sendCount, mpiSendType,
                             dest, tag, m_comm, &request), "MPI_Isend");
    }

    return request;
}

MPI_Request MpiComm::Ibsend(void *sendBuf, int sendCount, int sendType,
                            int dest, int tag) const
{
    MPI_Request request;
    MPI_Datatype mpiSendType = getDataType(sendType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Ibsend(sendBuf, sendCount, mpiSendType,
                              dest, tag, m_comm, &request), "MPI_Ibsend");
    }

    return request;
}

MPI_Request MpiComm::Issend(void *sendBuf, int sendCount, int sendType,
                            int dest, int tag) const
{
    MPI_Request request;
    MPI_Datatype mpiSendType = getDataType(sendType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Issend(sendBuf, sendCount, mpiSendType,
                              dest, tag, m_comm, &request), "MPI_Issend");
    }

    return request;
}

MPI_Request MpiComm::Irsend(void *sendBuf, int sendCount, int sendType,
                            int dest, int tag) const
{
    MPI_Request request;
    MPI_Datatype mpiSendType = getDataType(sendType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Irsend(sendBuf, sendCount, mpiSendType,
                              dest, tag, m_comm, &request), "MPI_Irsend");
    }

    return request;
}

MPI_Request MpiComm::Irecv(void *recvBuf, int recvCount, int recvType,
                           int source, int tag) const
{
    MPI_Request request;
    MPI_Datatype mpiRecvType = getDataType(recvType);

    if (mpiIsRunning())
    {
        errCheck(::MPI_Irecv(recvBuf, recvCount, mpiRecvType,
                             source, tag, m_comm, &request), "MPI_Irecv");
    }

    return request;
}

MPI_Comm MpiComm::getComm() const
{
    return m_comm;
}

void MpiComm::errCheck(int mpierr, const std::string& methodName)
{
    if (mpierr != 0)
    {
        std::cerr << "MPI function " << methodName
                  << " returned error code=" << mpierr << std::endl;
    }
}

MPI_Datatype MpiComm::getDataType(int type)
{
    if(type == INT) return MPI_INT;
    if(type == FLOAT) return MPI_FLOAT;
    if(type == DOUBLE) return MPI_DOUBLE;
    if(type == LONG_DOUBLE) return MPI_LONG_DOUBLE;

    return MPI_CHAR;
}

MPI_Op MpiComm::getOp(int op)
{
    if( op == SUM) return MPI_SUM;
    else if( op == MAX) return MPI_MAX;
    else if( op == MIN) return MPI_MIN;
    return MPI_PROD;
}

void MpiComm::init()
{
    if (mpiIsRunning())
    {
        errCheck(::MPI_Comm_rank(m_comm, &m_rank), "MPI_Comm_rank");
        errCheck(::MPI_Comm_size(m_comm, &m_size), "MPI_Comm_size");
    }
    else
    {
        m_size = 1;
        m_rank = 0;
        std::cerr << "WARNING ! You are using a MPI communicator outside a distributed region !\n";
    }
}

int MpiComm::mpiIsRunning() const
{
    int mpi_is_finalized = -1;
    errCheck(::MPI_Finalized(&mpi_is_finalized), "MPI_Finalized");
    int mpi_is_initialized = -1;
    errCheck(::MPI_Initialized(&mpi_is_initialized), "MPI_Initialized");
    return static_cast<bool>(mpi_is_initialized) && !static_cast<bool>(mpi_is_finalized);
}

}
