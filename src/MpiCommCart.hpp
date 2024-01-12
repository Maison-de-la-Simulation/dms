//! @author Thomas Padioleau
//! @brief Class representation of a cartesian MPI intra-communicator.
//! @file MpiCommCart.hpp
//! @date 1 Sept 2019
#pragma once

#include "MpiComm.hpp"

#include <array>
#include <mpi.h>

namespace distributed_memory_session
{

//! @brief Non-owning object representation of a cartesian MPI intra-communicator.
template <int dim>
class MpiCommCart : public MpiComm
{
public:
    //! @brief Constructs a MpiCommCart using a N-D cartesian virtual topology.
    //! @deprecated There is no good default constructor.
    MpiCommCart();

    MpiCommCart(MPI_Comm comm);

    //! @brief Constructs a MpiCommCart using a N-D cartesian virtual topology.
    MpiCommCart(std::array<int, dim> m, int isPeriodic, int allowReorder);

    //! @brief use MPI_Cart_rank to get rank of the shifted process.
    int getCartRank(const std::array<int, dim>& coords) const;

    //! @return Cartesian coordinates of a process of a given rank.
    std::array<int, dim> getCoords(int rank) const;

    //! Uses MPI_Cart_shift to get rank of the shifted proces
    //! regarding the given direction.
    int shift(int direction, int disp, int rank_source) const;

    //! @return rank of the neighbor process identified by the template parameter
    // template <NeighborLocation nl>
    // int getNeighborRank() const;
};

template <int dim>
MpiCommCart<dim>::MpiCommCart()
    : MpiComm{}
{
}

template <int dim>
MpiCommCart<dim>::MpiCommCart(MPI_Comm comm)
    : MpiComm{comm}
{
}

template <int dim>
MpiCommCart<dim>::MpiCommCart(std::array<int, dim> m, int isPeriodic, int allowReorder)
    : MpiComm{}
{
    std::array<int, dim> periods;

    for (int idim=0; idim<dim; ++idim)
    {
        periods[idim] = isPeriodic;
    }

    // create virtual topology cartesian 2D
    const int mpierr = ::MPI_Cart_create(MPI_COMM_WORLD, dim, m.data(), periods.data(),
                                         allowReorder, &m_comm);
    errCheck(mpierr, "MPI_Cart_create");

    // fill m_size and m_rank
    init();
}

template <int dim>
int MpiCommCart<dim>::getCartRank(const std::array<int, dim>& coords) const
{
    int rank;
    const int mpierr = ::MPI_Cart_rank(m_comm, coords.data(), &rank);
    errCheck(mpierr, "MPI_Cart_rank");

    return rank;
}

template <int dim>
std::array<int, dim> MpiCommCart<dim>::getCoords(int rank) const
{
    std::array<int, dim> coords {};
    const int mpierr = ::MPI_Cart_coords(m_comm, rank, dim, coords.data());
    errCheck(mpierr, "MPI_Cart_coords");

    return coords;
}

template <int dim>
int MpiCommCart<dim>::shift(int direction, int disp, int rank_source) const
{
    int rank_dest;
    const int mpierr = ::MPI_Cart_shift(m_comm, direction, disp, &rank_source, &rank_dest);
    errCheck(mpierr, "MPI_Cart_shift");

    return rank_dest;
}

// template <int dim>
// template <NeighborLocation nl>
// int MpiCommCart::getNeighborRank() const
// {
//    std::array<int, dim> coords {m_coords};

//    if (nl == X_MIN)
//    {
//       coords[0] -= 1;
//    }
//    else if (nl == X_MAX)
//    {
//       coords[0] += 1;
//    }
//    else if (nl == Y_MIN)
//    {
//       coords[1] -= 1;
//    }
//    else if (nl == Y_MAX)
//    {
//       coords[1] += 1;
//    }

//    return getCartRank(coords);
// }

extern template class MpiCommCart<1>;
extern template class MpiCommCart<2>;
extern template class MpiCommCart<3>;

}
