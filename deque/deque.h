// https://github.com/pew-pew/mipt-hw/tree/master/deque

#include <vector>
#include <cassert>
#include <algorithm>


template <typename T>
class Deque {
private:
    template <typename U>
    class ShrinkingVector : public std::vector<U> {
        typedef std::vector<U> Base;
    
    public:
        using Base::Base;

        void tryShrink() {
            if (Base::size() * 4 < Base::capacity())
                Base::shrink_to_fit();
        }
    };

    typedef ShrinkingVector<T> Vector;

    // buffer_front_:  .- 2 1 0 [front] < first push back
    // buffer_back_:   '- 3 4 5 [back]  < last push back
    Vector buffer_front_;
    Vector buffer_back_;

public:
    Deque(): buffer_front_(0), buffer_back_(0) {}

    const T& front() const { return (buffer_front_.empty() ? buffer_back_.front()  : buffer_front_.back());}
    const T& back()  const { return (buffer_back_.empty()  ? buffer_front_.front() : buffer_back_.back());}
    const T& operator[](size_t i) const {
        if (i < buffer_front_.size())
            return *(buffer_front_.rbegin() + i);
        return buffer_back_[i - buffer_front_.size()];
    }

    T& front() {return const_cast<T&>(const_cast<const Deque*>(this)->front());}
    T& back()  {return const_cast<T&>(const_cast<const Deque*>(this)->back());}
    T& operator[](size_t i) {return const_cast<T&>(const_cast<const Deque*>(this)->operator[](i));}

    bool empty() const {return buffer_front_.empty() && buffer_back_.empty();};
    size_t size() const {return buffer_front_.size() + buffer_back_.size();};

    void push_back(const T& val){buffer_back_.push_back(val);}
    void push_front(const T& val) {buffer_front_.push_back(val);}
    
    void push_back(T&& val) {buffer_back_.push_back(std::move(val));}
    void push_front(T&& val) {buffer_front_.push_back(std::move(val));}
    
private:
    template <Vector Deque::*BUF_FRONT, Vector Deque::*BUF_BACK>
    void pop_back_() {
        assert(size() > 0);
        Vector &buf_front = (*this).*BUF_FRONT;
        Vector &buf_back  = (*this).*BUF_BACK;

        if (buf_back.empty()) {
            const size_t pop_size = buf_front.size() - buf_front.size() / 2;
            buf_back.reserve(pop_size);
            std::move(buf_front.rend() - pop_size, buf_front.rend(), std::back_inserter(buf_back));
            std::move(buf_front.begin() + pop_size, buf_front.end(), buf_front.begin());
            buf_front.resize(buf_front.size() - pop_size);
            buf_front.tryShrink();
        }
        buf_back.pop_back();
        buf_back.tryShrink();
    }

public:
    void pop_back() {
        pop_back_<&Deque::buffer_front_, &Deque::buffer_back_>();
    }

    void pop_front() {
        pop_back_<&Deque::buffer_back_, &Deque::buffer_front_>();
    }

private:
    template <typename DequeT, typename ValT>
    class DequeIteratorT : public std::iterator<std::random_access_iterator_tag, ValT> {
    private:
        DequeT *p_deque;
        size_t index;
        typedef typename std::iterator_traits<DequeIteratorT>::difference_type diff_type;
    
        DequeIteratorT(DequeT *deque, size_t index): p_deque(deque), index(index) {}
        friend class Deque<T>;
    
    public:
        DequeIteratorT(): p_deque(NULL), index(0) {}

        ValT& operator[](diff_type i) {
            assert(p_deque != NULL);
            return (*p_deque)[index + i];
        }

        ValT& operator*()  {return  (*this)[0];}
        ValT* operator->() {return &(*this)[0];}

        bool operator<(const DequeIteratorT &other) {return index < other.index;}
        bool operator>(const DequeIteratorT &other) {return index > other.index;}
        bool operator<=(const DequeIteratorT &other) {return index <= other.index;}
        bool operator>=(const DequeIteratorT &other) {return index >= other.index;}
        bool operator==(const DequeIteratorT &other) {return index == other.index;}
        bool operator!=(const DequeIteratorT &other) {return index != other.index;}

        DequeIteratorT operator++(int) {return DequeIteratorT(p_deque, index++);}
        DequeIteratorT operator--(int) {return DequeIteratorT(p_deque, index--);}
        DequeIteratorT& operator++() {index++; return *this;}
        DequeIteratorT& operator--() {index--; return *this;}
        DequeIteratorT& operator+=(diff_type diff) { index += diff; return *this;}
        DequeIteratorT& operator-=(diff_type diff) { index -= diff; return *this;}
        
        DequeIteratorT operator+(diff_type diff) const {return DequeIteratorT(*this) += diff;}
        DequeIteratorT operator-(diff_type diff) const {return DequeIteratorT(*this) -= diff;}
        
        friend DequeIteratorT operator+(diff_type diff, const DequeIteratorT& it) {
            return it + diff;
        }

        diff_type operator-(const DequeIteratorT &other) const {
            return static_cast<diff_type>(index) - static_cast<diff_type>(other.index);
        }
    };

public:
    typedef DequeIteratorT<Deque, T> iterator;
    typedef DequeIteratorT<const Deque, const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator       begin()        {return       iterator(this, 0);}
    const_iterator begin()  const {return const_iterator(this, 0);}
    const_iterator cbegin() const {return const_iterator(this, 0);}
   
    iterator       end()        {return       iterator(this, size());}
    const_iterator end()  const {return const_iterator(this, size());}
    const_iterator cend() const {return const_iterator(this, size());}
    
    reverse_iterator       rbegin()        {return       reverse_iterator(end());}
    const_reverse_iterator rbegin()  const {return const_reverse_iterator(end());}
    const_reverse_iterator crbegin() const {return const_reverse_iterator(end());}
   
    reverse_iterator       rend()        {return       reverse_iterator(begin());}
    const_reverse_iterator rend()  const {return const_reverse_iterator(begin());}
    const_reverse_iterator crend() const {return const_reverse_iterator(begin());}
};

