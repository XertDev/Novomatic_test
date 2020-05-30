#ifndef NOVOMATIC_CIRCULARBUFFER_HPP
#define NOVOMATIC_CIRCULARBUFFER_HPP

#include <memory>
#include <limits>

template<typename T, typename A = std::allocator<T>>
class CircularBuffer
{
public:
	typedef A allocator_type;
	typedef typename allocator_type::value_type value_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type  difference_type;

	static constexpr size_type DEFAULT_SIZE = 20;

	explicit CircularBuffer(size_type capacity = DEFAULT_SIZE, const allocator_type& allocator = allocator_type());
	~CircularBuffer();

	size_type size() const;
	size_type capacity() const;
	[[nodiscard]] bool empty() const;
	size_type max_size() const;


	reference front();
	const_reference front() const;

	reference back();
	const_reference back() const;

	void clear();

	void push_back(const_reference val);
	void pop_front();

	allocator_type get_allocator() const;

private:
	allocator_type allocator_;
	size_type capacity_;
	pointer buffer_;
	size_type size_;
	size_type head_;

	size_type internal_index(size_type i) const;
};

template<typename T, typename A>
CircularBuffer<T, A>::CircularBuffer(size_type capacity, const allocator_type& allocator)
:allocator_(allocator),
capacity_(capacity),
buffer_(allocator_.allocate(capacity, 0)),
size_(0),
head_(0)
{
}

template<typename T, typename A>
CircularBuffer<T, A>::~CircularBuffer()
{
	clear();
	allocator_.deallocate(buffer_, capacity_);
}

template<typename T, typename A>
void CircularBuffer<T, A>::clear()
{
	for(size_type i = 0; i < size_; ++i)
	{
		allocator_.destroy(buffer_ + internal_index(i));
	}
	size_ = 0;
	head_ = 0;
}

template<typename T, typename A>
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::internal_index(size_type i) const
{
	return (head_ + i) % capacity_;
}

template<typename T, typename A>
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::size() const
{
	return size_;
}

template<typename T, typename A>
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::capacity() const
{
	return capacity_;
}

template<typename T, typename A>
bool CircularBuffer<T, A>::empty() const
{
	return size_ == 0;
}

template<typename T, typename A>
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::max_size() const
{
	return std::numeric_limits<size_type>::max();
}

template<typename T, typename A>
typename CircularBuffer<T, A>::reference CircularBuffer<T, A>::front()
{
	return buffer_[head_];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::const_reference CircularBuffer<T, A>::front() const
{
	return buffer_[head_];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::reference CircularBuffer<T, A>::back()
{
	return buffer_[internal_index(size_-1)];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::const_reference CircularBuffer<T, A>::back() const
{
	return buffer_[internal_index(size_-1)];
}

template<typename T, typename A>
void CircularBuffer<T, A>::push_back(const_reference val)
{
	const size_type index = internal_index(head_+size_);

	if(capacity_ != size_)
	{
		allocator_.construct(buffer_ + index, val);
		++size_;
	}
	else
	{
		buffer_[index] = val;
		if(++head_ >= capacity_)
		{
			head_ -= capacity_;
		}
	}
}

template<typename T, typename A>
void CircularBuffer<T, A>::pop_front()
{
	allocator_.destroy(buffer_ + head_);
	if(++head_ >= capacity_)
	{
		head_ -= capacity_;
	}
	--size_;
}

template<typename T, typename A>
typename CircularBuffer<T, A>::allocator_type CircularBuffer<T, A>::get_allocator() const
{
	return allocator_;
}


#endif //NOVOMATIC_CIRCULARBUFFER_HPP
