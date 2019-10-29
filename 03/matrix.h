class Matrix_row
{
    size_t col;
    int* row;
public:
    
    Matrix_row(int* tmp, size_t count_col):row(tmp), col(count_col) {}

    int& operator [](int index) const
    {
        if (index >= col){
            throw std::out_of_range("Index columns co big");
        }
        return row[index];
    }
};


class Matrix
{
    size_t rows;
    size_t col;
    int* matr;
public:
    Matrix(size_t count_rows, size_t count_col):rows(count_rows), col(count_col) 
    {
        matr = (int*) calloc(count_rows * count_col, sizeof(int));
        if (matr == nullptr){
            throw std::bad_alloc();
        }
    }

    Matrix(const Matrix &x)
    {
        rows = x.getRows();
        col = x.getColumns();
        matr = (int*) malloc(rows * col * sizeof(int));
        if (matr == nullptr){
            throw std::bad_alloc();
        }
        for (size_t i = 0; i < rows; ++i){
            for(size_t j = 0; j < col; ++j){
               matr[i * col + j] = x[i][j];
            }
        }
    }


    size_t getRows() const
    {
        return rows;
    }

    size_t getColumns() const
    {
        return col;
    }

    Matrix_row operator[] (int index_row) const
    {
        if (index_row >= rows){
            throw std::out_of_range("Index row so big");
        } 
        return Matrix_row(matr + index_row * col, col);
    }

    Matrix& operator*= (int mul)
    {
        size_t len = rows * col;
        for (size_t i = 0; i < len; ++i){
            matr[i] *= mul;
        }
        return *this;
    }

    bool operator== (const Matrix &x) const
    {
        if (rows != x.getRows() || col != x.getColumns()){
            return false;
        }
        for (size_t i = 0; i < rows; ++i){
            for (size_t j = 0; j < col; ++j){
                if ((*this)[i][j] != x[i][j]){
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!= (const Matrix &x) const
    {
        return !((*this) == x);
    }

    ~Matrix(){
        if (matr != nullptr){
            free(matr);
        }
    }
};
