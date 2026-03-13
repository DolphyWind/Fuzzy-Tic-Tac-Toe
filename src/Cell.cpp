#include <Cell.hpp>
#include <Exceptions.hpp>
#include <gmpxx.h>

namespace fttt
{

Cell::Cell() : m_Xval(0), m_Oval(0), m_cellType(CellType::EMPTY) {}
Cell::Cell(const mpf_class& x_value, const mpf_class& o_value) { set_cell(x_value, o_value); }

void Cell::set_cell(const mpf_class& x_value, const mpf_class& o_value)
{
    if (m_cellType != CellType::EMPTY)
    {
        throw CellOccupiedException{};
    }
    if (x_value + o_value > 1 || x_value < 0 || o_value < 0)
    {
        throw InvalidCellStateException{};
    }
    const mpf_class half("0.5");
    m_cellType = CellType::EMPTY;
    if (x_value > half)
    {
        m_cellType = CellType::X_CAPTURED;
    }
    else if (o_value > half)
    {
        m_cellType = CellType::O_CAPTURED;
    }
}

const mpf_class& Cell::get_Xval() const { return m_Xval; }

const mpf_class& Cell::get_Oval() const { return m_Oval; }

CellType Cell::get_cell_type() const { return m_cellType; }

} // namespace fttt
