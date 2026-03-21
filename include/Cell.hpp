#ifndef __CELL_HPP__
#define __CELL_HPP__
#include <Exceptions.hpp>
#include <gmpxx.h>

namespace fttt
{
enum class CellState
{
    EMPTY = 0,
    X_CAPTURED = (1 << 0),
    O_CAPTURED = (1 << 1),
};

class Cell
{
public:
    Cell();
    Cell(const mpq_class& capture_low_bound);
    Cell(const mpq_class& x_value, const mpq_class& o_value, const mpq_class& capture_low_bound=mpq_class(1, 2));

    void set_cell(const mpq_class& x_value, const mpq_class& o_value);
    const mpq_class& get_Xval() const;
    const mpq_class& get_Oval() const;
    CellState get_cell_state() const;
private:
    mpq_class m_capture_low_bound;
    mpq_class m_Xval;
    mpq_class m_Oval;
    CellState m_cellState;
};
} // namespace ftt

#endif // !__CELL_HPP__
