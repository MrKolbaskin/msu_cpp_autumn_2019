#ifndef BIGINT
#define BIGINT


class BigInt
{
    char* num;
    bool sign;
    size_t len;
    BigInt(char *n, int l, bool s=true);
    size_t get_len() const;
    bool get_sign() const;
    char* get_num() const;
    char* get_num_rev() const;
    int compare(const BigInt &x) const;
public:
    BigInt();
    BigInt(long long n);
    BigInt(BigInt&& x);
    BigInt(const BigInt &x);
    ~BigInt();
    BigInt operator + (const BigInt &y) const;
    BigInt operator - () const;
    BigInt operator -(const BigInt &y) const;
    void operator = (BigInt&& n);
    void operator = (const BigInt &x);
    bool operator < (BigInt const &x) const;
    bool operator <= (BigInt const &x) const;
    bool operator > (BigInt const &x) const;
    bool operator >= (BigInt const &x) const;
    bool operator == (BigInt const &x) const;
    bool operator != (BigInt const &x) const;
    friend std::ostream& operator << (std::ostream &out, const BigInt &x);
};

#endif