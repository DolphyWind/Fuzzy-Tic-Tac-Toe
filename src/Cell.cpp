#include <Cell.hpp>
#include <Exceptions.hpp>
#include <gmpxx.h>

namespace fttt
{

Cell::Cell() : m_caputure_limit("0.5"), m_Xval(0), m_Oval(0), m_cellState(CellState::EMPTY) {}

Cell::Cell(const mpf_class& capture_limit) : m_Xval(0), m_Oval(0), m_cellState(CellState::EMPTY)
{ m_caputure_limit = capture_limit; }
Cell::Cell(const mpf_class& x_value, const mpf_class& o_value, const mpf_class& capture_limit)
{
    m_caputure_limit = capture_limit;
    set_cell(x_value, o_value);
}

void Cell::set_cell(const mpf_class& x_value, const mpf_class& o_value)
{
    if (m_cellState != CellState::EMPTY)
    {
        throw CellIsAlreadyOccupiedException{};
    }
    if (x_value + o_value > 1.0 || x_value < 0.0 || o_value < 0.0)
    {
        throw InvalidCellStateException{};
    }
    m_cellState = CellState::EMPTY;
    if (x_value > m_caputure_limit)
    {
        m_cellState = CellState::X_CAPTURED;
    }
    else if (o_value > m_caputure_limit)
    {
        m_cellState = CellState::O_CAPTURED;
    }
    m_Xval = x_value;
    m_Oval = o_value;
}

const mpf_class& Cell::get_Xval() const { return m_Xval; }

const mpf_class& Cell::get_Oval() const { return m_Oval; }

CellState Cell::get_cell_state() const { return m_cellState; }

} // namespace fttt
