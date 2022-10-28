//
// Created by Amo on 2022/10/22.
//

#ifndef LABS_BIGINT_H
#define LABS_BIGINT_H
#include "SLList.h"

/**
 * @author
 * Amo
 * @brief
 * It is a BigInteger class theoretically can contain infinite integer.\n
 * Implemented with SLList.
 */
class BigInt {
public:
    BigInt();

    ~BigInt();

    explicit BigInt(int val);

    explicit BigInt(std::string val);

    BigInt(const BigInt & rhs);

    BigInt & operator =(const BigInt & rhs);

    BigInt(BigInt && rhs) noexcept;

    BigInt & operator =(BigInt && rhs) noexcept;

    void print(std::ostream &out) const;

    BigInt & add(const BigInt & rhs);

    BigInt & operator +=(const BigInt & rhs);

    BigInt operator +(const BigInt & rhs);

    BigInt & minus(const BigInt &rhs);

    BigInt & operator -=(const BigInt &rhs);

    BigInt operator -(const BigInt &rhs);

    BigInt & multiply(const BigInt &rhs);

    BigInt & operator *=(const BigInt & rhs);

    BigInt operator *(const BigInt & rhs);

    BigInt & divideByTwo();

    BigInt & power(const BigInt &rhs);

private:
    // internal functions


    /**
     * Is lhs larger than rhs regardless of sign.
     * @param lhs a BigInt
     * @param rhs a BigInt
     * @return true if abs(lhs) > abs(rhs)
     */
    static bool isLarger( const BigInt &lhs, const BigInt &rhs);

    /**
     * Is lhs larger than rhs
     * @param lhs a BigInt
     * @param rhs a BigInt
     * @return true if lhs > rhs
     */
    static bool isLargerWithSign( const BigInt &lhs, const BigInt &rhs);

    /**
     * Determine whether two BigInt is equal regardless of sign
     * @param lhs a BigInt
     * @param rhs a BigInt
     * @return true if abs(lhs) == abs(rhs)
     */
    static bool isEqual(const BigInt & lhs, const BigInt & rhs);

    /**
     * Check if bits is null
     * @param opr a BigInt
     * @return true if opr.bits_.size() == 1 && opr.front()==0;
     */
    static bool isNull(const BigInt & opr);

    /**
     * Add two BigInts regardless of sign bit.
     * @param lhs a BigInt
     * @param rhs a BitInt
     * @return sum of two BigInts
     */
    static BigInt addDirectly( const BigInt & lhs, const BigInt & rhs);

    /**
     * Minus two BigInts regardless of sign bit.
     * @param lhs a BigInt
     * @param rhs a BigInt
     * @return Absolute difference between lhs and rhs.
     */
    static BigInt minusDirectly( const BigInt & lhs, const BigInt & rhs);

    /**
     * Just for storing product result, only call by an empty BigInt!
     * @param n number of bits that expands.
     */
    void expandN(int n);

    /**
     * Multiply two BigInts regardless of sign
     * @param lhs a BigInt
     * @param rhs a BitIng
     * @return Absolute product of lhs and rhs
     */
    static BigInt multiplyDirectly( const BigInt & lhs, const BigInt & rhs);

    /**
     * Calculate base^n
     * @param base a BigInt
     * @param n an integer
     * @return Absolute result of power
     */
    static BigInt powerDirectly(const BigInt & b, const BigInt & ex);

    /**
     * Helper function of divideByTwo.\n
     * Return carry for the current bit.\n
     * Implemented by recursion.
     * @param itr
     * @return
     */
    int divideBitByTwo(SLList<int>::Iterator itr);

    [[nodiscard]] std::string toString() const;
    [[nodiscard]] std::string bitToString(SLList<int>::ConstIterator itr) const;

    /**
     * Restructure bits_, expand or shrink bits_ to a formal decimal int
     */
    void restructure();


private:
    /**
     * SLList bits
     * @Invairaint
     *      Each nodes of the list represents a bit of the BigInt. \n
     *      bits_.begin() is the least significant bit.\n
     *      bits_.getTail() is the most significant bit.\n
     *      bits_ is the absolute value of the BigInt.
     */
    SLList<int> bits_;

    /**
     * Flag for positive or negative
     * @Invariant
     *      when BigInt >=0 flag = 0\n
     *      when BigInt < 0 flag = 1
     */
    bool flag;
};

#endif //LABS_BIGINT_H
