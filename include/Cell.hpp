#ifndef __CELL_HPP_INCLUDE_GUARD__
#define __CELL_HPP_INCLUDE_GUARD__
#include <Exceptions.hpp>
#include <gmpxx.h>

namespace fttt
{
enum class CellType
{
    EMPTY = 0,
    X_CAPTURED = (1 << 1),
    O_CAPTURED = (1 << 2),
};

class Cell
{
public:
    Cell();
    Cell(const mpf_class& x_value, const mpf_class& o_value);

    void set_cell(const mpf_class& x_value, const mpf_class& o_value);
    const mpf_class& get_Xval() const;
    const mpf_class& get_Oval() const;
    CellType get_cell_type() const;
private:
    mpf_class m_Xval;
    mpf_class m_Oval;
    CellType m_cellType;
};
} // namespace ftt

#endif // !__CELL_HPP_INCLUDE_GUARD__
