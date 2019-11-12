#include <iostream>
#include "BigInt.h"

    BigInt::BigInt(){
        len = 1;
        sign = true;
        num = new char;
        *num = 0;
    }

    BigInt::BigInt(long long n)
    {
        if (n == 0){
            len = 1;
            sign = true;
            num = new char;
            *num = 0;
        } else {
            sign = true;
            len = 0;
            if (n < 0){
                sign = false;
                n *= -1;
            }
            long long cp_n = n;
            while (cp_n != 0){
                ++len;
                cp_n /= 10;
            }
            num = new char [len];
            for (size_t i = 0; i < len; ++i){
                char tmp = n % 10;
                n /= 10;
                num[i] = tmp;
            }

        }
    }

    BigInt::BigInt(char *n, int l, bool s)
    {
        len = l;
        sign = s;
        num = new char [len];
        for (int i = 0; i < len; ++i){
            num[i] = n[i];
        }
    }

    BigInt::BigInt(BigInt&& x)
    {
        num = x.get_num();
        len = x.get_len();
        sign = x.get_sign();
    }

    size_t BigInt::get_len() const
    {
        return len;
    }

    bool BigInt::get_sign() const
    {
        return sign;
    }

    char* BigInt::get_num() const
    {
        char* num_cp = new char [len];
        for (int i = 0; i < len; ++i){
            num_cp[i] = num[i];
        }
        return num_cp;
    }

    char* BigInt::get_num_rev() const
    {
        char* num_cp = new char [len];
        size_t j = 0;
        for (int i = len - 1; i >= 0; --i){
            num_cp[j] = num[i];
            j++;
        }
        return num_cp;
    }

    BigInt::BigInt(const BigInt &x)
    {
        num = x.get_num();
        len = x.get_len();
        sign = x.get_sign();
    }

    BigInt BigInt::operator + (const BigInt &y) const
    {
        if (!this->get_sign()){
            BigInt x(this->get_num(), this->get_len());
            return (y - x);
        }
        else if (!y.get_sign()){
            BigInt x(y.get_num(), y.get_len());
            return ((*this) - x);
        }
        char* num1 = nullptr;
        char* num2 = nullptr;
        size_t len_num1, len_num2;
        if (this->get_len() >= y.get_len())
        {
            num1 = this->get_num();
            num2 = y.get_num();
            len_num1 = this->get_len();
            len_num2 = y.get_len();
        } else {
            num1 = y.get_num();
            num2 = this->get_num();
            len_num1 = y.get_len();
            len_num2 = this->get_len();
        }

        int sum = 0;
        for(size_t i = 0; i < len_num2; ++i){
            sum = num1[i] + num2[i];
            if ((sum >= 10) && (i != len_num1 - 1)){
                num1[i + 1] += sum / 10;
                num1[i] = sum % 10;
            }else{
                num1[i] = sum;
            }
        }

        for (size_t i = len_num2; i < len_num1 - 1; ++i){
            if (num1[i] >= 10){
                num1[i + 1] += num1[i] / 10;
                num1[i] %= 10;
            } else {
                break;
            }
        }

        if (num1[len_num1 - 1] >= 10){
            char* tmp = num1;
            num1 = new char [len_num1 + 1];
            for (int i = 0; i < len_num1; ++i){
                num1[i] = tmp[i];
            }
            delete [] tmp;
            num1[len_num1] = num1[len_num1 - 1] / 10;
            num1[len_num1 - 1] %= 10;
            len_num1++;
        }
        
        delete [] num2;
        return BigInt(num1, len_num1); 
    }

    BigInt BigInt::operator - () const{
        if (len == 1 && *num == 0){
            return BigInt(num, len, true);
        }
        return BigInt(num, len, !sign);
    }

    void BigInt::operator = (BigInt&& n){
        delete [] num;
        num = n.get_num();
        len = n.get_len();
        sign = n.get_sign();
    }

    void BigInt::operator = (const BigInt &x){
        delete [] num;
        num = x.get_num();
        len = x.get_len();
        sign = x.get_sign();
    }


    int BigInt::compare(const BigInt &x) const
    {
        if (sign && !x.sign){
            return 1;
        }
        if (!sign && x.sign){
            return -1;
        }

        int chk = 1;
        if (!sign && !x.sign){
            chk = -1;
        }

        if (len < x.len) {
            return -1 * chk;
        }
        if (len > x.len) {
            return chk;
        }
        for (int i = len - 1; i >= 0; --i){
            if (num[i] < x.num[i]){
                return -1 * chk;
            }
            if (num[i] > x.num[i]){
                return chk;
            }
        }
        return 0;
    }

    bool BigInt::operator < (BigInt const &x) const
    {
        return compare(x) == -1;
    }

    bool BigInt::operator <= (BigInt const &x) const
    {
        int compared = compare(x);
        return compared == 0 || compared == -1;
    }

    bool BigInt::operator > (BigInt const &x) const
    {
        return compare(x) == 1;
    }

    bool BigInt::operator >= (BigInt const &x) const
    {
        int compared = compare(x);
        return compared == 0 || compared == 1;
    }

    bool BigInt::operator == (BigInt const &x) const
    {
        return compare(x) == 0;
    }

    bool BigInt::operator != (BigInt const &x) const
    {
        return !(*this == x);
    }


    BigInt BigInt::operator -(const BigInt &y) const
    {
        if (!this->get_sign()){
            BigInt x(this->get_num(), this->get_len());
            return -(x + y);
        }
        else if (!y.get_sign()){
            BigInt x(y.get_num(), y.get_len());
            return (*this + x);
        } else if (*this < y){
            return -(y - *this);
        }
        char* num1 = this->get_num();
        char* num2 = y.get_num();
        size_t len_num1 = this->get_len();
        size_t len_num2 = y.get_len();
        int dif = 0;
        size_t i = 0;
        while ((i < len_num1) || (i < len_num2)){
            if (i < len_num1){
                dif += num1[i];
            }
            if (i < len_num2){
                dif -= num2[i];
            }
            if (dif < 0){
                num1[i] = dif + 10;
                dif = -1;
            } else {
                num1[i] = dif % 10;
                dif /= 10;
            }
            ++i;
        }

        if (len_num1 > 1){
            do
            {
                i = len_num1 - 1;
                if (num1[i] == 0){
                    len_num1--;
                } else {
                    break;
                }
            } while (len_num1 > 1);
        }

        char *tmp = num1;
        num1 = new char[len_num1];
        for (int i = 0; i < len_num1; ++i){
            num1[i] = tmp[i];
        }
        delete [] num2;
        delete [] tmp;

        if (dif < 0) {
            return BigInt(num1, len_num1, false);
        } else {
            return BigInt(num1, len_num1);
        }

    }

    BigInt::~BigInt()
    {
        if (num != nullptr){
            if (len == 1) {
                delete num;
            } else {
                delete [] num;
            }
        }
    }

std::ostream& operator << (std::ostream &out, const BigInt &x)
{
    if (!x.get_sign()){
        out << '-';
    }
    for (int i = x.len - 1; i >= 0; i--){
            out << (int)x.num[i];
        }
        return out;
}
