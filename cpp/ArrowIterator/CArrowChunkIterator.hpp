/*
 * Copyright (c) 2013-2019 Snowflake Computing
 */
#ifndef PC_ARROWCHUNKITERATOR_HPP
#define PC_ARROWCHUNKITERATOR_HPP

#include <Python.h>
#include <vector>
#include <arrow/python/platform.h>
#include <arrow/api.h>
#include <arrow/python/pyarrow.h>
#include "IColumnConverter.hpp"

namespace sf
{

/**
 * Arrow chunk iterator implementation in C++. The caller (python arrow chunk iterator object)
 * will ask for nextRow to be returned back to Python
 */
class CArrowChunkIterator
{
public:
    /**
     * Constructor
     */
    CArrowChunkIterator();

    /**
     * Desctructor
     */
    ~CArrowChunkIterator()
    {
        Py_XDECREF(m_latestReturnedRow);
    }

    /**
     * Add Arrow RecordBach to current chunk
     * @param rb recordbatch to be added
     */
    void addRecordBatch(PyObject *rb);

    /**
     * @return a python tuple object which contains all data in current row
     */
    PyObject * nextRow();

private:
    /** list of all record batch in current chunk */
    std::vector<std::shared_ptr<arrow::RecordBatch>> m_cRecordBatches;

    /** number of columns */
    int m_columnCount;

    /** number of record batch in current chunk */
    int m_batchCount;

    /** current index that iterator points to */
    int m_currentBatchIndex;

    /** row index inside current record batch (start from 0) */
    int m_rowIndexInBatch;

    /** total number of rows inside current record batch */
    int64_t m_rowCountInBatch;
   
    /** pointer to the latest returned python tuple(row) result */
    PyObject* m_latestReturnedRow;

    /** list of column converters*/
    std::vector<std::shared_ptr<sf::IColumnConverter>> m_currentBatchConverters;

    void reset();

    /**
     * @return python object of tuple which is tuple of all row values
     */
    PyObject * currentRowAsTuple();

    void initColumnConverters();
};

}



#endif // PC_ARROWCHUNKITERATOR_HPP

