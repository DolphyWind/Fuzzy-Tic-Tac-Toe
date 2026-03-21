#include <Cell.hpp>
#include <Exceptions.hpp>
#include <gmpxx.h>

namespace fttt
{

Cell::Cell() : m_capture_low_bound(mpq_class(1, 2)), m_Xval(0), m_Oval(0), m_cellState(CellState::EMPTY) {}

Cell::Cell(const mpq_class& capture_low_bound) : m_Xval(0), m_Oval(0), m_cellState(CellState::EMPTY)
{ m_capture_low_bound = capture_low_bound; }
Cell::Cell(const mpq_class& x_value, const mpq_class& o_value, const mpq_class& capture_low_bound)
{
    m_capture_low_bound = capture_low_bound;
    set_cell(x_value, o_value);
}

void Cell::set_cell(const mpq_class& x_value, const mpq_class& o_value)
{
    if (m_cellState != CellState::EMPTY)
    {
        throw CellIsAlreadyOccupiedException{};
    }
    if (x_value + o_value > mpq_class{1} || x_value < mpq_class{0} || o_value < mpq_class{0})
    {
        throw InvalidCellStateException{};
    }
    m_cellState = CellState::EMPTY;
    if (x_value > m_capture_low_bound)
    {
        m_cellState = CellState::X_CAPTURED;
    }
    else if (o_value > m_capture_low_bound)
    {
        m_cellState = CellState::O_CAPTURED;
    }
    m_Xval = x_value;
    m_Oval = o_value;
}

const mpq_class& Cell::get_Xval() const { return m_Xval; }

const mpq_class& Cell::get_Oval() const { return m_Oval; }

CellState Cell::get_cell_state() const { return m_cellState; }

} // namespace fttt
