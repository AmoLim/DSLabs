//
// Created by Amo on 2022/10/22.
//

#include "BigInt.h"

BigInt::BigInt() {
    flag = 0;
}

BigInt::~BigInt() {
    bits_.clear();
}

BigInt::BigInt(int val) {
    if (val < 0) {
        flag = 1;
        val = abs(val);
    }
    else
        flag = 0;

    for (;val != 0;) {
        int lsb = val % 10;
        bits_.pushBack(lsb);
        val /= 10;
    }
}

BigInt::BigInt(const BigInt &rhs) = default;

BigInt::BigInt(std::string val) : flag(0) {
    int len = val.size();
    int start = 0;
    if (val[0] == '-') {
        flag = 1;
        start = 1;
    }
    for (int i = len - 1; i >= start; i--) {
        if (!isdigit(val[i]))
            throw("Not a number!");
        bits_.pushBack(val[i] - '0');
    }

    this->restructure();
}

BigInt::BigInt(BigInt &&rhs) noexcept : bits_(std::move(rhs.bits_)), flag(rhs.flag) {
    // empty
}

BigInt & BigInt::operator=(const BigInt &rhs) {
    if (&rhs == this)
        return *this;

    bits_.clear();

    for (auto & x : rhs.bits_)
        bits_.pushBack(x);

    return *this;
}

BigInt & BigInt::operator=(BigInt &&rhs) noexcept {
    if (&rhs == this)
        return *this;

    bits_ = rhs.bits_;
    return *this;
}


void BigInt::restructure() {
    int carry = 0;
    for (auto & x : this->bits_) {
        x += carry;
        carry = x / 10;
        x %= 10;
    }

    // deal with non-zero carry, pushing back msb
    for (;carry != 0; carry /= 10)
        bits_.pushBack(carry % 10);

    // pop out zero msb.
    SLList<int>::Iterator itr = bits_.getTail();
    for (;*itr == 0 && bits_.size() > 1;itr = bits_.getTail())
        bits_.popBack();
}

// TODO: Is recursion a better idea?
std::string BigInt::toString() const {
    /*
    std::string st;
    for (auto x : this->bits_) {
        st.insert(0, std::to_string(x));
    }
     */

    /* time checking
    clock_t start = clock();
    std::string st=bitToString(bits_.begin());
    clock_t end = clock();
    double duration = end - start;
    printf("%lf\n", duration);
    return st;
     */
    return bitToString(bits_.begin());
}
std::string BigInt::bitToString(SLList<int>::ConstIterator itr) const {
    if (itr == bits_.end())
        return "";
    SLList<int>::ConstIterator next = itr;
    next++;

    std::string st = std::move(bitToString(next));
    char ch = (*itr) + '0';
    st.push_back(ch);
    return st;
}

void BigInt::print(std::ostream & out) const {
    std::string st;
    st = toString();
    if (flag)
        out << "-" + st;
    else out << st;
}

bool BigInt::isLarger(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.bits_.size() != rhs.bits_.size())
        return lhs.bits_.size() > rhs.bits_.size();

    std::string lhs_st = lhs.toString();
    std::string rhs_st = rhs.toString();
    return (lhs_st > rhs_st);
}

bool BigInt::isEqual(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.bits_.size() != rhs.bits_.size())
        return false;
    std::string lhs_st = lhs.toString();
    std::string rhs_st = rhs.toString();
    return (lhs_st == rhs_st);
}

bool BigInt::isNull(const BigInt &opr) {
    return (opr.bits_.size() == 1) && (opr.bits_.front() == 0);
}


/*
 * Amo 's Note:
 *      You can notice that I first convert BigInt to string,
 *      then compare its value,
 *      because I can't figure out a more clever way.
 *      Algorithm that compare bit by bit or convert to string
 *      then compare are both O(n^2) because the list is singly-linked.
 *
 *      Issue fixed by using recursion?
 */
bool BigInt::isLargerWithSign(const BigInt &lhs, const BigInt &rhs) {
    /*
     * Condition 1:
     * lhs.flag != rhs.flag
     * Then the sign bit of two BigInt is different.
     * if lhs.flag == 0 then lhs is positive, larger than rhs
     * vice versa.
     */
    if (lhs.flag != rhs.flag)
        return !lhs.flag;

    /* ----------------------------------
     * Condition 2
     * lhs.flag == rhs.flag
     */

    /*
     * Sub condition 1
     * lhs.flag == rhs.flag == 0
     * lhs and rhs are both positive.
     */
    if (lhs.flag == 0 && rhs.flag == 0) {
        if (lhs.bits_.size() != rhs.bits_.size())
            return lhs.bits_.size() > rhs.bits_.size();

        // lhs.bits_.size() == rhs.bits.size()
        std::string lhs_st = lhs.toString();
        std::string rhs_st = rhs.toString();
        return (lhs_st > rhs_st);
    }

    /*
     * Sub condition 2
     * lhs.flag == rhs.flag == 1
     * lhs and rhs are both negative.
     */
    if (lhs.flag == 1 && rhs.flag == 1) {
        if (lhs.bits_.size() != rhs.bits_.size())
            return lhs.bits_.size() < rhs.bits_.size();

        // lhs.bits_.size() == rhs.bits.size()
        std::string lhs_st = lhs.toString();
        std::string rhs_st = rhs.toString();
        return (lhs_st < rhs_st);
    }

    throw std::invalid_argument("something wrong with isLarger()");
}

BigInt BigInt::addDirectly(const BigInt &lhs, const BigInt &rhs) {
    BigInt result = isLarger(lhs, rhs)? lhs: rhs;
    const BigInt & addend = isLarger(lhs, rhs)? rhs : lhs;

    // add each bit
    SLList<int>::Iterator itr = result.bits_.begin();
    for (auto & x : addend.bits_) {
        *itr += x;
        itr++;
    }

    result.restructure();   // restructure the result
    return result;
}


BigInt BigInt::minusDirectly(const BigInt &lhs, const BigInt &rhs) {
    BigInt result = isLarger(lhs, rhs)? lhs: rhs;
    const BigInt & subtrahend = isLarger(lhs, rhs)? rhs: lhs;

    // subtract each bit
    SLList<int>::Iterator itr = result.bits_.begin();
    for (auto & x : subtrahend.bits_) {
        /*
         * if the current bit of minuend is less than subtrahend,
         * carry a 10 from next bit to the current bit.
         */
        if (*itr < x) {
            SLList<int>::Iterator next = itr;
            next++;
            (*next)--;
            *itr += 10;
        }
        *itr -= x;
        itr++;
    }

    result.restructure();
    return result;
}



void BigInt::expandN(int n) {
    for (int i = 0; i < n; i++)
        bits_.pushBack(0);
}

BigInt BigInt::multiplyDirectly(const BigInt &lhs, const BigInt &rhs) {
    BigInt result;
    result.expandN(lhs.bits_.size() + rhs.bits_.size());
    const BigInt &multiplicand = lhs.bits_.size() > rhs.bits_.size()? lhs: rhs;
    const BigInt &multiplier = lhs.bits_.size() > rhs.bits_.size()? rhs: lhs;


    SLList<int>::Iterator start = result.bits_.begin();
    for (auto & mlr : multiplier.bits_) {
        SLList<int>::Iterator itr = start;
        for (auto & mlc : multiplicand.bits_) {
            (*itr) += mlc * mlr;
            itr++;
        }
        start++;
    }

    result.restructure();
    return result;

}

int BigInt::divideBitByTwo(SLList<int>::Iterator itr) {
    if (itr == bits_.end())
        return 0;
    SLList<int>::Iterator next = itr;
    next++;
    int old_val = (*itr);
    (*itr) = ((*itr) >> 1) + divideBitByTwo(next);
    return ((old_val & 1) * 5);
}

BigInt & BigInt::divideByTwo() {
    this->divideBitByTwo(bits_.begin());
    this->restructure();
    return *this;
}

BigInt & BigInt::add(const BigInt &rhs) {
    // if the sign bits are equal
    if (!(this->flag ^ rhs.flag)) {
        (*this) = std::move(addDirectly(*this, rhs));
        return *this;
    }

    // if one is positive and another is negative
    if (!this->flag && rhs.flag) {
        (*this) = std::move(minusDirectly(*this, rhs));
        this->flag = isLarger(rhs, *this);
        return (*this);
    }

    if (this->flag && !rhs.flag) {
        (*this) = std::move(minusDirectly(*this, rhs));
        this->flag = isLarger(*this, rhs);
        return (*this);
    }

    throw std::invalid_argument("Error in add()");
}

BigInt & BigInt::minus(const BigInt &rhs) {

    // if the sign bits are equal
    if (this->flag ^ rhs.flag) {
        (*this) = std::move(addDirectly(*this, rhs));
        return (*this);
    }

    // if one is positive and another is negative
    if (!this->flag && !rhs.flag) {
        (*this) = std::move(minusDirectly(*this, rhs));
        this->flag = isLarger(rhs, *this);
        return (*this);
    }

    if (this->flag && rhs.flag) {
        (*this) = std::move(minusDirectly(*this, rhs));
        this->flag = isLarger(*this, rhs);
        return (*this);
    }

    throw std::invalid_argument("Error in minus()");
}

BigInt & BigInt::multiply(const BigInt &rhs) {
    this->flag = this->flag ^ rhs.flag;
    (*this) = std::move(multiplyDirectly(*this, rhs));
    if (isNull(*this)) this->flag = 0;
    return *this;
}


BigInt BigInt::powerDirectly(const BigInt &b, const BigInt &ex) {
    BigInt base = b, exp = ex;
    BigInt result(1);
    while (!isNull(exp)) {
        if (exp.bits_.front() & 1)
            result.multiply(base);
        base.multiply(base);
        exp.divideByTwo();
    }
    return result;
}

BigInt & BigInt::power(const BigInt &rhs) {
    if (this->flag == 1 || rhs.flag == 1)
        throw std::invalid_argument("Can not power with a negative BigInt.");
    *this = std::move(powerDirectly(*this, rhs));
    *this;
}

BigInt & BigInt::operator +=(const BigInt &rhs) {
    this->add(rhs);
    return *this;
}

BigInt BigInt::operator +(const BigInt &rhs) {
    BigInt sum = *this;
    sum.add(rhs);
    return sum;
}

BigInt & BigInt::operator -=(const BigInt &rhs) {
    this->minus(rhs);
    return *this;
}

BigInt BigInt::operator -(const BigInt &rhs) {
    BigInt difference = *this;
    difference.minus(rhs);
    return difference;
}

BigInt & BigInt::operator *=(const BigInt &rhs) {
    this->multiply(rhs);
    return *this;
}

BigInt BigInt::operator *(const BigInt &rhs) {
    BigInt product = *this;
    product.multiply(rhs);
    return *this;
}

