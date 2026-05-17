#include "Cell.hpp"
#include "FTTTBoard.hpp"
#include <boost/python.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <gmpxx.h>
using namespace boost::python;

struct mpq_from_tuple {
    mpq_from_tuple() {
        converter::registry::push_back(
            &convertible,
            &construct,
            type_id<mpq_class>()
        );
    }

    static void* convertible(PyObject* obj_ptr) {
        if (!PyTuple_Check(obj_ptr)) return nullptr;
        if (PyTuple_Size(obj_ptr) != 2) return nullptr;

        PyObject* a = PyTuple_GetItem(obj_ptr, 0);
        PyObject* b = PyTuple_GetItem(obj_ptr, 1);

        if (!PyLong_Check(a) || !PyLong_Check(b)) return nullptr;

        return obj_ptr;
    }

    static void construct(
        PyObject* obj_ptr,
        converter::rvalue_from_python_stage1_data* data
    ) {
        long a = PyLong_AsLong(PyTuple_GetItem(obj_ptr, 0));
        long b = PyLong_AsLong(PyTuple_GetItem(obj_ptr, 1));

        void* storage = (
            (converter::rvalue_from_python_storage<mpq_class>*)data
        )->storage.bytes;

        new (storage) mpq_class(a, b);

        data->convertible = storage;
    }
};

struct mpq_from_int {
    mpq_from_int() {
        converter::registry::push_back(
            &convertible,
            &construct,
            type_id<mpq_class>()
        );
    }

    static void* convertible(PyObject* obj_ptr) {
        if (!PyLong_Check(obj_ptr)) return nullptr;
        return obj_ptr;
    }

    static void construct(
        PyObject* obj_ptr,
        converter::rvalue_from_python_stage1_data* data
    ) {
        long a = PyLong_AsLong(obj_ptr);

        void* storage = (
            (converter::rvalue_from_python_storage<mpq_class>*)data
        )->storage.bytes;

        new (storage) mpq_class(a, 1);

        data->convertible = storage;
    }
};

struct mpq_to_python {
    static PyObject* convert(const mpq_class& x) {
        const mpz_class& n = x.get_num();
        const mpz_class& d = x.get_den();

        PyObject* num = PyLong_FromString(n.get_str().c_str(), nullptr, 10);
        PyObject* den = PyLong_FromString(d.get_str().c_str(), nullptr, 10);

        return Py_BuildValue("(NN)", num, den);
    }
};

typedef fttt::FTTTBoard<3> DefaultBoard;

BOOST_PYTHON_MODULE(fttt)
{
    mpq_from_tuple();
    mpq_from_int();
    to_python_converter<mpq_class, mpq_to_python>();

    enum_<fttt::CellState>("CellState")
        .value("EMPTY", fttt::CellState::EMPTY)
        .value("X_CAPTURED", fttt::CellState::X_CAPTURED)
        .value("O_CAPTURED", fttt::CellState::O_CAPTURED)
    ;

    class_<fttt::Cell>("Cell", init<>())
        .def(init<mpq_class>())
        .def(init<mpq_class, mpq_class>())
        .def(init<mpq_class, mpq_class, mpq_class>())
        .def(
            "set_cell",
            &fttt::Cell::set_cell
        )
        .def(
            "get_Xval",
            &fttt::Cell::get_Xval,
            return_value_policy<copy_const_reference>()
        )
        .def(
            "get_Oval",
            &fttt::Cell::get_Oval,
            return_value_policy<copy_const_reference>()
        )
        .add_property("cell_state", &fttt::Cell::get_cell_state)
    ;

    class_<DefaultBoard::row_t>("Row")
        .def("__getitem__", +[](DefaultBoard::row_t& r, std::size_t i) -> fttt::Cell& {
            return r.at(i);
        }, return_internal_reference<>())
        .def("__setitem__", +[](DefaultBoard::row_t& r, std::size_t i, const fttt::Cell& v) {
            r.at(i) = v;
        })
        .def("__len__", +[](const DefaultBoard::row_t& row) -> std::size_t {
            return row.size();
        })
    ;

    class_<DefaultBoard::board_t>("Board")
        .def("__getitem__", +[](DefaultBoard::board_t& r, std::size_t i) -> DefaultBoard::row_t& {
            return r.at(i);
        }, return_internal_reference<>())
        .def("__setitem__", +[](DefaultBoard::board_t& r, std::size_t i, const DefaultBoard::row_t& v) {
            r.at(i) = v;
        })
        .def("__len__", +[](const DefaultBoard::board_t& board) -> std::size_t {
            return board.size();
        })
    ;

    class_<DefaultBoard>("DefaultBoard", init<>{})
        .def(init<mpq_class>{})
        .def("place", &DefaultBoard::place)
        .def("get_board",
            static_cast<const DefaultBoard::board_t& (DefaultBoard::* const)() const>(
                &DefaultBoard::get_board
            ),
            return_value_policy<copy_const_reference>()
        )
        .def(
            "is_valid_placement",
            &DefaultBoard::is_valid_placement
        )
    ;
}
