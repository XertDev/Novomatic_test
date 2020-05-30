#ifndef NOVOMATIC_CIRCULARBUFFER_HPP
#define NOVOMATIC_CIRCULARBUFFER_HPP

#include <memory>
#include <limits>

template<typename T, typename A = std::allocator<T>>
class CircularBuffer
{
public:
	typedef CircularBuffer<T, A> self_type;
	typedef A allocator_type;
	typedef typename std::allocator_traits<A>::value_type value_type;
	typedef typename std::allocator_traits<A>::pointer pointer;
	typedef typename std::allocator_traits<A>::const_pointer const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename std::allocator_traits<A>::size_type size_type;
	typedef typename std::allocator_traits<A>::difference_type  difference_type;

	static constexpr size_type DEFAULT_SIZE = 20;

	explicit CircularBuffer(size_type capacity = DEFAULT_SIZE, const allocator_type& allocator = allocator_type());
	CircularBuffer(const self_type& other);
	~CircularBuffer();

	size_type size() const noexcept;
	size_type capacity() const noexcept ;
	[[nodiscard]] bool empty() const noexcept ;
	size_type max_size() const noexcept ;

	reference operator[](size_type index);
	const_reference operator[](size_type index) const;

	reference at(size_type index);
	const_reference at(size_type index) const;

	reference front() noexcept;
	const_reference front() const noexcept;

	reference back() noexcept;
	const_reference back() const noexcept ;

	void clear();

	void push_back(const_reference val);
	void pop_front() noexcept ;

	allocator_type get_allocator() const;

	void swap(self_type &other) noexcept;

private:
	allocator_type allocator_;
	size_type capacity_;
	pointer buffer_;
	size_type size_;
	size_type head_;

	size_type internal_index(size_type i) const;
	static size_type check_capacity(size_type capacity, const allocator_type& allocator);
};

template<typename T, typename A>
CircularBuffer<T, A>::CircularBuffer(size_type capacity, const allocator_type& allocator)
:allocator_(allocator),
capacity_(check_capacity(capacity, allocator_)),
buffer_(std::allocator_traits<A>::allocate(allocator_, capacity, 0)),
size_(0),
head_(0)
{
}

template<typename T, typename A>
CircularBuffer<T, A>::~CircularBuffer()
{
	clear();
	std::allocator_traits<A>::deallocate(allocator_, buffer_, capacity_);
}

template<typename T, typename A>
void CircularBuffer<T, A>::clear()
{
	for(size_type i = 0; i < size_; ++i)
	{
		std::allocator_traits<A>::destroy(allocator_, buffer_ + internal_index(i));
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
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::size() const noexcept
{
	return size_;
}

template<typename T, typename A>
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::capacity() const noexcept
{
	return capacity_;
}

template<typename T, typename A>
bool CircularBuffer<T, A>::empty() const noexcept
{
	return size_ == 0;
}

template<typename T, typename A>
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::max_size() const noexcept
{
	return std::allocator_traits<A>::max_size(allocator_);
}

template<typename T, typename A>
typename CircularBuffer<T, A>::reference CircularBuffer<T, A>::front() noexcept
{
	return buffer_[head_];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::const_reference CircularBuffer<T, A>::front() const noexcept
{
	return buffer_[head_];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::reference CircularBuffer<T, A>::back() noexcept
{
	return buffer_[internal_index(size_-1)];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::const_reference CircularBuffer<T, A>::back() const noexcept
{
	return buffer_[internal_index(size_-1)];
}

template<typename T, typename A>
void CircularBuffer<T, A>::push_back(const_reference val)
{
	const size_type index = internal_index(head_+size_);

	if(capacity_ != size_)
	{
		std::allocator_traits<A>::construct(allocator_, buffer_ + index, val);
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
void CircularBuffer<T, A>::pop_front() noexcept
{
	std::allocator_traits<A>::destroy(allocator_, buffer_ + head_);
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

template<typename T, typename A>
void CircularBuffer<T, A>::swap(self_type& other) noexcept
{
	using std::swap;

	swap(buffer_, other.buffer_);
	swap(capacity_, other.capacity_);
	swap(size_, other.size_);
	swap(head_, other.head_);
}

template<typename T, typename A>
typename CircularBuffer<T, A>::reference CircularBuffer<T, A>::operator[](size_type index)
{
	return buffer_[internal_index(index)];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::const_reference CircularBuffer<T, A>::operator[](CircularBuffer::size_type index) const
{
	return buffer_[internal_index(index)];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::reference CircularBuffer<T, A>::at(CircularBuffer::size_type index)
{
	if(index >= size_)
	{
		throw std::out_of_range("");
	}
	return (*this)[index];
}

template<typename T, typename A>
typename CircularBuffer<T, A>::const_reference CircularBuffer<T, A>::at(CircularBuffer::size_type index) const
{
	if(index >= size_)
	{
		throw std::out_of_range("");
	}
	return (*this)[index];}

template<typename T, typename A>
typename CircularBuffer<T, A>::size_type CircularBuffer<T, A>::check_capacity(
		size_type capacity, const allocator_type& allocator
		)
{
	if(capacity < 1)
	{
		throw std::length_error("capacity must be bigger than 0");
	}
	if(capacity > std::allocator_traits<A>::max_size(allocator))
	{
		throw std::length_error("capacity is bigger than allocator max_size");
	}
	return capacity;
}

template<typename T, typename A>
CircularBuffer<T, A>::CircularBuffer(const CircularBuffer::self_type& other)
:allocator_(other.allocator_),
capacity_(other.capacity_),
buffer_(std::allocator_traits<A>::allocate(allocator_, capacity_, 0)),
size_(0),
head_(other.head_)
{
	try
	{
		for(size_type i = 0; i < other.size_; ++i)
		{
			push_back(other[i]);
		}
	}
	catch (...)
	{
		clear();
		std::allocator_traits<A>::deallocate(allocator_, buffer_, capacity_);
		throw;
	}
}

namespace std
{
		template<typename T, typename A>
		void swap(CircularBuffer<T, A>& lhs, CircularBuffer<T, A>& rhs)
		{
			lhs.swap(rhs);
		}
}

#endif //NOVOMATIC_CIRCULARBUFFER_HPP
