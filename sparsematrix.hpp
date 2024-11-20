#ifndef HF7_SKELETON_SPARSEMATRIX_HPP
#define HF7_SKELETON_SPARSEMATRIX_HPP

#include "SparseMatrixExceptions.hpp"
#include <forward_list>

/*
 * I've decided to use a single dynamically-allocated array to store my values. A vector would provide a bit of extra overhead (an extra range check for example)
 * I've also decided to use forward_lists to store the values instead of maps. We're trying to save space here by turning the matrix into a hash-table, and I don't think the extra time map provides would be worth the extra memory
 */

class SparseMatrix {

    struct Entry;

    int rowSize;
    int colSize;
    double defaultValue;
    size_t hashLim;
    std::forward_list<Entry>* table;


    /**
     * @param row sor
     * @param col oszlop
     * Hash is (the row-major ordering of elements in the matrix) % (the maximum elements in the hash table)
     * This number is deterministic, uniform-distributive and takes a constant time to calculate
     * */
    [[nodiscard]] int hashFunction(int row, int col) const {
        return (row*rowSize+col) % hashLim;
    }

    struct Entry {
        int rowNum, colNum;
        double value;
        bool operator<(const Entry& other) const {
            if(other.rowNum > rowNum) {
                return true;
            }
            if(other.rowNum == rowNum) {
                if(other.colNum > colNum) return true; else return false;
            }
            return false;
        }
        bool operator==(const Entry& other) const {return (rowNum == other.rowNum) && (colNum == other.colNum);}
        Entry(int rowNum, int colNum, double value) : rowNum(rowNum), colNum(colNum), value(value) {}
    };

public:
    /**
     * Konstruktor.
     * @param size_row sorok száma
     * @param size_col oszlopok száma
     * @param defaultValue alapérték
     * @param hashLim hash tábla mérete
     * */
    SparseMatrix(int size_row, int size_col, double defaultValue, size_t hashLim) : rowSize(size_row), colSize(size_col), defaultValue(defaultValue), hashLim(hashLim) {
        if(rowSize < 1 || colSize < 1) throw InvalidSizeException();
        table = new std::forward_list<Entry>[hashLim];
    }

    /**
     * Destruktor.
     * */
    ~SparseMatrix() {
        delete[] table;
    }

    //The function call time is not worth erasing the inline keyword
    inline int checkIndices(int row, int col) const {
        if(row >= rowSize || col >= colSize || row < 0 || col < 0) throw IndexOutOfRangeException();
        return hashFunction(row, col);
    }

    /**
     * @returns érték (x,y) helyen
     * @throws IndexOutOfRangeException kiindexelés esetén
     * */
    double operator() (int row, int col) const {
        int hashVal = checkIndices(row, col);
        if(table[hashVal].empty()) return defaultValue;
        for(auto& entry : table[hashVal]) {
            if(entry.rowNum == row && entry.colNum == col) return entry.value;
        }
        return defaultValue;
    }

    /**
     * Mátrix összeadás.
     * Visszaad egy új SparseMatrix-ot, ami az eredeti és a paraméterként kapott SparseMatrix összege.
     * @throws SizeMismatchException ha a két mátrix mérete nem egyezik
     * */
    //Pray you've got a move constructor
    SparseMatrix operator+ (const SparseMatrix& other) const {
        if(rowSize != other.rowSize && colSize != other.colSize) throw SizeMismatchException();
        SparseMatrix m{rowSize,colSize,defaultValue+other.defaultValue,hashLim}; //Making a new matrix with the default value of the sum of the two default values

        //I've made some hideous calculations on a piece of paper to deduce that these two will result in the correct values
        for(size_t i = 0; i < hashLim; ++i) {
            if(table[i].empty()) continue;
            for(auto& entry : table[i]) {
                m.set(entry.rowNum,entry.colNum,entry.value+other.defaultValue);
            }
        }

        for(size_t i = 0; i < other.hashLim; ++i) {
            if(other.table[i].empty()) continue;
            for(auto& entry : other.table[i]) {
                m.set(entry.rowNum,entry.colNum,m(entry.rowNum,entry.colNum)-other.defaultValue+entry.value);
            }
        }

        return m;
    }

    /**
     * Setter.
     * Matrix az (x,y) helyen value érték lesz.
     * @throws IndexOutOfRangeException kiindexelés esetén
     * */
    void set(int row, int col, double value) {
        if(value == defaultValue) {
            clear(row, col);
            return;
        }
        int hashVal = checkIndices(row, col);

        if(table[hashVal].empty()) {
            table[hashVal].emplace_front(row,col,value); //https://cplusplus.com/reference/forward_list/forward_list/emplace_front/
            return;
        }

        for(auto& entry : table[hashVal]) {
            if(entry.rowNum == row && entry.colNum == col) {
                entry.value = value;
                return;
            }
        }
        table[hashVal].emplace_front(row,col,value);
    }

    /**
     * Setter törlésre.
     * Visszaállítja (x,y) értékét default értékre.
     * @throws IndexOutOfRangeException kiindexelés esetén
     * */
    void clear(int row, int col) {
        int hashVal = checkIndices(row, col);
        if(table[hashVal].empty()) return;
        std::forward_list<Entry>::iterator entry1 = table[hashVal].begin();
        std::forward_list<Entry>::iterator entry2 = table[hashVal].before_begin();
        while(entry1 != table[hashVal].end()) {
            if(entry1->colNum == col && entry1->rowNum == row) {
                table[hashVal].erase_after(entry2);
                return;
            }
            ++entry2;
            ++entry1;
        }
    }
	
	/**
	 * Kiírja a teljes Mátrix értékét a konzolra.
	 * */
    void printMatrix() const {
        for(size_t i = 0; i < hashLim; ++i) {
            if(table[i].empty()) continue;
            for(auto& entry : table[i]) {
                std::cout << "\n[" << entry.rowNum << ", " << entry.colNum << "] : " << entry.value;
                std::cout << '\n';
            }
        }
    }

    /**
     * Visszaadja a default értéket.
     * @returns defaultValue
     * */
    double getDefaultValue() const {
        return defaultValue;
    }

    /**
     * Mátrix négyzetes összeadás.
     * Teszteléshez használatos.
     * Visszaad egy új SparseMatrix-ot, ami az eredeti és a paraméterként kapott SparseMatrix összege.
     * @throws SizeMismatchException ha a két mátrix mérete nem egyezik
     * */
    SparseMatrix add(const SparseMatrix& other) const {
        if(rowSize != other.rowSize or colSize != other.colSize)
            throw SizeMismatchException();

        SparseMatrix result = SparseMatrix(rowSize, colSize, defaultValue + other.defaultValue, hashLim);
        for (int row = 0; row < rowSize; ++row) {
            for (int col = 0; col < colSize; ++col) {
                result.set(row, col, (*this)(row, col) + other(row, col));
            }
        }
        return result;
    }

};
#endif //HF7_SKELETON_SPARSEMATRIX_HPP
